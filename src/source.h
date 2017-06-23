#ifndef SOURCE_H
#define SOURCE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include <math.h>

// Structs

typedef struct
{
    int ID;             // Identifica��o do passageiro
    int tempoInicial;   // Instante em que a chamada foi realizada
    int tempoEntrada;   // Instante em que o passageiro entrou no elevador
    int tempoSaida;     // Instante em que o passageiro saiu do elevador
    int andarOrigem;
    int andarDestino;
    int elevadorDesignado;
    bool sentido;
} Chamada;

typedef struct
{
    Chamada * chamadas; // Vetor de chamadas que � alocado dinamicamente pela fun��o iniciaVetor()
    size_t qntd;        // Representa a quantidade de elementos atual do vetor;
    size_t tam;         // Representa o tamanho total do vetor, que pode ser modificado em tempo de execu��o
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

// Enumera��es para definir constantes
enum sentido { DESCENDO, SUBINDO};
enum origem { ALEATORIO, ARQUIVO, TECLADO};

// Fun��es de implementa��o do simulador

int mostraMenu();

void instrucoes();

void simula();

void mostraEstatisticas();

void defineParametros();

void iniciaVariaveis();

// Fun��es relacionadas ao funcionamento da simula��o

void defineSentido();

void defineElevador(Chamada c);

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

// Fun��es para a struct Vetor_Chamada

void iniciaVetor(Vetor_Chamada * c, size_t tamInicial);

void insereChamada(Vetor_Chamada * c, Chamada call);

void removeChamada(Vetor_Chamada * c, int index);

void limpaVetor(Vetor_Chamada * c);

// Fun��es auxiliares

void ordenaDistancias(int * vec, int * index, int tam);

void ordenaVetor(int * vec, int tam);

void printaEspacos(int qntd);

double media(int * vec, int tam);

double mediana(int * vec, int tam);

double desvioPadrao(int * vec, int tam);

// Fun��o do console

void mostraCursor(bool flag);

void pausa();

#endif
