#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Board.h"


Cell::Cell(int row, int col, CellVal val) :
    row_(row), col_(col), val_(val) {}

int Cell::getRow() const {
    return row_;
}

int Cell::getCol() const {
    return col_;
}

CellVal Cell::getVal() const {
    return val_;
}

void Cell::setVal(CellVal val) {
    val_ = val;
}

bool Cell::isBomb() const {
    return val_ == CellType::Bomb;
}

bool Cell::isFlagged() const {
    return val_ == CellType::Flag;
}

bool Cell::isUnknown() const {
    return val_ == CellType::Unknown;
}

char Cell::getRepresentation() const {
    switch (val_) {
        case CellType::Flag:
            return 'F';
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

bool Board::isValidCell(int r, int c) const {
    return r > 0 && r < numRows_ && c > 0 && c < numCols_;
}

std::vector<Cell*> Board::getAdjacent(Cell * cell) const {
    std::vector<Cell*> adjacent;
    int r = cell->getRow();
    int c = cell->getCol();
    for (int i=-1; i<2; i++) {
        for (int j=-1; j<2; j++) {
            if (i == 0 && j == 0) {
                // don't add current one to adjacent list
                continue;
            }
            if (isValidCell(r+i, c+j)) {
                adjacent.push_back(getCell(r+i, c+j));
            }
        }
    }
    return adjacent;
}

void Board::evaluate() {
    Cell * tmp;
    std::vector<Cell *> adjacent;
    for (int col=0; col<numCols_; col++) {
        for (int row=0; row<numRows_; row++) {
           tmp = getCell(row, col); 
           adjacent = getAdjacent(tmp);
           tmp->setVal(adjacent.size());
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
