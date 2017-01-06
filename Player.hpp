#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <deque>
using std::cout;
using std::endl;
using std::string;
using std::mutex;
using std::lock_guard;
using std::condition_variable;
using std::unique_lock;
using std::deque;

#include "Game.hpp"
#include "Card.hpp"
#include "Rules.hpp"

class Game;

class Player {
        Game& game;
        string name;
        int turn;
        int skill;
        bool isOut;
        deque<Card> hand;
        Rules rules;
        int faceTestCardsRemaining;
        bool playedCard;
    public:
        Player(Game& theGame, string name, int turnToPlay) : game(theGame),
            turn(turnToPlay),
            skill(0),
            isOut(false),
            faceTestCardsRemaining(-1),
            playedCard(false) {
            this->name = name;
        }

        void setSkill(int skillLevel) {
            this->skill = skillLevel;
        }

        void addCardToHand(Card card) {
            hand.push_back(card);
        }

        bool isRetired(bool notifyGame = true);
        void retire();

        void playGame();
    private:
        void playCard();
        bool checkForPlayable(const Card& playedCard);
        void grabPile();
        void skillLevelWait();
        void incrementTurn();
        bool inFaceCardTest();
        int getNextPlayerTurn();
        int getNextPlayerTurn(int fromPlayer);
        int calculateRemainingTriesForTest();
        void resetTestForPlayer();
        bool checkForTestSuccess();
        void checkIfFaceCard(const Card& cardPlayed);
};

#endif /* Player_hpp */
