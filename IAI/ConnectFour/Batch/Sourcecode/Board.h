#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include <bitset>

class Board
{
private:
	static const int MAX_SIZE = 144; // 最大支持 12x12 的棋盘
	std::bitset<MAX_SIZE> board;	 // 棋盘状态，每个点用 1 位表示
	int M, N;						 // 棋盘的行数和列数

	// 将二维坐标 (x, y) 转换为一维索引
	inline int index(int x, int y) const { return x * N + y; }

public:
	Board(int m, int n);
	void place(int x, int y);
	bool get(int x, int y) const;
	bool win() const;
};

#endif /* GAMEBOARD_HPP */
