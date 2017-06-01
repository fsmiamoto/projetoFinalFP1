#ifndef SOURCE_H
#define SOURCE_H

#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL_image.h>

// Constantes utilizadas no simulador

static const bool DESCENDO = false;
static const bool SUBINDO = true;
static const int TELA_LARGURA = 1024;
static const int TELA_ALTURA = 600;
static const int TEMPO_ABERTURA = 1;
static const int TEMPO_FECHAMENTO = 1;
static const int TEMPO_POR_ANDAR = 5;
static const int TEMPO_MAX = 600;
static const int VEL_SCROLL = 300;
static const int NUM_ANDARES_STD = 5;
static const int NUM_ELEVADORES_STD = 2;
static const int CAP_ELEVADOR_STD = 10;
static const int TECLADO = 1;
static const int ALEATORIO = 2;
static const int ARQUIVO = 3;
static const Uint32 FLAGS_REND = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

// Structs

typedef struct
{
    bool temPassageiros;
    bool sentido;         // Sentido: Subindo ou Descendo
    int qntdPassageiros;  // Quantidade atual de passageiros
    int totalPassageiros; // Total de passageiros transportados
    int andarDestino;
    int andarAtual;
    bool * andarSel; // Vetor que indicará quais andares foram selecionados.
} Elevador;

typedef struct
{
    bool dentroElevador;
    int andarDestino;
} Passageiro;

typedef struct
{
    int numElevadores;
    int numAndares;
    int capElevador;
    Elevador * elevadores;
} Predio;

typedef struct
{
    int tempo;
    int andar;
    int qntdPassageiros;
} Chamada;

// Funções de implementação do simulador

int mostraMenu();

void instrucoes();

void defineParametros(Predio * p);

void estatisticas();

void simula(Predio * p);

int moveElevador(Elevador * e);

bool geraEstatisticas();

Chamada geraChamadas(int origem, FILE * arq);

bool setaParametros();

bool registraEvento();

void mostraCursor(bool flag);

// Funcões de utilização da biblioteca SDL2

void animaSDL();

void fechaSDL();

bool inicializaSDL(const char * titulo);

bool carregaImagem(const char * nome);

#endif
