#include "game.hpp"

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

Game::Game(size_t cnt, size_t del) :
    delay(del),
    playersCnt(cnt),
    userIdx(randomUtils::generateRandomInt(0, playersCnt - 1)),
    currentPlayerIdx(0),
    directionIsReversed(false),
    drawsTwo(false),
    drawsFour(false),
    skips(false)
{
    startNewGame();
    fillPile();
    shufflePile();
    dealCards();

    Card topCard = drawPile.back();
    while (topCard.getType() == Card::Type::WILD_DRAW4) {
        shufflePile();
        topCard = drawPile.back();
    }
    drawPile.pop_back();
    discardPile.push_back(topCard);

    while (true) {
        printGameInfo();
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        const bool gameContinues = tryNextMove();
        if (!gameContinues)
            break;
        nextPlayer();
    }
    printGameOver();
}

void Game::startNewGame() {
    textUtils::clearScreen();
    textUtils::moveCursorToStart();
    std::cout << "------  Uno Game Simulation  ------" << std::endl;
    std::cout << "Starting new game..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(delay));
}

void Game::fillPile() {
    for (Card::Color color : { Card::Color::BLUE, Card::Color::GREEN, Card::Color::RED, Card::Color::YELLOW }) {
        Card card0 = Card(Card::Type::COMMON, color, 0);
        drawPile.push_back(card0);
        for (int i = 0; i < 2; ++i) {
            for (int val = 1; val <= 9; ++val) {
                Card card = Card(Card::Type::COMMON, color, val);
                drawPile.push_back(card); 
            }
        }
        for (int i = 0; i < 2; ++i) {
            Card card = Card(Card::Type::SKIP, color, -1);
            drawPile.push_back(card); 
        }
        for (int i = 0; i < 2; ++i) {
            Card card = Card(Card::Type::REVERSE, color, -1);
            drawPile.push_back(card); 
        }
        for (int i = 0; i < 2; ++i) {
            Card card = Card(Card::Type::DRAW2, color, -1);
            drawPile.push_back(card); 
        }
    }
    for (int i = 0; i < 4; ++i) {
        Card wild = Card(Card::Type::WILD_COMMON, Card::Color::NONE, -1);
        drawPile.push_back(wild);
        Card wildTake = Card(Card::Type::WILD_DRAW4, Card::Color::NONE, -1);
        drawPile.push_back(wildTake);
    }
}

void Game::shufflePile() {
    randomUtils::shuffleVector(drawPile);
}

void Game::dealCards() {
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    for (size_t i = 0; i < playersCnt; ++i) {
        if (i == userIdx)
            players.push_back(std::make_shared<User>(User()));
        else
            players.push_back(std::make_shared<Bot>(Bot()));
        for (int j = 0; j < 7; ++j)
            players[i]->takeCard(takeCardFromDrawPile());
    }
}

void Game::printGameInfo() {
    textUtils::clearScreen();
    textUtils::moveCursorToStart();
    std::cout << "------  Uno Game Simulation  ------" << std::endl;
    std::cout << std::endl;
    for (size_t i = 0; i < playersCnt; ++i) {
        std::cout << "Player " << i + 1;
        if (i == userIdx)
            std::cout << " (you)";
        std::cout << ": ";
        std::cout << players[i]->getCardsNumber() << " cards" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Direction (default / reversed): ";
    if (directionIsReversed)
        std::cout << "reversed";
    else
        std::cout << "default";
    std::cout << std::endl;
    std::cout << "Top card: " << discardPile.back() << std::endl;
    std::cout << "Your cards:" << std::endl;
    players[userIdx]->printCards();
    std::cout << std::endl;
    std::cout << "Current player index: " << currentPlayerIdx + 1 << std::endl;
}

void Game::printUno() {
    std::cout << "Player " << currentPlayerIdx + 1;
    if (currentPlayerIdx == userIdx)
        std::cout << "(you) ";
    std::cout << " shouts \"Uno!\"" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(delay));
}

void Game::printGameOver() {
    std::cout << "Player " << currentPlayerIdx + 1 << " won!" << std::endl;
    std::cout << "Game over" << std::endl;
}

void Game::nextPlayer() {
    if (directionIsReversed) {
        if (currentPlayerIdx == 0)
            currentPlayerIdx = playersCnt;
        --currentPlayerIdx;
        return;
    }
    ++currentPlayerIdx;
    if (currentPlayerIdx == playersCnt)
        currentPlayerIdx = 0;
}

bool Game::tryNextMove() {
    if (skips) {
        std::cout << "Player " << currentPlayerIdx + 1 << " skips this move." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        skips = false;
        return true;
    }

    if (drawsTwo) {
        std::cout << "Player " << currentPlayerIdx + 1 << " draws two cards." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        for (int i = 0; i < 2; ++i) {
            Card card = takeCardFromDrawPile();
            players[currentPlayerIdx]->takeCard(card);
            drawPile.pop_back();
        }
        drawsTwo = false;
        return true;
    }

    if (drawsFour) {
        std::cout << "Player " << currentPlayerIdx + 1 << " draws four cards." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        for (int i = 0; i < 4; ++i) {
            Card card = takeCardFromDrawPile();
            players[currentPlayerIdx]->takeCard(card);
            drawPile.pop_back();
        }
        drawsFour = false;
        return true;
    }

    if (currentPlayerIdx == userIdx)
        std::cout << "It's your turn now. Enter the index of card you want to use, or type \"take\" to take from the pile:" << std::endl;
    else
        std::cout << "Player " << currentPlayerIdx + 1 << " (bot) is making a move..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    Card card = players[currentPlayerIdx]->makeMove(discardPile.back());
    std::cout << "Player " << currentPlayerIdx + 1 << " has made its move." << std::endl;

    if (card.getType() == Card::Type::NONE) {
        Card card = takeCardFromDrawPile();
        players[currentPlayerIdx]->takeCard(card);
        std::cout << "Player " << currentPlayerIdx + 1 << " takes a card from the pile." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        return true;
    }

    const int cardsCnt = players[currentPlayerIdx]->getCardsNumber();
    if (cardsCnt == 1)
        printUno();
    if (cardsCnt == 0)
        return false;
    
    if (card.getType() == Card::Type::WILD_COMMON || card.getType() == Card::Type::WILD_DRAW4) {
        std::string col = players[currentPlayerIdx]->getCardColor();
        if (col == "blue")
            card.setColor(Card::Color::BLUE);
        if (col == "green")
            card.setColor(Card::Color::GREEN);
        if (col == "red")
            card.setColor(Card::Color::RED);
        if (col == "yellow")
            card.setColor(Card::Color::YELLOW);
    }

    if (card.getType() == Card::Type::REVERSE)
        directionIsReversed ^= true;
    if (card.getType() == Card::Type::DRAW2)
        drawsTwo = true;
    if (card.getType() == Card::Type::WILD_DRAW4)
        drawsFour = true;
    if (card.getType() == Card::Type::SKIP)
        skips = true;

    discardPile.push_back(card);
    std::this_thread::sleep_for(std::chrono::seconds(delay));
    return true;
}

Card Game::takeCardFromDrawPile() {
    if (drawPile.empty()) {
        Card topCard = discardPile.back();
        discardPile.pop_back();
        while (!discardPile.empty()) {
            Card card = discardPile.back();
            discardPile.pop_back();
            drawPile.push_back(card);
        }
        discardPile.push_back(topCard);
        shufflePile();
    }
    Card card = drawPile.back();
    drawPile.pop_back();
    return card;
}