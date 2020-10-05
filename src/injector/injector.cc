#include "debug/Inj.hh"
#include "sim/sim_exit.hh"
#include "injector/injector.hh"

#include <fstream>
#include <iostream>

using namespace X86ISA;

namespace FaultInjector
{

void FI::FlipBit(Tick _injTick, int injR, int injBit, int regType)
{
    uint64_t currVal; // = thread->readIntReg(injR);
    uint64_t bitMask= 1 << injBit;

    if (regType == 0)
    {
        currVal = thread->readIntReg(injR); 
        thread->setIntReg(injR, currVal ^ bitMask); // flip bit using mask
    }
    else if (regType == 1)  // floating point register
    {
        currVal = thread->readFloatReg(injR);
        // thread->setFloatReg(injR, currVal ^ bitMask);
        thread->setFloatReg(injR, ~currVal);
    }
    else if (regType == 2)  // double precision register (SPARC)
    {
        bool upper = false;
        if (injBit >= 32) // flipping higher order bits
        {
            upper = true;
            bitMask = 1 << (injBit - 32);
        }
        if (upper)
        {
            currVal = thread->readFloatReg(injR);
            thread->setFloatReg(injR, currVal ^ bitMask);
        }
        else
        {
            currVal = thread->readFloatReg(injR+1);
            thread->setFloatReg(injR+1, currVal ^ bitMask);
        }
    }
}

Injector::Injector(InjectorParams *params) : 
    SimObject(params),
    startPC(strtoul(params->startPC.c_str(), NULL, 16)),
    endPC(strtoul(params->endPC.c_str(), NULL, 16)),
    reliability(params->reliability),
    gen(rd()),
    dis(0.0, 1.0)
{
    //DPRINTF("Testing object!\n");
    printf("start pc: %ld\n", startPC);
    printf("end pc: %ld\n", endPC);
}

void Injector::PerformFI(ThreadContext* _thread, Tick _when, 
                      Tick _injTick, int injR, int injBit, int regType)
{
    if (dis(gen) > reliability) {
        FI fi(_thread, _when);
        fi.FlipBit(_when, injR, injBit, regType);
    }
}

}   // end namespace


FaultInjector::Injector* InjectorParams::create()
{
    return new FaultInjector::Injector(this);
}

