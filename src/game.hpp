#pragma once

#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "card.hpp"
#include "player.hpp"
#include "utils.hpp"

class Game {
public:
    explicit Game(size_t cnt, size_t delay);

private:
    constexpr static size_t kMinPlayersNumber = 2;
    constexpr static size_t kMaxPlayersNumber = 10;

    const size_t delay;
    const size_t playersCnt;
    const size_t userIdx;

    size_t currentPlayerIdx;
    bool directionIsReversed;
    bool drawsTwo;
    bool drawsFour;
    bool skips;

    std::vector<Card> drawPile;
    std::vector<Card> discardPile;
    std::vector<std::shared_ptr<Player>> players;

    void startNewGame();
    void fillPile();
    void shufflePile();
    void dealCards();
    void printGameInfo();
    void printUno();
    void printGameOver();
    void nextPlayer();
    bool tryNextMove();
    Card takeCardFromDrawPile();
};
