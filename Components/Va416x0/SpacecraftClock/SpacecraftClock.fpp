module Va416x0 {
    @ Tracks the current time on the Mission/Spacecraft Clock
    passive component SpacecraftClock {

        @ Imports the standard F' Svc.Time ports
        import Svc.Time

        @ Output port to capture RTI time from fprime-vorago support lib
        output port getRtiTime : Va416x0.GetRtiTime

        @ Command to set spacecraft clock
        sync command SET_CURRENT_TIME(
            secondsNow: U32  @< Time in seconds to set spacecraft clock to
        )

        @ Store current clock time (upper 32-bits - seconds, lower 32-bits - subseconds)
        telemetry CurrentTime: U64 update on change

        ###############################################################################
        # Standard AC Ports: Required for Channels, Events, Commands, and Parameters  #
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending command registrations
        command reg port cmdRegOut

        @ Port for receiving commands
        command recv port cmdIn

        @ Port for sending command responses
        command resp port cmdResponseOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut
    }
}
