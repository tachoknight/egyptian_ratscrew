#ifndef Rules_hpp
#define Rules_hpp

#include <iostream>
#include <functional>
#include <vector>
#include <stack>
#include <string>
using std::cout;
using std::endl;
using std::function;
using std::vector;
using std::stack;
using std::string;

#include "Card.hpp"

struct Rule {
    string name;
    function<bool(stack<Card>)> rule;

    Rule(string name, function<bool(stack<Card>)> rule) {
        this->name = name;
        this->rule = rule;
    }
};


// The collection of rules for the game
class Rules {
        vector<Rule> rules;
    public:
        Rules();
        const vector<Rule>& getRules() const {
            return rules;
        }
};

// Red 10
bool red10(stack<Card> cards);
// Runs
bool runs(stack<Card> cards);
// Doubles
bool doubles(stack<Card> cards);
// Sandwiches
bool sandwiches(stack<Card> cards);
// Jack-7
bool jack7(stack<Card> cards);

#endif /* Rules_hpp */
