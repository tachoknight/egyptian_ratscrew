#include "Card.hpp"

Suit& operator++(Suit& target) {
    target = static_cast<Suit>(to_integral(target) + 1);
    return target;
}

string getSimpleSuitDescription(Suit suit) {
    string simpleDescription;

    switch (suit) {
        case Suit::CLUBS:
            simpleDescription = "Clubs";
            break;

        case Suit::DIAMONDS:
            simpleDescription = "Diamonds";
            break;

        case Suit::HEARTS:
            simpleDescription = "Hearts";
            break;

        case Suit::SPADES:
            simpleDescription = "Spades";
            break;

        default:
            simpleDescription = "INVALID";
            break;
    }

    return simpleDescription;
}

string getColor(Suit suit) {
    string color;

    switch (suit) {
        case Suit::CLUBS:
        case Suit::SPADES:
            color = "Black";
            break;

        case Suit::DIAMONDS:
        case Suit::HEARTS:
            color = "Red";
            break;

        default:
            color = "INVALID";
            break;
    }

    return color;
}

string getSuitSymbol(Suit suit) {
    string symbol;

    switch (suit) {
        case Suit::CLUBS:
            symbol = "♣";
            break;

        case Suit::DIAMONDS:
            symbol = "♦";
            break;

        case Suit::HEARTS:
            symbol = "♥";
            break;

        case Suit::SPADES:
            symbol = "♠";
            break;

        default:
            symbol = "INVALID";
            break;
    }

    return symbol;
}

Rank& operator++(Rank& target) {
    target = static_cast<Rank>(to_integral(target) + 1);
    return target;
}

string getSimpleRankDescription(Rank rank) {
    string description;

    switch (rank) {
        case Rank::JACK:
            description = "Jack";
            break;

        case Rank::QUEEN:
            description = "Queen";
            break;

        case Rank::KING:
            description = "King";
            break;

        case Rank::ACE:
            description = "Ace";
            break;

        default:
            // +1 so that the 2 really comes out as 2 and not 1
            description = to_string(to_integral(rank) + 1);
    }

    return description;
}

string getRankSymbol(Rank rank) {
    string symbol;

    switch (rank) {
        case Rank::JACK:
            symbol = "J";
            break;

        case Rank::QUEEN:
            symbol = "Q";
            break;

        case Rank::KING:
            symbol = "K";
            break;

        case Rank::ACE:
            symbol = "A";
            break;

        default:
            // +1 so that the 2 really comes out as 2 and not 1
            symbol = to_string((to_integral(rank) + 1));
    }

    return symbol;
}

vector<Card> createDeck() {
    vector<Card> deck;

    for (Rank r = Rank::TWO; r != Rank::INVALID; ++r) {
        for (Suit s = Suit::HEARTS; s != Suit::INVALID; ++s) {
            deck.push_back(Card(r, s));
        }
    }

    return deck;
}

int calculateDeckCount(int playerCount) {
    // Assumption: Four players per deck of cards (52 cards - defined as DECK_SIZE)
    const double MAX_PLAYERS_PER_DECK = 4.0;
    const double ONE_DECK = 1.0;

    return static_cast<int>(ceil(static_cast<double>(playerCount) * (ONE_DECK / MAX_PLAYERS_PER_DECK)));
}

vector<Card> shuffleDeck(vector<Card>& deck) {
    random_device randomDevice;
    mt19937 merTwist(randomDevice());
    shuffle(deck.begin(), deck.end(), merTwist);

    return deck;
}
