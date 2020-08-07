# ==============================================================
# File generated on Tue Aug 04 15:12:15 +0800 2020
# Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
# SW Build 2405991 on Thu Dec  6 23:38:27 MST 2018
# IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
# Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
# ==============================================================
add_files -tb ../../Sourcecode/ImgPrejection_test.cpp -cflags { -Wno-unknown-pragmas}
add_files Sourcecode/ImgPrejection.h
add_files Sourcecode/ImgPrejection.cpp
set_part xc7z020clg400-1
create_clock -name default -period 10
config_dataflow -default_channel=fifo
config_dataflow -fifo_depth=2
config_dataflow -scalar_fifo_depth=0
config_dataflow -start_fifo_depth=0
config_dataflow -strict_mode=warning
set_directive_pipeline det/find_line_loop 
set_directive_pipeline det/chang_line_loop 
set_directive_dataflow ImgPrejection 
set_directive_pipeline ImgPrejection/ImgProjection_loop2 
set_directive_pipeline det/clear_num_inLoop 
set_directive_pipeline GetMatrix/GetMatrix_loop5 
set_directive_pipeline GetMatrix/GetMatrix_loop3 
set_directive_pipeline GetMatrix/GetMatrix_label8 
set_directive_pipeline GetMatrix/GetMatrix_loop2 
set_directive_pipeline ImgPrejection/ImgProjection_loop1 
set_directive_pipeline ImgPrejection/ImgPrejection_label7 
