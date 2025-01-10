#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include "card.hpp"

void textUtils::clearScreen() {
    std::cout << "\033[2J" << std::endl;
}

void textUtils::clearFromCurrentLine() {
    std::cout << "\033[0J";
}

void textUtils::moveCursorToStart() {
    std::cout << "\033[H" << std::endl;
}

void textUtils::moveCursorUp() {
    std::cout << "\033[1A";
}

void textUtils::printInBlue(std::ostream& os, const std::string& line) {
    os << "\033[34m" << line << "\033[0m" << std::endl;
}

void textUtils::printInGreen(std::ostream& os, const std::string& line) {
    os << "\033[32m" << line << "\033[0m" << std::endl;
}

void textUtils::printInRed(std::ostream& os, const std::string& line) {
    os << "\033[31m" << line << "\033[0m" << std::endl;
}

void textUtils::printInYellow(std::ostream& os, const std::string& line) {
    os << "\033[33m" << line << "\033[0m" << std::endl;
}

// [min, max]
int randomUtils::generateRandomInt(int min, int max) {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
}

void randomUtils::shuffleVector(std::vector<Card>& vector) {
    auto randomEngine = std::default_random_engine();
    std::ranges::shuffle(vector, randomEngine);
}