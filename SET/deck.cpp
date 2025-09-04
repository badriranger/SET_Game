#include "deck.h"
#include <algorithm>
#include <random>
#include <chrono>

Deck::Deck() {
    // Create all combinations of card attributes (3x3x3x3 = 81 cards)
    for (int shape = 0; shape <= 2; ++shape) {
        for (int color = 0; color <= 2; ++color) {
            for (int shading = 0; shading <= 2; ++shading) {
                for (int number = 0; number <= 2; ++number) {
                    cards.push_back(Card{
                        static_cast<Shape>(shape),
                        static_cast<CardColor>(color),
                        static_cast<Shading>(shading),
                        static_cast<Number>(number)
                        });
                }
            }
        }
    }
    shuffle();
}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

bool Deck::isEmpty() const {
    return cards.empty();
}

Card Deck::drawCard() {
    if (cards.empty()) {
        throw std::out_of_range("Deck is empty!");
    }
    Card topCard = cards.back();
    cards.pop_back();
    return topCard;
}
