#include <iostream>
#include <algorithm> // pour std::max et std::min
#include <limits>
#include <chrono>

#include "morpion.h"

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

int evaluateStrategic(char** tab, int boardSize, int K, int i, int j, char pionAI, char pionHuman) {
    int score = 0;

    // possible directions: vertical, horizontal, diagonal (both directions)
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};

    for (auto& dir : directions) {
        int di = dir[0];
        int dj = dir[1];

        // count aligned pions and potential empty spaces for both players
        int countAI = 0;   // number of AI pions in this direction
        int countHuman = 0; // number of human pions in this direction
        int emptyAI = 0;   // number of empty spaces in the AI's potential alignment
        int emptyHuman = 0; // number of empty spaces in the opponent's potential alignment

        // check the direction +di, +dj for AI's alignment
        int x = i;
        int y = j;
        while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            if (tab[x][y] == pionAI) {
                countAI++; // AI's pion is found
            } else if (tab[x][y] == pionHuman) {
                break; // alignment blocked by the opponent's pion
            } else {
                emptyAI++; // empty space that can be used by the AI
            }
            x += di;
            y += dj;
        }

        // check the opposite direction -di, -dj for AI's alignment
        x = i - di;
        y = j - dj;
        while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            if (tab[x][y] == pionAI) {
                countAI++; // AI's pion is found
            } else if (tab[x][y] == pionHuman) {
                break; // alignment blocked by the opponent's pion
            } else {
                emptyAI++; // empty space that can be used by the AI
            }
            x -= di;
            y -= dj;
        }

        // evaluate the score for this direction for the AI
        if (countAI + emptyAI >= K) {
            score += countAI; // the more pions aligned, the higher the score
        }

        // check the direction +di, +dj for the opponent's alignment
        x = i;
        y = j;
        while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            if (tab[x][y] == pionHuman) {
                countHuman++; // opponent's pion is found
            } else if (tab[x][y] == pionAI) {
                break; // alignment blocked by the AI's pion
            } else {
                emptyHuman++; // empty space that can be used by the opponent
            }
            x += di;
            y += dj;
        }

        // check the opposite direction -di, -dj for the opponent's alignment
        x = i - di;
        y = j - dj;
        while (x >= 0 && x < boardSize && y >= 0 && y < boardSize) {
            if (tab[x][y] == pionHuman) {
                countHuman++; // opponent's pion is found
            } else if (tab[x][y] == pionAI) {
                break; // alignment blocked by the AI's pion
            } else {
                emptyHuman++; // empty space that can be used by the opponent
            }
            x -= di;
            y -= dj;
        }

        // evaluate the defensive score for this direction
        if (countHuman + emptyHuman >= K) {
            score -= countHuman; // penalize if the opponent is close to aligning
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
            int strategicScore = 0;
            for (int i = 0; i < boardSize; i++) {
                for (int j = 0; j < boardSize; j++) {
                    if (tab[i][j] == ' ') {
                        strategicScore += evaluateStrategic(tab, boardSize, K, i, j, pionAI, pionHuman);
                    }
                }
            }
            return strategicScore;
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
    std::cout << "\nTour de l'IA" << std::endl;

    // Variation of depth according to the board size (it is playable until a size of 17 because it's under 10 sec, otherwise a size of 20 is like 25 seconds)
    if (N == 5)
    {
        DEPTH_MAX = 4;
    }
    else if (N > 5 && N < 8)
    {
        DEPTH_MAX = 3;
    }
    else if (N == 8 || N == 9 || N == 10)
    {
        DEPTH_MAX = 2;
    }
    else if (N >= 11)
    {
        DEPTH_MAX = 1;
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