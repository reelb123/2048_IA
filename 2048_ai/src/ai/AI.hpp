#pragma once
#include "gameboard.hpp"

enum class Move { UP, DOWN, LEFT, RIGHT };

inline Game::GameBoard applyMove(Game::GameBoard gb, Move m) {
    unblockTilesOnGameboard(gb);
    switch (m) {
        case Move::UP:    tumbleTilesUpOnGameboard(gb);    break;
        case Move::DOWN:  tumbleTilesDownOnGameboard(gb);  break;
        case Move::LEFT:  tumbleTilesLeftOnGameboard(gb);  break;
        case Move::RIGHT: tumbleTilesRightOnGameboard(gb); break;
    }
    return gb;
}

class AI {
public:
    virtual ~AI() = default;
    virtual Move bestMove(Game::GameBoard gb) = 0;
};
