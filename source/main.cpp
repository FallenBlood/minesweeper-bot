
#include "Board.h"
#include <iostream>

int main(int argc, char ** argv) {

    Board * b = new Board(30, 30);
    b->setCell(10, 20, 5);
    b->setCell(20, 10, 1);

    b->dump();

    std::cout << std::endl;

    return 0;
}
