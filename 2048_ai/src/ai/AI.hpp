#pragma once
#include "gameboard.hpp"

enum class Move { UP, DOWN, LEFT, RIGHT };

class AI {
public:
    virtual ~AI() = default;
    virtual Move bestMove(Game::GameBoard gb) = 0;
};
