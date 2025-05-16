import { BOARD_LENGTH } from "../constants/constants";
import { Board } from "../utils/types";
import Square from "./Square";

interface BoardUIProps {
    board: Board;
    flip: (i: number, j: number) => void;
}

const BoardUI = (props: BoardUIProps) => {
    const rowList: any[] = [];

    for (let i = 0; i < BOARD_LENGTH; ++i) {
        const cellList: any[] = [];

        for (let j = 0; j < BOARD_LENGTH; ++j) {
            cellList.push(
                <div onClick={() => props.flip(i, j)} key={j}>
                    <Square
                        color={props.board[i][j] === 1 ? "red" : "white"}
                    />
                </div>
            );
        }

        rowList.push(
            <div key={i} style={{ display: "flex", flexDirection: "row" }}>
                {cellList}
            </div>
        );
    }

    return (
        <div style={{ display: "flex", flexDirection: "column" }}>
            {rowList}
        </div>
    );
};

export default BoardUI;
