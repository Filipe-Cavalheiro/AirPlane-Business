/**
 * @author Filipe Cavalheiro
 * @name Airplane Business
 * @date 13-07-2024
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <conio.h>
#include <windows.h>
#include "AsyncScrollDetect.h" //detect mouse scroll functionality

#define PATH ".\\"
#define NAMES_PATH ".\\names"
#define SURNAMES_PATH ".\\surnames"
#define ESC_KEY 27
#define writexy(x, y, ch) printf("\e[%d;%dH", (y), (x)); printf("%c", ch)
#define moveCursor(x,y) printf("\e[%d;%dH", (y), (x));
#define clrscr_full() printf("\e[0;0H"); printf("\e[2J")
#define hideCursor() printf("\e[?25l")
#define showCursor() printf("\e[?25h")
#define blinkCursor() printf("\e[3 q")
#define stopBlinkingCursor() printf("\e[?12l")

int columns, rows;

typedef struct {
    int numberFiles;
    char **files;
} FileArray;

int selectionList(int arg_count, char** list, int Xpos, int Ypos);
int selectionListInstant(int arg_count, char** list, int Xpos, int Ypos, int currentOption);
void clrscr();
FileArray* allocateStringArray(int numberFiles);
FileArray* ListDirectoryContents(const char *sDir, const char *mask);
void freeFileArray(FileArray* fileArray);
void print_line(int line);

struct pessoas{
	char nome[20];
	char sobrenome[20];
	int linha;
	int coluna;
	int id_mala;
	int id_reserva;
};

typedef struct plane_inf{
	char name[10];
	char type[10];
	int pass_occy;
	int executive;
	int economy;
	char matriz[6][35];
	struct pessoas pessoas[210];
	char location[3];
	char destination[3];
	time_t takeoff;
	time_t landing;
} PLANE;

int numRand(int MIN, int MAX){
	int Rnum;
	Rnum = (rand() % (MAX - MIN +1))+ MIN;
	return Rnum;
}

void press_enter(){
	fflush(stdin);
	printf("press any key to continue...");
	getchar();
	clrscr();
}

void clrscr() {
    // Move to the beginning of the screen
    printf("\e[1;1H");
	char* blanktext = (char*)malloc(columns - 1); // columns - 2 spaces + 1 for null terminator
	memset(blanktext, ' ', columns - 2);
	blanktext[columns - 2] = '\0';
    for (int r = 2; r < rows; ++r) {
        printf("\e[%d;2H%s", r, blanktext);  // Move to column 2 of row r and clear row
    }
	free(blanktext);
}

void printBox(){
    char *array = (char *)malloc(columns+1);
    if (array == NULL) {
        printf("Memory allocation failed\n");
		exit(-1);
    }

    memset(array, 205, columns-1);
	array[0] = 201;
	array[columns-1] = 187;
	array[columns] = '\0';
    printf("%s\n", array);

	memset(array, ' ', columns-1);
	array[0] = 186;
	array[columns-1] = 186;
	array[columns] = '\0';
	for(int i = 0; i < rows-2; ++i)
		printf("%s\n", array);

	memset(array, 205, columns-1);
	array[0] = 200;
	array[columns-1] = 188;
    printf("%s\n", array);
    free(array);
}

void printLogo(){
	int startingPos = (columns/2) - 36;
	moveCursor(startingPos,3);
	printf("  ___  _                     __ _    ______           _");
	moveCursor(startingPos,4);
	printf(" / _ \\(_)                   / _| |   | ___ \\         (_)");     
	moveCursor(startingPos,5);               
	printf("/ /_\\ \\_ _ __ ___ _ __ __ _| |_| |_  | |_/ /_   _ ___ _ _ __   ___  ___ ___");
	moveCursor(startingPos,6);
	printf("|  _  | | '__/ __| '__/ _` |  _| __| | ___ \\ | | / __| | '_ \\ / _ \\/ __/ __|");
	moveCursor(startingPos,7);
	printf("| | | | | | | (__| | | (_| | | | |_  | |_/ / |_| \\__ \\ | | | |  __/\\__ \\__ \\");
	moveCursor(startingPos,8);
	printf("\\_| |_/_|_|  \\___|_|  \\__,_|_|  \\__| \\____/ \\__,_|___/_|_| |_|\\___||___/___/");
	moveCursor(startingPos,9);
}


void show_struct(PLANE plane, int Xpos, int Ypos){
	print_line(Ypos-2);
	moveCursor(Xpos, Ypos+3);
	for(int num = 1; num <= (plane.executive + plane.economy); num++)
		printf("%2d", num);

	int num_coluna_act, num_linha_act;
	for(num_linha_act = 1; num_linha_act <= 6; num_linha_act++){
			if(num_linha_act == 4){
				Ypos++;
			}
			for(num_coluna_act = 1; num_coluna_act <= (plane.executive + plane.economy); num_coluna_act++){
				moveCursor(Xpos, Ypos);	
				if(plane.matriz[num_linha_act-1][num_coluna_act-1] != '.'){
					if(num_coluna_act <= plane.executive){
						switch(num_linha_act){
							case 1: printf("  "); break;
							case 2: printf(" a"); break;
							case 3: printf(" b"); break;
							case 4: printf(" c"); break;
							case 5: printf(" d"); break;
							case 6: printf("  "); break;
						}
					}
					else{
						switch(num_linha_act){
							case 1: printf(" a"); break;
							case 2: printf(" b"); break;
							case 3: printf(" c"); break;
							case 4: printf(" d"); break;
							case 5: printf(" e"); break;
							case 6: printf(" f"); break;
						}
					}
				}	
				else
					printf(" .");
				if(num_coluna_act == (plane.executive + plane.economy))
					Xpos = (columns/6);
				else
					Xpos += 2;
			}
		if(num_linha_act == 6)
			Ypos = rows - 8;
		else
			Ypos++;
	}
}

void print_line(int line){
    char *array = (char *)malloc(columns+1);
    if (array == NULL) {
        printf("Memory allocation failed\n");
		exit(-1);
    }

	memset(array, 205, columns-1);
	array[0] = 204;
	array[columns-1] = 185;
	array[columns] = '\0';
	printf("\e[%d;0H%s\n",line, array);
	free(array);
}

FileArray* allocateStringArray(int numberFiles) {
    FileArray* fileArray = (FileArray*)malloc(sizeof(FileArray));
    if (fileArray == NULL) {
        return NULL;
    }

    fileArray->numberFiles = numberFiles + 1; // number of files + "exit"
    fileArray->files = (char**)malloc(sizeof(char*) * fileArray->numberFiles);
    if (fileArray->files == NULL) {
        free(fileArray);
        return NULL;
    }

    // Allocate space for each string
    for (int i = 0; i < fileArray->numberFiles; i++) {
        fileArray->files[i] = (char*)malloc(sizeof(char) * 11);//11 is the max size of a plane name
        if (fileArray->files[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(fileArray->files[j]);
            }
            free(fileArray->files);
            free(fileArray);
            return NULL;
        }
    }

    return fileArray;
}

void freeFileArray(FileArray* fileArray) {
    if (fileArray == NULL) {
        return;
    }

    for (int i = 0; i < fileArray->numberFiles - 1; i++) {
        free(fileArray->files[i]);
    }

    free(fileArray->files);

    free(fileArray);
}

FileArray* ListFileContents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    int numberFiles = 0;
    char buffer[2048];

    // First pass: count the number of lines
    while (fgets(buffer, sizeof(buffer), file)) {
        ++numberFiles;
    }
	numberFiles--; //i get one too many!?

    // Allocate the array of strings
    FileArray *fileArray = allocateStringArray(numberFiles);
    if (!fileArray) {
        fclose(file);
        return NULL;
    }

    // Second pass: read the lines into the array
    rewind(file);
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove the newline character, if any
        buffer[strcspn(buffer, "\n")] = '\0';
        fileArray->files[i] = strdup(buffer);
        if (!fileArray->files[i]) {
            freeFileArray(fileArray);
            fclose(file);
            return NULL;
        }
        ++i;
    }

    fclose(file);
    return fileArray;
}

FileArray* ListDirectoryContents(const char *sDir, const char *mask) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    // Specify a file mask.
    sprintf(sPath, "%s\\*.%s", sDir, mask);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    int numberFiles = 0;
    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            numberFiles++;
        }
    } while (FindNextFile(hFind, &fdFile)); // get the number of files

    FileArray* fileArray = allocateStringArray(numberFiles);
    if (fileArray == NULL) {
        FindClose(hFind);
        return NULL;
    }

    FindClose(hFind); // Reset find handle for the second iteration
    hFind = FindFirstFile(sPath, &fdFile);
    if (hFind == INVALID_HANDLE_VALUE) {
        // Cleanup in case of failure
        for (int j = 0; j < numberFiles; j++) {
            free(fileArray->files[j]);
        }
        free(fileArray->files);
        free(fileArray);
        return NULL;
    }

	int i = 1;
    do {
        if (strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0) {
            strcpy(fileArray->files[i], fdFile.cFileName);
            ++i;
        }
    } while (FindNextFile(hFind, &fdFile)); // Find the next file.

    FindClose(hFind); // Always, always, clean things up!

    return fileArray;
}

int selectionList(int arg_count, char** list, int Xpos, int Ypos){
	char* text;
	int tmpYpos = Ypos;
	for (int i = 0; i < arg_count; ++i){
		text = list[i];
		moveCursor(Xpos, tmpYpos); 
		printf("%s", text);
		++tmpYpos;
	}

	Xpos--;
	int currentOption = 0;
	char keyInput;
	while(1){
		writexy(Xpos, currentOption + Ypos, '>');
		keyInput = getch();
		switch(keyInput){
			case 'H':
			case 'w': 
				if(currentOption <=  0){
					writexy(Xpos, Ypos, ' ');
					writexy(Xpos, Ypos + arg_count - 1, '>');
					currentOption = arg_count - 1;
					break;
				}
				currentOption -= 1;
				writexy(Xpos, currentOption + Ypos + 1, ' ');
				writexy(Xpos, currentOption + Ypos, '>');
				break;
			case 'P':
			case 's':
				if(currentOption >= arg_count - 1){
					writexy(Xpos, Ypos + arg_count - 1, ' ');
					writexy(Xpos, Ypos, '>');
					currentOption = 0;
					break;
				}
				currentOption += 1;
				writexy(Xpos, currentOption + Ypos - 1, ' ');
				writexy(Xpos, currentOption + Ypos, '>');
				break;
			case '\r':
				tmpYpos = Ypos;
				for (int i = 0; i < arg_count; ++i){
					moveCursor(Xpos, tmpYpos); 
					printf(" ");
					++tmpYpos;
				}
				return currentOption;
			case ESC_KEY:
				tmpYpos = Ypos;
				for (int i = 0; i < arg_count; ++i){
					moveCursor(Xpos, tmpYpos); 
					printf(" ");
					++tmpYpos;
				}
				return 0;
		}
	}
}

int selectionListInstant(int arg_count, char** list, int Xpos, int Ypos, int currentOption){
	char* text;
	int tmpYpos = Ypos;
	for (int i = 0; i < arg_count; ++i){
		text = list[i];
		moveCursor(Xpos, tmpYpos); 
		printf("%s", text);
		++tmpYpos;
	}

	Xpos--;
	char keyInput;
	while(1){
		writexy(Xpos, Ypos + currentOption, '>');
		keyInput = getch();
		switch(keyInput){
			case 'H':
			case 'w': 
				if(currentOption <=  0){
					writexy(Xpos, Ypos, ' ');
					writexy(Xpos, Ypos + arg_count - 1, '>');
					currentOption = arg_count - 1;
					break;
				}
				currentOption--;
				writexy(Xpos, currentOption + Ypos + 1, ' ');
				writexy(Xpos, currentOption + Ypos, '>');
				break;
			case 'P':
			case 's':
				if(currentOption >= arg_count - 1){
					writexy(Xpos, Ypos + arg_count - 1, ' ');
					writexy(Xpos, Ypos, '>');
					currentOption = 0;
					break;
				}
				currentOption++;
				writexy(Xpos, currentOption + Ypos - 1, ' ');
				writexy(Xpos, currentOption + Ypos, '>');
				break;
			case '\r':
				tmpYpos = Ypos;
				for (int i = 0; i < arg_count; ++i){
					moveCursor(Xpos, tmpYpos); 
					printf(" ");
					++tmpYpos;
				}
				return currentOption;
			case ESC_KEY:
				tmpYpos = Ypos;
				for (int i = 0; i < arg_count; ++i){
					moveCursor(Xpos, tmpYpos); 
					printf(" ");
					++tmpYpos;
				}
				return 0;
		}
		if(currentOption)
			return currentOption;
	}
}

void struct_plane(){
	int executivo = 0; 
	int turista = 0; 
	int num_filas = 0;
	int option = 0;
	char ch1;
	FILE *tipoF;
	FileArray* planesF;

	planesF = ListDirectoryContents(PATH, "txt");
	strcpy(planesF->files[0], "exit");
	while(1){
		option = selectionListInstant(planesF->numberFiles, planesF->files, (columns/2) - 20, (rows*2/5), option);
		
		//clear the last plane struc drawing
		char* blanktext = (char*)malloc(columns - 1); // columns - 2 spaces + 1 for null terminator
		memset(blanktext, ' ', columns - 2);
		blanktext[columns - 2] = '\0';
		int Xpos = (columns/6);
		int Ypos = rows - 7; 
		for (int i = Ypos; i < Ypos+7; ++i) {
			printf("\e[%d;2H%s", i, blanktext);  // Move to column 2 of row r and clear row
		}
		free(blanktext);

		char plane_loc[100] = PATH; 
		if(option == 0){
			fclose(tipoF);
			freeFileArray(planesF); 
			writexy(0, Ypos-2, 186); 
			writexy(columns, Ypos-2, 186); 
			return;
		}else{
			strcat(plane_loc, planesF->files[option]);
		}
		tipoF = fopen(plane_loc, "r");

		Xpos = (columns/6);
		Ypos = rows - 7; 
		print_line(Ypos-2);
		do{
			ch1 = fgetc(tipoF);
			if(ch1 == '\n'){
				if (num_filas == 4){
					moveCursor(Xpos,Ypos+1);
					printf(" a");
					moveCursor(Xpos,Ypos+2);
					printf(" b");
					moveCursor(Xpos,Ypos+3);
					printf(" c");
					moveCursor(Xpos,Ypos+4);
					printf(" d");
					executivo++;
					Xpos += 2;
				}
				else{
					moveCursor(Xpos,Ypos);
					printf(" a");
					moveCursor(Xpos,Ypos+1);
					printf(" b");
					moveCursor(Xpos,Ypos+2);
					printf(" c");
					moveCursor(Xpos,Ypos+3);
					printf(" d");
					moveCursor(Xpos,Ypos+4);
					printf(" e");
					moveCursor(Xpos,Ypos+5);
					printf(" f");
					turista++;
					Xpos += 2;
				}
				num_filas = 0;
			}
			else
				num_filas++;
		}while(!(feof(tipoF)));

		moveCursor(2, Ypos-3);
		printf("The plane has %d executive and %d turist rows", executivo, turista);

		num_filas = 0;
		executivo = 0;
		turista = 0;
		fclose(tipoF);
	}
}

void flgh_occy(){
	FILE *planeF;
	PLANE plane;
	FileArray* planesF;
	int option = 0;
	char plane_loc[128] = PATH;
	int Ypos = rows - 8;
	
	planesF = ListDirectoryContents(PATH, "pln");
	if(planesF == NULL){
		moveCursor(columns/2 - 10, rows/4);
		printf(">exit");
		moveCursor(columns/2 - 8, rows/4 + 1);
        printf("No planes available");
		getch();
		return;
	}
	strcpy(planesF->files[0], "exit");
	while(1){
		option = selectionListInstant(planesF->numberFiles, planesF->files, (columns/2) - 20, (rows*2/5), option);
		strcpy(plane_loc, PATH); 
		if(option == 0){
			fclose(planeF);
			freeFileArray(planesF); 
			writexy(0, Ypos-2, 186); 
			writexy(columns, Ypos-2, 186); 
			return;
		}else{
			strcat(plane_loc, planesF->files[option]);
		}
		planeF = fopen(plane_loc, "rb+");
		if(planeF == NULL){
			freeFileArray(planesF); 
			writexy(0, Ypos-2, 186); 
			writexy(columns, Ypos-2, 186); 
			fprintf(stderr, "failed to open file at location %s", plane_loc);
			return;
		}

		int num_turista = 0, per_turista = 0, num_executivo = 0, per_executivo = 0, num_linha_act = 0, num_coluna_act = 0, num = 0, janela = 0, per_janela = 0, meio = 0, per_meio, centro = 0, per_centro = 0;		
		// ler a informação do avião
		while(fread(&plane, sizeof(struct plane_inf), 1, planeF)){
			show_struct(plane, (columns/6), rows - 8);
			//get number of executives and tourists
			for(num_coluna_act = 1; num_coluna_act <= (plane.executive + plane.economy); num_coluna_act++){
				for(num_linha_act = 1; num_linha_act <= 6; num_linha_act++){
					if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.'){
						if(num_coluna_act <= plane.executive)
							num_executivo++;
						else
							num_turista++;
					}
				}
			}
			
			for(num_coluna_act = 1; num_coluna_act <= (plane.executive + plane.economy); num_coluna_act++){
				for(num_linha_act = 1; num_linha_act <= 6; num_linha_act++){
					if(num_coluna_act > plane.executive){
						if((num_linha_act == 1) || (num_linha_act == 6))
							if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.')
								janela++;
						if((num_linha_act == 2) || (num_linha_act == 5))
							if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.')
								meio++;
						if((num_linha_act == 3) || (num_linha_act == 4))
							if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.')
								centro++;
					}
					else{
						if((num_linha_act == 2) || (num_linha_act == 5))
							if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.')
								janela++;
						if((num_linha_act == 3) || (num_linha_act == 4))
							if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.')
								centro++;
					}
				}
			}
			per_janela = janela*70/(plane.economy*2+plane.executive*2);
			per_meio = meio*70/(plane.economy*2);
			per_centro = centro*70/(plane.economy*2 + plane.executive*2);
			per_executivo = num_executivo*70/(plane.executive*4);
			per_turista = num_turista*70/(plane.economy*6);
			moveCursor(2, Ypos-7);
			printf("window:    ");
			for(num = 0; num < per_janela; num++)
				printf("*");
			printf(" %.2f%% \n", (float)janela*50/(plane.economy+plane.executive)); // multiplico por 50 pois janela são duas linha
			moveCursor(2, Ypos-6);
			printf("aisle:     ");
			for(num = 0; num < per_meio; num++)
				printf("*");
			printf(" %.2f%% ", (float)meio*50/(plane.economy)); // multiplico por 50 pois meio são duas linha
			moveCursor(2, Ypos-5);
			printf("center:    ");
			for(num = 0; num < per_centro; num++)
				printf("*");
			printf(" %.2f%% ", (float)centro*50/(plane.economy+plane.executive)); // multiplico por 50 pois centro são duas linha
			moveCursor(2, Ypos-4);
			printf("executive: ");
			for(num = 0; num < per_executivo; num++)
				printf("*");
			printf(" %.2f%% ", (float)num_executivo*100/(plane.executive*4));
			moveCursor(2, Ypos-3);
			printf("tourist:   ");
			for(num = 0; num < per_turista; num++)
				printf("*");
			printf(" %.2f%% ", (float)num_turista*100/(plane.economy*6));
		}
		fclose(planeF);
	}
}

void passager_names() {
    int num_pessoa, num_linha_act, num_coluna_act, option, Ypos, Xpos;
	const int initialYpos = (rows/5 + 1);
	const int initialXpos = 3;
	const int Xpos_spacer = 30;
	const int passager_names_window_size = rows - initialYpos - 1;
	float percent_of_bar;
	char plane_loc[128];
	FileArray* planesF;
    PLANE plane;
	FILE *planeF;

	print_line(rows/5);

	Ypos = initialYpos;
	Xpos = initialXpos;
	planesF = ListDirectoryContents(PATH, "pln");
	if(planesF == NULL){
		moveCursor(2,2);
		printf(">exit");
		moveCursor(3, 3);
        printf("No planes available");
		getch();
		writexy(0, rows/5, 186); 
		writexy(columns, rows/5, 186);
		return;
	}
	strcpy(planesF->files[0], "exit");

	while(1){
		Xpos = 3;
		Ypos = initialYpos;
		strcpy(plane_loc, PATH);
		option = selectionList(planesF->numberFiles, planesF->files, 3, 2);
		if(option == 0){
			freeFileArray(planesF); 
			writexy(0, rows/5, 186); 
			writexy(columns, rows/5, 186);
			return;
		}else{
			strcat(plane_loc, planesF->files[option]);
		}
		planeF = fopen(plane_loc, "rb");
		if(planeF == NULL){
			freeFileArray(planesF); 
			writexy(0, rows/5, 186); 
			writexy(columns, rows/5, 186); 
			fprintf(stderr, "failed to open file at location %s", plane_loc);
			return;
		}

		//clear the last passager names
		char* blanktext = (char*)malloc(columns - 1); // columns - 2 spaces + 1 for null terminator
		memset(blanktext, ' ', columns - 2);
		blanktext[columns - 2] = '\0';
		for (int i = Ypos; i < rows; ++i) {
			printf("\e[%d;2H%s", i, blanktext);  // Move to column 2 of row r and clear row
		}
		free(blanktext);

		while (fread(&plane, sizeof(struct plane_inf), 1, planeF)) {
			if (plane.pass_occy == 0) {
				moveCursor(2, (rows/5 + 1)); 
				printf("The plane is empty\n");
				fclose(planeF);
				break;
			}
				
			percent_of_bar = passager_names_window_size/(float)(plane.pass_occy/3);
			
			for (num_coluna_act = 1; num_coluna_act <= plane.executive; num_coluna_act++) {
				for (num_linha_act = 2; num_linha_act <= 5; num_linha_act++) {
					for (num_pessoa = 0; num_pessoa < plane.pass_occy; num_pessoa++) {
						if ((num_coluna_act == plane.pessoas[num_pessoa].coluna) && (num_linha_act == plane.pessoas[num_pessoa].linha)) {
							moveCursor(Xpos, Ypos);
							Xpos += Xpos_spacer;
							if(Xpos + Xpos_spacer >= columns){
								Xpos = initialXpos;
								Ypos++;
							}
							if(Ypos >= rows){
								Ypos = initialYpos;
								moveCursor(columns, Ypos);
								printf("^");
								Ypos++;
								for(int i = 0; i < 0; i++){
									moveCursor(columns, Ypos);
									printf("%c", 186);
									Ypos++;
								}
								for(int i = 0; i < (int)(passager_names_window_size * percent_of_bar); i++){
									moveCursor(columns, Ypos);
									printf("%c\n", 219);
									Ypos++;
								}
								for(int i = 0; i < (int)(passager_names_window_size * (1-percent_of_bar)); i++){
									moveCursor(columns, Ypos);
									printf("%c\n", 186);
									Ypos++;
								}
								moveCursor(columns, Ypos);
								printf("v");
								break;
							}
							printf("%d %c: %s %s", 
								(plane.pessoas[num_pessoa].coluna), 
								(plane.pessoas[num_pessoa].linha + 'a' - 2), 
								plane.pessoas[num_pessoa].nome, 
								plane.pessoas[num_pessoa].sobrenome);
						}
					}
				}
			}

			for (num_coluna_act = 1; num_coluna_act <= (plane.executive + plane.economy); num_coluna_act++) {
				for (num_linha_act = 1; num_linha_act <= 6; num_linha_act++) {
					for (num_pessoa = 0; num_pessoa < plane.pass_occy; num_pessoa++) {
						if ((num_coluna_act == plane.pessoas[num_pessoa].coluna) && (num_linha_act == plane.pessoas[num_pessoa].linha)) {
							if (num_coluna_act > plane.executive) {
								moveCursor(Xpos, Ypos);
								Xpos += Xpos_spacer;
								if(Xpos + Xpos_spacer >= columns){
									Xpos = initialXpos;
									Ypos++;
								}
								if(Ypos >= rows){
									Ypos = initialYpos;
									moveCursor(columns, Ypos);
									printf("^");
									Ypos++;
									for(int i = 0; i < 0; i++){
										moveCursor(columns, Ypos);
										printf("%c", 186);
										Ypos++;
									}
									for(int i = 0; i < (int)(passager_names_window_size * percent_of_bar); i++){
										moveCursor(columns, Ypos);
										printf("%c\n", 219);
										Ypos++;
									}
									for(int i = 0; i < (int)(passager_names_window_size * (1-percent_of_bar)); i++){
										moveCursor(columns, Ypos);
										printf("%c\n", 186);
										Ypos++;
									}
									moveCursor(columns, Ypos);
									printf("v");
									break;
								}
								printf("%d %c: %s %s", 
									(plane.pessoas[num_pessoa].coluna), 
									(plane.pessoas[num_pessoa].linha + 'a' - 1), 
									plane.pessoas[num_pessoa].nome, 
									plane.pessoas[num_pessoa].sobrenome);
							}
						}
					}
				}
			}
		}	
		fclose(planeF);
	}
}

void reserve_loc(){
	int coluna, num_pessoa, num, int_id_reserva, int_id_mala, id_mala, id_reserva;
	int Xpos = (columns/6);
	int Ypos = (rows - 8);
	int option;
	char plane_loc[128];
	char linha;
	FileArray* planesF;
	FILE *planeF;
	PLANE plane;
	
	planesF = ListDirectoryContents(PATH, "pln");
	if(planesF == NULL){
		moveCursor(columns/2 - 10, rows/4);
		printf(">exit");
		moveCursor(columns/2 - 8, rows/4 + 1);
        printf("No planes available");
		getch();
		return;
	}
	strcpy(planesF->files[0], "exit");
	while(1){
		strcpy(plane_loc, PATH);
		option = selectionList(planesF->numberFiles, planesF->files, columns/2 - 8, rows/4);
		if(option == 0){
			freeFileArray(planesF); 
			return;
		}else{
			strcat(plane_loc, planesF->files[option]);
		}
		planeF = fopen(plane_loc, "rb+");
		if(planeF == NULL){
			freeFileArray(planesF); 
			fprintf(stderr, "failed to open file at location %s", plane_loc);
			return;
		}
	
		while(fread(&plane, sizeof(PLANE), 1, planeF)){
			if(plane.pass_occy == (plane.executive*4 + plane.economy*6)){
				fclose(planeF);
				freeFileArray(planesF); 
				moveCursor(2, Ypos-4);
				printf("The plane is full\n");
				getch();
				return;
			}
			
			show_struct(plane, Xpos, Ypos);
			
			while(1){
				moveCursor(2, Ypos-4);
				printf("The column of the seat: ");
				moveCursor(2, Ypos-3);
				printf("The row of the seat: ");
				moveCursor(26, Ypos-4);
				showCursor();
				blinkCursor();
				scanf(" %d", &coluna);
				fflush(stdin);
				moveCursor(24, Ypos-3);
				linha = getchar();
				hideCursor();
				stopBlinkingCursor();
		
				//clear the last Error
				char* blanktext = (char*)malloc(columns - 1); // columns - 2 spaces + 1 for null terminator
				memset(blanktext, ' ', columns - 2);
				blanktext[columns - 2] = '\0';
				printf("\e[%d;2H%s",Ypos-5, blanktext);  // Move to column 2 of row r and clear row
				free(blanktext);

				//printf para os possiveis erro de utilizador
				if((coluna > plane.executive + plane.economy) || (coluna < 0)){
					moveCursor(2, Ypos-5);
					printf("Please choose an existing column\n");
					continue;
				}
				if(((linha > 'd') || (linha < 'a')) && (coluna < plane.executive)){
					moveCursor(2, Ypos-5);
					printf("Please choose an existing row\n");
					continue;
				}
				else if((linha > 'f') || (linha < 'a')){
					moveCursor(2, Ypos-5);
					printf("Please choose an existing row\n");
					continue;
				}
				if(coluna <= plane.executive)
					linha -= 95;
				if(coluna > plane.executive)
					linha -= 96;
				if(plane.matriz[linha-1][coluna-1] == '.'){
					printf("Please choose a non ocupied seat \n");
					continue;
				}
				break;
			}

			num_pessoa = plane.pass_occy;
			plane.pass_occy++;
			//obter o nome/sobrenome
			moveCursor(2, Ypos-5);
			printf("name and surname: ");
			scanf(" %s %s", plane.pessoas[num_pessoa].nome, plane.pessoas[num_pessoa].sobrenome);

			while(1){
				moveCursor(2, Ypos-4);
				printf("Baggage code (0;999): ");
				scanf("%d", &id_mala);
				if(id_mala < 0 || id_mala > 999)
					continue;
				break;
			}
			plane.pessoas[num_pessoa].id_mala = id_mala;
			
			while(1){
				moveCursor(2, Ypos-3);
				printf("Reservation code (0;999): ");
				scanf("%d", &id_reserva);
				if(id_reserva < 0 || id_reserva > 999)
					continue;
				break;
			}
			plane.pessoas[num_pessoa].id_reserva = id_reserva;

			plane.matriz[linha-1][coluna-1] = '.';
			plane.pessoas[num_pessoa].linha = linha;
			plane.pessoas[num_pessoa].coluna = coluna;
		}
		rewind(planeF);
		fwrite (&plane, sizeof(PLANE), 1, planeF);
		fclose(planeF);

		//clear the last plane struc drawing
		char* blanktext = (char*)malloc(columns - 1); // columns - 2 spaces + 1 for null terminator
		memset(blanktext, ' ', columns - 2);
		blanktext[columns - 2] = '\0';
		for (int i = rows - 8; i < rows; ++i) {
			printf("\e[%d;2H%s", i, blanktext);  // Move to column 2 of row r and clear row
		}
		for (int i = rows - 13; i < rows - 10; ++i) {
			printf("\e[%d;2H%s", i, blanktext);  // Move to column 2 of row r and clear row
		}
		free(blanktext);
	}
}

void flgh_rand_pass(){
	char nome[20], sobrenome[20];
	char ch1;
	int num_lugares = 0, num_lugares_livres = 0, num_lugares_encher = 0, lugar_linha = 0, lugar_coluna = 0, num_pessoa, linha = 0, linha_act = 0, num;
	int option;
	int planeFull = 0;
	float ocupacao_percent = 0.;
	char plane_loc[128];
	FileArray* planesF;
	FILE *planeF, *names, *surnames;
	PLANE plane;
	
	planesF = ListDirectoryContents(PATH, "pln");
	if(planesF == NULL){
		moveCursor(columns/2 - 10, rows/4);
		printf(">exit");
		moveCursor(columns/2 - 8, rows/4 + 1);
        printf("No planes available");
		getch();
		return;
	}
	strcpy(planesF->files[0], "exit");
	while(1){
		strcpy(plane_loc, PATH);
		option = selectionList(planesF->numberFiles, planesF->files, columns/2 - 8, rows/4);
		if(option == 0){
			freeFileArray(planesF); 
			return;
		}else{
			strcat(plane_loc, planesF->files[option]);
		}
		planeF = fopen(plane_loc, "rb+");
		if(planeF == NULL){
			freeFileArray(planesF); 
			fprintf(stderr, "failed to open file at location %s", plane_loc);
			return;
		}

		while(fread(&plane, sizeof(PLANE), 1, planeF)){
			if(plane.pass_occy == (plane.executive*4 + plane.economy*6)){
				moveCursor(columns/2 - 8, rows/4 + option);
				printf("The plane is full\n");
				planeFull = 1;
				break;
			}

			num_lugares_livres = (plane.executive*4 + plane.economy*6) - plane.pass_occy;
			num_lugares_encher = numRand(1, num_lugares_livres);
			num_pessoa = plane.pass_occy;

			//posicionar pessoas nos lugares ocupados
			for(num = 0; num < num_lugares_encher; num++){
				while(1){
					lugar_linha = numRand(1, 6);
					lugar_coluna = numRand(1, (plane.executive + plane.economy));
					if((lugar_coluna <= plane.executive) && ((lugar_linha == 1) || (lugar_linha == 6))) continue;
					if(plane.matriz[lugar_linha-1][lugar_coluna-1] == '.') continue;
					// nome
					names = fopen(NAMES_PATH, "r");
					
					// ir até a uma certa linha
					linha_act = 0;
					linha = numRand(1,816);
					do{
						ch1 = getc(names);
						if(ch1 == '\n')
							linha_act++;
					}while((linha > linha_act) && (!(feof(names))));
					//obter essa linha	
					fgets(nome, sizeof(nome), names);
					nome[strcspn(nome, "\n")] = 0;
					fclose(names);
					
					//sobrenome
					surnames = fopen(SURNAMES_PATH, "r");

					// ir até a uma certa linha
					linha_act = 0;
					linha = numRand(1,753);
					do{
						ch1 = getc(surnames);

						if(ch1 == '\n')
							linha_act++;
					}while((linha > linha_act) && (!(feof(surnames))));
					
					//obter essa linha	
					fgets(sobrenome, sizeof(sobrenome), surnames);
					sobrenome[strcspn(sobrenome, "\n")] = 0;
					fclose(surnames);
					
					//passar informação para a estrutura
					plane.matriz[lugar_linha-1][lugar_coluna-1] = '.'; 				
					plane.pessoas[num_pessoa].linha = lugar_linha;
					plane.pessoas[num_pessoa].coluna = lugar_coluna;
					strcpy(plane.pessoas[num_pessoa].nome, nome);
					strcpy(plane.pessoas[num_pessoa].sobrenome, sobrenome);
					plane.pessoas[num_pessoa].id_mala = numRand(1, 999);
					plane.pessoas[num_pessoa].id_reserva = numRand(1, 999);
					
					num_pessoa++;
					break;
				}
			}
		}
		if(planeFull){
			planeFull = 0;
			getch();
			continue;
		}
	
		plane.pass_occy += num_lugares_encher;
		num_lugares = (plane.executive*4 + plane.economy*6); 
		ocupacao_percent = ((plane.pass_occy*100.)/num_lugares);
		moveCursor(2, rows-11);
		printf("The plane has %d of which %d were reserved being of the total occupation %.2f%%", num_lugares, num_lugares_encher, ocupacao_percent);
		
		rewind(planeF);
		fwrite (&plane, sizeof(PLANE), 1, planeF);
		fclose(planeF);
	}
}

void create_flgh(){
	char ch1;
	int num_filas = 0, num_colunas = 0, executivo = 0, turista = 0;
	int option;
	int Xpos = columns/4, Ypos = rows/4;
	char plane_loc[128];
	FileArray* planesF;
	FILE  *tipoF;
	FILE *planeF = NULL;
	PLANE plane;
	
	planesF = ListDirectoryContents(PATH, "txt");
	strcpy(planesF->files[0], "exit");

	strcpy(plane_loc, PATH);
	option = selectionList(planesF->numberFiles, planesF->files, Xpos, Ypos);
	if(option == 0){
		freeFileArray(planesF); 
		return;
	}else{
		strcat(plane_loc, planesF->files[option]);
	}
	tipoF = fopen(plane_loc, "r");
	if(tipoF == NULL){
		freeFileArray(planesF); 
		fprintf(stderr, "failed to open file at location %s", plane_loc);
		return;
	}
	
	// ler caracter a caracter o numero de linhas do avião
	while(!(feof(tipoF))){
		ch1 = fgetc(tipoF);
		if(ch1 == '\n'){
			if (num_filas == 4)
				executivo++;
			else
				turista++;
			num_filas = 0;
		}
		else
			num_filas++;
	};
	
	Xpos = columns/2;
	Ypos = rows*(3.0/4);

	// criar o ficheiro de avião
	char plane_name[10];
	while(planeF == NULL){
		strcpy(plane_loc, PATH);

		moveCursor(2, Ypos);
		printf("Write the name of the plane: ");
		showCursor();
		blinkCursor();
		scanf("%s", plane_name);
		fflush(stdin);
		hideCursor();
		stopBlinkingCursor();
		
		strcat(plane_loc, plane_name);
		strcat(plane_loc, ".pln");

		planeF = fopen(plane_loc, "r");
		if(planeF != NULL){
			moveCursor(2, rows - 9);
			printf("A plane with that name already exists write another or write 's' to exit\n\n");
			fclose(planeF);
			planeF = NULL;
			continue;
		}
		break;
	}

	planesF = ListFileContents("Locations.txt");
	if(planesF == NULL){
		moveCursor(Xpos, Ypos);
		printf(">exit");
		moveCursor(Xpos+2, Ypos + 1);
		printf("No Locations?!");
		getch();
		return;
	}
	strcpy(planesF->files[0], "exit");

	moveCursor(Xpos, Ypos);
	printf("current plane location");
	option = selectionList(planesF->numberFiles, planesF->files, Xpos, Ypos+1);
	if(option == 0){
		freeFileArray(planesF); 
		return;
	}else{
		strcpy(plane.location, planesF->files[option]);
	}

	strcpy(plane.name, plane_name);
	strcpy(plane.type, planesF->files[option]);
	plane.executive = executivo;
	plane.economy = turista;
	plane.pass_occy = 0;
	
	memset(plane.matriz, '0', sizeof(plane.matriz[0][0]) * 6 * 35);
	
	//inicializar todas as coordenadas de pessoa a (0,0)
	for(int num = 0; num < (executivo*4 + turista*6); num++){
		plane.pessoas[num].linha = 0;
		plane.pessoas[num].coluna = 0; 
	}

	planeF = fopen(plane_loc, "wb+");
	fwrite (&plane, sizeof(PLANE), 1, planeF);
		
	freeFileArray(planesF);
	fclose(tipoF);
	fclose(planeF);
}

void delete_flgh(){
	int option;
	int Xpos = (columns/2 - 8), Ypos = (rows/4);
	char plane_loc[128];
	FileArray* planesF;
	while(1){
		clrscr();
		planesF = ListDirectoryContents(PATH, "pln");
		if(planesF == NULL){
			moveCursor(columns/2 - 10, rows/4);
			printf(">exit");
			moveCursor(columns/2 - 8, rows/4 + 1);
			printf("No planes available");
			getch();
			return;
		}
		strcpy(planesF->files[0], "exit");

		strcpy(plane_loc, PATH);
		option = selectionList(planesF->numberFiles, planesF->files, Xpos, Ypos);
		if(option == 0){
			freeFileArray(planesF); 
			return;
		}else{
			strcat(plane_loc, planesF->files[option]);
		}
		if (remove(plane_loc) == 0){
			moveCursor(Xpos, Ypos + option);
			printf("Deleted successfully");
		}
		else{
			moveCursor(Xpos, Ypos + option);
			printf("Unable to delete the file");
		}
		freeFileArray(planesF);
		getch(); 
	}
}

void show_map(){
	FILE *mapF; 
	char ch; 
	const int startingXpos = 3;
	int Xpos = startingXpos, Ypos = 2;

	mapF = fopen("worldmap.txt", "r"); 
	if (mapF == NULL){ 
		moveCursor(2, rows-1);
		printf("Cannot open file \n"); 
		return; 
	} 

	ch = fgetc(mapF); 
	while (ch != EOF){ 
		moveCursor(Xpos, Ypos);
		printf ("%c", ch); 
		ch = fgetc(mapF); 
		if(ch == '\n'){
			Ypos++;
			Xpos = startingXpos;
		}
		Xpos++;
	} 

	fclose(mapF); 
	getch();
	return ; 
}

void start_flight(){
	int option;
	int Xpos = columns/3, Ypos = rows/4;
	char plane_loc[128];
	FileArray* planesArray;
	FileArray* LocationsArray;
	FILE *planeF;
	PLANE plane;

	strcpy(plane_loc, PATH);
	planesArray = ListDirectoryContents(PATH, "pln");
	if(planesArray == NULL){
		moveCursor(Xpos, Ypos);
		printf(">exit");
		moveCursor(Xpos + 2, Ypos + 1);
		printf("No planes available");
		getch();
		return;
	}
	strcpy(planesArray->files[0], "exit");
	option = selectionList(planesArray->numberFiles, planesArray->files, Xpos, Ypos);
	if(option == 0){
		freeFileArray(planesArray); 
		return;
	}else{
		strcat(plane_loc, planesArray->files[option]);
	}
	planeF = fopen(plane_loc, "rb+");
	if(planeF == NULL){
		freeFileArray(planesArray); 
		fprintf(stderr, "failed to open file at location %s", plane_loc);
		return;
	}
	
	Xpos = columns*(2.0/3);
	Ypos = rows/4;

	while(fread(&plane, sizeof(PLANE), 1, planeF)){
		LocationsArray = ListFileContents("Locations.txt");
		if(LocationsArray == NULL){
			moveCursor(Xpos, Ypos);
			printf(">exit");
			moveCursor(Xpos +2, Ypos + 1);
			printf("No Locations?!");
			getch();
			return;
		}
		strcpy(LocationsArray->files[0], "exit");

		option = selectionList(LocationsArray->numberFiles, LocationsArray->files, Xpos, Ypos);
		if(option == 0){
			freeFileArray(planesArray); 
			freeFileArray(LocationsArray); 
			return;
		}else{
			strcpy(plane.destination, LocationsArray->files[option]);
		}
		break;
	}
	fwrite (&plane, sizeof(PLANE), 1, planeF);
	fclose(planeF);
}

int main(){
	PLANE plane;
	char keyInput;
	int option;
	int xPos;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	srand (time(NULL));
	hideCursor();
	clrscr_full();
	
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	rows -= 1;
	xPos  = (columns/2) - 14;
	printBox();
	printLogo();
	moveCursor(xPos, 20);
	printf("press any button to continue");
	getch();
	while(1){
		//int scrollMouse = GetScrollDelta(); too slow right now
		option = selectionList(10, (char*[]){
			"Exit",
			"Show the structure of the plane",
			"Show the occupancy of a flight",
			"Show the passenger name with the reservation for this flight",
			"Reserve a flight with a seat number",
			"Fill in a flight with random passengers",
			"Create a plane of a specific type",
			"Delete a plane",
			"Show world map",
			"Start flight"
    	}, (columns/2) - 20, (rows*2/5));

		switch(option){
			case 0: clrscr_full(); exit(1); break;
			case 1: clrscr(); struct_plane(); break;
			case 2: clrscr(); flgh_occy(); break;
			case 3: clrscr(); passager_names(); break;
			case 4: clrscr(); reserve_loc(); break;
			case 5: clrscr(); flgh_rand_pass(&plane); break;	
			case 6: clrscr(); create_flgh(); break;		
			case 7: clrscr(); delete_flgh(); break;	
			case 8: clrscr(); show_map(); break;
			case 9: clrscr(); start_flight(); break;
		}
		clrscr();
		printLogo();
	}
}