#include <stdio.h>
#include <stdlib.h>


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
	
	
	int chifr;
	char * cle = argv[1];
	size_t tailleCle = 0;
	
	while(cle[tailleCle] != '\0'){
		tailleCle++;
	}
	
	int compteurLettre = 0;
	int i;
	while ((i = fgetc(entree)) != EOF) {
		if(i >= 65 && i <= 90){
			chifr = i + (cle[compteurLettre % tailleCle] - 65);
			
			int reste = (chifr % 91);
			if(reste < 65){
				chifr = reste + 65;
			}
			
			compteurLettre++;
			
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
