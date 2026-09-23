# ----------------------------------------------------------------------
# Instances used in the topology
# ----------------------------------------------------------------------

instance Va416x0.GdsSupport.mainLoop
instance Va416x0.GdsSupport.metronome
instance Va416x0.GdsSupport.vectorTable
instance Va416x0.GdsSupport.cmdDisp
instance Va416x0.GdsSupport.gdsStream
instance Va416x0.GdsSupport.tlmSend
instance Va416x0.GdsSupport.fatalAdapter
instance Va416x0.GdsSupport.fatalHandler
instance Va416x0.GdsSupport.exceptionHandler
instance Va416x0.GdsSupport.sclk
instance Va416x0.GdsSupport.rateGroup

# ----------------------------------------------------------------------
# Pattern graph specifiers
# ----------------------------------------------------------------------

command connections instance Va416x0.GdsSupport.cmdDisp

event connections instance Va416x0.GdsSupport.gdsStream

telemetry connections instance Va416x0.GdsSupport.tlmSend

time connections instance Va416x0.GdsSupport.sclk

# ----------------------------------------------------------------------
# Direct graph specifiers
# ----------------------------------------------------------------------

connections Sclk {
    Va416x0.GdsSupport.sclk.getRtiTime -> Va416x0.GdsSupport.metronome.getRtiTime
}

connections Downlink {
    Va416x0.GdsSupport.gdsStream.FatalAnnounce -> Va416x0.GdsSupport.fatalHandler.FatalReceive
}

connections Exceptions {
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_NMI] -> Va416x0.GdsSupport.exceptionHandler.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_NMI - Va416x0Svc.ExceptionHandler.EXCEPTION_START]
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_HARD_FAULT] -> Va416x0.GdsSupport.exceptionHandler.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_HARD_FAULT - Va416x0Svc.ExceptionHandler.EXCEPTION_START]
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_MEM_MANAGE] -> Va416x0.GdsSupport.exceptionHandler.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_MEM_MANAGE - Va416x0Svc.ExceptionHandler.EXCEPTION_START]
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_BUS_FAULT] -> Va416x0.GdsSupport.exceptionHandler.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_BUS_FAULT - Va416x0Svc.ExceptionHandler.EXCEPTION_START]
    Va416x0.GdsSupport.vectorTable.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_USAGE_FAULT] -> Va416x0.GdsSupport.exceptionHandler.exceptions[Va416x0Types.ExceptionNumber.EXCEPTION_USAGE_FAULT - Va416x0Svc.ExceptionHandler.EXCEPTION_START]
}
