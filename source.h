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
static const int TEMPO_MAX = 500;
static const int NUM_ANDARES_STD = 10;
static const int NUM_ELEVADORES_STD = 1;
static const int CAP_ELEVADOR_STD = 10;

// Nome dos arquivos utilizados:
static const char * nomeArqChamadas = "chamadas.txt";
static const char * nomeArqLog = "log.txt";

// Structs

typedef struct
{
    int ID;           // Identificação do passageiro
    int tempoInicial; // Instante em que a chamada foi realizada
    int tempoSaida;   // Instante em que o passageiro entrou no elevador
    int tempoEntrada; // Instante em que o passageiro saiu do elevador
    int andarOrigem;
    int andarDestino;
    int elevadorDesignado;
} Chamada;

typedef struct
{
    Chamada * chamadas;
    size_t qntd;
    size_t tam;
} Vetor_Chamada;

typedef struct
{
    Vetor_Chamada vCall;
    bool sentido;         // Sentido: Subindo ou Descendo
    bool estaMovendo;
    bool * andarSelect;   // Andares selecionados
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

// Funções de implementação do simulador

int mostraMenu();

void instrucoes();

void simula(int numE, int numA, int cap);

void mostraEstatisticas();

void defineParametros(int * elevadores, int * andares, int * cap);

void defineSentido();

void defineElevador();

void geraEstatisticas();

void entraSai();

int moveElevadores();

bool pegaChamadas();

void posicionaChamada();

void setupAndares();

void setupElevadores();

void fechaSimulacao();

void iniciaVetor(Vetor_Chamada * c, size_t tamInicial);

void insereChamada(Vetor_Chamada * c, Chamada call);

void removeChamada(Vetor_Chamada * c, int index);

void limpaVetor(Vetor_Chamada * c);


// Função do console

void mostraCursor(bool flag);

#endif
