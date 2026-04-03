
#include "Heuristics.hpp"
#include "point2d.hpp"
#include <cmath>

static ull getTile(const Game::GameBoard& gb, int x, int y) {
    return getTileOnGameboardDataArray(gb.gbda, point2D_t(x, y)).value;
}

double Heuristics::emptyCells(const Game::GameBoard& gb) {
    int size = (int)getPlaySizeOfGameboardDataArray(gb.gbda);
    int count = 0;
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++)
            if (getTile(gb, x, y) == 0) count++;
    return count * 270.0;
}

// Matrice de poids en zigzag depuis le coin haut-gauche.
// Le coin (0,0) reçoit le poids maximal (size²-1), ce qui force la plus grande
// tuile dans ce coin. Le chemin serpente ligne par ligne pour récompenser
// l'arrangement ordonné en cascade depuis le coin.
double Heuristics::snakeWeight(const Game::GameBoard& gb) {
    int size = (int)getPlaySizeOfGameboardDataArray(gb.gbda);
    double score = 0;
    int idx = size * size - 1; // poids max au coin (0,0)
    for (int row = 0; row < size; row++) {
        if (row % 2 == 0) {
            // gauche → droite
            for (int col = 0; col < size; col++)
                score += idx-- * (double)getTile(gb, col, row);
        } else {
            // droite → gauche
            for (int col = size - 1; col >= 0; col--)
                score += idx-- * (double)getTile(gb, col, row);
        }
    }
    return score;
}

double Heuristics::smoothness(const Game::GameBoard& gb) {
    int size = (int)getPlaySizeOfGameboardDataArray(gb.gbda);
    double score = 0;
    for (int x = 0; x < size; x++) {
        for (int y = 0; y < size - 1; y++) {
            ull a = getTile(gb, x, y);
            ull b = getTile(gb, x, y + 1);
            if (a && b) score -= std::abs((double)a - (double)b);
        }
    }
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size - 1; x++) {
            ull a = getTile(gb, x, y);
            ull b = getTile(gb, x + 1, y);
            if (a && b) score -= std::abs((double)a - (double)b);
        }
    }
    return score * 0.1;
}

double Heuristics::maxTileCorner(const Game::GameBoard& gb) {
    int size = (int)getPlaySizeOfGameboardDataArray(gb.gbda);
    int s = size - 1;
    ull maxTile = gb.largestTile;
    int corners[4][2] = {{0,0},{0,s},{s,0},{s,s}};
    for (auto& c : corners)
        if (getTile(gb, c[0], c[1]) == maxTile)
            return maxTile * 2.0;
    return 0;
}

double Heuristics::evaluate(const Game::GameBoard& gb) {
    return emptyCells(gb)
         + 3.0 * snakeWeight(gb)   // poids 3× pour imposer le pattern en coin
         + smoothness(gb)
         + maxTileCorner(gb);
}
