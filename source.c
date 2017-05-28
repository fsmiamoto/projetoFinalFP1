#include "source.h"
#include <conio.h>
#include <windows.h>

void mostraCursor(bool flag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = flag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

int mostraMenu(void)
{
    char escolha;
    bool valido = false;
    system("cls");
    printf("Simulador de Elevadores - V 1.0\n\n");
    printf("1) Instru��es\n2) Defini��o de par�metros\n3) Iniciar simula��o\n4) Estatist�cas de simula��o\n5) Sair\n");
    while(!valido)
    {
        fflush(stdin);
        char c = getch();
        switch(c)
        {
        case '1':
            valido = true;
            escolha = 1;
            break;
        case '2':
            valido = true;
            escolha = 2;
            break;
        case '3':
            valido = true;
            escolha = 3;
            break;
        case '4':
            valido = true;
            escolha = 4;
            break;
        case '5':
            valido = true;
            escolha = 5;
            break;
        default:
            valido = false;
        }
    }
    return escolha;
}

void instrucoes()
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("Aqui aparecer�o as instru��es!\n\nPressione v para voltar\n");
        fflush(stdin);
        c = getch();
    }
}

void defineParametros()
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("Aqui ser�o definidos os par�metros\n\nPressione v para voltar\n");
        fflush(stdin);
        c = getch();
    }
}

void estatisticas()
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("Aqui ser�o apresentadas as estat�sticas do simulador\n\nPressione v para voltar\n");
        fflush(stdin);
        c = getch();
    }
}

void simula()
{
    animaSDL();
}
// C�digo relacionado a biblioteca SDL

SDL_Window * janela;
SDL_Surface * imagem;
SDL_Renderer * rend;
SDL_Texture * tex;


void animaSDL()
{
    bool querSair = false;
    if(!inicializaSDL("Simulador") || !carregaImagem("images\\Kappa.bmp"))
    {
        fechaSDL();
        system("pause");
    }
    else
    {
        tex = SDL_CreateTextureFromSurface(rend,imagem);
        SDL_RenderClear(rend);
        SDL_RenderCopy(rend,tex,NULL,NULL);
        SDL_RenderPresent(rend);
        while(!querSair)
        {
            SDL_Event evento;
            while(SDL_PollEvent(&evento))
                if(evento.type == SDL_QUIT)
                    querSair = true;
        }
        fechaSDL();
    }
}

// Cria a janela e o renderizador
bool inicializaSDL(const char * titulo)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("N�o pode inicializar %s", SDL_GetError());
        printf( "SDL n�o pode ser inicializado! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    janela = SDL_CreateWindow(titulo, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TELA_LARGURA, TELA_ALTURA, 0);
    if(!janela)
    {
        printf("Janela n�o pode ser criada! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    rend = SDL_CreateRenderer(janela,-1,FLAGS_REND);
    if(!rend)
    {
        printf("N�o foi poss�vel criar o renderizador! SDL_Error: %s\n",SDL_GetError());
        return false;
    }

    return true;
}

// Carrega imagem BMP na Surface imagem
bool carregaImagem(const char * nome)
{
    imagem = SDL_LoadBMP(nome);
    if(!imagem)
    {
        printf("N�o foi poss�vel carregar a imagem %s! SDL_Error: %s\n", nome, SDL_GetError() );
        return false;
    }
    return true;
}

void fechaSDL()
{
    SDL_FreeSurface(imagem);
    SDL_DestroyWindow(janela);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_Quit();
}
