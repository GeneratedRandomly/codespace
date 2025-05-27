#include "Board.h"

Board::Board(int m, int n) : M(m), N(n) {}

void Board::place(int x, int y)
{
    board.set(index(x, y));
}

bool Board::get(int x, int y) const
{
    return board.test(index(x, y));
}

bool Board::win() const
{
    return ((board & (board >> N) & (board >> (2 * N)) & (board >> (3 * N))) |
            (board & (board >> 1) & (board >> 2) & (board >> 3)) |
            (board & (board >> (N - 1)) & (board >> (2 * (N - 1))) & (board >> (3 * (N - 1)))) |
            (board & (board >> (N + 1)) & (board >> (2 * (N + 1))) & (board >> (3 * (N + 1)))))
        .any();
}