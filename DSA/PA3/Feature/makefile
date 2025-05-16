# 编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++14 -O2

# 目标文件
TARGETS = correct feature battle

# 源文件
CORRECT_SRC = correct_lib.cpp correct.cpp
FEATURE_SRC = feature_lib.cpp feature.cpp
BATTLE_SRC = battle.cpp

# 生成目标文件
all: $(TARGETS)

correct: $(CORRECT_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

feature: $(FEATURE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

battle: $(BATTLE_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 清理生成的文件
clean:
	rm -f $(TARGETS) *.o *.txt

.PHONY: all clean