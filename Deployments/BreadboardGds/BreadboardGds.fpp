module BreadboardGds {

  instance buildInfo: Va416x0.BuildInfo base id 0x0200

  instance tlmTestA: TlmTest.TlmTestA base id 0x0240
  instance tlmTestB: TlmTest.TlmTestB base id 0x0260

  deployment topology BreadboardGds {

    include "../../Components/Va416x0/GdsSupport/GdsSupportConnections.fppi"
    include "../../Components/Va416x0/GdsSupport/GdsSupportCmdConnections.fppi"
    include "../../Components/Va416x0/GdsSupport/GdsSupportRateGroupConnections.fppi"

    # ----------------------------------------------------------------------
    # Instances used in the topology
    # ----------------------------------------------------------------------

    instance buildInfo

    instance tlmTestA
    instance tlmTestB

    connections TlmTest {
        Va416x0.GdsSupport.rateGroup.RateGroupMemberOut -> tlmTestA.run
        tlmTestB.tlmGet -> Va416x0.GdsSupport.tlmSend.TlmGet
    }
  }

  system BreadboardGds: BreadboardGds
}
