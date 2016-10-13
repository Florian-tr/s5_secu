#include <stdio.h>
#include <stdlib.h>
#include "subst_func.h"


int main(int argc, char *argv[]) {
	
	FILE * entree = stdin;
	FILE * sortie = stdout;
	
	if(argc != 4 && argc != 2){
		printf("USAGE : Il faut au moins passer la clé de chiffrement. Si vous ne passer rien d'autre, c'est l'entrée standard qui sera déchifrée et affiché par la suite sur la sortie strandard.\n Sinon, rentrez trois paramètres : la clé de chiffrement , le fichier chifré, suivi du fichier de sortie");
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
	
	
	
	char * cle = argv[1];
	int *cle26 = alphabet_subs(cle);
	
	int cle_inverse[26];
	
	for(int i = 0; i < 26; i++){
		cle_inverse[cle26[i] - 65] = i+65;
		//printf("%c", i+65);
	}
	
	/*printf("\n");
	for(int e = 0; e < 26; e++){
		printf("%c", cle26[e]);
	}
	printf("\n");
	for(int d = 0; d < 26; d++){
		printf("%c", cle_inverse[d]);
	}
	printf("\n");*/
	
	int dechifr;
	int i;
	while ((i = fgetc(entree)) != EOF) {
		if(i >= 65 && i <= 90){
			dechifr = cle_inverse[i-65];
			
			
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
	
	free(cle26);
	
	return 0;
}
