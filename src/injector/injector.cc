#include "debug/Inj.hh"
#include "sim/sim_exit.hh"
#include "injector/injector.hh"

#include <fstream>
#include <iostream>

using namespace X86ISA;

namespace FaultInjector
{

Injector::Injector(InjectorParams *params) : 
    SimObject(params),
    startPC(strtoul(params->startPC.c_str(), NULL, 16)),
    endPC(strtoul(params->endPC.c_str(), NULL, 16)),
    inMain(false),
    reliability(params->reliability),
    gen(rd()),
    dis(0.0, 1.0) {}

void Injector::advancePC(Addr nextPC) {
  if (nextPC == this->startPC) {
    inMain = true;
    printf("reached start pc: %ld\n", this->startPC);
  } else if (nextPC == this->endPC) {
    inMain = false;
    printf("reached end pc: %ld\n", this->endPC);
  }
}

void Injector::performFI(ThreadContext* thread,
    Addr instAddr,
    StaticInstPtr curStaticInst,
    StaticInstPtr curMacroStaticInst) {
  if (!inMain) {
    return;
  }

  //std::cout << std::hex << instAddr << std::dec << ": "
  // << curMacroStaticInst->disassemble(instAddr) << "\t\t"
  //    << curStaticInst->disassemble(instAddr) << std::endl;

  if (curStaticInst->isFloating() && curStaticInst->numDestRegs() > 0) {
    OpClass instOpClass = curStaticInst->opClass();
    if ((instOpClass >= OpClass::FloatAdd && instOpClass <= OpClass::FloatSqrt) || 
        (instOpClass >= OpClass::SimdFloatAdd && instOpClass <= OpClass::SimdFloatSqrt) ||
        (instOpClass >= OpClass::SimdFloatReduceAdd && instOpClass <= OpClass::SimdFloatReduceCmp)) {
      if (dis(gen) > reliability) {
        std::cout << "Inject!" << std::endl;
        std::cout << std::hex << instAddr << std::dec << ": "
          << curMacroStaticInst->disassemble(instAddr) << std::endl;
        // not sure if this is the register index we want yet.
        // Inject at bit 30.
        int injReg = curStaticInst->destRegIdx(0).index();
        this->flipBit(thread, injReg, 30, 1);
      }
    }
  }
}

void Injector::flipBit(ThreadContext* thread, int injR, int injBit, int regType) {
  uint64_t currVal; // = thread->readIntReg(injR);
  uint64_t bitMask = 1 << injBit;

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

} // namespace FaultInjector


FaultInjector::Injector* InjectorParams::create()
{
  return new FaultInjector::Injector(this);
}

