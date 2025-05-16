CXX = g++
CXXFLAGS = -std=c++11 -O2

all: a game generate_test battle

a: a.cpp
	$(CXX) $(CXXFLAGS) -o a a.cpp

game: game.cpp
	$(CXX) $(CXXFLAGS) -o game game.cpp

generate_test: generate_test.cpp
	$(CXX) $(CXXFLAGS) -o generate_test generate_test.cpp

battle: battle.cpp
	$(CXX) $(CXXFLAGS) -o battle battle.cpp

run: battle
	./battle

clean:
	rm -f a game generate_test battle input.txt output_a.txt output_game.txt