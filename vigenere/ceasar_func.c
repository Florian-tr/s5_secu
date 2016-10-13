#include "ceasar_func.h"

size_t indice_lettre_freq_max(int freq[26]){
	size_t ind_max_freq = 0;
	struct array tab_lette_freq_equivalente;
	array_create(&tab_lette_freq_equivalente);
	
	array_add(&tab_lette_freq_equivalente, 0);
	
	for(size_t l = 0; l < 26; l++){
		//printf("%c : %Lf\n", i+65, freq[i]);
		if(freq[l] > freq[ind_max_freq]){
			ind_max_freq = l;
			tab_lette_freq_equivalente.size = 0;
			array_add(&tab_lette_freq_equivalente, l);
		}
		else{
			
			if(freq[l] == freq[ind_max_freq]){
				array_add(&tab_lette_freq_equivalente, l);
			}
		}
		
	}
			
	if(tab_lette_freq_equivalente.size > 1){
		//On creer un tableau de même taille pour ranger la somme des fréquences pour ce qui correpondrait à la lettre e , a et i
		int * somme_freq_aie = calloc(tab_lette_freq_equivalente.size, sizeof(int));
				
				
		for(size_t k = 0; k < tab_lette_freq_equivalente.size; k++){
			size_t indice_r = tab_lette_freq_equivalente.data[k];
			//					correspond à "e"		correspond à "i"
			somme_freq_aie[k] = freq[indice_r] + freq[(indice_r + 4) % 26];
			
			if((indice_r - 4) < 0){
				indice_r += 26;
			}
					
			//						correspond à "a"
			somme_freq_aie[k] += freq[indice_r - 4];
		}
				
		//On doit récuperer celui ou la somme est la plus grande car on peut considerer que c'est ce qui remplace réellement le "e"
		size_t memoir_case_plu_grande = 0;
		for(size_t s = 0; s < tab_lette_freq_equivalente.size; s++){
			if(somme_freq_aie[s] > somme_freq_aie[memoir_case_plu_grande]){
				memoir_case_plu_grande = s;
			}
		}
				
		ind_max_freq =  tab_lette_freq_equivalente.data[memoir_case_plu_grande];
				
		free(somme_freq_aie);
	}
	else{
		ind_max_freq = tab_lette_freq_equivalente.data[0];
		//printf("YOUOUOUOUOU\n");
		
	}
	
	return ind_max_freq;
}
