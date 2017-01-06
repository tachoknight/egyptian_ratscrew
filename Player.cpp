#include "Game.hpp"
#include "Player.hpp"
#include "Card.hpp"
#ifndef _WIN32
#include <unistd.h>
#else
#include <Windows.h>
#endif

void Player::playGame() {
    game.gameLog << "Hi! I'm " << this->name << " with a skill level of " << this->skill << " and I am playing turn " << this->turn << lendl;
    game.gameLog << name << " is going to start playing..." << lendl;

    bool alreadyIncrementedTurn = false;

    if (this->turn == 1) {
        // First player starts the game
        playCard();
        // And indicate it's the next player's turn
        incrementTurn();
        alreadyIncrementedTurn = true;
    }

    // Our main game loop which we exit only when the game is over
    while (1) {
        // Is the game over?
        if (game.gameOver) {
            game.gameLog << this->name << " has been told the game is over." << lendl;

            // If the game is over and we're not retired, we won so let's grab the rest
            // of the pile so the numbers add up
            if (isRetired() == false) {
                grabPile();
                game.gameLog << "HEY! " << this->name << " won!!!!" << lendl;
            }

            break;
        }

        // Is it our turn to play a card?
        if (game.currentTurn == this->turn && isRetired() == false) {
            if (game.inFaceCardTest && game.faceCardTurnChallenger == this->turn) {
                if (checkForTestSuccess()) {
                    grabPile();
                    // And reset the test
                    resetTestForPlayer();
                }
            }

            playCard();
        }

        //
        // This is our barrier that all the players wait on
        //
        game.turnBarrier.Wait();

        if (isRetired()) {
            continue;
        }
       
        if (game.discardPile.size() == 0) {
            // The last player must have run out of cards so we move on to the next player
            // to play a card
            continue;
        }

        // What's the new card played?
        Card topCard = game.discardPile.top();
        game.gameLog << name << " sees the " << topCard.getSimpleDescription() << lendl;

        // Wait for our skill level
        skillLevelWait();

        // Now check whether there's a play on the stack of cards
        if (checkForPlayable(topCard)) {
            // We found one, so let's try to grab the pile (other players may
            // have also found it too)
            grabPile();
        }

        if (this->playedCard) {
            game.gameLog << name << " has " << hand.size() << " cards." << lendl;
            game.gameLog << "Discard pile has " << game.discardPile.size() << " cards." << lendl;

            // Do we have to play another card or can we increment the turn to the next player?
            if (inFaceCardTest() == false) {
                if (alreadyIncrementedTurn == false) {
                    incrementTurn();
                }
            }
        }

        this->playedCard = false;
        alreadyIncrementedTurn = false;
    }

    game.gameLog << name << " ended the game with " << hand.size() << " cards." << lendl;

    return;
}

void Player::playCard() {
    game.currentPlayer = name;

    game.gameLog << name << " is going to play a card..." << lendl;

    if (hand.size() == 0) {
        game.gameLog << name << " is out of cards." << lendl;
        // Aww man, we're out of cards, so we're out of the game
        this->isOut = true;
        // And tell the game - this is because it's a separate object in the
        // vector and it shouldn't be (next version where we new the player objects)
        game.playerOut(this->turn);
    } else {
        Card cardToPlay = hand.front();
        hand.pop_front();
        game.gameLog << name << " is going to put down " << cardToPlay.getSimpleDescription() << lendl;
        game.discardPile.push(cardToPlay);

        // If we put down a face card, then we start the face card test
        checkIfFaceCard(cardToPlay);

        playedCard = true;
    }
}

void Player::checkIfFaceCard(const Card& cardPlayed) {
    if (cardPlayed.isFaceCard()) {
        game.gameLog << name << " played a face card (" << cardPlayed.getSimpleDescription() << ")." << lendl;

        // This player played a face card, so we can go on to the next
        // player, *but* that player has to place a face card too
        game.inFaceCardTest = true;
        // And this is the card that was played
        game.faceCardForTest = cardPlayed;
        // And tell the game that it was this player that put it into test
        game.faceCardTurnChallenger = this->turn;
    }
}

bool Player::checkForTestSuccess() {
    // 1. Are we in the state of failed test
    // 2. Are we *in* the test
    // 3. Are we the player that put down the face card
    if (game.testFailed &&
            game.inFaceCardTest == true &&
            game.faceCardTurnChallenger == this->turn) {
        game.gameLog << name << " can grab the pile because of the test." << lendl;

        return true;
    }

    return false;
}

bool Player::inFaceCardTest() {
    // If there are no cards on the discard pile, we don't have to do anything
    if (game.discardPile.size() == 0) {
        resetTestForPlayer();
        return false;
    }

    // This is the card that was played
    Card topCard = game.discardPile.top();

    if (game.inFaceCardTest == false && topCard.isFaceCard() == false) {
        // We're not in the test, and we don't have a face card
        return false;
    }

    // Are we in the face card test and do we have to deal with it?
    if (game.inFaceCardTest &&
            getNextPlayerTurn(game.faceCardTurnChallenger) == this->turn) {
        game.gameLog << name << " has to deal with the face card " << game.faceCardForTest.getSimpleDescription() << " with " << topCard.getSimpleDescription() << lendl;

        // Yes we are. That means that we can't move on to the next player
        // unless we played a face card
        if (topCard.isFaceCard()) {
            game.gameLog << name << " has its own face card with " << topCard.getSimpleDescription() << "..." << lendl;

            // We did play a face card, so it's the next player's problem
            game.faceCardTurnChallenger = this->turn;
            // And we can increment to the next player
            return false;
        } else {
            game.gameLog << name << " does not have a face card because it played " << topCard.getSimpleDescription() << "..." << lendl;

            // We did *not* play a face card so we need to figure
            // out how many times we can try to get our own face
            // card
            if (this->faceTestCardsRemaining == -1) {
                this->faceTestCardsRemaining = (calculateRemainingTriesForTest() - 1);
            } else {
                --this->faceTestCardsRemaining;
            }

            if (this->faceTestCardsRemaining < 0) {
                this->faceTestCardsRemaining = 0;
            }
            
            game.gameLog << name << " has " << this->faceTestCardsRemaining << " turns remaining to find a face card." << lendl;

            // Are we out of chances?
            if (this->faceTestCardsRemaining == 0) {
                // Yes, so the other player gets the pile
                // and we don't have to go around again
                game.testFailed = true;
            } else {
                // And indicate that we're going around again.
                return true;
            }
        }
    }

    return false;
}

void Player::resetTestForPlayer() {
    game.gameLog << name << " is resetting the test." << lendl;

    // Reset the player
    this->faceTestCardsRemaining = -1;

    // And reset the game test
    game.inFaceCardTest = false;
    game.testFailed = false;
    game.faceCardTurnChallenger = 0;
    game.faceCardForTest.rank = Rank::INVALID;
    game.faceCardForTest.suit = Suit::INVALID;
}

int Player::calculateRemainingTriesForTest() {
    int remainingTries = 0;

    switch (game.faceCardForTest.rank) {
        case Rank::ACE:
            remainingTries = 4;
            break;

        case Rank::KING:
            remainingTries = 3;
            break;

        case Rank::QUEEN:
            remainingTries = 2;
            break;

        case Rank::JACK:
            remainingTries = 1;
            break;

        default:
            remainingTries = 0;
            break;
    }

    return remainingTries;
}

bool Player::checkForPlayable(const Card& playedCard) {
    // Get our rules of the game...
    auto ruleSet = rules.getRules();

    // And shuffle the order so that we simulate random application
    // of the rules
    random_device randomDevice;
    mt19937 merTwist(randomDevice());
    shuffle(ruleSet.begin(), ruleSet.end(), merTwist);

    for (Rule test : ruleSet) {
        if (test.rule(game.discardPile)) {
            game.gameLog << name << " found playable with rule " << test.name << lendl;
            return true;
        }
    }

    return false;
}

void Player::grabPile() {
    lock_guard<mutex> pileLock(game.discardPileMutex);

    if (game.discardPile.size() > 0) {
        // Whee, we can get the cards!
        game.gameLog << name << " is taking the discard pile!" << lendl;

        while (game.discardPile.empty() == false) {
            this->hand.push_back(game.discardPile.top());
            game.discardPile.pop();
        }
    }
}

void Player::retire() {
    // And if the player is out, we reset the
    // test
    resetTestForPlayer();
    isOut = true;
}

bool Player::isRetired(bool notifyGame /* = true */) {
    if (hand.size() == 0) {
        retire();

        if (notifyGame) {
            game.playerOut(turn);
        }
    }

    return isOut;
}

void Player::skillLevelWait() {
    auto pauseTime = 1.0 / double(this->skill);
#ifdef _WIN32
    Sleep((DWORD)(pauseTime * 60.0));
#else
    sleep(pauseTime);
#endif
}

void Player::incrementTurn() {
    // Notify the next player it's their turn
    game.currentTurn = getNextPlayerTurn();
    game.gameLog << "It's player " << game.currentTurn << "'s turn" << lendl;
}

int Player::getNextPlayerTurn() {
    unsigned int newTurn = this->turn + 1;

    if (newTurn > game.players.size()) {
        newTurn = 1;
    }

    return newTurn;
}

int Player::getNextPlayerTurn(int fromPlayer) {
    unsigned int newTurn = fromPlayer + 1;

    if (newTurn > game.players.size()) {
        newTurn = 1;
    }

    return newTurn;
}
