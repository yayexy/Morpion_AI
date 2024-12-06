#include <iostream>
#include <map>
#include <algorithm> // pour std::max et std::min
#include <limits>
#include <chrono>

#include "morpion.h"

const int WIN_SCORE = 10;
const int LOSS_SCORE = -10;
const int TIE_SCORE = 0;
//const int DEPTH_MAX = 6;

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

int minimax(char** tab, int boardSize, int K, int depth, int alpha, int beta, bool isMaximizing){
    char pionHuman = 'O';
    char pionAI = 'X';
    bool resultAI = victoire_morpion(tab, boardSize, K, pionAI);
    bool resultHuman = victoire_morpion(tab, boardSize, K, pionHuman);
    bool resultTie = isTie(tab, boardSize);
    int score = 0;

    // End condition
    if (resultAI == true || resultHuman == true || resultTie == true)
    {
        if (resultAI)
        {
            return WIN_SCORE;
        }
        if (resultHuman){
            return LOSS_SCORE;
        }
        if (resultTie){
            return TIE_SCORE;
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
                        i = boardSize;
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
                        i = boardSize;
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
    Move move;

    // AI turn
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            
            //std::cout << "(" << i << ", " << j << ") : " << tab[i][j] << std::endl;
            
            // if the spot is available
            if (tab[i][j] == ' ')
            {
                tab[i][j] = pion;
                
                score = minimax(tab, boardSize, K, 0, alpha, beta, false);
                
                //std::cout << "score : " << score << "\n" << std::endl;
                
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

void jouerX(char** tab, int N, int K, char pion){
    std::cout << "\nIA" << std::endl;
    
    // Capturer le temps de début
    auto start = std::chrono::high_resolution_clock::now();

    // Appeler la fonction pour obtenir le meilleur coup
    getBestMove(tab, N, K, pion);

    // Capturer le temps de fin
    auto end = std::chrono::high_resolution_clock::now();

    // Calculer la durée écoulée
    std::chrono::duration<double> duration = end - start;

    // Afficher la durée en secondes
    std::cout << "Temps de calcul de getBestMove: " << duration.count() << " secondes" << std::endl;

}