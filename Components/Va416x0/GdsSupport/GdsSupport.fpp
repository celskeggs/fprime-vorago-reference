module Va416x0 {
  module GdsSupport {
    # Use of mainLoop is required for GDS compatibility
    instance mainLoop: Va416x0Svc.MainLoop base id 0x0000

    instance metronome: Va416x0Svc.Metronome base id 0x0020 {
      phase Fpp.ToCpp.Phases.instances """
      Va416x0Svc::Metronome metronome(FW_OPTIONAL_NAME("metronome"), metronome_config);
      """
    }

    instance vectorTable: Va416x0Svc.VectorTable base id 0x0040

    instance cmdDisp: Baremetal.PassiveCmdDispatcher base id 0x0060

    instance gdsStream: Va416x0.GdsStream base id 0x0080

    instance tlmSend: Va416x0.TlmGdsChan base id 0x00A0

    instance fatalAdapter: Svc.AssertFatalAdapter base id 0x00C0

    instance fatalHandler: Va416x0Svc.FatalHandler base id 0x00E0

    instance exceptionHandler: Va416x0Svc.ExceptionHandler base id 0x0100

    instance sclk: Va416x0.SpacecraftClock base id 0x0120

    instance rateGroup: Svc.PassiveRateGroup base id 0x0140
  }
}
