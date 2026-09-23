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
# Components/Va416x0/GdsSupport/fprime_jlink_comm.py
# F-Prime GDS communications plugin supporting the Va416x0.GdsStream and
# Va416x0.TlmGdsChan components.
###############################################################################

import logging
import pathlib
import typing

from fprime_gds.common.communication.adapters.base import BaseAdapter
from fprime_gds.common.communication.framing import FpFramerDeframer
from fprime_gds.common.models.dictionaries import Dictionaries
from fprime_gds.plugin.definitions import gds_plugin

from tools.common.jlink import (
    JLinkCommDriver,
    JLinkConnection,
    JLinkError,
    verify_protocol_version,
)
from tools.common.jlink.commands import CommandDriver
from tools.common.jlink.events import EventDriver
from tools.common.jlink.telemetry import TlmDriver

LOGGER = logging.getLogger("fprime_jlink_comm")


@gds_plugin(BaseAdapter)
class JLinkCommAdapter(BaseAdapter):
    """
    F-Prime GDS communication adapter used for communicating with FSW over the
    JLink connection with the Va416x0.GdsStream and Va416x0.TlmGdsChan
    components.
    """

    DRIVERS: list[type[JLinkCommDriver]] = [
        TlmDriver,
        EventDriver,
        CommandDriver,
    ]

    def __init__(
        self,
        deployment: typing.Optional[str],
        dictionary: typing.Optional[str],
        jlink_speed: typing.Optional[int],
    ):
        self.is_open = False
        self.jlink = JLinkConnection()
        self.jlink_speed = jlink_speed
        self.framer = FpFramerDeframer()
        # Find the dictionary given the command-line arguments
        if dictionary is not None:
            dictionary_path = pathlib.Path(dictionary)
        elif deployment is not None:
            # This assumes the deployment name is the last folder in the path and that the
            # deployment name follows the format "Deployments_<deployment_name>"
            deployment_name = deployment.split("Deployments_")[-1]
            # Use the external memory dictionary in order to find external SRAM addresses
            dictionary_path = (
                pathlib.Path(deployment)
                / f"dict/extmem/{deployment_name}TopologyDictionary-extmem.json"
            )
        else:
            raise ValueError(
                f"{self.get_name()} cannot determine the dictionary path; missing one of argument "
                "--deployment or --dictionary"
            )
        LOGGER.info(f"Using dictionary: {dictionary_path}")
        fprime_dictionary = Dictionaries()
        fprime_dictionary.load_dictionaries(str(dictionary_path), None, None)
        # Verify that the GDS protocol version matches
        verify_protocol_version(fprime_dictionary)
        # Create the driver instances using the dictionary
        self.drivers = [driver(fprime_dictionary) for driver in self.DRIVERS]

    @classmethod
    def get_name(cls):
        return "jlink-comm"

    @classmethod
    def get_arguments(cls):
        return {
            ("--deployment",): {
                "type": pathlib.Path,
                "help": "Deployment installation/build output directory.",
            },
            ("--dictionary",): {
                "type": pathlib.Path,
                "help": "Path to dictionary. Overrides automatic dictionary detection.",
            },
            ("--jlink-speed",): {
                "type": int,
                "help": "JLink speed to use for the connection.",
            },
        }

    ###########################################################################
    # BaseAdapter interface
    ###########################################################################

    def open(self):
        """
        Open the JLink connection to the target.
        """
        self.jlink.close()

        LOGGER.info("Opening JLink connection")
        # Try to open the JLink connection and perform driver validations; if the connect or
        # validation fails, do not mark the connection as open and will can be retried later
        try:
            self.jlink.connect(speed=self.jlink_speed)
            for driver in self.drivers:
                driver.on_open(self.jlink)
        except JLinkError as ex:
            # FIXME: consider throttling errors to prevent ballooning the logs
            LOGGER.error(f"JLink error during connect: {ex}")
        else:
            self.is_open = True

    def close(self):
        """
        Close the JLink connection and clean up.
        """
        self.is_open = False
        LOGGER.info("Closing JLink connection")
        self.jlink.close()

    def read(self, timeout: float = 0) -> bytes:
        """
        Read downlink data from FSW over the JLink connection.
        """
        packets = []
        # If the connection is closed, first attempt to re-open it
        if not self.is_open:
            self.open()
        # Check if the connection was successfully opened and bail out otherwise
        if not self.is_open:
            return b""

        # Read packets using the configured drivers
        try:
            for driver in self.drivers:
                packets.extend(driver.read_packets(self.jlink))
        except JLinkError as ex:
            LOGGER.error(f"JLink error during read: {ex}")
            # Close the JLink connection
            self.close()
            return b""

        # Frame the packets in the expected F-Prime format
        frames = [self.framer.frame(packet) for packet in packets]

        # Concatenate the frames into a single bytestream
        return b"".join(frames)

    def write(self, frame: bytes) -> bool:
        """
        Write uplink data to FSW over the JLink connection.
        """
        # If the connection is closed, first attempt to re-open it
        if not self.is_open:
            self.open()
        # Check if the connection was successfully opened and bail out otherwise
        if not self.is_open:
            return False

        # Deframe the packet from the given frame
        packet, remaining, discarded = self.framer.deframe(frame)
        if packet is None or remaining or discarded:
            LOGGER.warning(
                f"Failed to fully de-frame uplink packet: {len(discarded)} bytes discarded, "
                f"{len(remaining)} bytes left over"
            )
            return False

        # Write the packet using the configured drivers
        try:
            status = True
            for driver in self.drivers:
                write_status = driver.write_packet(self.jlink, packet)
                if write_status != NotImplemented:
                    status = status and write_status
        except JLinkError as ex:
            LOGGER.error(f"JLink error during read: {ex}")
            # Close the JLink connection
            self.close()
            return False

        return status
