#include "Rules.hpp"

Rules::Rules() {
    rules.push_back(Rule("Red 10", red10));
    rules.push_back(Rule("Three-In-A-Row", runs));
    rules.push_back(Rule("Doubles", doubles));
    rules.push_back(Rule("Sandwiches", sandwiches));
    rules.push_back(Rule("Jack-7", jack7));
}

// red10:
//      If top card is either a 10 and either a diamond or heart,
//      return true
bool red10(stack<Card> cards) {
    // Presumably there's a card
    if (cards.size() < 1) {
        return false;
    }

    Card topCard = cards.top();

    if (topCard.rank == Rank::TEN && (topCard.suit == Suit::HEARTS || topCard.suit == Suit::DIAMONDS))
        return true;

    return false;
}

// runs:
//      Three in a row, regardless of suit, return true
bool runs(stack<Card> cards) {
    // Need at least three cards
    if (cards.size() < 3) {
        return false;
    }

    Card card1 = cards.top();
    cards.pop();
    Card card2 = cards.top();
    cards.pop();
    Card card3 = cards.top();

    if ((to_integral(card3.rank) - to_integral(card2.rank) == 1) && (to_integral(card2.rank) - to_integral(card1.rank) == 1)) {
        return true;
    }

    return false;
}

// doubles:
//      Two of a kind returns true
bool doubles(stack<Card> cards) {
    if (cards.size() < 2) {
        return false;
    }

    Card card1 = cards.top();
    cards.pop();
    Card card2 = cards.top();
    cards.pop();

    if (card1.rank == card2.rank) {
        return true;
    }

    return false;
}

// sandwiches:
//      Three cards, the first and third are the same rank
bool sandwiches(stack<Card> cards) {
    // Need at least three cards
    if (cards.size() < 3) {
        return false;
    }

    Card card1 = cards.top();
    cards.pop();
    // Card 2, but don't need it here
    cards.pop();
    Card card3 = cards.top();

    if (card1.rank == card3.rank) {
        return true;
    }

    return false;
}

// jackSeven
//      The jack (regardless of suit) and the seven (regardless
//      of suit) are the first two cards
bool jack7(stack<Card> cards) {
    if (cards.size() < 2) {
        return false;
    }

    Card card1 = cards.top();
    cards.pop();
    Card card2 = cards.top();
    cards.pop();

    if ((card1.rank == Rank::JACK && card2.rank == Rank::SEVEN)
            || (card1.rank == Rank::SEVEN && card2.rank == Rank::JACK))
        return true;

    return false;
}
