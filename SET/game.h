#pragma once
#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include "card.h"
#include "deck.h"

enum class PlayerType { Human, Computer };

class Game {
private:
    Deck deck;
    std::vector<Card> tableCards;
    int playerPoints;
    int computerPoints; // Added computer points
    PlayerType currentTurn; // Added to track whose turn it is
    std::string gameStateMessage; // Added to display messages

public:
    Game();
    void startGame();
    void displayTable() const; // This is now less relevant for Raylib
    void checkSET(int index1, int index2, int index3);
    void addCardsToTable();
    bool isGameOver() const;
    int getTableSize() const;
    std::string getCardText(int index) const; // Still useful for debugging
    bool isValidSET(int index1, int index2, int index3) const;
    const Card& getCard(int index) const { return tableCards[index]; }

    int getPlayerPoints() const { return playerPoints; }
    int getComputerPoints() const { return computerPoints; } // Getter for computer points
    int getDeckSize() const { return deck.size(); } // Getter for deck size
    PlayerType getCurrentTurn() const { return currentTurn; } // Getter for current turn
    std::string getGameStateMessage() const { return gameStateMessage; } // Getter for game state message

    void setGameStateMessage(const std::string& message) { gameStateMessage = message; }
    void switchTurn(); // Function to switch turns
    void computerTurn(); // Function for computer's turn logic
    std::vector<int> findSETOnTable() const; // Helper to find a SET
};

#endif // GAME_H