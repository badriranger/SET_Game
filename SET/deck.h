#pragma once
#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

class Deck {
private:
    std::vector<Card> cards;

public:
    Deck();
    void shuffle();
    bool isEmpty() const;
    Card drawCard();
    int size() const { return static_cast<int>(cards.size()); }
};

#endif // DECK_H
