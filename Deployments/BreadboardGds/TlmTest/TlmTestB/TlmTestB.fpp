module TlmTest {

    @ Telemetry test, component B
    passive component TlmTestB {

        sync command TRIGGER()
        sync command ECHO(s: string)

        telemetry U8Channel: U8
        telemetry U16Channel: U16
        telemetry U32Channel: U32
        telemetry I8Channel: I8
        telemetry I16Channel: I16
        telemetry I32Channel: I32
        telemetry StringEvenLength: string
        telemetry StringOddLength: string
        telemetry ArrayChannel: TlmTest.U8x3
        telemetry StructChannel: TlmTest.StructChan

        event ReceivedTelemetry(u8chan: U8, u16chan: U16, u32chan: U32, i8chan: I8, i16chan: I16, i32chan: I32, stringEven: string, stringOdd: string, $array: TlmTest.U8x3, structure: TlmTest.StructChan) \
            severity activity low \
            id 0x00 \
            format "U8: 0x{x}: U16: 0x{x}: U32: 0x{x}: I8: {}: I16: {}: I32: {}: String even: {}: String odd: {}: Array: {}: Struct: {}"

        event Echo(s: string) \
            severity activity low \
            id 0x01 \
            format "Echo: {}"

        output port tlmGet: Fw.TlmGet

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################

        @ Port for requesting the current time
        time get port timeCaller

        @ Enables command handling
        import Fw.Command

        @ Enables event handling
        import Fw.Event

        @ Enables telemetry channels handling
        import Fw.Channel
    }
}