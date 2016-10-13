#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	
	FILE * entree = stdin;
	FILE * sortie = stdout;
	
	if(argc != 4 && argc != 2){
		printf("USAGE : Il faut au moins passer l'indice en argument. Si vous ne passer rien d'autre, c'est l'entrée standard qui sera déchifrée et affiché par la suite sur la sortie strandard.\n Sinon, rentrez trois paramètres : l'indice , le fichier chifré, suivi du fichier de sortie");
		exit(EXIT_FAILURE);
	}
	else{
		if(argc != 2){
			entree = fopen(argv[2], "r");
			if(!entree){
				printf("Erreur lors de l'ouverture du fichier chiffré...\n");
				exit(EXIT_FAILURE);
			}
			sortie = fopen(argv[3], "w");
			if(!sortie){
				printf("Erreur lors de l'ouverture du fichier de sortie...\n");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	
	
	int indice = atoi(argv[1]);
	int dechifr;
	
	
	int i;
	while ((i = fgetc(entree)) != EOF) {
		if(i >= 65 && i <= 90){
			dechifr = i - indice;
			
			if(dechifr < 65){
				dechifr+=26;
			}
		}
		else{
		//Si ce n'es pas une majuscule, alors on renvoie le caractère tel quel
			dechifr = i;
		}
		
		
		unsigned char c = (unsigned char) dechifr;
		fwrite(&c, sizeof(unsigned char), 1, sortie);
	}
	
	
	//Fermeture des descripteurs de fichiers ouvert
	if(entree != stdin){
		fclose(entree);
	}
	if(sortie != stdout){
		fclose(sortie);
	}
	
	return 0;
}
