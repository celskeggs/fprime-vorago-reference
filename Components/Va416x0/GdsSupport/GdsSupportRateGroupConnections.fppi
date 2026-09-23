# ----------------------------------------------------------------------
# Direct graph specifiers
# ----------------------------------------------------------------------

connections RateGroup {
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_RESET] -> Va416x0.GdsSupport.mainLoop.reset_vector

    Va416x0.GdsSupport.mainLoop.start_metronome -> Va416x0.GdsSupport.metronome.start_metronome
    Va416x0.GdsSupport.metronome.start_rti -> Va416x0.GdsSupport.mainLoop.start_rti

    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.INTERRUPT_TIM_14] -> Va416x0.GdsSupport.metronome.main_timer_isr
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.INTERRUPT_TIM_15] -> Va416x0.GdsSupport.metronome.proxy_timer_isr

    Va416x0.GdsSupport.metronome.end_rti -> Va416x0.GdsSupport.vectorTable.EndRti

    Va416x0.GdsSupport.mainLoop.cycle -> Va416x0.GdsSupport.rateGroup.CycleIn
    Va416x0.GdsSupport.rateGroup.RateGroupMemberOut -> Va416x0.GdsSupport.tlmSend.Run
    Va416x0.GdsSupport.rateGroup.RateGroupMemberOut -> Va416x0.GdsSupport.gdsStream.Run
}
