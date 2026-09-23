module Va416x0 {

    @ Reports metadata about the running flight software build (version, name, etc)
    passive component BuildInfo {

        @ [DEBUG] Report the requested build information via events and telemetry
        sync command BUILD_INFO() opcode 0

        @ Deployment name
        telemetry DeploymentName: string size FW_TLM_STRING_MAX_SIZE
        @ Build identifier; includes the compilation time, location, and repository branch
        telemetry BuildIdentifier: string size FW_TLM_STRING_MAX_SIZE
        @ Project version
        telemetry ProjectVersion: string size FW_TLM_STRING_MAX_SIZE
        @ fprime framework version
        telemetry FprimeVersion: string size FW_TLM_STRING_MAX_SIZE
        @ fprime-vorago library version
        telemetry FprimeVoragoVersion: string size FW_TLM_STRING_MAX_SIZE
        @ fprime-baremetal library version
        telemetry FprimeBaremetalVersion: string size FW_TLM_STRING_MAX_SIZE
        # FIXME: Remove FwDirectPortCalls when no longer swapping between configurations
        @ FW_DIRECT_PORT_CALLS macro setting 
        telemetry FwDirectPortCalls: I32

        event DeploymentName(deployment_name: string) \
            severity activity low \
            id 0x00 \
            format "Deployment Name: {}"
        event BuildIdentifier(build_identifier: string) \
            severity activity low \
            id 0x01 \
            format "Build ID: {}"
        event ProjectVersion(version: string) \
            severity activity low \
            id 0x02 \
            format "Project version: {}"
        event FprimeVersion(version: string) \
            severity activity low \
            id 0x03 \
            format "fprime@{}"
        event FprimeVoragoVersion(version: string) \
            severity activity low \
            id 0x04 \
            format "{}"
        event FprimeBaremetalVersion(version: string) \
            severity activity low \
            id 0x05 \
            format "{}"
        # FIXME: Remove FwDirectPortCalls when no longer swapping between configurations
        event FwDirectPortCalls(macro: I32) \
            severity activity low \
            id 0x06 \
            format "FW_DIRECT_PORT_CALLS is set to {}"

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

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

        @ Port for sending telemetry channels to downlink
        telemetry port tlmOut
    }
}
