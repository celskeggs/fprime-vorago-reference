# ----------------------------------------------------------------------
# Direct graph specifiers
# ----------------------------------------------------------------------

connections Uplink {
    # These connections are required for deployments that expect direct GDS commanding/status
    Va416x0.GdsSupport.gdsStream.SendCommandPacket -> Va416x0.GdsSupport.cmdDisp.seqCmdBuff
    Va416x0.GdsSupport.cmdDisp.seqCmdStatus -> Va416x0.GdsSupport.gdsStream.cmdResponseIn
}
