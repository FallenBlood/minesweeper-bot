#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Board.h"


Cell::Cell(int row, int col, CellVal val) :
    row_(row), col_(col), val_(val) {}

CellVal Cell::getVal() const {
    return val_;
}

void Cell::setVal(CellVal val) {
    val_ = val;
}

bool Cell::isBomb() const {
    return val_ == CellType::Bomb;
}

bool isFlagged() const {
    return val_ == CellType::Flag;
}

bool isUnknown() const {
    return val_ == CellType::Unknown;
}

char Cell::getRepresentation() const {
    switch (val_) {
        case CellType::Bomb:
            return 'B';
        case CellType::Unknown:
            return '?';
        case CellType::Empty:
            return ' ';
        default:
            return '0' + val_;
    }
}


Board::Board(int numRows, int numCols) {
    numRows_ = numRows;
    numCols_ = numCols;

    // Create numRows empty column vectors
    board_.resize(numRows);
    Cell * tmp;
    for (int row=0; row<numRows; row++) {
        for (int col=0; col<numCols; col++) {
            tmp = new Cell(row, col, CellType::Unknown);
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

std::vector<Cell> getAdjacent(Cell * cell) const {
    std::vector<Cell> adjacent;
    // la la la
}

void Board::evaluate() {
    Cell * tmp;
    for (int col=0; i<numCols_; col++) {
        for (int row=0; i<numRows_; row++) {
            
        }
    }
}

void Board::dump() const {
    Cell * tmp;
    for (int col=0; col<numCols_; col++) {
        for (int row=0; row<numRows_; row++) {
            tmp = getCell(row, col);
            std::cout << tmp->getRepresentation() << " ";
        }
        std::cout << std::endl;
    }
}
