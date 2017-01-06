#include "Game.hpp"
#ifndef _WIN32
#include <unistd.h>
#endif

const int MAX_SKILL_LEVEL = 5;

Game::Game(int playerCount) : turnBarrier(playerCount), gameOver(false), currentTurn(1), inFaceCardTest(false), faceCardTurnChallenger(0), testFailed(false) {
    gameLog << "GAME: Creating a game with " << playerCount << " players" << lendl;

    createGameDeck(playerCount);
    createPlayers(playerCount);
    dealCards();
}

Game::~Game() {
    players.clear();

    this->gameLog.dumpData();

    gameLog << "GAME: *** GAME OVER ***" << lendl;
}

void Game::createGameDeck(int playerCount) {
    auto decksCount = calculateDeckCount(playerCount);
    gameLog << "GAME: Creating game deck made of " << decksCount << " decks..." << lendl;

    deck.reserve(decksCount * DECK_SIZE);

    for (int x = 0; x < decksCount; ++x) {
        auto newDeck = createDeck();
        std::copy(newDeck.begin(), newDeck.end(), std::back_inserter(deck));
    }

    deck = shuffleDeck(deck);
}

void Game::createPlayers(int playerCount) {
    gameLog << "GAME: Creating " << playerCount << " players..." << lendl;

    for (int x = 0; x < playerCount; ++x) {
        stringstream playerName;
        playerName << "Player " << (x + 1);
        Player p(*this, playerName.str(), (x + 1));
        p.setSkill(getRandomSkillLevel());
        this->players.push_back(p);
    }
}

int Game::getRandomSkillLevel() {
    random_device rng;
    mt19937 urng(rng());

    auto skill = std::bind(std::uniform_int_distribution<int>(1, MAX_SKILL_LEVEL), urng);

    return skill();
}

void Game::dealCards() {
#if 0
    players[0].addCardToHand(Card(Rank::JACK, Suit::CLUBS));
    players[0].addCardToHand(Card(Rank::SEVEN, Suit::CLUBS));
    players[0].addCardToHand(Card(Rank::TWO, Suit::CLUBS));
    players[1].addCardToHand(Card(Rank::TEN, Suit::CLUBS));
    players[1].addCardToHand(Card(Rank::ACE, Suit::CLUBS));
    players[1].addCardToHand(Card(Rank::FIVE, Suit::CLUBS));
#endif
//#if 0
    gameLog << "GAME: Dealing out all the cards... (" << deck.size() << " cards)" << lendl;

    int player = 0;

    for (std::vector<Card>::iterator it = deck.begin(); it != deck.end(); ) {
        players[player].addCardToHand(*it);
        it = deck.erase(it);

        if (++player == players.size()) {
            player = 0;
        }
    }

    gameLog << "GAME: We've dealt all the cards. (Deck size is now " << deck.size() << ")" << lendl;
//#endif
}

void Game::start() {
    for (Player player : players) {
        playerThreads.push_back(thread(&Player::playGame, player));
    }

    for (unsigned int x = 0; x < playerThreads.size(); ++x) {
        playerThreads[x].join();
    }
}

void Game::playerOut(int playerNum) {
    players[playerNum - 1].retire();

    // If there's only one remaining player, the game is over
    int activePlayers = 0;

    for (Player player : players) {
        if (player.isRetired(false) == false) {
            ++activePlayers;
        }
    }

    gameLog << "GAME: There are currently " << activePlayers << " in the game." << lendl;

    if (activePlayers == 1) {
        gameLog << "GAME: Hey, the game is over!" << lendl;
        // Game is over
        gameOver = true;
    }
}
