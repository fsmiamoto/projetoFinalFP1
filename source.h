#ifndef SOURCE_H
#define SOURCE_H

#include <stdbool.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

// Constantes utilizadas no simulador

static const bool DESCENDO = false;
static const bool SUBINDO = true;
static const int TELA_LARGURA = 1024;
static const int TELA_ALTURA = 600;
static const int TEMPO_ABERTURA = 1;
static const int TEMPO_FECHAMENTO = 1;
static const int TEMPO_POR_ANDAR = 5;
static const int VEL_SCROLL = 300;
static const Uint32 FLAGS_REND = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;

// Structs

typedef struct
{
    bool temPassageiros;
    bool sentido;
    int qntdPassageiros;
    int andarDestino;
    int andarOrigem;
} Elevador;

typedef struct
{
    bool dentroElevador;
    int numero;
    int andarDestino;
} Passageiro;

typedef struct
{
    int tempoDecorrido;
    Elevador * elevadores;
} Predio;

// Funções de implementação do simulador

int mostraMenu();

void instrucoes();

void defineParametros();

void estatisticas();

void simula();

bool moveElevador();

bool geraEstatisticas();

bool geraChamadas();

bool setaParametros();

bool registraEvento();

void mostraCursor(bool flag);

// Funcões de utilização da biblioteca SDL2

void animaSDL();

void fechaSDL();

bool inicializaSDL(const char * titulo);

bool carregaImagem(const char * nome);

#endif
