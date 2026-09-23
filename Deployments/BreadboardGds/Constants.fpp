module BreadboardGds {
    @ RTI default duration in microseconds (64 Hz)
    dictionary constant RtiDefaultDurationMicros = Va416x0.RtiDurationMicrosecs64Hz
    @ RTI minimum duration in microseconds (64 Hz - 0.125 ms)
    dictionary constant RtiMinDurationMicros = RtiDefaultDurationMicros - 125
    @ RTI maximum duration in microseconds (64 Hz + 0.125 ms)
    dictionary constant RtiMaxDurationMicros = RtiDefaultDurationMicros + 125
}
