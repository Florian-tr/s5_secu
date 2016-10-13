#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	
	FILE * entree = stdin;
	FILE * sortie = stdout;
	
	if(argc != 4 && argc != 2){
		printf("USAGE : Il faut au moins rentrer l'indice de décalage souhaiter.\n Si vous ne rentrez aucun autre argument, c'est l'entrée standard qui sera chiffré et affiché par la suite sur la sortie strandard.\n Sinon, rentrez trois paramètres : l'indice de décalage, le fichier d'entrée, et pour finir, le fichier de sortie\n");
		exit(EXIT_FAILURE);
	}
	else{
		if(argc != 2){
			entree = fopen(argv[2], "r");
			if(!entree){
				printf("Erreur lors de l'ouverture du fichier d'entrée...\n");
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
	int chifr;
	
	
	int i;
	while ((i = fgetc(entree)) != EOF) {
		if(i >= 65 && i <= 90){
			chifr = i + indice;
			int reste = (chifr % 91);
			if(reste < 65){
				chifr = reste + 65;
			}
		}
		else{
		//Si ce n'es pas une majuscule, alors on renvoie le caractère tel quel
			chifr = i;
		}
		
		
		unsigned char c = (unsigned char) chifr;
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
