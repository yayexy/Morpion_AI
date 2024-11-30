#include "morpion.h"
#include "ia.h"

int main() {

	char ** tab;
	int size = 5;
	int nbPion = 3;
	
	init_morpion(tab,size);
	
	bool alignement = false;
	bool joueur = false;
	
	int nbPl = 0;
	
	while(!alignement && nbPl < size*size) {
		
		if (nbPl == 0)
		{
			affichage_morpion(tab, size);
		}
	
		char pion;
		(joueur ? pion = 'X' : pion = 'O');
		joueur = not(joueur);
		
		int x = -1; 
		int y = -1;
		
		
		if (!joueur){
			jouerX(tab, size, nbPion, pion);
		}
		else{
			do {
				std::cin >> x >> y;
				std::cout<<std::endl;
			} while(!estLibre_morpion(tab,size,x,y));
		
			placer_morpion(tab,size,x,y,pion);
		}
		
		affichage_morpion(tab,size);
	
	
		if(pion == 'O') alignement = victoire_morpion(tab,size,nbPion,'O');
		else if(pion == 'X') alignement = victoire_morpion(tab,size,nbPion,'X');
		nbPl++; 
	}

	delete_morpion(tab,size);
}
