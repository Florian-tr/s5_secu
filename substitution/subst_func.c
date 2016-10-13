#include "subst_func.h"

int nextChar(int charac){
	int nextchara = (charac + 1) % 91;
	if(nextchara < 65){
		nextchara += 65;
	}
	
	return nextchara;
}

int *alphabet_subs(char * cle){
	
	int *cle26 = calloc(26, sizeof(int));
	int freq[26];
	size_t tailleCle = 0;
	size_t tailleCle26 = 0;
	
	for(size_t y = 0; y < 26; y++){
		cle26[y] = -1;
		freq[y] = 0;
	}
	
	while(cle[tailleCle] != '\0'){
		if(freq[cle[tailleCle]-65] == 0){
			freq[cle[tailleCle]-65] = 1;
			cle26[tailleCle26] = cle[tailleCle];
			tailleCle26++;
		}
		tailleCle++;
	}
	
	int newChar = nextChar(cle26[tailleCle26-1]);
	
	while(tailleCle26 < 26){
		if(freq[newChar - 65] == 0){
			cle26[tailleCle26] = newChar;
			tailleCle26++;
		}
		newChar = nextChar(newChar);
	}
	
	return cle26;
}
