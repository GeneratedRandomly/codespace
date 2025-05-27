root_dir="/root/codespace/IAI/ConnectFour/Batch/"  # your path here

for dir in "${root_dir}Sourcecode"/
do
    cd "$dir" || continue
    pwd
    g++ -Wall -std=c++11 -O2 -fpic -shared *.cpp -o "${root_dir}/so/$(basename "$dir").so"
    cd -
done