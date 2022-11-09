#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#define ADDITIONAL_BUFFER 0
#define ENCRYPT 0
#define DECRYPT 1

char* readPassword() {
	char* l_password = calloc(300, 1);
	char ch;
	int i = 0;
	while((ch = _getch()) != 13) {
		l_password[i] = ch;
		i++;
		printf("*");
	}
	l_password[i] = '\0';
	return l_password;
}

char* generateKey(int file_size, char*password) {
	char* key = calloc(file_size, 1);
	strcpy(key, password);
	while( strlen(key) + strlen(password) < file_size) {
		strcat(key, password);
	}
	int i = 0;
	while( strlen(key) < file_size) {
		int index = strlen(key);
		key[index] = password[i];
		i+=1;
	}
	key[file_size] = '\0';
	return key;
}


int main(int args, char* argv[]) {

	//get mode
	int MODE;
	char* substr = calloc(10, 1);
	strncpy(substr, &argv[1][strlen(argv[1])-10], 10);
	if(strcmp(substr, ".encrypted") == 0) {
		MODE = DECRYPT;
	} else {
		MODE = ENCRYPT;
	}
	
	// open files

	char temp[256];
	if(MODE == ENCRYPT) {
		strcpy(temp ,argv[1]);
		strcat(temp,".encrypted");
	} else {
		strncpy(temp, argv[1], (strlen(argv[1])-10));
		temp[strlen(argv[1])-10] = '\0';
	}

	FILE* output_file = fopen(temp, "w");
	FILE* input_file = fopen(argv[1], "r");

	//get file size
	fseek(input_file, 0, SEEK_END);
	int file_size = ftell(input_file);
	rewind(input_file);
	if(file_size < 1) {
		printf("Empty file");
		return -1;
	}
	
	//read file
	char* buffer = calloc(file_size, 1);
	printf("\nFilesize: %i", file_size);
	
	int reading_index = 0;
	while(reading_index < file_size) {
		buffer[reading_index] = fgetc(input_file);
		reading_index++;
	}

	printf("\nRead: %s", buffer);

	//read password
	printf("\nPlease enter password: ");
	char* password = readPassword();
	
	//read password another time
	printf("\nPlease retype password: ");
	char* temp_pwd = readPassword();
	
	//compare passwords and retry while they do not match
	while(strcmp(password, temp_pwd) != 0) {
		printf("\nPlease try again: ");
		password = readPassword();
		printf("\nPlease retype password: ");
		temp_pwd = readPassword();
	}

	
	//generate key
	char* key = generateKey(file_size, password);

	char* output = calloc(file_size, 1);

	//encrypt
	if(MODE == ENCRYPT) {
		printf("\n\nEncrypting\n");
		int i = 0;
		for(i=0; i < file_size; i++) {
			char temp_xor =  (char)(buffer[i] ^ key[i]);
			if(temp_xor != '\0') {
				printf("\n%2i: key: %c output: %c", i, key[i], output[i]);
				output[i] = temp_xor;
			} else {
				output[i] = (char)0b11111111;
				printf("\n%2i: key: %c output: %c", i, key[i], output[i]);
			}

			int binary_index;
			for(binary_index = 0; binary_index < 7; binary_index++) {
				printf("%u", (output[i] >> binary_index) & 1);
			}
			
		}
	}

	//decrypt
	if(MODE == DECRYPT) {
		printf("\n\nDecrypting\n");
		int i = 0;
		for(i=0; i < file_size; i++) {
			
			char temp_xor =  (buffer[i] ^ key[i]);
			if(temp_xor != 0b11111111) {
				output[i] = temp_xor;
				printf("\n%2i: key: %c output: %c ", i, key[i], output[i]);
				
			} else {
				output[i] = key[i];
				printf("\n%2i: key: %c output: %c", i, key[i], output[i]);
			}
			
			int binary_index;	
			for(binary_index = 0; binary_index < 7; binary_index++) {
				printf("%u", (output[i] >> binary_index) & 1);
			}
			if(buffer[i]=='\0') {
				printf(" Null byte detected");
			}
		}
	}

	//write output
	printf("\n\nPrinting %i bytes. output: %s", strlen(output), output);
	fprintf(output_file, "%s", output);
	free(output);

	//close files
	fclose(input_file);
	fclose(output_file);
	free(buffer);
	printf("\n\n");
	return 0;
}
