#include "raylib.h"
#include "card.h"

Color getRaylibColor(CardColor color) {
    switch (color) {
    case CardColor::Red:    return RED;
    case CardColor::Purple: return PURPLE;
    case CardColor::Green:  return GREEN;
    }
    return GRAY;
}