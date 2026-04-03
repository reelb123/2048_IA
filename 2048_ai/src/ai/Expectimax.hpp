#pragma once
#include "AI.hpp"

class Expectimax : public AI {
public:
    explicit Expectimax(int depth = 3) : maxDepth(depth) {}
    Move bestMove(Game::GameBoard gb) override;

private:
    int maxDepth;
    double maximize(Game::GameBoard gb, int depth);
    double expectation(Game::GameBoard gb, int depth);
};
