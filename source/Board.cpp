#include <vector>
#include <iostream>
#include "Board.h"

Cell::Cell(int row, int col, CellVal val) :
    row_(row), col_(col), val_(val) {}

CellVal Cell::getVal() const {
    return val_;
}

void Cell::setVal(CellVal val) {
    val_ = val;
}


Board::Board(int numRows, int numCols) {
    numRows_ = numRows;
    numCols_ = numCols;

    // Create numRows empty column vectors
    board_.resize(numRows);
    Cell * tmp;
    for (int row=0; row<numRows; row++) {
        for (int col=0; col<numCols; col++) {
            tmp = new Cell(row, col, CellType::UNCLICKED);
            board_[row].push_back(tmp);
        }
    }
}

void Board::setCell(int row, int col, CellVal val) {
    board_[row][col]->setVal(val);
}

Cell * Board::getCell(int row, int col) const {
    return board_[row][col];
}


void Board::dump() const {
    Cell * tmp;
    for (int col=0; col<numCols_; col++) {
        for (int row=0; row<numRows_; row++) {
            tmp = getCell(row, col);
            std::cout << tmp->getVal() << " ";
        }
        std::cout << std::endl;
    }
}
