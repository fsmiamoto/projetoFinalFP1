#ifndef SOURCE_H
#define SOURCE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <locale.h>

// Flags do programa

static const int ORIGEM_CHAMADAS = 2; // 1: Aleat�rio 2: Arquivo 3: Teclado

// Constantes utilizadas no simulador

static const bool DESCENDO = false;   // Constantes booleanas arbitr�rias
static const bool SUBINDO = true;
static const int TEMPO_ABERTURA = 1; // Tempo de abertura da porta
static const int TEMPO_FECHAMENTO = 1; // Tempo de fechamento da porta
static const int TEMPO_POR_ANDAR = 5; // Tempo necess�rio para subir/descer um andar
static const int TEMPO_MAX = 300;   // Tempo m�ximo de simula��o
static const int ANDARES_MAX = 200; // N�mero m�ximo de andares
static const int CAP_MAX = 20;      // Maior capacidade poss�vel
static const int ELEVADORES_MAX = 5; // N�mero m�ximo de elevadores
static const int NUM_ANDARES_STD = 12; // N�mero padr�o de andares
static const int NUM_ELEVADORES_STD = 2; // N�mero padr�o de elevadores
static const int CAP_ELEVADOR_STD = 12; // Capacidade m�xima padr�o dos elevadores

// Structs

typedef struct
{
    int ID;           // Identifica��o do passageiro
    int tempoInicial; // Instante em que a chamada foi realizada
    int tempoEntrada;   // Instante em que o passageiro entrou no elevador
    int tempoSaida; // Instante em que o passageiro saiu do elevador
    int andarOrigem;
    int andarDestino;
    bool sentido;
} Chamada;

typedef struct
{
    Chamada * chamadas; // Vetor de chamadas que � alocado dinamicamente pela fun��o iniciaVetor()
    size_t qntd; // Representa a quantidade de elementos atual do vetor;
    size_t tam; // Representa o tamanho total do vetor, que pode ser modificado em tempo de execu��o
} Vetor_Chamada;

typedef struct
{
    Vetor_Chamada vCall;  // Vetor din�mico de chamadas
    bool sentido;         // Sentido: Subindo ou Descendo
    bool estaAtendendo;   // Define se o elevador est� atendendo alguma chamada
    bool * andar_sel_int; // Andares selecionados internos
    bool * andar_sel_ext; // Andares selecionados externamente
    int numP;             // Quantidade atual de passageiros
    int totalP;           // Total de passageiros transportados
    int andarAtual;       // Andar atual
    int entreAndares;
} Elevador;

typedef struct
{
    Vetor_Chamada vCall;
    bool temChamada;
} Andar;

// Fun��es de implementa��o do simulador

int mostraMenu();

void instrucoes();

void simula(int numE, int numA, int cap);

void mostraEstatisticas();

void defineParametros(int * elevadores, int * andares, int * cap);

void defineSentido();

void defineElevador();

void geraEstatisticas();

void entraSai(int ID);

int moveElevadores();

bool pegaChamadas();

void posicionaChamada();

void setupAndares();

void setupElevadores();

void abreArquivos();

void fechaSimulacao();

void alocaEA();

void animacao();

void printaEspacos(int qntd);

void pausa();

// Fun��es para a struct Vetor_Chamada

void iniciaVetor(Vetor_Chamada * c, size_t tamInicial);

void insereChamada(Vetor_Chamada * c, Chamada call);

void removeChamada(Vetor_Chamada * c, int index);

void limpaVetor(Vetor_Chamada * c);

// Fun��es auxiliares

void ordena(int * vec, int * index, int tam);

// Fun��o do console

void mostraCursor(bool flag);

#endif
