#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100
#define MIN 30
#define R 2
#define MM 28

typedef struct {
	char* name;
} ht;

void initializeHash( ht* hashTable, long long int M) {
	int i;
	
	for (i = 0; i < M; i++) {
		hashTable[i].name = (char*) calloc(MAX, sizeof(char));
		hashTable[i].name[0] = '\0';
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
	int flag = 0;
	f1 = open_file(fname1, dir1);
	f2 = open_file(fname2, dir2);
	
	fscanf(f1, "%s", line1);
	fscanf(f2, "%s", line2);
	
	do {
		flag = strcmp(line1, line2);
	} 
	while((!flag) && (fscanf(f1, "%s", line1) != EOF) && (fscanf(f2, "%s", line2) != EOF));
	
	if ((!flag)) 
		return 1; // dosyalar ayniysa 1 return eder
	else
		return 0;
}

int insert_file (int key, char* fname1, char* dir1, char* dir2, ht* hashTable, long long int M) {
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
		return 0;
	}	
	else if ( hashTable[adr].name[0] == '\0' ) { //ht[adr] bossa 
		strcpy(hashTable[adr].name, fname1); 
		printf("%s inserted %dth address. \n", fname1, adr);
		return 1;
	} 
	else {
		printf("Table is full! \n");
		return 0;
	}
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
		printf("%s inserted %dth address. \n", fname, adr);
	} 
	else {
		printf("Table is full! \n");
	}
	printf("\n");
}

long long int file_hasher (char* fname, char* dir, long long int M, ht* hashTable) {
	char ch;
	int n = 0, i = 0;
	long long int key = 0;
	
	FILE* f1 = open_file(fname, dir);
	if (f1 == NULL) {
		return -1;
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
	while ( ch != EOF) { 
		key += (long long int) (ch * powl(R, n - i - 1)) % (long long int) M; //
		ch = getc(f1);
		i++;
	} 	
	printf("%s dosyasinin key degeri: %llu \n", fname, key);
	return key;
}

void copy_to_dir(char* fname, char* dir1, char* dir2) {
	FILE *f1, *f2;
	char dummy[30], ch;
	
	strcpy(dummy, dir2);
	strcat(dummy, fname);
	
	f1 = open_file(fname, dir1);
	f2 = fopen(dummy, "w");
	
	ch = getc(f1);
	
	while(ch != EOF) {
		putc(ch, f2);
		ch = getc(f1);
	}	
	
	fclose(f1);
	fclose(f2);	
}

void file_to_table (char* fname, char* dir1, char* dir2, long long int M, ht* hashTable) {
	FILE *f1;
	long long int key;
	int sonuc;
	char dummy[30];
	
	key = file_hasher(fname, dir1, M, hashTable);
	sonuc = insert_file(key, fname, dir1, dir2, hashTable, M);
	
	if (sonuc) {
		strcpy(dummy, dir2);
		strcat(dummy, "samples.txt");
		printf("%s \n", dummy);
		f1 = fopen(dummy, "a"); // append
		
		if (f1 == NULL)
			printf("%s could not opened!!\n", dummy);
		
		fputc('\n', f1);
		fprintf(f1, fname);
		copy_to_dir(fname, dir1, dir2);
		
		fclose(f1);				
	}
}

void read_dir(char* dir, long long int M, ht* hashTable) {
	char fname[MAX];
	FILE *f1;
	long long int key = 0;
	
	f1 = open_file("samples.txt", dir);
	
	while (fgets(fname, MAX, f1) && (fname[0] != EOF)) {
		printf("Dosya: %s \n", fname);
		key = file_hasher(fname, dir, M, hashTable);
		if (key != -1)
			insert_from_dir(key, fname, hashTable, M);
	}
	
	fclose(f1);
}

void print_hash(ht* hashTable, long long int M) {
	int i;
	for(i = 0; i < M; i++) {
		printf("hashTable[%d]: %s \n", i, hashTable[i].name);
	}
}

int main(int argc, char **argv)
{
	char dir1[MIN], dir2[MIN], fname[MIN];
	long long int M = 29;
	int fonk, out; 
	
	ht* hashTable = (ht*) calloc(M, sizeof(ht));
	initializeHash(hashTable, M);
	
	do {
		out = 1;
		system("cls");
		printf("_________HASHING ALGORITMASI__________ \n");
		printf("0 - Hash Tablosunu Olusturma \n");
		printf("1 - Veri Tabanina Dokuman Ekleme \n");
		printf("2 - Hash Tablosunu Goruntule \n");
		printf("3 - EXIT \n");
		printf("Islem yapmak istediginiz fonksiyonu seciniz : ");
		scanf("%d", &fonk);
		switch(fonk) {
			case 0:
				do {
					printf("_________________________________\n");
					printf("0 - Hash Tablosunu Olusturma \n");
					printf("Dosyalari iceren yolu giriniz: ");
					scanf("%s", dir1);
					printf("\n");
					read_dir(dir1, M, hashTable);
					
					printf("\nPress 0 to return the menu: ");
					scanf("%d", &out);
				} while(out != 0);
				break;
			case 1: 
				do {
					printf("_________________________________\n");
					printf("1 - Veri Tabanina Dokuman Ekleme \n");
					printf("Eklenecek dokumanin yolunu giriniz: ");
					scanf("%s", dir2);
					printf("Eklenecek dokumanin adini giriniz: ");
					scanf("%s", fname);
					printf("\n");
					file_to_table(fname, dir2, dir1, M, hashTable);
					
					printf("\nPress 0 to return the menu: ");
					scanf("%d", &out);
				} while(out != 0);
				break;
			case 2: 
			do {
				printf("_________________________________\n");
				printf("2 - Hash Tablosunu Goruntule \n");
				print_hash(hashTable, M);
				
				printf("\nPress 0 to return the menu: ");
				scanf("%d", &out);
			} while(out != 0);
			break;
		}
		
	} while(fonk != 3);
    
	
	
	
}
