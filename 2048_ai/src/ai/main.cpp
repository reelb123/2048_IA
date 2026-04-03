#include <iostream>
#include <iomanip>
#include <string>
#include "point2d.hpp"
#include "gameboard.hpp"
#include "Expectimax.hpp"

// Profondeur par défaut selon la taille : plus le plateau est petit,
// plus on peut se permettre une profondeur élevée.
static int defaultDepth(int size) {
    if (size <= 3) return 5;
    if (size <= 4) return 3;
    return 2;
}

// Affichage visuel en carreaux avec caractères de dessin de boîte Unicode.
// Chaque cellule fait 6 caractères de large.
static void displayBoard(const Game::GameBoard& gb) {
    int size = (int)getPlaySizeOfGameboardDataArray(gb.gbda);
    const int W = 6; // largeur d'une cellule

    auto hline = [&](const std::string& l, const std::string& m,
                     const std::string& r, const std::string& h) {
        std::cout << l;
        for (int i = 0; i < size; i++) {
            for (int k = 0; k < W; k++) std::cout << h;
            if (i < size - 1) std::cout << m;
        }
        std::cout << r << "\n";
    };

    hline("╔", "╦", "╗", "═");
    for (int row = 0; row < size; row++) {
        std::cout << "║";
        for (int col = 0; col < size; col++) {
            ull val = getTileOnGameboardDataArray(gb.gbda, point2D_t(col, row)).value;
            if (val == 0)
                std::cout << std::setw(W) << " ";
            else
                std::cout << std::setw(W) << val;
            std::cout << "║";
        }
        std::cout << "\n";
        if (row < size - 1)
            hline("╠", "╬", "╣", "═");
    }
    hline("╚", "╩", "╝", "═");
}

static const char* moveName(Move m) {
    switch(m) {
        case Move::UP:    return "↑ HAUT";
        case Move::DOWN:  return "↓ BAS";
        case Move::LEFT:  return "← GAUCHE";
        case Move::RIGHT: return "→ DROITE";
    }
    return "";
}

int main(int argc, char* argv[]) {
    int size  = 0;
    int depth = -1;

    if (argc >= 2) { try { size  = std::stoi(argv[1]); } catch (...) {} }
    if (argc >= 3) { try { depth = std::stoi(argv[2]); } catch (...) {} }

    if (size < 3 || size > 8) {
        std::cout << "Choisissez la taille du plateau (3 à 8, recommandé : 4) : ";
        std::cin >> size;
        if (size < 3 || size > 8) {
            std::cerr << "Taille invalide, 4 utilisé par défaut.\n";
            size = 4;
        }
    }
    if (depth <= 0) depth = defaultDepth(size);

    std::cout << "\n  Plateau " << size << "×" << size
              << "  |  Profondeur : " << depth << "\n\n";

    Game::GameBoard gb(size);
    addTileOnGameboard(gb);
    addTileOnGameboard(gb);

    Expectimax ai(depth);
    int moves = 0;

    while (canMoveOnGameboard(gb) && !gb.win) {
        std::cout << "Coup " << ++moves
                  << "  |  Score : " << gb.score
                  << "  |  Max : " << gb.largestTile << "\n";
        displayBoard(gb);

        Move m = ai.bestMove(gb);
        std::cout << "  → " << moveName(m) << "\n\n";

        unblockTilesOnGameboard(gb);
        switch (m) {
            case Move::UP:    tumbleTilesUpOnGameboard(gb);    break;
            case Move::DOWN:  tumbleTilesDownOnGameboard(gb);  break;
            case Move::LEFT:  tumbleTilesLeftOnGameboard(gb);  break;
            case Move::RIGHT: tumbleTilesRightOnGameboard(gb); break;
        }
        registerMoveByOneOnGameboard(gb);
        addTileOnGameboard(gb);
    }

    std::cout << "\n══════════════════════════════\n";
    std::cout << "       PARTIE TERMINÉE\n";
    std::cout << "══════════════════════════════\n";
    displayBoard(gb);
    std::cout << "  Score final  : " << gb.score     << "\n";
    std::cout << "  Tuile max    : " << gb.largestTile << "\n";
    std::cout << "  Coups joués  : " << moves         << "\n";
    if (gb.win) std::cout << "\n  ★ VICTOIRE ! ★\n";
    std::cout << "══════════════════════════════\n";
    return 0;
}
