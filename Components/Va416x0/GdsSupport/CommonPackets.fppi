@ Telemetry packet definitions

@ Packet containing group 2 of common telemetry
packet commonTelemetry2 id 2 group 255 {
  Va416x0.GdsSupport.sclk.CurrentTime
  Va416x0.GdsSupport.rateGroup.MaxCycleTime
  Va416x0.GdsSupport.rateGroup.CycleTime
  Va416x0.GdsSupport.rateGroup.CycleCount
  Va416x0.GdsSupport.vectorTable.RtiIrqDutyCycleHwm
  Va416x0.GdsSupport.gdsStream.DroppedEvents
  Va416x0.GdsSupport.gdsStream.DroppedConsoleBytes
}
@ Packet containing telemetry for the rate group cycle times
packet rateGroupCycleTimeTelemetry1 id 3 group 255 {
  Va416x0.GdsSupport.rateGroup.PortCycleTime
}
@ Packet containing telemetry for the rate group high watermark cycle times
packet rateGroupCycleTimeTelemetry2 id 4 group 255 {
  Va416x0.GdsSupport.rateGroup.PortCycleTimeHWM
}
@ Packet contains portion of BuildInfo telemetry
packet buildInfoTelemetry1 id 5 group 255 {
  buildInfo.DeploymentName
  buildInfo.BuildIdentifier
}
@ Packet contains portion of BuildInfo telemetry
packet buildInfoTelemetry2 id 6 group 255 {
  buildInfo.ProjectVersion
  buildInfo.FprimeVersion
}
@ Packet contains portion of BuildInfo telemetry
packet buildInfoTelemetry3 id 7 group 255 {
  buildInfo.FprimeVoragoVersion
  buildInfo.FprimeBaremetalVersion
}
@ Packet contains portion of BuildInfo telemetry
packet buildInfoTelemetry4 id 8 group 255 {
  buildInfo.FwDirectPortCalls
}
