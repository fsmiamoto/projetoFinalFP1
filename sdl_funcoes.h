#ifndef SDL_FUNCOES_H
#define SDL_FUNCOES_H

#include <SDL_image.h>
#include <SDL.h>
#include <stdbool.h>

static const Uint32 FLAGS_REND = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
static const int TELA_LARGURA = 1024;
static const int TELA_ALTURA = 600;
static const int VEL_SCROLL = 300;

// Funcões de utilização da biblioteca SDL2

void animaSDL();

void fechaSDL();

bool inicializaSDL(const char * titulo);

bool carregaImagem(const char * nome);

#endif


