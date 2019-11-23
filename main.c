#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100
#define MIN 100
#define R 2
#define MM 28

typedef struct {
	char* name;
	int flag;
} ht;

void initializeHash( ht* hashTable, long long int M) {
	int i;
	
	for (i = 0; i < M; i++) {
		hashTable[i].name = (char*)calloc(MAX, sizeof(char));
		hashTable[i].name[0] = '\0';
		hashTable[i].flag = 0;
	}
}

int hash(long long int key, int i, long long int M) {
	return (key % M) + i * (1 + key % MM);
}

FILE* open_file(char* fname, char* dir) {
	char dummy[MAX], ch;
	int i = 0;
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
	return f1;
}

int compare_files(char* fname1, char* fname2, char* dir1, char* dir2) {
	FILE *f1, *f2;
	char line1[MAX], line2[MAX];
	int flag;
	f1 = open_file(fname1, dir1);
	f2 = open_file(fname2, dir2);
	
	while (!flag) {
		fgets(line1, MAX, f1);
		fgets(line2, MAX, f2);
		if ((line1[0] != EOF) && (line2[0] != EOF))
			flag = strcmp(line1, line2);
	}
	if ( !flag && (line1[0] == EOF) && (line2[0] == EOF) )
		return 1; // files are the same
	else 
		return 0;	
}

void insert_from_dir (int key, char* fname, ht* hashTable, long long int M) {
	int adr, i = 0;
	adr = hash(key, i, M);
	
	while( ( hashTable[adr].name[0] != '\0')  && (i < M) ) {
		i++;
		adr = hash(key, i, M);
	}
	
	if ( hashTable[adr].name[0] == '\0' ) {
		strcpy(hashTable[adr].name, fname); 
		hashTable[adr].flag = 0;
		printf("%s inserted %dth address. \n", fname, adr);
	} 
	else {
		printf("Table is full! \n");
	}
	printf("\n");
}

void insert_file (int key, char* fname1, char* dir1, char* dir2, ht* hashTable, long long int M) {
	int adr, i = 0, sonuc = 0;
	char fname2[MAX];
	adr = hash(key, i, M);
	
	while( (!sonuc) && (hashTable[adr].name[0] != '\0')  && (i < M)) { //ht[adr] dolu oldugu surece buraya girecek
		strcpy(fname2, hashTable[adr].name);
		sonuc = compare_files(fname1, fname2, dir1, dir2); //esit olurlarsa 1
		if (!sonuc) { // esit degiller
			i++;
			adr = hash(key, i, M);
		}
	}
	if (sonuc) {
		printf("%s file is the same with %s. It did not inserted. . . \n", fname1, fname2);
	}	
	else if ( hashTable[adr].name[0] == '\0' ) { //ht[adr] bossa 
		strcpy(hashTable[adr].name, fname1); 
		hashTable[adr].flag = 0;
		printf("%s inserted %dth address. \n", fname1, adr);
	} 
	else {
		printf("Table is full! \n");
	}
}

long long int file_hasher (char* fname, char* dir, long long int M, ht* hashTable) {
	char ch;
	int n = 0, i = 0;
	long long int key = 0;
	
	FILE* f1 = open_file(fname, dir);
	
	ch = getc(f1);
	while( ch != EOF) { // file uzunlugunu olc
		n++; 
		ch = getc(f1);
	}
	printf("%s dosyasi uzunlugu: %d \n", fname, n);
	
	fseek(f1, 0, SEEK_SET); 
	ch = getc(f1);
	i = 0;
	while ( ch != EOF) { 
		// printf("%d %lld \n", ch, (long long int) powl(R, n - i - 1));
		// printf("%lld \n", (long long int) ch * (long long int) powl(R, n - i - 1));
		key += (long long int) (ch * powl(R, n - i - 1)) % (long long int) M; //
		ch = getc(f1);
		i++;
	} 	
	printf("%s dosyasinin key degeri: %llu \n", fname, key);
	return key;
}

void read_dir(FILE* f1, char* dir, long long int M, ht* hashTable) {
	char fname[MAX];
	long long int key = 0;
	fseek(f1, 0, SEEK_SET);
	while (fgets(fname, MAX, f1) && (fname[0] != EOF)) {
		key = file_hasher(fname, dir, M, hashTable);
		insert_from_dir(key, fname, hashTable, M);
	}
}

void file_to_table(char* fname, char* dir1, char* dir2, long long int M, ht* hashTable) {
	FILE* f1 = open_file(fname, dir1);
	long long int key = file_hasher(fname, dir1, M, hashTable);
	insert_file(key, fname, dir1, dir2, hashTable, M);
}

int main(int argc, char **argv)
{
	char dir[20], path[20];
	FILE* f1;
	long long int M = 29;
	
	ht* hashTable = (ht*) calloc(M, sizeof(ht));
	initializeHash(hashTable, M);
		
	strcpy(dir, "D:\\Dir\\");
	strcpy(path, dir);
	strcat(path, "samples.txt");
	f1 = fopen(path, "r");
	
	if (f1 == NULL)
		printf("%s could not opened!!\n", path);
	else
		read_dir(f1, dir, M, hashTable);
	
	file_to_table("c.txt", "D:\\", "D:\\Dir\\", M, hashTable);
	
}
