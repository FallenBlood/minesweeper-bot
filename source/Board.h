#ifndef BOARD_H
#define BOARD_H

#include <vector>

namespace CellType {
    const int UNCLICKED = -1;
    const int EMPTY = 0;
}

typedef int CellVal;

class Cell {
    public:
        // Default constructor - for a blank cell
        Cell(int row, int col, CellVal val);
        CellVal getVal() const;
        void setVal(CellVal val);

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
        void dump() const;

    private:
        // Access with board_[row][col]
        std::vector< std::vector<Cell *> > board_;
        int numRows_;
        int numCols_;
};

#endif
