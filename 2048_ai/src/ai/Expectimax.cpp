#include "point2d.hpp"
#include "Expectimax.hpp"
#include "Heuristics.hpp"
#include <limits>
#include <vector>
#include <algorithm>
#include <random>

double Expectimax::maximize(Game::GameBoard gb, int depth) {
    if (depth == 0 || !canMoveOnGameboard(gb))
        return Heuristics::evaluate(gb);

    double best = std::numeric_limits<double>::lowest();
    for (auto m : {Move::UP, Move::DOWN, Move::LEFT, Move::RIGHT}) {
        Game::GameBoard next = applyMove(gb, m);
        if (next.moved)
            best = std::max(best, expectation(next, depth - 1));
    }
    return best;
}

double Expectimax::expectation(Game::GameBoard gb, int depth) {
    int size = (int)getPlaySizeOfGameboardDataArray(gb.gbda);
    std::vector<std::pair<int,int>> emptyCells;

    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            if (getTileOnGameboardDataArray(gb.gbda, point2D_t(x,y)).value == 0)
                emptyCells.push_back({x, y});

    if (emptyCells.empty()) return Heuristics::evaluate(gb);

    // RNG statique : initialisé une seule fois, pas à chaque appel.
    static std::mt19937 rng{std::random_device{}()};
    std::shuffle(emptyCells.begin(), emptyCells.end(), rng);

    int limit = std::min((int)emptyCells.size(), 4);
    double score = 0;

    for (int i = 0; i < limit; i++) {
        auto [x, y] = emptyCells[i];

        Game::GameBoard gb2 = gb;
        std::get<1>(gb2.gbda)[y * size + x].value = 2;
        score += 0.9 * maximize(gb2, depth); // 90% de chances que la tuile soit un 2

        Game::GameBoard gb4 = gb;
        std::get<1>(gb4.gbda)[y * size + x].value = 4;
        score += 0.1 * maximize(gb4, depth); // 10% de chances que ce soit un 4
    }
    return score / limit;
}

Move Expectimax::bestMove(Game::GameBoard gb) {
    Move best = Move::UP;
    double bestScore = std::numeric_limits<double>::lowest();

    for (auto m : {Move::UP, Move::DOWN, Move::LEFT, Move::RIGHT}) {
        Game::GameBoard next = applyMove(gb, m);
        if (next.moved) {
            double s = expectation(next, maxDepth - 1);
            if (s > bestScore) { bestScore = s; best = m; }
        }
    }
    return best;
}
