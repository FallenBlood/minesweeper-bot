#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "Board.h"


Cell::Cell(int row, int col, int val) :
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

void Cell::setVal(int val) {
    val_ = val;
}

void Cell::flag() {
    val_ = CellType::Flag;
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

bool Cell::isNumeric() const {
    // Include 0? Hey, it's a number 
    return val_ >= 0;
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

void Board::setCell(int row, int col, int val) {
    board_[row][col]->setVal(val);
}

Cell * Board::getCell(int row, int col) const {
    return board_[row][col];
}

bool Board::isValidCell(int r, int c) const {
    return r >= 0 && r < numRows_ && c >= 0 && c < numCols_;
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
    std::vector<Cell *> unknown;
    std::vector<Cell *> flagged;
    for (int col=0; col<numCols_; col++) {
        for (int row=0; row<numRows_; row++) {
            unknown.clear();
            flagged.clear();
            tmp = getCell(row, col); 
            if (tmp->isUnknown() || tmp->isFlagged()) {
                continue;
            }
            adjacent = getAdjacent(tmp);
            for (size_t i=0; i<adjacent.size(); i++) {
                if (adjacent[i]->isUnknown()) {
                     unknown.push_back(adjacent[i]);
                } else if (adjacent[i]->isFlagged()) {
                     flagged.push_back(adjacent[i]);
                }
            }
            if (unknown.size() == tmp->getVal() - flagged.size()) {
                // Flag all adjacent squares
                for (size_t i=0; i<unknown.size(); i++) {
                    unknown[i]->flag();
                }
            } else if (unknown.size() > tmp->getVal()) {
                for (size_t i=0; i<unknown.size(); i++) {
                    std::cout << "Click cell at (" << tmp->getRow() << ", " << tmp->getCol() << ")" << std::endl;
                }
            }
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
