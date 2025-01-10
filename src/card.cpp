#include "card.hpp"

#include <cstddef>
#include <ostream>
#include <string>

#include "utils.hpp"

Card::Card(Card::Type tp, Card::Color col, int val) :
    type(tp),
    color(col),
    value(val)
{}

Card::Type Card::getType() const {
    return type;
}

void Card::setColor(Color col) {
    color = col;
}

// checks if card1 can be used against card2
bool Card::canBePlayed(const Card& card1, const Card& card2) {
    if (card1.type == Card::Type::WILD_COMMON || card1.type == Card::Type::WILD_DRAW4)
        return true;
    if (card1.type == Card::Type::COMMON && card1.type == card2.type) {
        if (card1.color == card2.color)
            return true;
        if (card1.value == card2.value)
            return true;
        return false;
    }
    if (card1.type == card2.type)
        return true;
    if (card1.color == card2.color)
        return true;
    return false;
}

std::ostream& operator <<(std::ostream& os, const Card& card) {
    std::string str;
    switch (card.color) {
    case Card::Color::BLUE:
        str += "Blue ";
        break;
    case Card::Color::GREEN:
        str += "Green ";
        break;
    case Card::Color::RED:
        str += "Red ";
        break;
    case Card::Color::YELLOW:
        str += "Yellow ";
        break; 
    case Card::Color::NONE:
    default:
        break;
    }

    switch (card.getType()) {
    case Card::Type::COMMON:
        str += std::to_string(card.value);
        break;
    case Card::Type::REVERSE:
        str += "Reverse";
        break;
    case Card::Type::DRAW2:
        str += "Draw 2";
        break;
    case Card::Type::SKIP:
        str += "Skip";
        break;
    case Card::Type::WILD_COMMON:
        str += "Wild";
        break;
    case Card::Type::WILD_DRAW4:
        str += "Wild Draw 4";
        break;
    case Card::Type::NONE:
    default:
        break;
    }

    switch (card.color) {
    case Card::Color::BLUE:
        textUtils::printInBlue(os, str); 
        break;
    case Card::Color::GREEN:
        textUtils::printInGreen(os, str); 
        break;
    case Card::Color::RED:
        textUtils::printInRed(os, str); 
        break;
    case Card::Color::YELLOW:
        textUtils::printInYellow(os, str); 
        break; 
    case Card::Color::NONE:
    default:
        std::cout << str << std::endl;
        break;
    }
    return os;
}