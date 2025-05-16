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
	Board(int m, int n) : M(m), N(n) {}

	// 在 (x, y) 位置放置棋子
	void place(int x, int y) { board.set(index(x, y)); }

	// 检查 (x, y) 位置是否有棋子
	bool get(int x, int y) const { return board.test(index(x, y)); }

	// 判断是否胜利（四连子）
	bool win() const
	{
		return (board & (board >> N) & (board >> (2 * N)) & (board >> (3 * N)) |
				board & (board >> 1) & (board >> 2) & (board >> 3) |
				board & (board >> (N - 1)) & (board >> (2 * (N - 1))) & (board >> (3 * (N - 1))) |
				board & (board >> (N + 1)) & (board >> (2 * (N + 1))) & (board >> (3 * (N + 1))))
			.any();
	}
};

#endif /* GAMEBOARD_HPP */
