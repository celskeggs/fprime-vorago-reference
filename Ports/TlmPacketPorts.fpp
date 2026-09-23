module UartPackets {
    @ Port for getting telemetry, without a timestamp
    port TlmGetNoTime(
        $id: FwChanIdType @< Telemetry Channel ID
        @ Buffer containing serialized telemetry value. 
        @ Size set to 0 if channel not found, or if no value
        @ has been received for this channel yet.
        ref val: Fw.TlmBuffer) -> Fw.TlmValid
}