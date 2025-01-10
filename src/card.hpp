#pragma once

#include <cstddef>
#include <ostream>
#include <string>

class Card {
public:
    enum class Type {
        NONE,
        COMMON,
        REVERSE,
        DRAW2,
        SKIP,
        WILD_COMMON,
        WILD_DRAW4
    };

    enum class Color {
        NONE,
        BLUE,
        GREEN,
        RED,
        YELLOW
    };

    Card(const Card& other) = default;
    explicit Card(Type tp, Color col, int val = -1);
    
    Type getType() const;
    void setColor(Color col);
    static bool canBePlayed(const Card& card1, const Card& card2);

private:
    Type type;
    Color color;
    int value;
    friend std::ostream& operator <<(std::ostream& os, const Card& card);
};

std::ostream& operator <<(std::ostream& os, const Card& card);