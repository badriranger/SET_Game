#include "game.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

Game::Game() : playerPoints(0), computerPoints(0), currentTurn(PlayerType::Human) {
    // Initial fill of the table with 12 cards
    addCardsToTable();
    setGameStateMessage("Player's Turn. Select 3 cards.");
}

void Game::startGame() {
    // This function can be used for more complex game startup logic if needed.
}

void Game::displayTable() const {
    // This is less relevant for the Raylib GUI, but kept for potential console debugging
    std::cout << "Cards on table:\n";
    for (size_t i = 0; i < tableCards.size(); ++i) {
        std::cout << i << ": ";
        tableCards[i].print();
    }
}

bool Game::isValidSET(int i1, int i2, int i3) const {
    // Check for valid indices and uniqueness
    if (i1 < 0 || i1 >= (int)tableCards.size() ||
        i2 < 0 || i2 >= (int)tableCards.size() ||
        i3 < 0 || i3 >= (int)tableCards.size() ||
        i1 == i2 || i1 == i3 || i2 == i3)
        return false;

    const Card& c1 = tableCards[i1];
    const Card& c2 = tableCards[i2];
    const Card& c3 = tableCards[i3];

    // For each attribute, either all same or all different
    auto checkAttribute = [](auto a, auto b, auto c) {
        return ((a == b && b == c) || (a != b && b != c && a != c));
        };

    return checkAttribute(c1.shape, c2.shape, c3.shape) &&
        checkAttribute(c1.color, c2.color, c3.color) &&
        checkAttribute(c1.shading, c2.shading, c3.shading) &&
        checkAttribute(c1.number, c2.number, c3.number);
}

void Game::checkSET(int i1, int i2, int i3) {
    if (isValidSET(i1, i2, i3)) {
        std::string finder = (currentTurn == PlayerType::Human ? "Player" : "Computer");
        std::cout << finder << " found a Valid SET!\n";
        setGameStateMessage(finder + " found a SET! +5 points.");

        if (currentTurn == PlayerType::Human) {
            playerPoints += 5;
        }
        else {
            computerPoints += 5;
        }

        // Collect indices, sort in descending order to avoid invalidating indices
        std::vector<int> indices = { i1, i2, i3 };
        std::sort(indices.begin(), indices.end(), std::greater<int>());
        for (int idx : indices) {
            tableCards.erase(tableCards.begin() + idx);
        }

        // Refill cards immediately after a SET is found
        addCardsToTable();

        // In the real Set game, finding a set means you get another turn.
        // For simplicity of this turn-based system, we'll keep switching.
        switchTurn();
    }
    else {
        std::string finder = (currentTurn == PlayerType::Human ? "Player" : "Computer");
        std::cout << finder << " did NOT find a valid SET.\n";
        setGameStateMessage(finder + " did NOT find a SET. -5 points.");

        if (currentTurn == PlayerType::Human) {
            playerPoints -= 5;
        }
        else {
            computerPoints -= 5;
        }
        switchTurn(); // Switch turn after an invalid SET
    }

    if (isGameOver()) {
        setGameStateMessage("Game Over! No more SETs or cards. Check console for winner.");
        std::cout << "Game Over! Final Scores:\n";
        std::cout << "Player: " << playerPoints << "\n";
        std::cout << "Computer: " << computerPoints << "\n";
        if (playerPoints > computerPoints) {
            std::cout << "Player Wins!\n";
        }
        else if (computerPoints > playerPoints) {
            std::cout << "Computer Wins!\n";
        }
        else {
            std::cout << "It's a Tie!\n";
        }
    }
}

void Game::addCardsToTable() {
    // Always try to bring the table back to 12 cards if there are cards in the deck
    while (tableCards.size() < 12 && !deck.isEmpty()) {
        tableCards.push_back(deck.drawCard());
    }

    // If there are 12 or more cards on the table and no SET is currently found,
    // add 3 more cards from the deck until a SET is present or deck is empty.
    // This handles cases where no SET is findable among the current 12.
    // We only do this if the deck is not empty and no SET is currently on the table.
    // This loop prevents getting stuck with no available SETs.
    while (tableCards.size() >= 12 && findSETOnTable().empty() && !deck.isEmpty()) {
        for (int i = 0; i < 3; ++i) {
            if (!deck.isEmpty()) { // Ensure deck isn't empty before drawing
                tableCards.push_back(deck.drawCard());
            }
            else {
                break; // Break if deck becomes empty mid-addition
            }
        }
        // After adding, re-check if a SET is now available
    }
}


bool Game::isGameOver() const {
    // Game is over if the deck is empty AND no SETs can be found on the table
    if (deck.isEmpty()) {
        return findSETOnTable().empty();
    }
    return false; // Not over if deck isn't empty (implies more cards can be added)
}

int Game::getTableSize() const {
    return (int)tableCards.size();
}

std::string Game::getCardText(int index) const {
    if (index < 0 || index >= (int)tableCards.size()) return "";
    const Card& c = tableCards[index];
    return getNumberString(c.number) + " " + getColorString(c.color) + " " + getShapeString(c.shape) + " " + getShadingString(c.shading);
}

void Game::switchTurn() {
    if (currentTurn == PlayerType::Human) {
        currentTurn = PlayerType::Computer;
        setGameStateMessage("Computer's Turn...");
    }
    else {
        currentTurn = PlayerType::Human;
        setGameStateMessage("Player's Turn. Select 3 cards.");
    }
}

std::vector<int> Game::findSETOnTable() const {
    int n = (int)tableCards.size();
    for (int i = 0; i < n - 2; ++i) {
        for (int j = i + 1; j < n - 1; ++j) {
            for (int k = j + 1; k < n; ++k) {
                if (isValidSET(i, j, k)) {
                    return { i, j, k }; // Return the indices of the first SET found
                }
            }
        }
    }
    return {}; // No SET found
}

void Game::computerTurn() {
    // Basic AI: Find the first SET available and take it.
    std::vector<int> foundSet = findSETOnTable();
    if (!foundSet.empty()) {
        checkSET(foundSet[0], foundSet[1], foundSet[2]);
    }
    else {
        // If computer can't find a SET, it means there isn't one on the table.
        // It should then add more cards or pass its turn.
        std::cout << "Computer could not find a SET.\n";
        setGameStateMessage("Computer could not find a SET. -5 points.");
        computerPoints -= 5;
        addCardsToTable(); // Try to add more cards if no set on table
        switchTurn(); // Switch back to human player
    }
}