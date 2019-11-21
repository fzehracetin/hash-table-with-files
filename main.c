#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100
#define MIN 100
#define R 2
#define MM 28


int hash (long long int key, int i, long long int M) {
	return (key % M) + i*(1 + key % MM);
}

void insert (int key, int* ht, long long int M, int* flag) {
	int adr, i = 0;
	adr = hash(key, i, M);
	
	while( ( ht[adr])  && (i < M) ) {
		i++;
		adr = hash(key, i, M);
	}
	
	if ( !ht[adr] ) {
		ht[adr] = key;
		flag[adr] = 0;
		printf("%d inserted %dth address. \n", key, adr);
	} 
	else {
		printf("Table is full! \n");
	}
}

void file_hasher(char* fname, char* dir, long long int M, int* ht, int* flag) {
	char dummy[MAX], ch;
	int n = 0, i = 0;
	long long int key = 0;
	ch = fname[i];
	while( (ch != '\0') && (ch != '\n') ) { // \n varsa temizle
		i++;
		ch = fname[i];
	}
	if (ch == '\n') 
		fname[i] = '\0';
	
	strcpy(dummy, dir);
	strcat(dummy, fname);
	FILE* f1 = fopen(dummy, "r");
	if (f1 == NULL){
		printf("%s could not opened!!\n", dummy);
	}
	
	ch = getc(f1);
	while( ch != EOF) { // file uzunlugunu olc
		n++; 
		ch = getc(f1);
	}
	printf("%s dosyasi uzunlugu: %d \n", fname, n);
	
	fseek(f1, 0, SEEK_SET); 
	ch = getc(f1);
	i = 0;
	while( ch != EOF) { 
		//printf("%d %lld \n", ch, (long long int) powl(R, n - i - 1));
		//printf("%lld \n", (long long int) ch * (long long int) powl(R, n - i - 1));
		key += (long long int) (ch * powl(R, n - i - 1)) % (long long int) M; //
		ch = getc(f1);
		i++;
	} 
	
	printf("%s dosyasinin key degeri: %llu \n", fname, key);
	printf("\n");
	insert(key, ht, M, flag);
}

void read_file(FILE* f1, char* dir, long long int M, int* ht, int* flag) {
	char fname[MAX];
	fseek(f1, 0, SEEK_SET);
	while (fgets(fname, MAX, f1) && (fname[0] != '\n') && (fname[0] != EOF)) {
		file_hasher(fname, dir, M, ht, flag);
	}
}

int main(int argc, char **argv)
{
	char dir[20], path[20];
	FILE* f1;
	long long int M = 29;
	int* ht = (int*) calloc(M, sizeof(int));
	int* flag = (int*) calloc(M, sizeof(int));
	
	strcpy(dir, "D:\\Dir\\");
	strcpy(path, dir);
	strcat(path, "samples.txt");
	f1 = fopen(path, "r");
	if (f1 == NULL){
		printf("%s could not opened!!\n", path);
	} 
	else{
		read_file(f1, dir, M, ht, flag);
	}
	
}
