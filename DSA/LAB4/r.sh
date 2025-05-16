#!/bin/bash

# 编译排序程序
g++ sort.cpp -o sort

# # 生成测试数据
# g++ dg.cpp -o dg
# ./dg

time ./sort < data.txt
