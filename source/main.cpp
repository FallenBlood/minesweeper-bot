
#include "Board.h"
#include <iostream>

int main(int argc, char ** argv) {

    Board * b = new Board(30, 30);
    b->debugPopulate(30);
    b->debugEvaluate();
    b->dump();

    std::cout << std::endl;

    return 0;
}
