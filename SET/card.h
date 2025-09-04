#pragma once
#ifndef CARD_H
#define CARD_H

#include <string>

enum class Shape { Diamond = 0, Circle = 1, Rectangle = 2 }; // Keeping Rectangle as is
enum class CardColor { Red = 0, Purple = 1, Green = 2 };
enum class Shading { Filled = 0, Empty = 1, Dashed = 2 };
enum class Number { One = 0, Two = 1, Three = 2 };


struct Card {
    Shape shape;
    CardColor color;
    Shading shading;
    Number number;

    void print() const;
};

std::string getShapeString(Shape s);
std::string getColorString(CardColor c);
std::string getShadingString(Shading s);
std::string getNumberString(Number n);
int getNumberAsInt(Number n);

#endif // CARD_H