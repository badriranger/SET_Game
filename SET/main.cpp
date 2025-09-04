//Time  28.06.2025 9:09 p.m
//Mohammad Badri 100003064
//Dima Barada 100004374
//Abd Alah Fashesh 100001721
#include "raylib.h"
#include "game.h"
#include "reylib_utils.h"
#include <string>
#include <iostream>
#include <vector> // Add this for std::vector
#include "card.h"

// Forward declaration for drawing card features
void drawCardFeatures(const Card& card, Rectangle rect);

int main() {
    InitWindow(1000, 750, "SET Card Game"); // Increased window size to match previous steps
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    Game game;
    std::vector<int> selectedCardsIndices; // To store indices of selected cards

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        int xOffset = 20;
        int yOffset = 20;
        int cardWidth = 100; // Increased card width to match previous steps
        int cardHeight = 150; // Increased card height to match previous steps
        int padding = 15; // Increased padding to match previous steps

        // --- Card Drawing ---
        for (int i = 0; i < game.getTableSize(); i++) {
            Card card = game.getCard(i);
            Color borderColor = getRaylibColor(card.color);

            int row = i / 4; // 4 cards per row
            int col = i % 4;

            Rectangle rect = { static_cast<float>(xOffset + (cardWidth + padding) * col),
                               static_cast<float>(yOffset + (cardHeight + padding) * row),
                               static_cast<float>(cardWidth),
                               static_cast<float>(cardHeight) };

            // Draw card background
            DrawRectangleRec(rect, LIGHTGRAY);

            // Highlight selected cards (from previous steps, included for completeness if you re-add selection)
            bool isSelected = false;
            for (int selectedIdx : selectedCardsIndices) {
                if (selectedIdx == i) {
                    isSelected = true;
                    break;
                }
            }

            if (isSelected) {
                DrawRectangleLinesEx(rect, 5, GOLD); // Thicker, gold border for selected cards
            }
            else {
                DrawRectangleLinesEx(rect, 3, borderColor);
            }

            // This needs the drawCardFeatures helper function, which relies on Card details.
            // If drawCardFeatures isn't defined or card attributes aren't used, these will be "empty".
            // The image you provided has the simple text, but assuming you want the shapes based on recent discussions.
            drawCardFeatures(card, rect); // Draw the shapes/features on the card

            // Draw card index for debugging/reference
            DrawText(TextFormat("%d", i), (int)rect.x + 5, (int)rect.y + 5, 15, BLACK);
        }

        // --- Input Handling (Re-adding the basic input loop for selection, as it was in a prior working state) ---
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePoint = GetMousePosition();
            for (int i = 0; i < game.getTableSize(); i++) {
                int row = i / 4;
                int col = i % 4;
                Rectangle rect = { static_cast<float>(xOffset + (cardWidth + padding) * col),
                                   static_cast<float>(yOffset + (cardHeight + padding) * row),
                                   static_cast<float>(cardWidth),
                                   static_cast<float>(cardHeight) };

                if (CheckCollisionPointRec(mousePoint, rect)) {
                    bool alreadySelected = false;
                    for (size_t j = 0; j < selectedCardsIndices.size(); ++j) {
                        if (selectedCardsIndices[j] == i) {
                            selectedCardsIndices.erase(selectedCardsIndices.begin() + j);
                            alreadySelected = true;
                            break;
                        }
                    }
                    if (!alreadySelected) {
                        if (selectedCardsIndices.size() < 3) {
                            selectedCardsIndices.push_back(i);
                        }
                    }
                    break;
                }
            }
        }

        // --- Check for SET when 3 cards are selected ---
        if (selectedCardsIndices.size() == 3) {
            int idx1 = selectedCardsIndices[0];
            int idx2 = selectedCardsIndices[1];
            int idx3 = selectedCardsIndices[2];

            game.checkSET(idx1, idx2, idx3); // This will update score and remove cards
            selectedCardsIndices.clear(); // Clear selections after checking
        }


        // --- Display Score and Game State ---
        // Calculate the Y position for the score display, below the cards
        int scoreY = yOffset + (cardHeight + padding) * 4 + 20; // 4 rows of cards + padding

        // Player Score
        std::string playerScoreText = TextFormat("Player Score: %d", game.getPlayerPoints());
        DrawText(playerScoreText.c_str(), xOffset, scoreY, 20, BLACK);

        // Computer Score - placed next to Player Score
        // We'll get the width of the player score text to position the computer score
        int playerScoreTextWidth = MeasureText(playerScoreText.c_str(), 20);
        int gapBetweenScores = 50; // Adjust this value for more or less space

        std::string computerScoreText = TextFormat("Computer Score: %d", game.getComputerPoints());
        DrawText(computerScoreText.c_str(), xOffset + playerScoreTextWidth + gapBetweenScores, scoreY, 20, BLACK);

        // Cards on Table (moved down to avoid overlapping with new score layout)
        DrawText(TextFormat("Cards on Table: %d", game.getTableSize()), xOffset, scoreY + 30, 20, BLACK);

        // Removed Deck Size as requested
        // Removed Game State Message as it was not in the provided main.cpp, but can be added back if needed with the turn logic.


        EndDrawing();
    }

    CloseWindow();
    return 0;
}

// Helper function to draw a single shape with specified color and shading
void drawShape(Shape shape, Shading shading, Color color, Rectangle drawRect) {
    float shapeWidth = drawRect.width * 0.6f;
    float shapeHeight = drawRect.height * 0.2f;

    Color shapeColor = color;
    if (shading == Shading::Dashed) {
        shapeColor = ColorAlpha(color, 0.5f); // Semi-transparent for dashed
    }
    else if (shading == Shading::Empty) {
        shapeColor = RAYWHITE; // Filled with white for empty, outline in color
    }

    // Offset for centering the shape group within the card's dedicated drawing rectangle
    float offsetX = (drawRect.width - shapeWidth) / 2;
    float offsetY = (drawRect.height - shapeHeight) / 2;


    switch (shape) {
    case Shape::Diamond:
    {
        Vector2 p1 = { drawRect.x + offsetX + shapeWidth / 2, drawRect.y + offsetY };
        Vector2 p2 = { drawRect.x + offsetX + shapeWidth, drawRect.y + offsetY + shapeHeight / 2 };
        Vector2 p3 = { drawRect.x + offsetX + shapeWidth / 2, drawRect.y + offsetY + shapeHeight };
        Vector2 p4 = { drawRect.x + offsetX, drawRect.y + offsetY + shapeHeight / 2 };

        if (shading == Shading::Filled) {
            DrawTriangle(p1, p2, p4, shapeColor);
            DrawTriangle(p2, p3, p4, shapeColor);
        }
        else {
            DrawLineEx(p1, p2, 2, color);
            DrawLineEx(p2, p3, 2, color);
            DrawLineEx(p3, p4, 2, color);
            DrawLineEx(p4, p1, 2, color);
            if (shading == Shading::Dashed) {
                for (int i = 0; i < 4; ++i) {
                    DrawLineEx({ p4.x + (shapeWidth / 4) * i + 5, p4.y + 5 }, { p1.x - (shapeWidth / 4) * (3 - i) - 5, p1.y + 5 }, 1, color);
                }
            }
        }
    }
    break;
    case Shape::Circle:
        if (shading == Shading::Filled) {
            DrawCircle(static_cast<int>(drawRect.x + offsetX + shapeWidth / 2), static_cast<int>(drawRect.y + offsetY + shapeHeight / 2), shapeHeight / 2, shapeColor);
        }
        else {
            DrawCircleLines(static_cast<int>(drawRect.x + offsetX + shapeWidth / 2), static_cast<int>(drawRect.y + offsetY + shapeHeight / 2), shapeHeight / 2, color);
            if (shading == Shading::Dashed) {
                for (int i = 0; i < 3; ++i) {
                    DrawCircleLines(static_cast<int>(drawRect.x + offsetX + shapeWidth / 2), static_cast<int>(drawRect.y + offsetY + shapeHeight / 2), shapeHeight / 2 - (i * 2) - 2, color);
                }
            }
        }
        break;
    case Shape::Rectangle:
        if (shading == Shading::Filled) {
            DrawRectangleRec({ drawRect.x + offsetX, drawRect.y + offsetY, shapeWidth, shapeHeight }, shapeColor);
        }
        else {
            DrawRectangleLinesEx({ drawRect.x + offsetX, drawRect.y + offsetY, shapeWidth, shapeHeight }, 2, color);
            if (shading == Shading::Dashed) {
                for (int i = 0; i < 3; ++i) {
                    DrawLineEx({ drawRect.x + offsetX + 5, drawRect.y + offsetY + 5 + i * (shapeHeight / 4) },
                        { drawRect.x + offsetX + shapeWidth - 5, drawRect.y + offsetY + 5 + i * (shapeHeight / 4) }, 1, color);
                }
            }
        }
        break;
    }
}


// Function to draw all features of a card within its rectangle
void drawCardFeatures(const Card& card, Rectangle cardRect) {
    Color raylibColor = getRaylibColor(card.color);
    int numShapes = getNumberAsInt(card.number);

    // Calculate effective drawing area for shapes (leaving some margin)
    float effectiveCardHeight = cardRect.height * 0.8f; // Use 80% of card height for shapes
    float startY = cardRect.y + (cardRect.height - effectiveCardHeight) / 2; // Center the effective area

    // Adjust drawing rectangle for each shape based on the number
    float totalSpacing = (numShapes == 1) ? 0 : (numShapes - 1) * 10; // Simple spacing between shapes
    float shapeHeightForDrawing = (effectiveCardHeight - totalSpacing) / numShapes;

    for (int i = 0; i < numShapes; ++i) {
        Rectangle drawRect = {
            cardRect.x,
            startY + i * (shapeHeightForDrawing + 10), // position based on index and spacing
            cardRect.width,
            shapeHeightForDrawing
        };
        drawShape(card.shape, card.shading, raylibColor, drawRect);
    }
}