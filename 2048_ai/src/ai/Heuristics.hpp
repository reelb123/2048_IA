#pragma once
#include "gameboard.hpp"

namespace Heuristics {
    double emptyCells(const Game::GameBoard& gb);
    double snakeWeight(const Game::GameBoard& gb);
    double smoothness(const Game::GameBoard& gb);
    double maxTileCorner(const Game::GameBoard& gb);
    double evaluate(const Game::GameBoard& gb);
}
