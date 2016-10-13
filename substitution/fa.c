#include <string.h>
#include <stdbool.h>
#include "fa.h"
#include "state.h"


/**
 * \file fa.c
 * \brief Programme de tests.
 * \author Florian GILLOT
 * \version 0.1
 * \date 6 septembre 2016
 *
 * Programme de test skjertheiestion des chaînes de caractères Str_t.
 *
 */


//___________________________________________________________________
/**
 * \fn void fa_create(struct fa *self, size_t alpha_count, size_t state_count)
 * Fonction qui crée un automate
 *
 * \param self	Pointeur sur un obbjet de type fa
 * \param alpha_count	\e size_t	nombre de lettre composant le language reconnu par cet automate
 * \param state_count	\e size_t	nombre d'états composant l'automate
*/
void fa_create(struct fa *self, size_t alpha_count, size_t state_count) {
	self->alpha_count = alpha_count;
	self->state_count = state_count;
	self->states = calloc(self->state_count, sizeof(struct state));

	for(size_t i = 0; i < self->state_count; i++){
		self->states[i].is_initial = false;
		self->states[i].is_final = false;
	}

	/*self->transitions = calloc(self->state_count * self->alpha_count, sizeof(struct state_set));


    for(size_t i = 0; i < self->state_count; i++){
    	for(size_t j = 0; j < self->alpha_count; j++){
    		state_set ss;
			ss.size = 0;
			ss.capacity = 10;
			ss.states = calloc(ss.capacity, sizeof(size_t));
    		self->transitions[i][j] = ss;
    	}
    }*/

    self->transitions = calloc(self->state_count, sizeof(struct state_set*));
	for(size_t i = 0; i < self->state_count; i++){
    	self->transitions[i] = calloc(self->alpha_count, sizeof(struct state_set));

    	for(size_t j = 0; j < self->alpha_count; j++){
    		self->transitions[i][j].size = 0;
    		self->transitions[i][j].capacity = 10;
    		self->transitions[i][j].states = calloc(self->transitions[i][j].capacity, sizeof(size_t));
    	}
    }


}


//___________________________________________________________________
/**
 * Fonction qui détruit un automate
 *
 * \param self	Pointeur sur un obbjet de type fa
*/
void fa_destroy(struct fa *self){
	free(self->states);

	for(size_t i = 0; i < self->state_count; i++){
    	for(size_t j = 0; j < self->alpha_count; j++){
    		free(self->transitions[i][j].states);
    	}
    	
    }

    free(self->transitions);


}


void trans_add(struct state_set *self, size_t value) {
	printf("Ma capacité : %zu \n", self->capacity);
	printf("Ma taille : %zu \n", self->size);
	
	//Si le nombre de transition est égale à la capacité du tableau, alors on double la capacité du tableau
	if(self->size == self->capacity){

		self->capacity = self->capacity * 2;
		size_t * newArray = calloc(self->capacity, sizeof(size_t));

		memcpy(newArray, self->states, self->size * sizeof(size_t));
		free(self->states);
		self->states = newArray;
	}
	
	//Si le tableau est vide ou que la dernière transition enregistrée est "inférieur" à celle que l'on veut ajouter, alors on ajoute directement
	if(self->size == 0 || (value > self->states[self->size-1])){
		self->states[self->size] = value;
	}
	else{
		size_t index_a_inser;
		//On cherche l'indice ou inserer
		for(size_t j = 0; j < self->size; j++){
			if(self->states[j] > value){
				index_a_inser = j;
				break;
			}
			else{
				if(self->states[j] == value){
					index_a_inser = -1;
					break;
				}
			}
		
		}
		
		//Si la valeur est à -1 alors ca veut dire qu'on a déjà cette transition dans notre tableau et donc on l'ajoute pas...
		if(index_a_inser != -1){
			for(size_t i = self->size; i > index_a_inser; i--){
				self->states[i] = self->states[i-1];
			}
	
			self->states[index_a_inser]= value;
		}
		else{
			return;
		}
	}
	
	
	
	self->size = self->size + 1;
}


void trans_remove(struct state_set *self, size_t value) {
	
	if(self->size > 0){
		size_t index = -1;
	
		for(size_t j = 0; j < self->size; j++){
			if(self->states[j] == value){
				index = j;
				break;
			}
		}
		
		if(index != -1){
			for(size_t i = index; i < self->size; i++){
				self->states[i] = self->states[i+1];
			}
	
			self->size = self->size - 1;
		}
		
	}
}

void fa_set_state_initial(struct fa *self, size_t state){
	self->states[state].is_initial = true;
}

void fa_set_state_final(struct fa *self, size_t state){
	self->states[state].is_final = true;
}

void fa_add_transition(struct fa *self, size_t from, char alpha, size_t to){
	size_t charac = alpha - 'a';
	trans_add(&self->transitions[from][charac], to);
}

void fa_remove_transition(struct fa *self, size_t from, char alpha, size_t to){
	size_t charac = alpha - 'a';
	trans_remove(&self->transitions[from][charac], to);
}


void fa_remove_state(struct fa *self, size_t state){
	for(size_t i = 0; i < self->alpha_count;  i++){
		for(size_t j = 0; j < self->transitions[state][i].size; j++){
			fa_remove_transition(self, state, i + 'a', self->transitions[state][i].states[j]);
		}
		
		self->transitions[state][i].size = 0;
		self->transitions[state][i].capacity = 10;
		
		//free(self->transitions[state][i].states);
		
	}
	
	for(size_t i = 0; i < self->state_count; i++){
    	for(size_t j = 0; j < self->alpha_count; j++){
    		fa_remove_transition(self, i, j + 'a', state);
    	}
    }
}


size_t fa_count_transitions(const struct fa *self){
	
	size_t nb_transitions = 0;
	
	for(size_t i = 0; i < self->state_count; i++){
		for(size_t j = 0; j < self->alpha_count; j++){
			
			size_t nbEFT = self->transitions[i][j].size;
			for(size_t k = 0; k < nbEFT; k++){
				nb_transitions++;
			}
    	}
    }
    
    return 	nb_transitions;
    	
}


bool fa_is_deterministic(const struct fa *self){
	size_t nb_init_state = 0;
	for(size_t i = 0; i < self->state_count; i++){
		
		//Si l'automate à plus d'un état initial, alors il n'est pas déterministe...
		if(true == self->states[i].is_initial){
			nb_init_state++;
			if(nb_init_state > 1){
				return false;
			}
		}
		
		
		for(size_t j = 0; j < self->alpha_count; j++){
			
			if(1 < self->transitions[i][j].size){
				return false;
			}
    	}
    }
    
    return true;
}


bool fa_is_complete(const struct fa *self){
	
	for(size_t i = 0; i < self->state_count; i++){
		
		for(size_t j = 0; j < self->alpha_count; j++){
			
			if(1 != self->transitions[i][j].size){
				return false;
			}
    	}
    }
    
    return true;
}


void fa_make_complete(struct fa *self){

}








void fa_pretty_print(const struct fa *self, FILE *out){

	if(NULL == out){
		printf("Votre fichier de sortie est NULL.....");
		exit(EXIT_FAILURE);
	}

	fprintf(out, "Initial states : \n	");
	for(size_t i = 0; i < self->state_count; i++){
		if(true == self->states[i].is_initial){
			fprintf(out, "%zu  ",i);
		}
	}
	fprintf(out,"\n");


	fprintf(out, "Final states : \n	");
	for(size_t i = 0; i < self->state_count; i++){
		if(true == self->states[i].is_final){
			fprintf(out, "%zu  ",i);
		}
	}
	fprintf(out,"\n");


	fprintf(out, "Transititons : \n");
	for(size_t i = 0; i < self->state_count; i++){
		fprintf(out, "For state %zu : \n", i);
    	for(size_t j = 0; j < self->alpha_count; j++){
			fprintf(out, "	For lettre %c : ", j+'a');

			size_t nbEFT = self->transitions[i][j].size;
			for(size_t k = 0; k < nbEFT; k++){
				fprintf(out, "%zu ", self->transitions[i][j].states[k]);
			}
			fprintf(out, "\n");
    	}
    }
}

void fa_dot(const struct fa *self, FILE *out){
	if(NULL == out){
		printf("Votre fichier de sortie est NULL.....");
		exit(EXIT_FAILURE);
	}

	fprintf(out, "digraph finite_state_machine { \n\trankdir=LR;\n\tsize=\"10,5\"\n\t");
	fprintf(out, "node [shape = doublecircle]; ");

	for(size_t i = 0; i < self->state_count; i++){
		if(true == self->states[i].is_final){
			fprintf(out, "%zu  ",i);
		}
	}

	fprintf(out,"\n\tnode [shape = circle];\n\t");

	for(size_t i = 0; i < self->state_count; i++){

	    for(size_t j = 0; j < self->alpha_count; j++){

		size_t nbEFT = self->transitions[i][j].size;
		for(size_t k = 0; k < nbEFT; k++){
			fprintf(out, "%zu -> %zu [ label = \"%c\" ];\n\t", i, self->transitions[i][j].states[k], j+'a');
		}
		
		

	    }
	}
	fprintf(out, "}");
}
