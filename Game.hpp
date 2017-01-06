#ifndef Game_hpp
#define Game_hpp

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <stack>
#include <iterator>
using std::endl;
using std::cout;
using std::vector;
using std::string;
using std::random_device;
using std::mt19937;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::stack;

#include "Player.hpp"
#include "Logger.hpp"

class Barrier {
        mutex barrierMutex;
        condition_variable barrierCondition;
        size_t barrierThreshold;
        size_t barrierCount;
        size_t generation;
    public:
        explicit Barrier(size_t waitersCount) : barrierThreshold(waitersCount), barrierCount(waitersCount), generation(0) {
        }

        void Wait() {
            auto currentGeneration = generation;
            unique_lock<mutex> lock{barrierMutex};

            if (!--barrierCount) {
                generation++;
                barrierCount = barrierThreshold;
                barrierCondition.notify_all();
            } else {
                barrierCondition.wait(lock, [this, currentGeneration] {
                    return currentGeneration != generation;
                });
            }
        }
};

class Game {
        friend class Player;

        vector<Player> players;
        vector<thread> playerThreads;

        Barrier turnBarrier;
        mutex turnMutex;
        condition_variable turnCondition;
        string currentPlayer;
        int currentTurn;
        bool gameOver;

        vector<Card> deck;

        mutex discardPileMutex;
        stack<Card> discardPile;
        // Tells the next player that it has to play a face card to counter
        // the face card that was played by the previous player
        bool inFaceCardTest;
        // The player that played the face card and put it into test
        int faceCardTurnChallenger;
        // This is the face card that was played that the next
        // player has to deal with
        Card faceCardForTest;
        // Did the challenger fail the test? If so, the
        // player that put the game into test will get the
        // pile
        bool testFailed;

        // All the output of the game
        Logger gameLog;
    public:
        Game(int playerCount);
        ~Game();
        void start();
    private:
        void createGameDeck(int playerCount);
        void createPlayers(int playerCount);
        int getRandomSkillLevel();
        void dealCards();

        // From player
        void playerOut(int playerNum);
};


#endif /* Game_hpp */
