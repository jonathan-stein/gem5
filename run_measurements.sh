#!/bin/env bash

if [ "$#" != 5 ]; then
    echo "usage ./test_reliability.sh <benchmark_path> <function_name> <alu_reliability> <fpu_reliability> <total_iterations>"
    exit
fi

BENCHMARK_PATH=$1
FUNCTION_NAME=$2
ALU_RELIABILITY=$3
FPU_RELIABILITY=$4
TOTAL_RUNS=$5

CORRECT="$(build/X86/gem5.opt configs/rely_backend/se.py --cmd=$BENCHMARK_PATH --cpu-type=AtomicSimpleCPU --fi-fpu-reliability=1.0 --fi-alu-reliability=1.0 --fi-function_name=$FUNCTION_NAME | grep Result)"


NUM_CORRECT=0
NUM_ABORT=0
for i in $(seq 1 $TOTAL_RUNS)
do
   build/X86/gem5.opt configs/rely_backend/se.py --cmd=$BENCHMARK_PATH --cpu-type=AtomicSimpleCPU --fi-fpu-reliability=$FPU_RELIABILITY --fi-alu-reliability=$ALU_RELIABILITY --fi-function_name=$FUNCTION_NAME > log 2> err_log
   if grep "$CORRECT" log
   then
        let "NUM_CORRECT+=1"
   elif grep "aborted" err_log
   then
        let "NUM_ABORT+=1"
   fi
done

echo "Num correct: $NUM_CORRECT out of $TOTAL_RUNS"
echo "Num incorrect: $(($TOTAL_RUNS-$NUM_CORRECT-$NUM_ABORT)) out of $TOTAL_RUNS"
echo "Aborted: $NUM_ABORT out of $TOTAL_RUNS"

