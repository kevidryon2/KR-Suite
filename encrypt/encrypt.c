#include <stdint.h>

void sbox(char *data, uint32_t key) {
	
}

void rnd(char *data, uint32_t key) {
	
}

void add(char *data, uint32_t key) {
	
}

void encrypt(char *data, uint32_t key, char *type) {
	for (int i=0; i<strlen(type); i++) {
		switch (type[i]) {
			case 's': sbox(data, key); break;
			case 'r': rnd(data, key); break;
			case 'a': add(data, key); break;
		}
	}
}
