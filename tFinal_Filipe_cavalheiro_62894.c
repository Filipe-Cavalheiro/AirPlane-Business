/**
 * @author Filipe Cavalheiro
 * @name Airplane Business
 * @date 12-07-2024
 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include <conio.h>

#define PATH ".\\"
#define NAMES_PATH ".\\names"
#define SURNAMES_PATH ".\\surnames"

struct pessoas{
	char nome[20];
	char sobrenome[20];
	int linha;
	int coluna;
	int id_mala;
	int id_reserva;
};

typedef struct plane_inf{
	char voo[10];
	char tipo[10];
	int pass_occy;
	int executivo;
	int turista;
	char matriz[6][35];
	struct pessoas pessoas[230];
} PLANE;


int numRand(int MIN, int MAX){
	int Rnum;
	Rnum = (rand() % (MAX - MIN +1))+ MIN;
	return Rnum;
}


void press_enter(){
	char ch1;

	while ((ch1 = getchar()) != '\n' && ch1 != EOF);
		
	while(ch1 != '\n'){
	printf("\n\n\t\tPlease press ENTER\n\n"); 
	    ch1 = getchar();
	}
}


void show_struct(PLANE plane){
	int primeiro_correr = 1, num_coluna_act, num_linha_act, num;
	for(num_linha_act = 1; num_linha_act <= 6; num_linha_act++){
			for(num_coluna_act = 1; num_coluna_act <= (plane.executivo + plane.turista); num_coluna_act++){	
				if((num_linha_act == 4) && (primeiro_correr)){
					for(num = 1; num <= (plane.executivo + plane.turista); num++)
						printf("%2d", num);
					printf("\n");
					primeiro_correr = 0;
				}
				if(plane.matriz[num_linha_act-1][num_coluna_act-1] != '.'){
					if(num_coluna_act <= plane.executivo){
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
				if(num_coluna_act == (plane.executivo + plane.turista))
					printf("\n");
			}
		}
}

FILE *get_plane(char permissions[]){
	char plane[20];
	FILE *fileF;

	fileF = NULL;
	while(fileF == NULL){
		char voo_loc[100] = PATH;
		printf("write the name of the plane: ");
		scanf(" %s", plane);
		char tmp_plane[] = "plane_";
		strcat(tmp_plane, plane);
		strcat(voo_loc, tmp_plane);
		
		if((plane[0] == 's') && (strlen(plane) == 1))
			return NULL;
			
		fileF = fopen(voo_loc, permissions);
		if(fileF == NULL)
			printf("No match in data base, write a valid plane or write 's' to exit\n");
	}
	return fileF;
}

void struct_plane(){
	int executivo = 0; 
	int turista = 0; 
	int num_filas = 0;
	char ch1;
	FILE *tipoF;
	PLANE plane;
	
	// obter o ficheiro do tipo de avião
	tipoF = NULL;
	while(tipoF == NULL){	
		char tipo[10];
		char plane_loc[100] = PATH;
		printf("Plane Types: Airbus318, Airbus319, Airbus320, Airbus321\n\nType of desired plane: ");
		scanf(" %s", tipo);
		strcat(plane_loc, tipo);
		tipoF = fopen(plane_loc, "r");
		
		if(tipoF == NULL)
			printf("Invalid name, write 's' to exit\n");
	}

	// ler caracter a caracter o ficheiro
	do{
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
	}while(!(feof(tipoF)));
	printf("The plane has %d executive and %d turist rows\n", executivo, turista);

	fclose(tipoF);
	press_enter();
}

void flgh_occy(){
	FILE *planeF;
	PLANE plane;
	
	//obter o ficheiro do voo do avião
	planeF = get_plane("rb+");	
	if(planeF == NULL)
		return;

	while(fread(&plane, sizeof(PLANE), 1, planeF))
		show_struct(plane);

	fclose(planeF);
	press_enter();
}


void passager_names() {
    int num_pessoa, num_linha_act, num_coluna_act;
    FILE *planeF;
    PLANE plane;

    // obter o ficheiro do voo do avião
    planeF = get_plane("rb+");
    if (planeF == NULL)
        return;

    while (fread(&plane, sizeof(struct plane_inf), 1, planeF)) {
        if (plane.pass_occy == 0) {
            printf("the plane is empty\n");
            fclose(planeF);
            press_enter();
            return;
        }

        printf("Executive: \n");
        for (num_coluna_act = 1; num_coluna_act <= plane.executivo; num_coluna_act++) {
            for (num_linha_act = 2; num_linha_act <= 5; num_linha_act++) {
                for (num_pessoa = 0; num_pessoa < plane.pass_occy; num_pessoa++) {
                    if ((num_coluna_act == plane.pessoas[num_pessoa].coluna) && (num_linha_act == plane.pessoas[num_pessoa].linha)) {
                        printf("\tcolumn %d line %c: name = %s %s\n", 
                            (plane.pessoas[num_pessoa].coluna), 
                            (plane.pessoas[num_pessoa].linha + 'a' - 2), 
                            plane.pessoas[num_pessoa].nome, 
                            plane.pessoas[num_pessoa].sobrenome);
                    }
                }
            }
        }

        printf("Tourist: \n");
        for (num_coluna_act = 1; num_coluna_act <= (plane.executivo + plane.turista); num_coluna_act++) {
            for (num_linha_act = 1; num_linha_act <= 6; num_linha_act++) {
                for (num_pessoa = 0; num_pessoa < plane.pass_occy; num_pessoa++) {
                    if ((num_coluna_act == plane.pessoas[num_pessoa].coluna) && (num_linha_act == plane.pessoas[num_pessoa].linha)) {
                        if (num_coluna_act > plane.executivo) {
                            printf("\tcolumn %d line %c: name = %s %s\n", 
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
    press_enter();
}



void reserve_loc(){
	int coluna, num_pessoa, num, int_id_reserva, int_id_mala, id_mala, id_reserva;
	char linha;
	FILE *planeF;
	PLANE plane;
	
	//obter o ficheiro do voo do avião
	planeF = get_plane("rb+");
	if(planeF == NULL)
		return;
	
	while(fread(&plane, sizeof(PLANE), 1, planeF)){
		if(plane.pass_occy == (plane.executivo*4 + plane.turista*6)){
			printf("The plane is full\n");
			press_enter();
			return;
		}
		
		show_struct(plane);
		
		int run = 1;
		while(run){
			printf("The column of the seat: ");
			scanf(" %d", &coluna);
			fflush(stdin);
			printf("The row of the seat: ");
			scanf(" %d", &linha);
			
			//printf para os possiveis erro de utilizador
			if((coluna > plane.executivo + plane.turista) || (coluna < 0)){
				printf("Please choose an existing column\n");
				continue;
			}
			if(((linha > 'd') || (linha < 'a')) && (coluna <= plane.executivo)){
				printf("Please choose an existing row\n");
				continue;
			}
			else if((linha > 'f') || (linha < 'a')){
					printf("Please choose an existing row\n");
					continue;
			}
			if(coluna <= plane.executivo)
				linha = linha - 95;
			if(coluna > plane.executivo)
				linha = linha - 96;
			if(plane.matriz[linha-1][coluna-1] == '.'){
				printf("Porfavor escolha um lugar não ocupado \n");
				continue;
			}
			run = 0;
		}

		num_pessoa = plane.pass_occy;
		plane.pass_occy++;
		//obter o nome/sobrenome
		printf("name and surname: ");
		scanf(" %s %s", plane.pessoas[num_pessoa].nome, plane.pessoas[num_pessoa].sobrenome);

		run = 1;
		while(run){
			printf("Baggage code (0;999): ");
			scanf("%d", id_mala);
			run = 0;
			if(id_mala < 0 || id_mala > 999)
				run = 1;
		}
		plane.pessoas[num_pessoa].id_mala = id_mala;
		
		run = 1;
		while(run){
			printf("Reservation code (0;999): ");
			scanf("%d", id_reserva);
			run = 0;
			if(id_reserva < 0 || id_reserva > 999)
				run = 1;
		}
		plane.pessoas[num_pessoa].id_reserva = id_reserva;

		plane.matriz[linha-1][coluna-1] = '.';
		plane.pessoas[num_pessoa].linha = linha;
		plane.pessoas[num_pessoa].coluna = coluna;
	}
	rewind(planeF);
	fwrite (&plane, sizeof(PLANE), 1, planeF);
	fclose(planeF);
	press_enter();
}


void switch_pass(){
	int coluna, num_pessoa = 0, coluna_nova, run = 1;	
	char linha, linha_nova;
	FILE *planeF;
	PLANE plane;
	
	//obter o ficheiro do voo do avião
	planeF = get_plane("rb+");
	if(planeF == NULL)
		return;
	
	while(fread(&plane, sizeof(PLANE), 1, planeF)){
		if(plane.pass_occy == (plane.executivo*4 + plane.turista*6)){
			printf("O avião encontra-se cheio não é possivel reservar lugares \n");
			press_enter();
			return;
		}		
		
		show_struct(plane);
		
		do{	
			do{
				printf("A coluna do seu lugar: ");
				scanf(" %d", &coluna);
				fflush(stdin);
				if((coluna > plane.executivo + plane.turista) || (coluna < 0))
					printf("Porfavor escolha uma coluna existente \n");
			}while(coluna > plane.executivo + plane.turista);
			
			do{
				run = 1;
				printf("O fila do seu lugar: ");
					scanf(" %c", &linha);
				if(coluna <= plane.executivo){ //lugares executivos
					if((linha > 'd') || (linha < 'a')){
						printf("Porfavor escolha uma fila existente \n");
						continue;
					}
				}
				else{
					if((linha > 'f') || (linha < 'a')){
						printf("Porfavor escolha uma fila existente \n");
						continue;
					}
				}
				run = 0;
			}while(run);
			
			if(coluna <= plane.executivo)
				linha = linha - 95;
			if(coluna > plane.executivo)
				linha = linha - 96;
			if(plane.matriz[linha-1][coluna-1] != '.')
				printf("Porfavor escolha um lugar ocupado \n");
		}while(plane.matriz[linha-1][coluna-1] != '.');
		
		//procurar da esquerda para a direita de cima para baixo a pessoa nesse lugar
		for(num_pessoa = 0; num_pessoa <= plane.pass_occy; num_pessoa++){
			if((plane.pessoas[num_pessoa].linha == linha) && (plane.pessoas[num_pessoa].coluna == coluna)){ // procurar o numero da pessoa no lugar escolhido
				do{
					do{
						printf("A coluna do seu novo lugar: ");
						scanf(" %d", &coluna_nova);
						fflush(stdin);
						if((coluna_nova > plane.executivo + plane.turista) || (coluna_nova < 0))
							printf("Porfavor escolha uma coluna existente \n");
					}while(coluna_nova > plane.executivo + plane.turista);
					
					do{
						run = 1;
						printf("O fila do seu novo lugar: ");
							scanf(" %c", &linha_nova);
						if(coluna_nova <= plane.executivo){ //lugares executivos
							if((linha_nova > 'd') || (linha_nova < 'a')){
								printf("Porfavor escolha uma fila existente \n");
								continue;
							}
						}
						else{
							if((linha_nova > 'f') || (linha_nova < 'a')){
								printf("Porfavor escolha uma fila existente \n");
								continue;
							}
						}
						run = 0;
					}while(run);
	
					if(coluna_nova <= plane.executivo)
						linha_nova = linha_nova - 95;
					if(coluna_nova > plane.executivo)					
						linha_nova = linha_nova - 96;
					if(plane.matriz[linha_nova-1][coluna_nova-1] == '.' )
						printf("Porfavor escolha um lugar não ocupado \n");
				}while(plane.matriz[linha_nova-1][coluna_nova-1] == '.');
				plane.pessoas[num_pessoa].linha = linha_nova;
				plane.pessoas[num_pessoa].coluna = coluna_nova;
				plane.matriz[linha-1][coluna-1] = '0';
				plane.matriz[linha_nova-1][coluna_nova-1] = '.'; 	
			}
		}
	}
	rewind(planeF);
	fwrite (&plane, sizeof(PLANE), 1, planeF);
	fclose(planeF);
	press_enter();
}


void create_flgh(){
	char tipo[10];
	char ch1;
	int num_filas = 0, num_colunas = 0, executivo = 0, turista = 0, num = 0;
	FILE  *tipoF;
	PLANE plane;
	
	// obter o ficheiro do tipo de avião
	tipoF = NULL;
	while(tipoF == NULL){	
		char plane_loc[100] = PATH;
		printf("Plane Types: Airbus318, Airbus319, Airbus320, Airbus321\n\nType of desired plane: ");
		scanf(" %s", tipo);
		if((tipo[0] == 's') && (strlen(tipo) == 1))
			return;

		strcat(plane_loc, tipo);
		tipoF = fopen(plane_loc, "r");
		
		if(tipoF == NULL)
			printf("Invalid name, write 's' to exit\n");
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
	
	// criar o ficheiro de avião
	char plane_name[10];
	FILE *planeF = NULL;
	while(planeF == NULL){
		char plane_loc[100] = PATH;
		printf("Write the name of the plane: ");
		scanf(" %s", plane_name);
		if((plane_name[0] == 's') && (strlen(plane_name) == 1))
			return;
		
		char tmp_string[] = "plane_";
		strcat(tmp_string, plane_name);
		strcat(plane_loc, tmp_string);
		
		planeF = fopen(plane_loc, "rb+");
		if(planeF != NULL){
			printf("A plane with that name already exists write another or write 's' to exit\n\n");
			fclose(planeF);
			planeF = NULL;
			continue;
		}
		planeF = fopen(plane_loc, "wb+");
	}

	strcpy(plane.voo, plane_name);
	strcpy(plane.tipo, tipo);
	plane.executivo = executivo;
	plane.turista = turista;
	plane.pass_occy = 0;
	
	memset(plane.matriz, '0', sizeof(plane.matriz[0][0]) * 6 * 35);
	
	//inicializar todas as coordenadas de pessoa a (0,0)
	for(num = 0; num < (executivo*4 + turista*6); num++){
		plane.pessoas[num].linha = 0;
		plane.pessoas[num].coluna = 0; 
	}

	fwrite (&plane, sizeof(PLANE), 1, planeF);
		
	fclose(tipoF);
	fclose(planeF);
	press_enter();
}


void flgh_rand_pass(){
	char nome[20], sobrenome[20];
	char ch1;
	int num_lugares = 0, num_lugares_livres = 0, num_lugares_encher = 0, lugar_linha = 0, lugar_coluna = 0, num_pessoa, linha = 0, linha_act = 0, num;
	float ocupacao_percent = 0.;
	FILE *planeF, *names, *surnames;
	PLANE plane;
	
	//obter o ficheiro do voo do avião
	planeF = get_plane("rb+");
	if(planeF == NULL)
		return;

	while(fread(&plane, sizeof(PLANE), 1, planeF)){
		if(plane.pass_occy == (plane.executivo*4 + plane.turista*6)){
			printf("O avião encontra-se cheio não é possivel reservar lugares\n");
			press_enter();
			return;
		}

		num_lugares_livres = (plane.executivo*4 + plane.turista*6) - plane.pass_occy;
		num_lugares_encher = numRand(1, num_lugares_livres);
		num_pessoa = plane.pass_occy;

		//posicionar pessoas nos lugares ocupados
		for(num = 0; num < num_lugares_encher; num++){
			while(1){
				lugar_linha = numRand(1, 6);
				lugar_coluna = numRand(1, (plane.executivo + plane.turista));
				if((lugar_coluna <= plane.executivo) && ((lugar_linha == 1) || (lugar_linha == 6))) continue;
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

	
	plane.pass_occy += num_lugares_encher;
	num_lugares = (plane.executivo*4 + plane.turista*6); 
	ocupacao_percent = ((plane.pass_occy*100.)/num_lugares);
	printf("The plane has %d of which %d were reserved being of the total occupation %.2f%%\n\n", num_lugares, num_lugares_encher, ocupacao_percent);
	
	rewind(planeF);
	fwrite (&plane, sizeof(PLANE), 1, planeF);
	fclose(planeF);
	press_enter();
}


void historiograma(){
	int num_turista = 0, per_turista = 0, num_executivo = 0, per_executivo = 0, num_linha_act = 0, num_coluna_act = 0, num = 0, janela = 0, per_janela = 0, meio = 0, per_meio, centro = 0, per_centro = 0;
	FILE *planeF;
	PLANE plane;
	
	//obter o ficheiro do voo do avião
	planeF = get_plane("rb+");
	if(planeF == NULL)
		return;
	
	// ler a informação do avião
	while(fread(&plane, sizeof(struct plane_inf), 1, planeF)){
		show_struct(plane);
		
		//numero de executivos
		for(num_coluna_act = 1; num_coluna_act <= (plane.executivo + plane.turista); num_coluna_act++){
			for(num_linha_act = 1; num_linha_act <= 6; num_linha_act++){
				if(plane.matriz[num_linha_act-1][num_coluna_act-1] == '.'){
					if(num_coluna_act <= plane.executivo)
						num_executivo++;
					else
						num_turista++;
				}
			}
		}
		
		for(num_coluna_act = 1; num_coluna_act <= (plane.executivo + plane.turista); num_coluna_act++){
			for(num_linha_act = 1; num_linha_act <= 6; num_linha_act++){
				if(num_coluna_act > plane.executivo){
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
		per_janela = janela*70/(plane.turista*2+plane.executivo*2);
		per_meio = meio*70/(plane.turista*2);
		per_centro = centro*70/(plane.turista*2 + plane.executivo*2);
		per_executivo = num_executivo*70/(plane.executivo*4);
		per_turista = num_turista*70/(plane.turista*6);
		printf("janela:    ");
		for(num = 0; num < per_janela; num++)
			printf("*");
		printf(" %.2f%% \n", (float)janela*50/(plane.turista+plane.executivo)); // multiplico por 50 pois janela são duas linha
		printf("meio:      ");
		for(num = 0; num < per_meio; num++)
			printf("*");
		printf(" %.2f%% \n", (float)meio*50/(plane.turista)); // multiplico por 50 pois meio são duas linha
		printf("centro:    ");
		for(num = 0; num < per_centro; num++)
			printf("*");
		printf(" %.2f%% \n", (float)centro*50/(plane.turista+plane.executivo)); // multiplico por 50 pois centro são duas linha
		printf("executivo: ");
		for(num = 0; num < per_executivo; num++)
			printf("*");
		printf(" %.2f%% \n", (float)num_executivo*100/(plane.executivo*4));
		printf("turista:   ");
		for(num = 0; num < per_turista; num++)
			printf("*");
		printf(" %.2f%% \n", (float)num_turista*100/(plane.turista*6));
	}
	fclose(planeF);
	press_enter();	
}

int main(){
	srand (time(NULL));
	while(1){
		PLANE plane;
		char escolha[10];
		printf("\n\t\t\tAircarft Business\n\n0 - exit\n1 - Show the structure of the plane\n2 - Show the occupancy of a flight\n3 - Show the passeger name with the reservation for this flight\n\n4 - Reserve a flight with a seat number\n5 - Change the seat number\n6 - create a plane of a specific type\n\n7 - Fill in a flight with a random passagers\n\n8 - Histogram of flight occupancy per type of seat");
		printf("\nEscolha: ");
		scanf(" %s", escolha);
		if(strlen(escolha) == 1){
			switch(escolha[0]){
				case'0': printf("A sair do codigo\nObrigado\n"); exit(1); break;
				case'1': struct_plane(plane); break;
				case'2': flgh_occy(); break;
				case'3': passager_names(); break;
				case'4': reserve_loc(); break;
				case'5': switch_pass(&plane); break;
				case'6': create_flgh(); break;		
				case'7': flgh_rand_pass(&plane); break;		 			
				case'8': historiograma(); break;
				default: printf("Por favor faça uma escolha entre 0 e 10 \n");break;
			}
		}
		else{
			printf("Por favor faça uma escolha entre 0 e 9\n");
		}
	}
}