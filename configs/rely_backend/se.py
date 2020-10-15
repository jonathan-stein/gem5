# The Backend Simulator for Rely.
# It supports instruction-level error injection.
# Author: Ziang Wan, Jonathan Stein

from __future__ import print_function
from __future__ import absolute_import

import optparse
import sys
import os
import subprocess

import m5
from m5.defines import buildEnv
from m5.objects import *
from m5.params import NULL
from m5.util import addToPath, fatal, warn

addToPath('../')

from common import Options
from common import Simulation
from common import CacheConfig
from common import CpuConfig
from common import ObjectList
from common import MemConfig
from common.FileSystemConfig import config_filesystem
from common.Caches import *
from common.cpu2000 import *


def get_workload(options):
    """Interprets provided options and returns the process to execute"""
    process = Process(pid=100)
    process.executable = options.cmd
    process.cwd = os.getcwd()
    if options.options != "":
        process.cmd = [options.cmd] + options.options.split()
    else:
        process.cmd = [options.cmd]
    if options.env:
        with open(options.env, 'r') as f:
            process.env = [line.rstrip() for line in f]
    if options.input != "":
        process.input = options.input
    if options.output != "":
        process.output = options.output
    if options.errout != "":
        process.errout = options.errout
    return process


def get_start_end_pc(cmd, function_name):
    assembly = str(subprocess.check_output(["objdump", "-d", cmd])).split("\\n")
    start_pc = None
    end_pc = None
    in_func = False
    for line in assembly:
        if in_func and start_pc is None:
            start_pc = line.split()[0][:-1]
        if function_name in line:
            in_func = True
        if in_func and "ret" in line:
            end_pc = line.split()[0][:-1]
            in_func = False
    return start_pc, end_pc


# Argument Parsing.
parser = optparse.OptionParser()
Options.addCommonOptions(parser)
Options.addSEOptions(parser)
Options.addFIOptions(parser)

(options, args) = parser.parse_args()
if args:
    print("Error: script doesn't take any positional arguments")
    sys.exit(1)


# There is a benchmark option in the original se.py.
# Right now, we use the passed in --cmd=binary command only.
if options.cmd:
    workload = get_workload(options)
else:
    print("No workload specified. Exiting!\n", file=sys.stderr)
    sys.exit(1)


# Define the CPU. We are only using SimpleCPU. smt is not supported or needed.
# Also, set the system to have only 1 CPU.
options.cpu_type = "AtomicSimpleCPU"
options.num_cpus = 1

# The numThreads will always be 1.
# Additional handling for options.smt is needdd.
(CPUClass, test_mem_mode, FutureClass) = Simulation.setCPUClass(options)
CPUClass.numThreads = 1

system = System(
    cpu=[CPUClass(cpu_id=0)],
    mem_mode=test_mem_mode,
    mem_ranges=[AddrRange(options.mem_size)],
    cache_line_size=options.cacheline_size,
    workload = NULL,
)


# Create a top-level voltage domain
system.voltage_domain = VoltageDomain(voltage=options.sys_voltage)
# Create a source clock for the system and set the clock period
system.clk_domain = SrcClockDomain(
    clock=options.sys_clock,
    voltage_domain=system.voltage_domain,
)
# Create a CPU voltage domain
system.cpu_voltage_domain = VoltageDomain()
# Create a separate clock domain for the CPUs
system.cpu_clk_domain = SrcClockDomain(
    clock=options.cpu_clock,
    voltage_domain=system.cpu_voltage_domain,
)
# All cpus run at a common frequency.
for cpu in system.cpu:
    cpu.clk_domain = system.cpu_clk_domain

# Disable elastic tracing.
# There is some handling for KVM CPUs.
# there is some handling for options.simpoint_profile


# Setup the workload.
system.cpu[0].workload = workload
system.cpu[0].createThreads()

# Get the start and end Program Counter for the main function, which
# defines the range we will inject error.
start_pc, end_pc = get_start_end_pc(options.cmd, "<" + options.function_name + ">:")
system.cpu[0].injector = Injector(
    startPC=start_pc, endPC=end_pc, reliability=options.fi_reliability)


# Use the classic cache simulator.
MemClass = Simulation.setMemClass(options)
system.membus = SystemXBar()
system.system_port = system.membus.slave
CacheConfig.config_cache(options, system)
MemConfig.config_mem(options, system)
config_filesystem(system, options)


# Run the simulation.
root = Root(full_system=False, system=system)
Simulation.run(options, root, system, FutureClass)
