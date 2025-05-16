#!/bin/bash

# 定义哈希策略和碰撞处理策略
hashing_strategies=("bad")
collision_strategies=("overflow")

# 生成测试数据文件
test_files=(data.in)

for hashing in "${hashing_strategies[@]}"; do
    for collision in "${collision_strategies[@]}"; do
        for test_file in "${test_files[@]}"; do
            output_file="result_${hashing}_${collision}_${test_file}.txt"
            echo "Running test with $hashing hashing and $collision collision on $test_file"
            /usr/bin/time -f "%e" ./main $hashing $collision < $test_file > $output_file
        done
    done
done