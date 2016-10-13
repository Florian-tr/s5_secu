#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "array.h"
#include "ceasar_func.h"

long double indice_co(int *freq, long double nb_lettres_texte){
	//printf("Nb de lettres : %Lf\n", nb_lettres_texte);
	long double indice_coincidence = 0.0;
	for(int i = 0; i <26; i++){
		//printf("Nb de %c : %Lf\n", i+65, freq[i]);
		//printf("(freq[i] * (freq[i] - 1)) / (nb_lettres_texte * (nb_lettres_texte - 1)) = %Lf\n",(freq[i] * (freq[i] - 1)) / (nb_lettres_texte * (nb_lettres_texte - 1)));
		indice_coincidence += (freq[i] * (freq[i] - 1)) / (nb_lettres_texte * (nb_lettres_texte - 1));
	}
	return indice_coincidence;
}	

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
	
	
	long double nb_lettres_texte = 0;
	int freq[26];
	
	struct array tab_texte;
	array_create(&tab_texte);
	
	
	for(int j = 0; j < 26; j++){
		freq[j] = 0;
	}
	
	int i;
	while ((i = fgetc(entree)) != EOF) {
		
		if(i >= 65 && i <= 90){
			freq[i - 65]++;
			nb_lettres_texte++;
			array_add(&tab_texte, i);
		}
	}
	
	//printf("Nb de lettres : %Lf\n", nb_lettres_texte);
	
	long double indice_coincidence = indice_co(freq, nb_lettres_texte);
	printf("Indice de Coincidence : %Lf\n", indice_coincidence);
	
	//Si on à juste utiliser César :
	if(indice_coincidence > 0.07){
		
		char *arg[] = { "caesar_break", argv[1], argv[2], NULL };
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
			(void)execve("caesar_break", arg, env);
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
			
			array_destroy(&tab_texte);
			
			printf("Vous pouvez retrouver votre texte en claire dans le fichier \"%s\". \n",argv[2]);
		
			
		}
	}
	else{
		size_t taille_cle = 2;
		bool boucle;
		
		
		do{
			boucle = false;
			
			//Si k = taille_cle alors on etablit k message m1...mk à partir de l'original M où on prend une lettre sur k en commencant à la ieme
			for(int i = 0; i < taille_cle; i++){
								
				int freq_t_c[26];
				
				for(int j = 0; j < 26; j++){
					freq_t_c[j] = 0;
				}
						
				long double nb_lettres_tex_t_c = 0.0;
				
				//on prend une lettre sur k en commencant à la ieme
				for(size_t j = i; j < tab_texte.size; j += taille_cle){
				
					//printf("La clé est de %zu, et j est de %zu!\n", taille_cle, j);
					freq_t_c[tab_texte.data[j] - 65]++;
					nb_lettres_tex_t_c++;
					
				}
				if(indice_co(freq_t_c, nb_lettres_tex_t_c) < 0.06){
					boucle = true;
					break;
				}
			}
			
			
			taille_cle++;
		}while(boucle);
		
		printf("La taille de la clé est de %zu.\n", taille_cle - 1);
		
		char *cle = calloc(taille_cle, sizeof(char));
		//char cle[taille_cle]; //Je sais pas pourquoi ca marche pas...
		
		//Si k = taille_cle alors on etablit k message m1...mk à partir de l'original M où on prend une lettre sur k en commencant à la ieme
		for(int i = 0; i < taille_cle-1; i++){
								
			int freq_t_c[26];
			for(int j = 0; j < 26; j++){
				freq_t_c[j] = 0;
			}
			
			//on prend une lettre sur k en commencant à la ieme et on calcule sa fréquence
			for(size_t j = i; j < tab_texte.size; j += taille_cle-1){
				freq_t_c[tab_texte.data[j] - 65]++;
				
				/*if(i == 4){
					printf("%c", tab_texte.data[j]);
				}*/
				
			}
						
			/**** DEBUT CESAR ****/
			size_t ind_max_freq = indice_lettre_freq_max(freq_t_c);//voir cesar_break
			
			printf("Indice_max_freq: %zu\n", ind_max_freq+1);
			int indice = (ind_max_freq+1)-5;//POur A : -1
			if(indice < 0){
				indice += 26;
			}
			
			
			/**** FIN CESAR ****/
			
			char carac_cle = indice+65;
			//printf("Charactere %c;\n", carac_cle);
			cle[i] = carac_cle;
			
		}
		
		printf("La clé est %s.\n", cle);
		
		char *arg[] = { "vigenere_dechifr", cle, argv[1], argv[2], NULL };
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
			(void)execve("vigenere_dechifr", arg, env);
			perror("Bad command");//<-- If execvp Fails, go throught there and display perror
			
			exit(3);//<-- And return, the return code 3
		}
		else{
			free(cle);
			array_destroy(&tab_texte);
			printf("Vous pouvez retrouver votre texte en claire dans le fichier \"%s\". \n",argv[2]);
		}
			
		
	}
	
	
	
	return 0;
}
