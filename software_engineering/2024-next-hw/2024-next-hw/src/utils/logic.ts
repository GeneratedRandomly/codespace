import { SetStateAction, useActionState, useState } from "react";
import { BOARD_LENGTH } from "../constants/constants";
import { Board } from "./types";
import { Dispatch } from "@reduxjs/toolkit";

export const getBlankBoard = (): Board => Array.from(
    { length: BOARD_LENGTH },
    () => Array<0>(BOARD_LENGTH).fill(0),
);

export const boardToString = (board: Board): string => {
    return board.map((row) => row.join("")).join("");
};

export const stringToBoard = (str: string): Board => {
    if (str.length !== BOARD_LENGTH * BOARD_LENGTH) {
        throw new Error("Invalid parameter");
    }

    const board: Board = [];
    for (let i = 0; i < BOARD_LENGTH; ++i) {
        const row: (0 | 1)[] = [];
        for (let j = 0; j < BOARD_LENGTH; ++j) {
            const val = Number(str[i * BOARD_LENGTH + j]);
            if (val !== 0 && val !== 1) {
                return getBlankBoard();
            }

            row.push(val);
        }

        board.push(row);
    }

    return board;
};

export const stepBoard = (board: Board): Board => {
    const newBoard: Board = [];
    for (let i = 0; i < BOARD_LENGTH; i++) {
        newBoard.push([]);
        for (let j = 0; j < BOARD_LENGTH; j++) {
            let surrounding = 0;
            const i_normalized = BOARD_LENGTH + i;
            const j_normalized = BOARD_LENGTH + j;
            for (let x_i = -1; x_i <= 1; x_i++) {
                for (let x_j = -1; x_j <= 1; x_j++) {
                    surrounding += board[(x_i + i_normalized) % BOARD_LENGTH][(x_j + j_normalized) % BOARD_LENGTH];
                }
            }
            if (surrounding === 3) {
                newBoard[i].push(1);
            }
            else if (surrounding === 4 && board[i][j] === 1) {
                newBoard[i].push(1);
            }
            else {
                newBoard[i].push(0);
            }
        }
    }
    return newBoard;
};

export const flipCell = (board: Board, i: number, j: number): Board => {
    const newBoard: Board = [];
    for (let a = 0; a < BOARD_LENGTH; a++) {
        newBoard.push([]);
        for (let b = 0; b < BOARD_LENGTH; b++) {
            newBoard[a].push(board[a][b]);
        }
    }
    newBoard[i][j] = board[i][j] === 0 ? 1 : 0;
    return newBoard;
};

export const badFlipCell = (board: Board, i: number, j: number): Board => {
    board[i][j] = board[i][j] === 0 ? 1 : 0;
    return board;
};
