#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#endif

#include "Card.hpp"
#include "Game.hpp"

void playGame() {
    // Our start date/time
    std::time_t st = std::time(nullptr);
    std::tm stm = *std::localtime(&st);

    Game game(2);
    game.start();

    // Our finish date/time
    std::time_t ft = std::time(nullptr);
    std::tm ftm = *std::localtime(&ft);

    cout << "Game started:\t" << std::put_time(&stm, "%m/%d,%H:%M:%S") << endl;
    cout << "Game finished:\t" << std::put_time(&ftm, "%m/%d,%H:%M:%S") << endl;

    return;
}

int main(int argc, const char* argv[]) {
    cout << "Spawning game thread..." << endl;
    auto gameThread = thread(playGame);
    gameThread.join();

    std::cout << "Hello, World!\n";
    return 0;
}
