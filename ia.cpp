#include <iostream>
#include <map>
#include <algorithm> // pour std::max et std::min

#include "morpion.h"

const int WIN_SCORE = 1;
const int LOSS_SCORE = -1;
const int TIE_SCORE = 0;

struct AIMove {
    int x;
    int y;
    int score;
};

struct Move {
    int x;
    int y;
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

int minimax(char** tab, int boardSize, int K, char pion, int depth, bool isMaximizing){
    bool resultAI = victoire_morpion(tab, boardSize, K, pion);
    bool resultHuman = victoire_morpion(tab, boardSize, K, 'O');
    bool resultTie = isTie(tab, boardSize);
    int score;

    // End condition
    if (resultAI == true || resultHuman == true || resultTie == true)
    {
        if (resultAI)
        {
            score = WIN_SCORE;
        }
        else if (resultHuman){
            score = LOSS_SCORE;
        }
        else if (resultTie){
            score = TIE_SCORE;
        }
        
        return score;
    }
    
    // IA turn
    if (isMaximizing)
    {
        int bestScore = -1000;
        Move move;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pion;
                    score = minimax(tab, boardSize, K, pion, depth + 1, false);
                    tab[i][j] = ' ';
                    bestScore = std::max(score, bestScore);
                }  
            }
        }
        return bestScore;
    }
    else
    {
        int bestScore = 1000;
        Move move;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = 'O';
                    score = minimax(tab, boardSize, K, pion, depth + 1, true);
                    tab[i][j] = ' ';
                    bestScore = std::min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}

AIMove getBestMove(char** tab, int boardSize, int K, char pion){
    int bestScore = -1000;
    Move move;

    // AI turn
    for (int i = 0; i < boardSize; i++)
    {
        for (int j = 0; j < boardSize; j++)
        {
            std::cout << "(" << i << ", " << j << ") : " << tab[i][j] << std::endl;
            
            // if the spot is available
            if (tab[i][j] == ' ')
            {
                tab[i][j] = pion;
                int score = minimax(tab, boardSize, K, pion, 0, true);
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
    int x = -1;
    int y = -1;
    std::cout << "\nIA" << std::endl;

    getBestMove(tab, N, K, pion);
}