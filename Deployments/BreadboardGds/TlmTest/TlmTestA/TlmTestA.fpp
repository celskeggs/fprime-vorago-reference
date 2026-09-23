module TlmTest {

    array U8x3 = [3] U8
    struct StructChan {
        a: U8
        b: U16
        c: U32
    }

    @ Telemetry test, component A
    passive component TlmTestA {

        telemetry U8Channel: U8
        telemetry U16Channel: U16
        telemetry U32Channel: U32
        telemetry I8Channel: I8
        telemetry I16Channel: I16
        telemetry I32Channel: I32
        telemetry StringEvenLength: string
        telemetry StringOddLength: string
        telemetry ArrayChannel: U8x3
        telemetry StructChannel: StructChan

        sync input port run: Svc.Sched

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################

        @ Port for requesting the current time
        time get port timeCaller

        @ Enables telemetry channels handling
        import Fw.Channel
    }
}