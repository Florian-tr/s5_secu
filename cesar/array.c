#include "array.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


void array_create(struct array *self) {
	self->size = 0;
	self->capacity = 10;
	self->data = calloc(self->capacity, sizeof(int));
}

void array_destroy(struct array *self) {
	free(self->data);
	self->size = 0;
	self->capacity = 10;
}

bool array_equals(const struct array *self, const int *content, size_t size) {
	if(array_size(self) != size){
		return false;
	}
	for(int i=0; i < size; i++){
		if(self->data[i] != content[i]){
			return false;
		}
	}
	return true;
}

void array_add(struct array *self, int value) {
	if(self->size == self->capacity){
		
		self->capacity = self->capacity * 2;
		int * newArray = calloc(self->capacity, sizeof(int));
		
		memcpy(newArray, self->data, self->size * sizeof(int));
		free(self->data);
		self->data = newArray;
	}
	
	self->data[self->size] = value;
	self->size = self->size + 1;
}

void array_insert(struct array *self, int value, size_t index) {
	if(self->size == self->capacity){
		
		self->capacity = self->capacity * 2;
		int * newArray = calloc(self->capacity, sizeof(int));
		
		memcpy(newArray, self->data, self->size * sizeof(int));
		free(self->data);
		self->data = newArray;
	}
	
	
	
	for(size_t i = self->size; i > index; i--){
		self->data[i] = self->data[i-1];
	}
	
	self->data[index]= value;
	self->size = self->size + 1;
}

void array_remove(struct array *self, size_t index) {
	for(size_t i = index; i < self->size; i++){
		self->data[i] = self->data[i+1];
	}
	
	self->size = self->size - 1;
}

int *array_get(const struct array *self, size_t index) {
	if(self->size == 0){
		return NULL;
	}
	return self->data + index;
}

bool array_is_empty(const struct array *self) {
	if(self->size == 0){
		return true;
	}
	return false;
}

size_t array_size(const struct array *self) {
	return self->size;
}

size_t array_search(const struct array *self, int value) {
	size_t i = 0;
	
	while(i < self->size && self->data[i] != value){
		i++;
	
	}
	
	return i;
}

size_t array_search_sorted_dichotomic(const struct array *self, int value, int lo, int hi){
	
	if(lo == hi){
		return self->size;
	}
	
	size_t med = (lo + hi) / 2;
	
	if(value < self->data[med]){
		return array_search_sorted_dichotomic(self, value, lo, med);
	}
	
	if(value > self->data[med]){
		return array_search_sorted_dichotomic(self, value, med+1, hi);
	}
	
	return med;
}

size_t array_search_sorted(const struct array *self, int value) {
	return array_search_sorted_dichotomic(self, value, 0, self->size);
}

void array_import(struct array *self, const int *other, size_t size) {
	self->size = 0;
	
	for(size_t i = 0; i < size; i++){
		array_add(self, other[i]);
	} 
}

void array_dump(const struct array *self) {

}

bool array_is_sorted(const struct array *self) {
	if(self->size == 0){
		return true;
	}
	
	for(size_t i = 0; i < self->size - 1; i++){
		if(self->data[i] > self->data[i+1]){
			return false;
		}
	}
	return true;
}

void swap(struct array * self, size_t indice1, size_t indice2){
	int temp = self->data[indice1];
	
	 self->data[indice1] = self->data[indice2];
	 self->data[indice2] = temp;
}

void array_swap(int *data, size_t i, size_t j) {
	int tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

void array_selection_sort(struct array *self) {
	for(size_t i =0; i < self->size-1; i++){
		size_t minInd = i;
		
		for(size_t k=minInd+1; k < self->size; k++){
			if(self->data[k] < self->data[minInd]){
				minInd = k;
			}
		}
		
		swap(self, i, minInd);
	}
}

void array_bubble_sort(struct array *self) {
	for(size_t i = 0; i < self->size-1; i++){
		
		for(size_t j = self->size-1; j > i; j--){
			
			if(self->data[j] < self->data[j-1]){
				swap(self, j, j-1);
			}
			
		}
		
	}
}

void array_insertion_sort(struct array *self) {
	for(size_t i = 0; i < self->size; i++){
		
		int val = self->data[i];
		size_t j = i;
		
		while(j > 0 && self->data[j-1] > val){
			self->data[j] = self->data[j-1];
			j--;
		}
		
		self->data[j] = val;
	}
}


int array_sort_partition(struct array *self, size_t lo, size_t hi){
	int pivot_index = ( self->data[lo] > self->data[hi] )? lo : hi;
	
	int pivot = self->data[pivot_index];
	
	swap(self, pivot_index, hi);
	
	int newPosPiv = lo;
	
	for(int k = lo; k < self->size; k++){
		if(self->data[k] < pivot){
			swap(self, k, newPosPiv);
			newPosPiv++;
		}
	}
	swap(self, hi, newPosPiv);
	return newPosPiv;
}


void array_quick_sort_partiel(struct array *self, size_t lo, size_t hi){
	if(lo < hi){
		int p = array_sort_partition(self, lo, hi);
		
		array_quick_sort_partiel(self, lo, p-1);
		array_quick_sort_partiel(self, p+1, hi);
	}
}

void array_quick_sort(struct array *self) {
	array_quick_sort_partiel(self, 0, self->size-1);
}


void heap_insert(int *tas, size_t n, int value) {
	size_t i = n;
	
	tas[i] = value;
	
	while (i > 0) {
		int j = (i - 1) / 2;
		
		if (tas[i] < tas[j]) {
			break;
		}
		
		array_swap(tas, i, j);
		i = j;
	}
}


void heap_remove(int *tas, size_t n) {
	tas[0] = tas[n - 1];
	
	size_t i = 0;
	
	while (i < (n - 1) / 2) {
		size_t lt = 2 * i + 1;
		size_t rt = 2 * i + 2;
		
		if (tas[i] > tas[lt] && tas[i] > tas[rt]) {
			break;
		}
		
		size_t j = (tas[lt] > tas[rt]) ? lt : rt;
		
		array_swap(tas, i, j);
		i = j;
	}
}

void array_heap_sort(struct array *self) {
	for (size_t i = 0; i < self->size; ++i) {
		
		int value = self->data[i];
		heap_insert(self->data, i, value);
		
	}
	
	for (size_t i = 0; i < self->size; ++i) {
		
		int value = self->data[0];
		heap_remove(self->data, self->size - i);
		self->data[self->size - i - 1] = value;
		
	}
}


bool array_is_heap(const struct array *self) {
  if(self->data == NULL){
  	return false;
  }
  
  size_t mid = self->size/2;
  
  for(size_t i = 0; i < self->size; i++){
  	if(self->data[(i-1)/2] && self->data[i] > self->data[(i-1)/2]){
		return false;
  	}
  	/*if(self->data[(i-2)/2] && self->data[i] < self->data[(i-2)/2]){
	  		return false;
	}*/
  }
  
  return true;
}

void array_heap_add(struct array *self, int value) {
	size_t i = self->size;
	
	array_add(self, value);
	
	while (i > 0) {
		
		size_t j = (i - 1) / 2;
		
		if (self->data[i] < self->data[j]) {
			break;
		}
		
		array_swap(self->data, i, j);
		
		i = j;
	}
}

int array_heap_top(const struct array *self) {
	if(self->data == NULL){
		return 0;
	}
	return self->data[0];
}

void array_heap_remove_top(struct array *self) {

	if(self->size > 0){
		heap_remove(self->data, self->size);
		self->size--;
	}
	
	
}
