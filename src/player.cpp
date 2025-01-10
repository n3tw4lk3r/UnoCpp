#include "player.hpp"

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include <vector>

#include "card.hpp"
#include "utils.hpp"

void Player::takeCard(Card card) {
    cards.push_back(card);
}

void Player::printCards() const {
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << i + 1 << ") " << cards[i];
    }
}

size_t Player::getCardsNumber() const {
    return cards.size();
}

Card User::makeMove(const Card& topCard) {
    std::string s;
    std::cin >> s;
    try {
        int idx = stoi(s);
        if (!(1 <= idx && idx <= cards.size()))
            return Card(Card::Type::NONE, Card::Color::NONE, -1);
        --idx;
        Card card = cards[idx];
        if (!Card::canBePlayed(card, topCard))
            return Card(Card::Type::NONE, Card::Color::NONE, -1);
        cards.erase(cards.begin() + idx);
        return card;
    } catch (const std::exception& e) {
        return Card(Card::Type::NONE, Card::Color::NONE, -1);
    }
}

std::string User::getCardColor() {
    std::string col;
    do {
        std::cout << "Enter card's color (blue / green / red / yellow" << std::endl;
        std::cin >> col;
    } while (!(col == "blue" || col == "green" || col == "red" || col == "yellow"));
    return col;
}

// ToDo: add better bot logic
Card Bot::makeMove(const Card& topCard) {
    std::vector<Card> possibleCards;
    for (size_t i = 0; i < cards.size(); ++i) {
        if (Card::canBePlayed(cards[i], topCard)) {
            Card card = cards[i];
            cards.erase(cards.begin() + i);
            possibleCards.push_back(card);
        }
    }
    if (possibleCards.empty())
        return Card(Card::Type::NONE, Card::Color::NONE, -1);
    int idx = randomUtils::generateRandomInt(0, possibleCards.size() - 1);
    return possibleCards[idx];
}

// ToDo: add better bot logic
std::string Bot::getCardColor() {
    int num = randomUtils::generateRandomInt(0, 3);
    if (num == 0)
        return "blue";
    if (num == 1) 
        return "green";
    if (num == 2)
        return "red";
    return "yellow";
}