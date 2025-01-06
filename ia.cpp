#include <iostream>
#include <map>
#include <algorithm> // pour std::max et std::min
#include <limits>
#include <chrono>

#include "morpion.h"

bool debug = false;

const int WIN_SCORE = 1000;
const int LOSS_SCORE = -1000;
const int TIE_SCORE = 0;
int DEPTH_MAX = 5;
char pionHuman = 'O';
char pionAI = 'X';

int prunning_count = 0;

struct Move {
    int x;
    int y;
    int score;
};

bool isTie(char** tab, int boardSize){
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            if (tab[i][j] == ' ') // if there's one case empty, then the game's not ended
            {
                return false;
            }
        }
    }
    return true; // end game
}

int countAligned(char** tab, int boardSize, int i, int j, int di, int dj, char pion) {
    int align = 0;
    
    while (i >= 0 && i < boardSize && j >= 0 && j < boardSize && tab[i][j] == pion) {
        align++;
        i += di;
        j += dj;
    }

    return align;
}

int evaluateHeuristic(char** tab, int boardSize, char pionAI, char pionHuman) {
    int score = 0;

    // range each case
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (tab[i][j] == ' ') { // empty case
                continue; // do nothing
            }

            // Evaluate alignments for IA
            score += countAligned(tab, boardSize, i, j, 1, 0, pionAI);  // Vertical downwards
            score += countAligned(tab, boardSize, i, j, 0, 1, pionAI);  // Horizontal rightwards
            score += countAligned(tab, boardSize, i, j, 1, 1, pionAI);  // Diagonal bottom-right
            score += countAligned(tab, boardSize, i, j, 1, -1, pionAI); // Diagonal bottom-left

            // Evaluate alignments for opponent (block)
            score -= countAligned(tab, boardSize, i, j, 1, 0, pionHuman);  // Vertical downwards
            score -= countAligned(tab, boardSize, i, j, 0, 1, pionHuman);  // Horizontal rightwards
            score -= countAligned(tab, boardSize, i, j, 1, 1, pionHuman);  // Diagonal bottom-right
            score -= countAligned(tab, boardSize, i, j, 1, -1, pionHuman); // Diagonal bottom-left
        }
    }

    return score;
}


int minimax(char** tab, int boardSize, int K, int depth, int alpha, int beta, bool isMaximizing){
    bool resultAI = victoire_morpion(tab, boardSize, K, pionAI);
    bool resultHuman = victoire_morpion(tab, boardSize, K, pionHuman);
    bool resultTie = isTie(tab, boardSize);
    int score = 0;

    // End condition
    if (resultAI == true || resultHuman == true || resultTie == true || depth >= DEPTH_MAX)
    {
        if (resultAI)
        {   
            return WIN_SCORE - depth;
        }
        if (resultHuman){
            return LOSS_SCORE + depth;
        }
        if (resultTie){
            return TIE_SCORE;
        }
        if (depth >= DEPTH_MAX)
        {
            return evaluateHeuristic(tab, boardSize, pionAI, pionHuman);
        }
    }

    // IA turn
    if (isMaximizing)
    {
        int bestScore = std::numeric_limits<int>::min();
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {   
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pionAI;
                    score = minimax(tab, boardSize, K, depth + 1, alpha, beta, false);
                    tab[i][j] = ' ';

                    bestScore = std::max(bestScore, score);

                    alpha = std::max(alpha, score);

                    if (beta <= alpha)
                    {   
                        prunning_count++;
                        break;
                    }
                }  
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = std::numeric_limits<int>::max();
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pionHuman;
                    score = minimax(tab, boardSize, K, depth + 1, alpha, beta, true);
                    tab[i][j] = ' ';

                    bestScore = std::min(bestScore, score);

                    beta = std::min(beta, score);
    
                    if (beta <= alpha)
                    {
                        prunning_count++;
                        break;
                    }
                }
            }
        }
        return bestScore;
    }
}

void getBestMove(char** tab, int boardSize, int K, char pion){
    int bestScore = std::numeric_limits<int>::min();
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    int score = 0;
    int depth = 0;
    Move move;

    // AI turn
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {

            if (debug)
            {
                std::cout << "(" << i << ", " << j << ") : " << tab[i][j] << std::endl; // Aide
                std::cout << "score : " << score << "\n" << std::endl; // Aide
            }
            
            // if the spot is available
            if (tab[i][j] == ' ')
            {
                tab[i][j] = pion;
                
                score = minimax(tab, boardSize, K, depth, alpha, beta, false);
                
                tab[i][j] = ' ';
                if (score > bestScore)
                {
                    bestScore = score;
                    move.x = i;
                    move.y = j;
                }
            }
        }
    }
    tab[move.x][move.y] = pion;
}

void jouerX(char** tab, int N, int K){
    std::cout << "\nIA" << std::endl;

    // Variation of depth according to the board size (it is playable until a size of 10)
    if (N == 6 || N == 7)
    {
        DEPTH_MAX = 4;
    }
    else if (N > 7)
    {
        DEPTH_MAX = 3;
    }

    // Store time at the beginning
    auto start = std::chrono::high_resolution_clock::now();

    // Call the function to get the best move
    getBestMove(tab, N, K, pionAI);

    std::cout << "\n\n\nLe nombre d'elagage est : " << prunning_count << "\n\n\n" << std::endl;

    // Store time at the end
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate time spent to find the best move (function)
    std::chrono::duration<double> duration = end - start;

    // Print time in seconds
    std::cout << "Temps de calcul de getBestMove: " << duration.count() << " secondes" << std::endl;

}