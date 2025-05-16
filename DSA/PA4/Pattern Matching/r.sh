#!/bin/bash

# 编译数据生成器和两个待测试的程序
g++ data_generator.cpp -o data_generator
g++ 1mod.cpp -o 1mod #-fsanitize=address,undefined
# g++ fast.cpp -o fast

# 生成测试数据
./data_generator

# 测试 pm.cpp 的耗时
echo "Testing 1mod.cpp..."
time ./1mod < input.txt > output_1mod.txt

# 测试 fast.cpp 的耗时
echo "Testing fast.cpp..."
time ./fast < input.txt > output_fast.txt

# 比较输出结果是否一致
diff output_1mod.txt output_fast.txt > /dev/null
if [ $? -eq 0 ]; then
    echo "Outputs are identical."
else
    echo "Outputs differ."
fi