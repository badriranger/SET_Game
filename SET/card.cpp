#include "card.h"
#include <iostream>

void Card::print() const {
    std::cout << getNumberString(number) << " "
        << getColorString(color) << " "
        << getShapeString(shape) << " "
        << getShadingString(shading) << std::endl;
}

std::string getShapeString(Shape s) {
    switch (s) {
    case Shape::Diamond: return "Diamond";
    case Shape::Circle: return "Circle";
    case Shape::Rectangle: return "Rectangle"; // Keeping Rectangle for now
    }
    return "Unknown";
}

std::string getColorString(CardColor c) {
    switch (c) {
    case CardColor::Red: return "Red";
    case CardColor::Purple: return "Purple";
    case CardColor::Green: return "Green";
    }
    return "Unknown";
}

std::string getShadingString(Shading s) {
    switch (s) {
    case Shading::Filled: return "Filled";
    case Shading::Empty: return "Empty";
    case Shading::Dashed: return "Dashed";
    }
    return "Unknown";
}

std::string getNumberString(Number n) {
    switch (n) {
    case Number::One: return "One";
    case Number::Two: return "Two";
    case Number::Three: return "Three";
    }
    return "Unknown";
}

int getNumberAsInt(Number n) {
    switch (n) {
    case Number::One: return 1;
    case Number::Two: return 2;
    case Number::Three: return 3;
    }
    return 1; // Default to 1 if something goes wrong
}