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
    verbose(params->verbose),
    fpuReliability(params->fpuReliability),
    aluReliability(params->aluReliability),
    gen(rd()),
    dis(0.0, 1.0) {}

void Injector::advancePC(Addr nextPC) {
  if (nextPC == this->startPC) {
    if (!inMain && verbose) {
      std::cout << "[INFO] Reach start PC " << std::hex
        << this->startPC << std::dec << std::endl;
    }

    inMain = true;
  } else if (nextPC == this->endPC) {
    if (inMain && verbose) {
      std::cout << "[INFO] Reach end PC " << std::hex
        << this->endPC << std::dec << std::endl;
    }

    inMain = false;
  }
}

void Injector::performFI(ThreadContext* thread,
    Addr instAddr,
    StaticInstPtr curStaticInst,
    StaticInstPtr curMacroStaticInst) {
  if (!inMain) {
    return;
  }

  if (verbose) {
    std::cout << "[EXEC] ";
    this->printInst(instAddr, curStaticInst, curMacroStaticInst);
  }

  if (curStaticInst->isFloating() && curStaticInst->numDestRegs() > 0) {
    OpClass instOpClass = curStaticInst->opClass();
    if ((instOpClass >= OpClass::FloatAdd && instOpClass <= OpClass::FloatSqrt) || 
        (instOpClass >= OpClass::SimdFloatAdd && instOpClass <= OpClass::SimdFloatSqrt) ||
        (instOpClass >= OpClass::SimdFloatReduceAdd && instOpClass <= OpClass::SimdFloatReduceCmp)) {
      if (dis(gen) > fpuReliability) {
        if (verbose) {
          std::cout << "[INJE]" << std::endl;
        }

        int injReg = curStaticInst->destRegIdx(0).index();
        this->flipBit(thread, injReg, 30, 1);
      }
    }
  } else if (curStaticInst->isInteger() && curStaticInst->numDestRegs() > 0) {
    OpClass instOpClass = curStaticInst->opClass();
    if (instOpClass == OpClass::IntAlu || instOpClass == OpClass::SimdAlu || instOpClass == OpClass::SimdPredAlu || instOpClass == OpClass::SimdReduceAlu) {
      if (dis(gen) > aluReliability) {
        if (verbose) {
          std::cout << "[INJE]" << std::endl;
        }

        int injReg = curStaticInst->destRegIdx(0).index();
        this->flipBit(thread, injReg, 30, 0);
      }
    }
  }
}

/*
 * Helper Functions
 */

// TODO: We may need a better error injection function.
void Injector::flipBit(ThreadContext* thread, int injR, int injBit, int regType) {
  uint64_t currVal;
  uint64_t bitMask = 1 << injBit;

  if (regType == 0) {
    // Integer registers.
    currVal = thread->readIntReg(injR); 
    //thread->setIntReg(injR, currVal ^ bitMask);
    thread->setIntReg(injR, ~currVal);
  } else if (regType == 1) {
    // Floating point register.
    currVal = thread->readFloatReg(injR);
    // thread->setFloatReg(injR, currVal ^ bitMask);
    thread->setFloatReg(injR, ~currVal);
  } else if (regType == 2) {
    // Double precision registers (SPARC).
    bool upper = false;

    // flipping higher order bits
    if (injBit >= 32) { 
      upper = true;
      bitMask = 1 << (injBit - 32);
    }

    if (upper) {
      currVal = thread->readFloatReg(injR);
      thread->setFloatReg(injR, currVal ^ bitMask);
    } else {
      currVal = thread->readFloatReg(injR+1);
      thread->setFloatReg(injR+1, currVal ^ bitMask);
    }
  }
}

void Injector::printInst(Addr instAddr,
    StaticInstPtr curStaticInst,
    StaticInstPtr curMacroStaticInst) {
  if (curMacroStaticInst) {
    std::cout << std::hex << instAddr << std::dec << ": Macro ["
      << curMacroStaticInst->disassemble(instAddr) << "] Micro ["
      << curStaticInst->disassemble(instAddr) << "]" << std::endl;
  } else {
    std::cout << std::hex << instAddr << std::dec
      << ": Macro [None] Micro ["
      << curStaticInst->disassemble(instAddr) << "]" << std::endl;
  }
}

} // namespace FaultInjector


FaultInjector::Injector* InjectorParams::create()
{
  return new FaultInjector::Injector(this);
}

