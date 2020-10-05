from m5.params import *
from m5.SimObject import SimObject

class Injector(SimObject):
    type = 'Injector'
    cxx_class = 'FaultInjector::Injector'
    cxx_header = 'injector/injector.hh'

    startPC = Param.String("0", "starting PC of main")
    endPC = Param.String("0", "ending PC of main")
