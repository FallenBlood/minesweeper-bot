#ifndef BOARD_H
#define BOARD_H

#include <vector>

namespace CellType {
    const int Bomb = -3;
    const int Flag = -2;
    const int Unknown = -1;
    const int Empty = 0;
}

typedef int CellVal;

class Cell {
    public:
        // Default constructor - for a blank cell
        Cell(int row, int col, CellVal val);
        CellVal getVal() const;
        bool isBomb() const;
        bool isFlagged() const;
        bool isUnknown() const;
        void setVal(CellVal val);
        char getRepresentation() const;

    private:
        int row_;
        int col_;
        CellVal val_;
};


class Board {
    public:
        Board(int numRows, int numCols);
        void setCell(int row, int col, CellVal val);
        Cell * getCell(int row, int col) const;
        std::vector<Cell> getAdjacent(Cell * cell) const;
        void debugPopulate(int numMines);
        void evaluate();
        void dump() const;

    private:
        // Access with board_[row][col]
        std::vector< std::vector<Cell *> > board_;
        int numRows_;
        int numCols_;
};

#endif
