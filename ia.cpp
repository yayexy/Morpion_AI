#include <iostream>
#include <map>
#include <algorithm> // pour std::max et std::min
#include <limits>
#include <chrono>

#include "morpion.h"

const int WIN_SCORE = 10;
const int LOSS_SCORE = -10;
const int TIE_SCORE = 0;
const int DEPTH_MAX = 5;
const char pionHuman = 'O';
const char pionAI = 'X';

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

/*
// Vérifie si un joueur est à une case de gagner dans une direction donnée
bool closeToWin(char** tab, int size, int nbpion) {
    int requiredAlign = nbpion - 1; // Le nombre d'alignements requis pour être proche de gagner

    // Parcourt chaque case du plateau
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (tab[i][j] == pionHuman) { // Si la case appartient au joueur

                // Vérifie l'alignement vertical
                int align = 0;
                for (int k = 0; k < requiredAlign && i + k < size; ++k) {
                    if (tab[i + k][j] == pionHuman) {
                        align++;
                    }
                }
                if (align == requiredAlign) return true;

                // Vérifie l'alignement horizontal
                align = 0;
                for (int k = 0; k < requiredAlign && j + k < size; ++k) {
                    if (tab[i][j + k] == pionHuman) {
                        align++;
                    }
                }
                if (align == requiredAlign) return true;

                // Vérifie l'alignement diagonal (haut-gauche vers bas-droit)
                align = 0;
                for (int k = 0; k < requiredAlign && i + k < size && j + k < size; ++k) {
                    if (tab[i + k][j + k] == pionHuman) {
                        align++;
                    }
                }
                if (align == requiredAlign) return true;

                // Vérifie l'alignement diagonal (haut-droit vers bas-gauche)
                align = 0;
                for (int k = 0; k < requiredAlign && i + k < size && j - k >= 0; ++k) {
                    if (tab[i + k][j - k] == pionHuman) {
                        align++;
                    }
                }
                if (align == requiredAlign) return true;
            }
        }
    }

    return false; // Aucun alignement proche de la victoire n'a été trouvé
}


int evaluatePosition(char** tab, int boardSize, int nbpion){
    int score = 0;

    return score;
}

*/

int minimax(char** tab, int boardSize, int K, int depth, int alpha, int beta, bool isMaximizing){
    bool resultAI = victoire_morpion(tab, boardSize, K, pionAI);
    bool resultHuman = victoire_morpion(tab, boardSize, K, pionHuman);
    bool resultTie = isTie(tab, boardSize);
    int score = 0;

    // End condition
    if (resultAI == true || resultHuman == true || resultTie == true)
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
            
            //std::cout << "(" << i << ", " << j << ") : " << tab[i][j] << std::endl;
            
            // if the spot is available
            if (tab[i][j] == ' ')
            {
                tab[i][j] = pion;
                
                score = minimax(tab, boardSize, K, depth, alpha, beta, false);
                
                //std::cout << "score : " << score << "\n" << std::endl;
                
                tab[i][j] = ' ';
                if (score > bestScore)
                {
                    bestScore = score;
                    move.x = i;
                    move.y = j;
                }

                std::cout << "oui" << std::endl;
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