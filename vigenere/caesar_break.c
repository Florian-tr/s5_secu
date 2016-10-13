#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "array.h"
#include "ceasar_func.h"


int main(int argc, char *argv[]) {
	
	FILE * entree;
	FILE * sortie;
	
	if(argc != 3){
		printf("USAGE : Vous devez rentrer deux paramètres : le fichier chifré, suivi du fichier de sortie\n");
		exit(EXIT_FAILURE);
	}
	else{
		entree = fopen(argv[1], "r");
		if(!entree){
			printf("Erreur lors de l'ouverture du fichier chifré...\n");
			exit(EXIT_FAILURE);
		}
		sortie = fopen(argv[2], "w");
		if(!sortie){
			printf("Erreur lors de l'ouverture du fichier de sortie...\n");
			exit(EXIT_FAILURE);
		}
	}
	
	
	//int indice = 0;
	int freq[26];
	for(int j = 0; j < 26; j++){
		freq[j] = 0;
	}
	int i;
	while ((i = fgetc(entree)) != EOF) {
		
		if(i >= 65 && i <= 90){
			freq[i - 65]++;
			
		}
	}
	
	/*int ind_max_freq = 0; //Indice de la lettre ayant la fréquence maximum
	for(int i = 0; i <26; i++){
		//printf("%c : %d\n", i+65, freq[i]);
		if(freq[i] > freq[ind_max_freq]){
			ind_max_freq = i;
		}
	}*/
	
	size_t ind_max_freq = indice_lettre_freq_max(freq);
			
	printf("Indice_max_freq: %zu\n", ind_max_freq+1);
	int indice = (ind_max_freq+1)-5;//POur A : -1
	if(indice < 0){
		indice += 26;
	}
	
	
	
	printf("Indice : %d\n", indice);
	
	
	char ind[3];
	sprintf (ind, "%i", indice);
   //printf("Ind : %s \n", ind);
	char *arg[] = { "cesar_dechifr", ind, argv[1], argv[2], NULL };
	char *env[] = { NULL };
	
	int resFork;
    
	//-- Create a new process --//
	resFork = fork();
	
	//-- No son Detected --//
	if(resFork < 0){
		perror("fork");
		exit(EXIT_FAILURE);
	}

	//-- We are in the son --//
	if(resFork == 0){
		
			
	//-- Covers the process by the command --//
	(void)execve("cesar_dechifr", arg, env);
	perror("Bad command");//<-- If execvp Fails, go throught there and display perror
			
	exit(3);//<-- And return, the return code 3
		
	}
	else{
		//Fermeture des descripteurs de fichiers ouvert
		if(entree != stdin){
			fclose(entree);
		}
		if(sortie != stdout){
			fclose(sortie);
		}	
		printf("Vous pouvez retrouver votre texte en claire dans le fichier \"%s\". \n",argv[2]);
		
			
	}
	
	
	
	return 0;
}
