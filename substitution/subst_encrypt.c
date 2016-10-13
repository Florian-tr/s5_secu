#include <stdio.h>
#include <stdlib.h>
#include "subst_func.h"



int main(int argc, char *argv[]) {
	
	FILE * entree = stdin;
	FILE * sortie = stdout;
	
	if(argc != 4 && argc != 2){
		printf("USAGE : Il faut au moins rentrer la clé de chiffrement souhaitée.\n Si vous ne rentrez aucun autre argument, c'est l'entrée standard qui sera chiffré et affiché par la suite sur la sortie strandard.\n Sinon, rentrez trois paramètres : la clé de chiffrement, le fichier d'entrée, et pour finir, le fichier de sortie\n");
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
	
	
	
	char * cle = argv[1];
	int *cle26 = alphabet_subs(cle);
	
	/*for(int d = 0; d < 26; d++){
		printf("%c", cle26[d]);
	}
	printf("\n");*/
	
	int chifr;
	int i;
	while ((i = fgetc(entree)) != EOF) {
		if(i >= 65 && i <= 90){
			chifr = cle26[i-65];
			
			
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
	
	free(cle26);
	
	return 0;
}
