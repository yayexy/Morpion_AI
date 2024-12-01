#include <iostream>
#include <map>
#include <algorithm> // pour std::max et std::min

#include "morpion.h"

const int WIN_SCORE = 1;
const int LOSS_SCORE = -1;
const int TIE_SCORE = 0;

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

int minimax(char** tab, int boardSize, int K, char pion, int depth, bool isMaximizing){
    char pionHuman = 'O';
    bool resultAI = victoire_morpion(tab, boardSize, K, pion);
    bool resultHuman = victoire_morpion(tab, boardSize, K, pionHuman);
    bool resultTie = isTie(tab, boardSize);
    int score;

    // End condition
    if (resultAI == true || resultHuman == true || resultTie == true)
    {
        if (resultAI)
        {
            return WIN_SCORE;
            std::cout << "uihdoipufhsepiufhpseifuhspef1" << std::endl;
        }
        else if (resultHuman){
            return LOSS_SCORE;
            std::cout << "uihdoipufhsepiufhpseifuhspef2" << std::endl;
        }
        else if (resultTie){
            std::cout << "uihdoipufhsepiufhpseifuhspef3" << std::endl;
            return TIE_SCORE;
        }
        else{
            std::cout << "uihdoipufhsepiufhpseifuhspef4" << std::endl;
            return 0;
        }
        std::cout << "uihdoipufhsepiufhpseifuhspef5" << std::endl;
        return 0;
    }
    
    // IA turn
    if (isMaximizing)
    {
        std::cout << "kakakakakakaka" << std::endl;

        int bestScore = -1000;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pion;
                    score = minimax(tab, boardSize, K, pionHuman, depth + 1, false);
                    tab[i][j] = ' ';
                    bestScore = std::max(score, bestScore);
                }  
            }
        }
        return bestScore;
    }
    else
    {
        std::cout << "uihdoipufhsepiufhpseifuhspef" << std::endl;

        int bestScore = 1000;
        for (int i = 0; i < boardSize; i++)
        {
            for (int j = 0; j < boardSize; j++)
            {
                if (tab[i][j] == ' ')
                {
                    tab[i][j] = pionHuman;
                    score = minimax(tab, boardSize, K, pion, depth + 1, true);
                    tab[i][j] = ' ';
                    bestScore = std::min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}

void getBestMove(char** tab, int boardSize, int K, char pion){
    int bestScore = -1000;
    int score;
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
                score = minimax(tab, boardSize, K, pion, 0, true);
                std::cout << "score : " << score << std::endl;
                tab[i][j] = ' ';
                if (score > bestScore)
                {
                    std::cout << "c'est mieux la" << std::endl;
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
    //int x = -1;
    //int y = -1;
    std::cout << "\nIA" << std::endl;

    getBestMove(tab, N, K, pion);
}