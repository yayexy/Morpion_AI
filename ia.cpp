#include <iostream>
#include "morpion.h"

bool defendre(char** tab, int size, int nbpion, char pion) {

	for(int i = 0; i < size; ++i) {
	
		for(int j = 0; j < size; ++j) if (tab[i][j] == pion) {
		
			int align = 0, i2, j2; 
			
			i2 = i; align = 0;
			while( i2 < size && (i2-i) < nbpion ) { 
                align = align + (tab[i2][j] == pion); i2++; 
                }
			if(align == nbpion-1) return true;
			
			j2 = j; align = 0;
			while( j2 < size && (j2-j) < nbpion ) { 
                align = align + (tab[i][j2] == pion); j2++; 
                }
			if(align == nbpion-1) return true;
			
			j2 = j; i2 = i; align = 0;
			while( i2 < size && (j2-j) < nbpion && j2 < size && (j2-j) < nbpion) { 
                align = align + (tab[i2][j2] == pion); i2++; j2++; 
                }
			if(align == nbpion-1) return true;
			
			j2 = j; i2 = i; align = 0;
			while( i2 < size && (j2-j) < nbpion && j2 >=0 && (j-j2) < nbpion) { 
                align = align + (tab[i2][j2] == pion); i2++; j2--; 
                }
			if(align == nbpion-1) return true;
			
		}
	}
	
	return false;
}

void jouerX(char** tab, int N, int K, char pion){
    int x = -1;
    int y = -1;
    std::cout << "\nIA" << std::endl;

    if (defendre(tab, N, K, 'O'))
    {
        std::cout << "\nIl est a 2 doigts de gagner la partie" << std::endl;
    }
    
    do
    {
        std::cin >> x >> y;
        std::cout<<std::endl;    
    } while (!estLibre_morpion(tab, N, x, y));

    placer_morpion(tab, N, x, y, pion);
}