/*
Autores: Antonio Sant'Ana e Gabriel Honorato
Código: Testes com leitura de matrizes a partir de um arquivo
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct{
    char nome[30];
    int pontos;
}tJogador;

int BuscaPalavra(int nlinhas, int ncolunas, char **matriz, char* palavra);

int diagonal(int nlinhas, int ncolunas, int x, int y, char **matriz, char* palavra);  //Função para identificação de palavras na diagonal

int vertical(int nlinhas, int ncolunas, int x, int y, char **matriz, char* palavra);  //Função para identificação de palavras na vertical

int horizontal(int nlinhas, int ncolunas, int x, int y, char **matriz, char* palavra);

int main(int numargs, char *arg[]){
    if(numargs != 4){
        printf("\nNumero de argumentos invalido, reinicie o programa e tente novamente.\n");
        return 1;
    }

    FILE *fonteMatriz;
    tJogador player1, player2;
    player1.pontos = player2.pontos = 0;

    fonteMatriz = fopen(arg[1], "r");

    int nlinhas = (int)strtol (arg[2], NULL, 10);
    int ncolunas = (int)strtol (arg[3], NULL, 10);
    int maior, continuar = 1, rodada = 1;

    char *ptr;
    char **m;
    
    
    if(nlinhas > ncolunas){
        maior = nlinhas;
    }
    else{
        maior = ncolunas;
    }

    char *palavra = malloc((maior + 1)*sizeof(char));
    char *input = malloc((maior + 1)*sizeof(char));

    m = malloc((nlinhas*(int)sizeof(char*)));

    for(int i = 0; i < nlinhas; i++){
        m[i] = malloc(ncolunas*(int)sizeof(char));
    }

    for(int i = 0; i < nlinhas; i++){
        int pular = 0;
        for(int j = 0; j < ncolunas; j++){
            char ler = getc(fonteMatriz);
            if(ler == EOF){
                printf("\nA MATRIZ NAO CORRESPONDE AO TAMANHO ESPECIFICADO.\n");
                printf("\nReinicie o programa e tente novamente.\n");
                return 0;
            }
            if(ler == ' '){
                ler = getc(fonteMatriz);
                m[i][j] = (char)ler;
            }
            
            if(ler == '\n'){
                ler = getc(fonteMatriz);
                m[i][j] = (char)ler;
            }
            
            else{
                m[i][j] = (char)ler;
            }
            
        }
    }

    printf("\nNome Jogador 1: ");
    scanf("%s", player1.nome);

    printf("\nNome Jogador 2: ");
    scanf("%s", player2.nome);

    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
    printf("\n");

    for(int i = 0; i<nlinhas; i++){     //Loop para "plot" a matriz
        for(int j = 0; j<ncolunas; j++){
            printf("%c ", m[i][j]);
        }
        printf("\n");
    }

    while(continuar){
        printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");

        if(rodada % 2 == 0){
            printf("\n%s\n", player2.nome);
        }
        else{
            printf("\n%s\n", player1.nome);
        }
        

        printf("\nDigite uma palavra: ");
        scanf("%s", input);
        for (int i=0;i<strlen(input);i++) {    //Loop para ".upper" da palavra inserida
                palavra[i] = toupper(input[i]);
            }

        int i = strlen(input);
        palavra[i] = '\0';

        if(palavra[0] == '.' && palavra[1] == '\0'){    //Condição para encerramento do programa
                printf("\nPontuacao de %s: %d", player1.nome, player1.pontos);
                printf("\nPontuacao de %s: %d\n", player2.nome, player2.pontos);
                if(player1.pontos > player2.pontos){
                    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("\nVencedor: %s\n", player1.nome);
                    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("\nPROGRAMA ENCERRADO\n");
                }
                else if(player2.pontos > player1.pontos){
                    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("\nVencedor: %s\n", player2.nome);
                    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("\nPROGRAMA ENCERRADO\n");
                }
                else{
                    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("\nEMPATE!\n");
                    printf("\n=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=\n");
                    printf("\nPROGRAMA ENCERRADO\n");
                }
                for(int i = 0; i < nlinhas; i++){
                    free(m[i]);
                }
                free(m);
                return 0;
        }

        if(rodada % 2 == 0){
            player2.pontos += BuscaPalavra(nlinhas, ncolunas, m, palavra);
        }
        else{
            player1.pontos += BuscaPalavra(nlinhas, ncolunas, m, palavra);
        }
        rodada++;
    }

}

int BuscaPalavra(int nlinhas, int ncolunas, char **matriz, char* palavra) {   //Função principal que comanda as demais para identificação das palavras na matriz
    int i, j, achou = 0; //Declaração da variável boleana "achou" para representação da presença ou ausência da palavra na matriz
    int pontos = 0;
    for (i = 0; i < nlinhas; i++) {   //Loop que procura pela primeira letra da palavra na matriz, para posteriores comandos
        for (j = 0; j < ncolunas; j++) {
            if (matriz[i][j] == palavra[0]) {
                if (vertical(nlinhas, ncolunas, j,i,matriz, palavra)) {    //Comando que "chama" a função de identificação vertical
                    printf("\nPalavra encontrada NA VERTICAL em (%d,%d) a (%ld,%d)\n",i+1,j+1,i+strlen(palavra),j+1);
                    pontos++;
                }
                if (horizontal(nlinhas, ncolunas, j,i,matriz, palavra)) {  //Comando que "chama" a função de identificação horizontal
                    printf("\nPalavra encontrada NA HORIZONTAL em (%d,%d) a (%d,%ld)\n",i+1,j+1,i+1,j+strlen(palavra));
                    pontos++;
                }
                if (diagonal(nlinhas, ncolunas, i,j,matriz, palavra)) {    //Comando que "chama" a função de identificação diagonal
                    printf("\nPalavra encontrada NA DIAGONAL em (%d,%d) a (%ld,%ld)\n",i+1,j+1,i+strlen(palavra),j+strlen(palavra));
                    pontos++;
                }
            }
        }
    }
    if(pontos){
        return pontos;
    }
    else{   //Comando que printa a ausência da palavra na matriz
        printf("\nPalavra nao encontrada!\n");
        return 0;
    }
}

int diagonal(int nlinhas, int ncolunas, int x, int y, char **matriz, char* palavra){  //Função para identificação de palavras na diagonal
    int i,j,k = 1;

    if (x > (ncolunas - strlen(palavra))) return 0;  /*Limita a leitura da diagonal até que x ou y = TAMANHO - strlen(palavra)
                                                      para evitar possíveis falsos positivos*/
    if (y > (nlinhas - strlen(palavra))) return 0;

    for (i=x+1,j=y+1;i<nlinhas && j<ncolunas;i++,j++){   //Loop para comparação dos termos da palavra inserida com os das diagonais da matriz
        if (matriz[i][j]==palavra[k]){
            k++;
        }
        else if (!palavra[k]){break;}   //Encerra a leitura da diagonal e retorna para o código de comando principal

        else {return 0;}
    }

    return 1;
}

int vertical(int nlinhas, int ncolunas, int x, int y, char **matriz, char* palavra) {  //Função para identificação de palavras na vertical
    int i, k = 1;

    if (y > (nlinhas - strlen(palavra))) return 0;  /*Limita a leitura da vertical até que y = TAMANHO - strlen(palavra)
                                                      para evitar possíveis falsos positivos*/

    for (i = y+1; i < nlinhas; i++) {   //Loop para comparação dos termos da palavra inserida com as verticais da matriz
        if (matriz[i][x] == palavra[k]) {
            k++;
        }
        else if (!palavra[k]) break;   //Encerra a leitura da vertical e retorna para o código de comando principal
        else return 0;
    }
    return 1;
}

int horizontal(int nlinhas, int ncolunas, int x, int y, char **matriz, char* palavra) {    //Função para identificação de palavras na horizontal
    int j, k = 1;

    if (x > (ncolunas - strlen(palavra))) return 0;  /*Limita a leitura da horizontal até que x = TAMANHO - strlen(palavra)
                                                      para evitar possíveis falsos positivos*/

    for (j = x+1; j < ncolunas; j++) {   //Loop para comparação dos termos da palavra inserida com as horizontais da matriz
        if (matriz[y][j] == palavra[k]) {
            k++;
        }
        else if (!palavra[k]) break;    //Encerra a leitura da horizontal e retorna para o código de comando principal
        else return 0;
    }
    return 1;
}
