// The fault injector that is invoked at every execution cycle.
// Author: Jonathan Stein, Ziang Wan

#ifndef __TEST_OBJ_TEST_OBJECT_HH__
#define __TEST_OBJ_TEST_OBJECT_HH__

#include "params/Injector.hh"
#include "sim/sim_object.hh"

#include "arch/x86/regs/int.hh"
#include "arch/x86/regs/float.hh"
// #include "base/bigint.hh"
#include "base/types.hh"
#include "cpu/inst_seq.hh"
#include "cpu/static_inst.hh"

#include <random>
class ThreadContext;


namespace FaultInjector {

// convert string to correct register index
static std::map<std::string, int> x86IntRegConverter =
{
    {"rax", X86ISA::IntRegIndex::INTREG_RAX},
    {"eax", X86ISA::IntRegIndex::INTREG_EAX},
    {"ax", X86ISA::IntRegIndex::INTREG_AX},
    {"al", X86ISA::IntRegIndex::INTREG_AL},

    {"rcx", X86ISA::IntRegIndex::INTREG_RCX},
    {"ecx", X86ISA::IntRegIndex::INTREG_ECX},
    {"cx", X86ISA::IntRegIndex::INTREG_CX},
    {"cl", X86ISA::IntRegIndex::INTREG_CL},

    {"rdx", X86ISA::IntRegIndex::INTREG_RDX},
    {"edx", X86ISA::IntRegIndex::INTREG_EDX},
    {"dx", X86ISA::IntRegIndex::INTREG_DX},
    {"dl", X86ISA::IntRegIndex::INTREG_DL},

    {"rbx", X86ISA::IntRegIndex::INTREG_RBX},
    {"ebx", X86ISA::IntRegIndex::INTREG_EBX},
    {"bx", X86ISA::IntRegIndex::INTREG_BX},
    {"bl", X86ISA::IntRegIndex::INTREG_BL},

    {"rsp", X86ISA::IntRegIndex::INTREG_RSP},
    {"esp", X86ISA::IntRegIndex::INTREG_ESP},
    {"sp", X86ISA::IntRegIndex::INTREG_SP},
    {"spl", X86ISA::IntRegIndex::INTREG_SPL},
    {"ah", X86ISA::IntRegIndex::INTREG_AH},

    {"rbp", X86ISA::IntRegIndex::INTREG_RBP},
    {"ebp", X86ISA::IntRegIndex::INTREG_EBP},
    {"bp", X86ISA::IntRegIndex::INTREG_BP},
    {"bpl", X86ISA::IntRegIndex::INTREG_BPL},
    {"ch", X86ISA::IntRegIndex::INTREG_CH},

    {"rsi", X86ISA::IntRegIndex::INTREG_RSI},
    {"esi", X86ISA::IntRegIndex::INTREG_ESI},
    {"si", X86ISA::IntRegIndex::INTREG_SI},
    {"sil", X86ISA::IntRegIndex::INTREG_SIL},
    {"dh", X86ISA::IntRegIndex::INTREG_DH},

    {"rdi", X86ISA::IntRegIndex::INTREG_RDI},
    {"edi", X86ISA::IntRegIndex::INTREG_EDI},
    {"di", X86ISA::IntRegIndex::INTREG_DI},
    {"dil", X86ISA::IntRegIndex::INTREG_DIL},
    {"bh", X86ISA::IntRegIndex::INTREG_BH},

    {"r8", X86ISA::IntRegIndex::INTREG_R8},
    {"r9", X86ISA::IntRegIndex::INTREG_R9},
    {"r10", X86ISA::IntRegIndex::INTREG_R10},
    {"r11", X86ISA::IntRegIndex::INTREG_R11},
    {"r12", X86ISA::IntRegIndex::INTREG_R12},
    {"r13", X86ISA::IntRegIndex::INTREG_R13},
    {"r14", X86ISA::IntRegIndex::INTREG_R14},
    {"r15", X86ISA::IntRegIndex::INTREG_R15}
};

// each FP register may need different bit flips depending on which
// data we are accessing. An example is "movapd %xmm1,%xmm2" 
// that moves xmm1_high->xmm2_high, and xmm1_low->xmm2_low
static std::map<std::string, int> x86FloatRegConverter =
{
    {"fpr0", X86ISA::FloatRegIndex::FLOATREG_FPR0},
    {"fpr1", X86ISA::FloatRegIndex::FLOATREG_FPR1},
    {"fpr2", X86ISA::FloatRegIndex::FLOATREG_FPR2},
    {"fpr3", X86ISA::FloatRegIndex::FLOATREG_FPR3},
    {"fpr4", X86ISA::FloatRegIndex::FLOATREG_FPR4},
    {"fpr5", X86ISA::FloatRegIndex::FLOATREG_FPR5},
    {"fpr6", X86ISA::FloatRegIndex::FLOATREG_FPR7},
    {"fpr7", X86ISA::FloatRegIndex::FLOATREG_FPR7},

    {"xmm0", X86ISA::FloatRegIndex::FLOATREG_XMM0_LOW},
    {"xmm0_low", X86ISA::FloatRegIndex::FLOATREG_XMM0_LOW},
    {"xmm0_high", X86ISA::FloatRegIndex::FLOATREG_XMM0_HIGH},

    {"xmm1", X86ISA::FloatRegIndex::FLOATREG_XMM1_LOW},
    {"xmm1_low", X86ISA::FloatRegIndex::FLOATREG_XMM1_LOW},
    {"xmm1_high", X86ISA::FloatRegIndex::FLOATREG_XMM1_HIGH},

    {"xmm2", X86ISA::FloatRegIndex::FLOATREG_XMM2_LOW},
    {"xmm2_low", X86ISA::FloatRegIndex::FLOATREG_XMM2_LOW},
    {"xmm2_high", X86ISA::FloatRegIndex::FLOATREG_XMM2_HIGH},

    {"xmm3", X86ISA::FloatRegIndex::FLOATREG_XMM3_LOW},
    {"xmm3_low", X86ISA::FloatRegIndex::FLOATREG_XMM3_LOW},
    {"xmm3_high", X86ISA::FloatRegIndex::FLOATREG_XMM3_HIGH},

    {"xmm4", X86ISA::FloatRegIndex::FLOATREG_XMM4_LOW},
    {"xmm4_low", X86ISA::FloatRegIndex::FLOATREG_XMM4_LOW},
    {"xmm4_high", X86ISA::FloatRegIndex::FLOATREG_XMM4_HIGH},

    {"xmm5", X86ISA::FloatRegIndex::FLOATREG_XMM5_LOW},
    {"xmm5_low", X86ISA::FloatRegIndex::FLOATREG_XMM5_LOW},
    {"xmm5_high", X86ISA::FloatRegIndex::FLOATREG_XMM5_HIGH},

    {"xmm6", X86ISA::FloatRegIndex::FLOATREG_XMM6_LOW},
    {"xmm6_low", X86ISA::FloatRegIndex::FLOATREG_XMM6_LOW},
    {"xmm6_high", X86ISA::FloatRegIndex::FLOATREG_XMM6_HIGH},

    {"xmm7", X86ISA::FloatRegIndex::FLOATREG_XMM7_LOW},
    {"xmm7_low", X86ISA::FloatRegIndex::FLOATREG_XMM7_LOW},
    {"xmm7_high", X86ISA::FloatRegIndex::FLOATREG_XMM7_HIGH},

    {"xmm8", X86ISA::FloatRegIndex::FLOATREG_XMM8_LOW},
    {"xmm8_low", X86ISA::FloatRegIndex::FLOATREG_XMM8_LOW},
    {"xmm8_high", X86ISA::FloatRegIndex::FLOATREG_XMM8_HIGH},

    {"xmm9", X86ISA::FloatRegIndex::FLOATREG_XMM9_LOW},
    {"xmm9_low", X86ISA::FloatRegIndex::FLOATREG_XMM9_LOW},
    {"xmm9_high", X86ISA::FloatRegIndex::FLOATREG_XMM9_HIGH},

    {"xmm10", X86ISA::FloatRegIndex::FLOATREG_XMM10_LOW},
    {"xmm10_low", X86ISA::FloatRegIndex::FLOATREG_XMM10_LOW},
    {"xmm10_high", X86ISA::FloatRegIndex::FLOATREG_XMM10_HIGH},

    {"xmm11", X86ISA::FloatRegIndex::FLOATREG_XMM11_LOW},
    {"xmm11_low", X86ISA::FloatRegIndex::FLOATREG_XMM11_LOW},
    {"xmm11_high", X86ISA::FloatRegIndex::FLOATREG_XMM11_HIGH},
    
    {"xmm12", X86ISA::FloatRegIndex::FLOATREG_XMM12_LOW},
    {"xmm12_low", X86ISA::FloatRegIndex::FLOATREG_XMM12_LOW},
    {"xmm12_high", X86ISA::FloatRegIndex::FLOATREG_XMM12_HIGH},

    {"xmm13", X86ISA::FloatRegIndex::FLOATREG_XMM13_LOW},
    {"xmm13_low", X86ISA::FloatRegIndex::FLOATREG_XMM13_LOW},
    {"xmm13_high", X86ISA::FloatRegIndex::FLOATREG_XMM13_HIGH},

    {"xmm14", X86ISA::FloatRegIndex::FLOATREG_XMM14_LOW},
    {"xmm14_low", X86ISA::FloatRegIndex::FLOATREG_XMM14_LOW},
    {"xmm14_high", X86ISA::FloatRegIndex::FLOATREG_XMM14_HIGH},

    {"xmm15", X86ISA::FloatRegIndex::FLOATREG_XMM15_LOW},
    {"xmm15_low", X86ISA::FloatRegIndex::FLOATREG_XMM15_LOW},
    {"xmm15_high", X86ISA::FloatRegIndex::FLOATREG_XMM15_HIGH}
};

static std::vector<std::map<std::string, int>> x86Table = 
{
    x86IntRegConverter,
    x86FloatRegConverter   
};

static std::map<std::string, std::vector<std::map<std::string, int>>> archMap = 
{
    {"x86", x86Table},
};


class Injector : public SimObject {    
  public:
    Injector(InjectorParams *p);

    void advancePC(Addr nextPC);
    void performFI(ThreadContext* thread,
        Addr instAddr,
        StaticInstPtr curStaticInst,
        StaticInstPtr curMacroStaticInst);

  private:
    Addr startPC;
    Addr endPC;
    bool inMain;
    bool verbose;

    // Specify instruction-level reliability.
    double fpuReliability;
    double aluReliability;
    
    // A random number generator in the range of [0.0, 1.0].
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> dis;

    // For instruction hardening overhead measurement.
    int runCount;
    int instCountThisRun;
    int instCountTotal;

  private:
    // Helper functions
    void flipBit(ThreadContext* thread, int injR, int injBit, int regType);

    void printInst(Addr instAddr,
        StaticInstPtr curStaticInst,
        StaticInstPtr curMacroStaticInst);
};

} // namespace FaultInjector


#endif
