############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project ImgPrejection
set_top ImgPrejection
add_files Sourcecode/ImgPrejection.h
add_files Sourcecode/ImgPrejection.cpp
add_files -tb Sourcecode/ImgPrejection_test.cpp -cflags "-Wno-unknown-pragmas"
open_solution "solution2"
set_part {xc7z020clg400-1}
create_clock -period 10 -name default
config_dataflow -default_channel fifo -fifo_depth 2 -scalar_fifo_depth 0 -start_fifo_depth 0 -strict_mode warning
config_export -format ip_catalog -rtl verilog
source "./ImgPrejection/solution2/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -rtl verilog -format ip_catalog
