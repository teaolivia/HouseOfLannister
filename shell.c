#include <dirent.h>
#include <errno.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char input[256];			//variabel input
char currentPath[256];		//path yang sedang berjalan
char user[256];				//user pengguna

//list command
char strHelp[256] = "help";

//prosedur mendapatkan current path
int getExecPath() {
	
    char path[256];

    getcwd(path, 255);
	strcpy(currentPath, path);
	
	return 0;
	
}

//menampilkan list direktori pada folder yang sedang aktif
int listFile() {
	
	DIR *d;
	struct dirent *dir;
	
	d = opendir(".");
	if (d) {
		while((dir = readdir(d)) != NULL) {
			printf("%s\n", dir->d_name);
		}
		closedir(d);
	}
	
	return 0;
	
}

//prosedur naik ke parent folder
int moveToParent() {
	
	int index = 0;
	int indexTemp = 1;
	int setOff = 0;
	int iterator = 0;
	char temp[256];
	
	//menentukan pemotongan	
	while(index<strlen(currentPath)) {	
		if(currentPath[index]=='/' || currentPath[index]=='\0') {
			indexTemp = 1;
			setOff = index;
			index++;
		} else {
			temp[indexTemp] = currentPath[index];
			indexTemp++;
			index++;
		}
	}

	//mengkopi alamat baru ke alamat utama
	for(iterator;iterator<setOff;iterator++) {
		temp[iterator] = currentPath[iterator];
	}
	temp[setOff] = '\0';
	strcpy(currentPath, temp);
	
	return 0;
	
}

//prosedur cek folder
int goCheckFolder(char content[]) {
	
	struct stat s;
	int err = stat(content, &s);
		
	if(-1 == err) {
		if(ENOENT == errno) {
			printf("Input Error\n");
		} else {
			perror("stat");
		}
	} else {
		if(S_ISDIR(s.st_mode)) {
			strcpy(currentPath, content);
			chdir(currentPath);
		} else {
			printf("Input Error\n");
		}
	}
	
	return 0;
	
}

//prosedur jika ingin memasuki sebuah folder
int moveInTo(char content[]) {

	char tempAddress[256];

	strcpy(tempAddress, currentPath);
	strcat(tempAddress, "/");
	strcat(tempAddress, content);
	goCheckFolder(tempAddress);
	
	return 0;
	
}

//prosedur untuk pindah folder
int moveFolder() {

	int iterator = 0;
	int index = 3;
	char address[256];
	
	//membaca isi dari cd untuk disimpan kedalam address
	while(index < strlen(input)) {
		address[iterator] = input[index];
		iterator++;		
		index++;
	}
	
	address[iterator] = '\0';
	
	//jika ingin naik ke parent folder
	if(address[0]=='.' && address[1]=='.') {
		moveToParent();
	} else {
		moveInTo(address);
	}
		
	return 0;
	
}

//wcc Execute
int wccExec(char file[]) {
	
	struct stat st;
	stat(file, &st);
	int size = st.st_size;
	
	printf("File size : %i bytes\n",size);
	
	return 0;
	
}

//wcm Execute
int wcmExec(char filePar[]) {
	
	int c;
	int result = 0;
	FILE *file;
	
	file = fopen(filePar, "r");
	if(file) {
		while((c=getc(file)) != EOF) {
			result++;
		}
		fclose(file);
	}
	printf("Char count : %i chars\n",result);
	
	return 0;
	
}

//wc File
int wcFile() {
	
	int iterator = 0;
	int index = 3;
	char address[256];
	char nameFile[256];
	
	//membaca isi dari cd untuk disimpan kedalam address
	while(index < strlen(input)) {
		address[iterator] = input[index];
		iterator++;		
		index++;
	}
	
	address[iterator] = '\0';	
	index = 3;
	iterator = 0;
	
	//mendapatkan nama file
	while(index < strlen(address)) {
		nameFile[iterator] = address[index];
		iterator++;
		index++;
	}
	index = 0;
	
	//memproses input
	if(address[0]=='-' && address[1]=='c') {
		wccExec(nameFile);
	} else if(address[0]=='-' && address[1]=='m') {
		wcmExec(nameFile);
	} else {
		printf("Input Error\n");
	}
	
	return 0;
	
}

//prosedur untuk chmod
int chmodFolder() {

	int decrement;
	int iterator = 0;
	int bool;
	int index = 5;
	int indexPrint = 0;
	int indexCpyFile = 0;
	int indexCpy = 1;
	int chmodInt;
	char address[256];	//menyimpan konten beserta nama file
	char content[256];	//menyimpan angka konten chmod
	char nameFile[256]; //menyimpn nama file 
	
	//membaca isi dari cd untuk disimpan kedalam address
	while(index < strlen(input)) {
		address[iterator] = input[index];
		iterator++;		
		index++;
	}
	
	address[iterator] = '\0';
	iterator = 0;
	
	//memisahkan konten dan nama file
	decrement = strlen(address)-1;
	bool = decrement - 4;
	while (bool <= decrement) {
		content[decrement-bool-1] = address[decrement]; 
		decrement--;
	}
	for(indexCpy;indexCpy<(strlen(address)-strlen(content)-1);indexCpy++) {
		nameFile[indexCpyFile] = address[indexCpy];
		indexCpyFile++;
	}
	nameFile[indexCpyFile] = '\0';
	
	//melakukan fungsi chmod
	chmodInt = strtol(content, 0, 8);
	if(chmod(nameFile,chmodInt) < 0) {
		printf("Input Error\n");
	} else {
		printf("Success\n");
	}
	
	return 0;
	
}

//prosedur grep
int grepFile() {

	int iterator = 0;
	int index = 5;
	char address[256];
	char string[256];
	char nameFile[256];
	
	//membaca isi dari cd untuk disimpan kedalam address
	while(index < strlen(input)) {
		address[iterator] = input[index];
		iterator++;		
		index++;
	}
	
	address[iterator] = '\0';
	iterator = 0;
	index = 0;
	
	//mengambil konteks string
	while(address[index]!=' ') {
		string[iterator] = address[index];
		iterator++;
		index++;
	}
	index++;
	iterator = 0;
	
	//mengambil nama file
	while(address[index]!='\0') {
		nameFile[iterator] = address[index];
		iterator++;
		index++;		
	}
	
	//menjalankan command grep
	char* stringPtr = string;
	FILE* file;
	file = fopen(nameFile,"r");
	char line[256];
	while(fgets(line, sizeof(line), file)) {
		if(strstr(line,stringPtr)!=NULL) {
			printf("%s",line);
		}
	}
	fclose(file);
	
	return 0;

}

//prosedur help
int help() {

	printf("\n");	
	printf("ATHARVA SHELL v1.0\n");
	printf("--------------------------------\n");
	printf("\n");	
	printf("cd   : Mengganti direktori kerja\n");
	printf("help : Menampilkan menu bantuan\n");
	printf("\n");		
	printf("--------------------------------\n");

	return 0;
	
}

//prosedur cat
int catFolder() {
	
	FILE *file;
	int iterator = 0;
	int index = 4;
	int c;
	char address[256];
	
	//membaca isi dari cat untuk disimpan kedalam address
	while(index < strlen(input)) {
		address[iterator] = input[index];
		iterator++;
		index++;
	}

	address[iterator] = '\0';
	
	//mengecek nama file apakah ada atau tidak menggunakan address
	if(access(address, F_OK ) != -1) {
		file = fopen(address, "r");
		if (file) {
			while ((c = getc(file)) != EOF) {
				putchar(c);
			}
			fclose(file);
		}
		printf("\n");
	} else {
		printf("File doesn't exist\n");
	}

	return 0;
	
}

//prosedur membaca input
int readInput() {
	char* buf;
	
	printf("\n");	
	printf("%s\n$: ",currentPath);
	
	
	
	gets(input);
	
	return 0;
	
}

//prosedur utama
int main() {
	
	//inisialisasi path
	getExecPath();
	
	while(1) {
		while(1) {
			//mengganti current directory
			chdir(currentPath);
			
			//membaca input
			readInput();
				
			//jika input meminta help
			if(strcmp(input, strHelp)==0) {
				help();
				break;
			}

			//menampilkan list file
			if(input[0]=='l' && input[1]=='s') {
				listFile();
				break;
			}	
			
			//jika input mengganti lokasi
			if(input[0]=='c' && input[1]=='d') {
				if(input[2]=='\0' || input[3]=='\0' || input[3]==' ') {
					printf("Input Error\n");
				} else if(input[2]==' ') {
					moveFolder();
					break;
				}
			} 	

			//jika input memasukkan perintah wc
			if(input[0]=='w' && input[1]=='c') {
				if(input[2]=='\0' || input[3]=='\0' || input[3]==' ') {
					printf("Input Error\n");
				} else if(input[2]==' ') {
					wcFile();
					break;
				}
			} 		
			
			//jika input ingin melakukan chmod
			if(input[0]=='c' && input[1]=='h' && input[2]=='m' && input[3]=='o' && input[4]=='d') {
				if(input[5]=='\0' || input[6]=='\0' || input[6]==' ') {
					printf("Input Error\n");
					break;
				} else if(input[5]==' ') {
					chmodFolder();
					break;
				}
			} 		
			
			//jika input meminta cat file
			if(input[0]=='c' && input[1]=='a' && input[2]=='t') {
				if(input[3]=='\0' || input[4]=='\0' || input[4]==' ') {
					printf("Input Error\n");
					break;
				} else if(input[3]==' ') {
					catFolder();
					break;
				}
			} 

			//jika input adalah grep
			if(input[0]=='g' && input[1]=='r' && input[2]=='e'  && input[3]=='p') {
				if(input[4]=='\0' || input[5]=='\0' || input[5]==' ') {
					printf("Input Error\n");
					break;
				} else if(input[4]==' ') {
					grepFile();
					break;
				}
			} 
		}	
	}

	return 0;
	
}
