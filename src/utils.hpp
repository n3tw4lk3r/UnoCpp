#pragma once

#include <algorithm>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <vector>

#include "card.hpp"

// Wrapper functions for ANSI escape codes
namespace textUtils {
void clearScreen();
void clearFromCurrentLine();
void moveCursorToStart();
void moveCursorUp();
void printInBlue(std::ostream& os, const std::string& line);
void printInGreen(std::ostream& os, const std::string& line);
void printInRed(std::ostream& os, const std::string& line);
void printInYellow(std::ostream& os, const std::string& line);
} // textUtils

namespace randomUtils {
// [min, max]
int generateRandomInt(int min, int max);
void shuffleVector(std::vector<Card>& vector);
} // randomUtils