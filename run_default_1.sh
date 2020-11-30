#!/bin/env bash
BENCHMARK_PATH="/home/user/Desktop/CS523/gem5/benchmark/matvec/o1out"
FUNCTION_NAME="matvec"
START_PC="400560"
END_PC="4005b2"

ALU_RELIABILITY=0.98796697
FPU_RELIABILITY=1.0

TOTAL_RUNS=5000
LOG=log_1
ERRLOG=err_log_1

CORRECT="$(build/X86/gem5.opt configs/rely_backend/se.py \
  --cmd=$BENCHMARK_PATH \
  --cpu-type=AtomicSimpleCPU \
  --fi-fpu-reliability=1.0 \
  --fi-alu-reliability=1.0 \
  --fi-start-pc=$START_PC \
  --fi-end-pc=$END_PC \
  --fi-function_name=$FUNCTION_NAME \
  | grep Result)"
echo "The correct $CORRECT"

NUM_CORRECT=0
NUM_ABORT=0
for i in $(seq 1 $TOTAL_RUNS)
do
  echo "************* Run $i **********"
  build/X86/gem5.opt configs/rely_backend/se.py \
    --cmd=$BENCHMARK_PATH \
    --cpu-type=AtomicSimpleCPU \
    --fi-fpu-reliability=$FPU_RELIABILITY \
    --fi-alu-reliability=$ALU_RELIABILITY \
    --fi-function_name=$FUNCTION_NAME \
    --fi-start-pc=$START_PC \
    --fi-end-pc=$END_PC > $LOG 2> $ERRLOG

  if grep "$CORRECT" $LOG
  then
    let "NUM_CORRECT+=1"
  elif grep "aborted" $ERRLOG
  then
    let "NUM_ABORT+=1"
  fi
done

echo "Correct: $NUM_CORRECT out of $TOTAL_RUNS"
echo "Incorrect: $(($TOTAL_RUNS-$NUM_CORRECT-$NUM_ABORT)) out of $TOTAL_RUNS"
echo "Aborted: $NUM_ABORT out of $TOTAL_RUNS"
