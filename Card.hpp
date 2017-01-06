#ifndef Card_h
#define Card_h

#include <type_traits>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
using std::underlying_type;
using std::string;
using std::to_string;
using std::stringstream;
using std::vector;
using std::ceil;
using std::shuffle;
using std::random_device;
using std::mt19937;

template<typename E>
constexpr auto to_integral(E e) -> typename underlying_type<E>::type {
    return static_cast<typename underlying_type<E>::type>(e);
}

enum class Suit : int {
    HEARTS = 1,
    SPADES = 2,
    DIAMONDS = 3,
    CLUBS = 4,
    INVALID
};


Suit& operator++(Suit& target);
string getSimpleSuitDescription(Suit suit);
string getColor(Suit suit);
string getSuitSymbol(Suit suit);

enum class Rank : int {
    TWO = 1,	// Aces high, otherwise ace would be 1
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    ACE,
    INVALID
};

Rank& operator++(Rank& target);
string getSimpleRankDescription(Rank rank);
string getRankSymbol(Rank rank);

const int DECK_SIZE = 52;

struct Card {
    Rank rank;
    Suit suit;

    Card(Rank rank, Suit suit) {
        this->rank = rank;
        this->suit = suit;
    }

    Card() {
        this->rank = Rank::INVALID;
        this->suit = Suit::INVALID;
    }

    string getDescription() const {
        stringstream ss;
        ss << "The " << getSimpleRankDescription(rank) << " of " << getSimpleSuitDescription(suit);
        return ss.str();
    }

    string getSimpleDescription() const {
#ifdef _WIN32 // Windows doesn't like the symbol in the console
        return getDescription();
#else
        stringstream ss;
        ss << getSuitSymbol(suit) << getRankSymbol(rank);
        return ss.str();
#endif
    }

    bool isFaceCard() const {
        if (this->rank == Rank::ACE ||
                this->rank == Rank::KING ||
                this->rank == Rank::QUEEN ||
                this->rank == Rank::JACK) {
            return true;
        }

        return false;
    }
};

int calculateDeckCount(int playerCount);
vector<Card> createDeck();
vector<Card> shuffleDeck(vector<Card>& deck);

#endif /* Card_h */
