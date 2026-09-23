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
# Components/Va416x0/GdsSupport/fprime_jlink_terminal.py
# Script used to read events and console output from external SRAM
###############################################################################

import argparse
import logging
import pathlib
import os
import readline
import sys
import time
import typing
import Va416x0.Os.SeggerTerminal.config as config

from fprime_gds.common.data_types.cmd_data import CmdData, CommandArgumentsException
from fprime_gds.common.decoders.event_decoder import EventDecoder
from fprime_gds.common.encoders.cmd_encoder import CmdEncoder
from fprime_gds.common.models.dictionaries import Dictionaries
from fprime_gds.common.templates.cmd_template import CmdTemplate
from fprime_gds.common.templates.event_template import EventTemplate
from fprime_gds.common.utils import string_util
from fprime_gds.executables.cli import ConfigDrivenParser, ParserBase

from tools.common.jlink import JLinkConnection, JLinkError, verify_protocol_version
from tools.common.jlink.commands import CommandDriver
from tools.common.jlink.events import EventDriver, LogItem, LogType
from tools.common.jlink.telemetry import TlmDriver, TlmSnapshot, TlmItem

CONFIG_DEFAULT_PATH = pathlib.Path(
    os.environ.get("FPRIME_GDS_CONFIG_PATH", "fprime-gds.yml")
)
ConfigDrivenParser.set_default_configuration(CONFIG_DEFAULT_PATH)
ConfigDrivenParser.DESCRIPTION = "F-Prime GDS configuration"

TerminalDriver = typing.Union[CommandDriver, EventDriver, TlmDriver]
TerminalHandler = typing.Callable[[JLinkConnection, TerminalDriver], None]

COMMAND_HISTORY_PATH = pathlib.Path.home() / ".jlink_terminal_history"
LOGGER = logging.getLogger(__name__)
PROMPT = "CMD> "

# Event handler reads from the event buffer 4 times per second
NANOSECONDS = 1_000_000_000
EVENT_SLEEP_DURATION_NS = 0.25 * NANOSECONDS


class StandardParser(ParserBase):

    DESCRIPTION = "Standard arguments"

    def get_arguments(self) -> dict:
        return {
            ("-d", "--deployment"): {
                "dest": "deployment",
                "action": "store",
                "type": pathlib.Path,
                "help": "Deployment installation/build output directory",
            },
            ("--dictionary",): {
                "dest": "dictionary",
                "action": "store",
                "type": pathlib.Path,
                "help": "Dictionary path. Overrides automatic dictionary detection",
            },
            ("-s", "--jlink-speed"): {
                "dest": "jlink_speed",
                "action": "store",
                "metavar": "SPEED",
                "default": config.get_speed(),
                "type": int,
                "help": f"JLink speed (if not provided, {config.get_speed_src()})",
            },
            ("-o", "--output"): {
                "dest": "output",
                "action": "store",
                "type": pathlib.Path,
                "help": "Output file. Defaults to stdout if not provided",
            },
            ("--commander",): {
                "dest": "commander",
                "action": "store_true",
                "help": "Attach an interactive terminal that can be used to send commands to FSW "
                "using the JLink connection. Supports tab-completion and history. Note that this "
                "runs in a separate mode which does NOT print event or console output",
            },
            ("--telemetry",): {
                "dest": "telemetry",
                "action": "store_true",
                "help": "Display a one-shot snapshot of all telemetry channel values from both "
                "ping and pong buffers.",
            },
            ("--debug",): {
                "dest": "debug",
                "action": "store_true",
                "help": "Enable debug output",
            },
        }

    def handle_arguments(self, args, **kwargs):
        if args.dictionary is not None:
            dictionary_path = args.dictionary
        elif args.deployment is not None:
            # This assumes the deployment name is the last folder in the path and that the
            # deployment name follows the format "Deployments_<deployment_name>"
            deployment_name = args.deployment.name.split("Deployments_")[-1]
            # Use the external memory dictionary in order to find external SRAM addresses
            dictionary_path = (
                args.deployment
                / f"dict/extmem/{deployment_name}TopologyDictionary-extmem.json"
            )
        else:
            raise ValueError(
                "Cannot determine the dictionary path; missing one of argument --deployment or "
                "--dictionary"
            )
        args.dictionary = dictionary_path
        return args


class CommandTabCompleter:

    def __init__(self, dictionary: Dictionaries):
        assert dictionary.command_name is not None
        self.names = sorted(dictionary.command_name)

    def complete(self, text: str, state: int) -> typing.Optional[str]:
        options = [name for name in self.names if text in name]
        return options[state] if state < len(options) else None


def parse_args() -> argparse.Namespace:
    args, parser, remaining = ConfigDrivenParser.parse_known_args(
        [StandardParser],
        "Reads events and console output from FSW using JLink. Also provides a commanding "
        "interface that can be used in place of fprime-gds for sending commands.",
    )
    return args


def configure_logging(path: typing.Optional[pathlib.Path], debug: bool = False):
    if path is None:
        stream = sys.stdout
    else:
        stream = path.open("w")
    logging.basicConfig(
        format="%(message)s",
        stream=stream,
        level=logging.DEBUG if debug else logging.INFO,
    )


def connect_to_jlink(
    jlink: JLinkConnection, driver: TerminalDriver, attempts: int
) -> bool:
    try:
        jlink.connect()
        LOGGER.debug(
            f"Connected to target over JLink with speed {jlink.speed} after {attempts} attempts"
        )
        driver.on_open(jlink)
        LOGGER.debug("Validated JLink connection with driver")
        return True
    except JLinkError as ex:
        LOGGER.debug(f"JLink connection error: {ex}")
        jlink.close()
        # Log the connection error on the first attempt, and every 10 attempts afterwards
        if attempts == 1:
            LOGGER.warning("Failed to connect to JLink, retrying")
        elif attempts % 10 == 0:
            LOGGER.warning(
                f"Unable to connect to JLink after {attempts} attempts, retrying"
            )
        return False


def format_event_message(template: EventTemplate, args: bytes) -> str:
    _, parsed_args = EventDecoder.decode_args(args, 0, template)

    # Fill in the event string format string using the args bytes
    # Make the F-Prime format strings Python-friendly
    format_str = string_util.preprocess_fpp_format_str(template.format_str)
    format_args = (arg.val for arg in parsed_args)
    message = format_str.format(*format_args)

    return f"[{template.severity}] (0x{template.id:04X}) {template.name} : {message}"


def print_items(items: list[LogItem], dictionary: Dictionaries):
    # Buffer console output until a newline is received
    console_buffer = []

    for item in items:
        if item.log_type == LogType.Event:
            assert dictionary.event_id is not None
            try:
                event = dictionary.event_id[item.event_id]
            except KeyError:
                LOGGER.warning(f"Invalid event with ID 0x{item.event_id:X}: {item}")
                continue
            timestamp = f"{item.time_seconds}.{item.time_useconds:06}"
            formatted = format_event_message(event, item.args)
            LOGGER.info(f"{timestamp} {formatted}")
        elif item.log_type == LogType.Console:
            try:
                contents = item.args.decode()
            except UnicodeDecodeError:
                LOGGER.warning(f"Failed to decode console output: {item.args.hex()}")
                continue
            console_buffer.append(contents)
            # If the console output contains a newline, print the line along with any output that
            # has been buffered previously
            # Handle any output that comes after the newline, including other newline characters
            # GdsStream will always flush when it receives a newline but console output can be
            # buffered if it is received prior to the component being enabled
            if "\n" in contents:
                buffered_lines = "".join(console_buffer).split("\n")
                console_buffer.clear()
                # Print up to but not including the final item in the list. If the output was
                # terminated by a newline, this will be empty and we will ignore it. Otherwise,
                # add it back to the buffer
                for line in buffered_lines[:-1]:
                    LOGGER.info(line)
                if len(buffered_lines[-1]) > 0:
                    console_buffer.append(buffered_lines[-1])
        else:
            raise ValueError(f"Unexpected item type: {item.log_type}")

    if console_buffer:
        LOGGER.info("".join(console_buffer).strip())


def event_handler(jlink: JLinkConnection, driver: TerminalDriver):
    assert isinstance(driver, EventDriver)
    # Read the contents of the buffer
    buffer = driver.read_buffer(jlink)
    # Unpack the events and console output out of the buffer
    items = driver.unpack_events(buffer)
    # Print the contents to stdout
    print_items(items, driver.dictionary)


def parse_command(dictionary: Dictionaries, command: str) -> bytes:
    # FIXME: this is not the best way to parse commands, for example this will break if a string
    # argument has a comma inside it, figure out a better way to handle command parsing
    if "," in command:
        name, *args = command.split(",")
    else:
        name = command
        args = []

    assert dictionary.command_name is not None
    if name not in dictionary.command_name:
        raise ValueError(f"Invalid command name: {name}")
    template: CmdTemplate = dictionary.command_name[name]
    if len(args) != len(template.arguments):
        raise ValueError(
            f"Invalid command arguments: expected {len(template.arguments)}, found {len(args)}"
        )
    data = CmdData(args, template)
    packet = CmdEncoder().encode_api(data)
    # NOTE: strip the U32 description and U32 length off the front of the command
    return packet[8:]


def command_handler(jlink: JLinkConnection, driver: TerminalDriver):
    assert isinstance(driver, CommandDriver)
    # Prompt for command input
    command = input(PROMPT)
    try:
        packet = parse_command(driver.dictionary, command)
        driver.write_packet(jlink, packet)
    except (CommandArgumentsException, ValueError) as ex:
        LOGGER.error(ex)


def format_channel_value(item: typing.Optional[TlmItem], channel) -> str:
    """Format a telemetry channel value for display."""
    if item is None or not item.value:
        return "[no value stored]"

    try:
        # Deserialize the value using the channel type
        value_obj = channel.ch_type_obj()
        value_obj.deserialize(item.value, 0)
        timestamp = f"{item.time_seconds}.{item.time_useconds:06}"
        return f"{value_obj.val} ({item.value.hex().upper()}) @ {timestamp}"
    except Exception as ex:
        return f"<error: {ex}>"


def display_telemetry_snapshot(
    first_snapshot: TlmSnapshot,
    second_snapshot: TlmSnapshot,
    dictionary: Dictionaries,
):
    """Display telemetry snapshot showing values from both buffers."""
    assert dictionary.channel_id is not None

    # Create a mapping from channel_id to items for quick lookup
    first_items_by_id = {item.channel_id: item for item in first_snapshot.items}
    second_items_by_id = {item.channel_id: item for item in second_snapshot.items}

    # Get buffer names based on selector
    first_buffer_name = f"{first_snapshot.selector.name} Buffer"
    second_buffer_name = f"{second_snapshot.selector.name} Buffer"

    # Iterate through all channels in the dictionary (sorted by ID)
    for channel_id, channel in sorted(dictionary.channel_id.items()):
        channel_name = channel.get_full_name()
        # Skip ground-derived channels
        if "(Ground)" in channel_name:
            continue

        # Get items from both buffers if available
        first_item = first_items_by_id.get(channel_id)
        second_item = second_items_by_id.get(channel_id)

        # Format values for display
        first_value_str = format_channel_value(first_item, channel)
        second_value_str = format_channel_value(second_item, channel)

        # Print the channel with values from both buffers
        LOGGER.info(f"0x{channel_id:04X}: {channel_name}:")
        LOGGER.info(
            f"  0x{first_item.value_address:04x}: {first_buffer_name}: {first_value_str}"
        )
        LOGGER.info(
            f"  0x{second_item.value_address:04x}: {second_buffer_name}: {second_value_str}"
        )


def telemetry_snapshot_handler(jlink: JLinkConnection, driver: TerminalDriver):
    assert isinstance(driver, TlmDriver)
    # Read snapshot through driver interface
    first_snapshot, second_snapshot = driver.read_telemetry_snapshot(jlink)
    # Display the snapshot
    display_telemetry_snapshot(first_snapshot, second_snapshot, driver.dictionary)


def main_loop(
    handler: TerminalHandler,
    driver: TerminalDriver,
    jlink_speed: typing.Optional[int],
    # Defaults to the event handler period which is 4 Hz
    period_ns: float = EVENT_SLEEP_DURATION_NS,
):
    # Set up a JLink connection to the target
    jlink = JLinkConnection(speed=jlink_speed)
    jlink_connected = False
    connection_attempts = 0

    loop_start = 0
    while True:
        loop_start = time.time_ns()
        if not jlink_connected:
            connection_attempts += 1
            if connect_to_jlink(jlink, driver, connection_attempts):
                jlink_connected = True
                connection_attempts = 0
        if jlink_connected:
            try:
                handler(jlink, driver)
            except JLinkError as ex:
                # JLink error occurred during the main loop, close the connection and log
                # the error and then try to re-connect during the next iteration
                LOGGER.warning(
                    f"A JLink error occurred: {ex}, disconnecting from the target and will "
                    "try to re-establish the connection"
                )
                jlink.close()
                jlink_connected = False

        # Briefly sleep and continue
        elapsed = time.time_ns() - loop_start
        sleep_duration = max(0, period_ns - elapsed)
        LOGGER.debug(f"Loop duration: {elapsed} ns: Sleeping for {sleep_duration} ns")
        time.sleep(sleep_duration / NANOSECONDS)


def main():
    args = parse_args()
    configure_logging(args.output, debug=args.debug)

    # Find and open the F-Prime dictionary using the command-line arguments
    LOGGER.debug(f"Using dictionary {args.dictionary}")
    dictionary = Dictionaries.load_dictionaries_into_config(str(args.dictionary))
    # Verify that the GDS protocol version matches
    verify_protocol_version(dictionary)

    try:
        if args.commander:
            if not COMMAND_HISTORY_PATH.exists():
                COMMAND_HISTORY_PATH.touch()
            readline.read_history_file(COMMAND_HISTORY_PATH)
            readline.parse_and_bind("tab: complete")
            completer = CommandTabCompleter(dictionary)
            readline.set_completer(completer.complete)
            main_loop(
                command_handler,
                CommandDriver(dictionary),
                args.jlink_speed,
                period_ns=0.0,
            )
        elif args.telemetry:
            # Telemetry snapshot mode - one-shot, no loop
            driver = TlmDriver(dictionary)
            jlink = JLinkConnection(speed=args.jlink_speed)
            jlink.connect()
            driver.on_open(jlink)
            telemetry_snapshot_handler(jlink, driver)
            jlink.close()
        else:
            main_loop(
                event_handler,
                EventDriver(dictionary),
                args.jlink_speed,
                period_ns=EVENT_SLEEP_DURATION_NS,
            )
    except KeyboardInterrupt:
        LOGGER.info("\nCtrl+C received, exiting")
    except EOFError:
        LOGGER.info("\nCtrl+D received, exiting")
    finally:
        if args.commander:
            readline.write_history_file(COMMAND_HISTORY_PATH)


if __name__ == "__main__":
    main()
