
#include "Bot.h"
#include <iostream>

Bot::Bot(int a, int b) :
    a_(a), b_(b) {}

void Bot::doStuff(int a, int b) {
    std::cout << "Did stuff!!\n";
}
