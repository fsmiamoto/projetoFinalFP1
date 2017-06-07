#ifndef SOURCE_H
#define SOURCE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Flags do programa

static const int ORIGEM_CHAMADAS = 2; // 1: Aleatório 2: Arquivo 3: Teclado

// Constantes utilizadas no simulador
static const bool DESCENDO = false;
static const bool SUBINDO = true;

static const int TEMPO_ABERTURA = 1;
static const int TEMPO_FECHAMENTO = 1;
static const int TEMPO_POR_ANDAR = 5;
static const int TEMPO_MAX = 600;
static const int NUM_ANDARES_STD = 5;
static const int NUM_ELEVADORES_STD = 2;
static const int CAP_ELEVADOR_STD = 10;
static const char * arqChamadas = "chamadas.txt";

// Structs

typedef struct
{
    int tempoInicial;
    int tempoFinal;
    int andarOrigem;
    int andarDestino;
} Chamada;

typedef struct
{
    bool sentido;         // Sentido: Subindo ou Descendo
    int numPassageiros;  // Quantidade atual de passageiros
    int totalPassageiros; // Total de passageiros transportados
    int andarDestino;     // Andar destino atual
    int andarAtual;       // Andar atual
    int capMax;
    int entreAndares;
    Chamada * chamadas;
} Elevador;

// Funções de implementação do simulador

int mostraMenu();

void instrucoes();

void simula(int numElevadores, int numAndares,int capMaxElevador);

void estatisticas();

void defineParametros(int * elevadores, int * andares, int * cap);

void delegaElevador(Chamada c, Elevador * e, int numElevadores);

int moveElevadores(Elevador * e, int numElevadores);

bool geraEstatisticas();

bool pegaChamadas(Chamada * c, int origem, FILE * arq);

// Função do console

void mostraCursor(bool flag);

#endif
