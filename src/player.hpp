#pragma once

#include <chrono>
#include <exception>
#include <iostream>
#include <thread>
#include <vector>

#include "card.hpp"

class Player {
public:
    virtual ~Player() = default;
    
    void takeCard(Card card);
    virtual Card makeMove(const Card& topCard) = 0;
    virtual std::string getCardColor() = 0;
    void printCards() const;
    size_t getCardsNumber() const;

protected:
    std::vector<Card> cards;
};

class User : public Player {
public:
    User() = default;
    ~User() = default;
    Card makeMove(const Card& topCard) override;
    std::string getCardColor() override;
};

class Bot : public Player {
public:
    Bot() = default;
    ~Bot() = default;
    Card makeMove(const Card& topCard) override;
    std::string getCardColor() override;
};