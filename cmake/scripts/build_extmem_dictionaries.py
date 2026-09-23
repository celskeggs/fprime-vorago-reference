#!/usr/bin/env python3
# Copyright 2026 California Institute of Technology
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###############################################################################
# cmake/scripts/build_extmem_dictionaries.py
###############################################################################

import argparse
import cpp_demangle
import dataclasses
import elftools.elf.elffile
import elftools.elf.sections
import json
import pathlib
import sys

from fprime_gds.common.loaders.constant_json_loader import ConstantJsonLoader


@dataclasses.dataclass
class Symbol:
    section: str
    name: str
    address: int
    size: int


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Parse the linker allocations of external memory regions for the given "
        "deployment and feed them into the dictionary."
    )
    parser.add_argument("elf_path", type=pathlib.Path)
    parser.add_argument("dictionary_path", type=pathlib.Path)
    parser.add_argument("output_path", type=pathlib.Path)
    parser.add_argument("sections", help="Semicolon-delimited list in CMake format")
    return parser.parse_args()


def parse_symbol_table(elf_path: pathlib.Path, sections: set[str]) -> list[Symbol]:
    symbols = []

    elffile = elftools.elf.elffile.ELFFile.load_from_path(elf_path)
    symbol_table = elffile.get_section_by_name(".symtab")
    if not symbol_table:
        raise ValueError(f"Symbol table not found in ELF file {elf_path}")
    if not isinstance(symbol_table, elftools.elf.sections.SymbolTableSection):
        raise ValueError(
            f"Symbol table in ELF file {elf_path} has unexpected type: {type(symbol_table)}"
        )

    section_names = {i: s.name for i, s in enumerate(elffile.iter_sections())}

    for symbol in symbol_table.iter_symbols():
        # The st_shndx field gives the index of the section
        section_index = symbol.entry["st_shndx"]
        if section_index == "SHN_UNDEF" or section_index == "SHN_ABS":
            continue
        section = section_names[section_index]
        if section not in sections:
            continue

        # Only parse variables with type STT_OBJECT
        if symbol.entry["st_info"]["type"] != "STT_OBJECT":
            continue

        # Clean up the symbol name
        link_name = symbol.name
        if symbol.name.startswith(".Lswitch.table._Z"):
            link_name = symbol.name.replace(".Lswitch.table._Z", "_Z")
        elif symbol.name.startswith(".L_Z"):
            link_name = symbol.name.replace(".L_Z", "_Z")
        try:
            name = cpp_demangle.demangle(link_name)
        except ValueError:
            name = link_name

        symbols.append(
            Symbol(
                section,
                name,
                symbol.entry.st_value,
                symbol.entry.st_size,
            )
        )

    return symbols


def make_constant(symbol: Symbol) -> dict:
    # Clean up the symbol name to be fprime-appropriate
    name = symbol.name.replace("::", ".")
    return {
        "kind": "constant",
        "qualifiedName": name,
        "type": {"name": "U32", "kind": "integer", "size": 32, "signed": False},
        "value": symbol.address,
        "annotation": f"Allocation in section {symbol.section}",
    }


def main(args: argparse.Namespace):
    # Parse the symbols for the given sections from the ELF file
    sections = set(args.sections.split(";"))
    symbols = parse_symbol_table(args.elf_path, sections)

    # Parse the deployment dictionary
    with args.dictionary_path.open() as ifile:
        dictionary = json.load(ifile)

    # Output symbols as constants into the dictionary
    for symbol in symbols:
        dictionary[ConstantJsonLoader.CONSTANTS_FIELD].append(make_constant(symbol))

    # Flush the contents of the updated dictionary to the file
    if not args.output_path.parent.exists():
        args.output_path.parent.mkdir(parents=True)
    with args.output_path.open("w") as ofile:
        json.dump(dictionary, ofile, indent=2)


if __name__ == "__main__":
    try:
        args = parse_args()
        main(args)
    except Exception as ex:
        sys.exit(f"ERROR: {ex}")
