#include "sdl_funcoes.h"

/* Funcões da biblioteca SDL */

SDL_Window * janela;
SDL_Surface * imagem;
SDL_Renderer * rend;
SDL_Texture * tex;
SDL_Rect dest;

// Gera animação na janela

void animaSDL()
{
    bool querSair = false;
    if(!inicializaSDL("Simulador") || !carregaImagem("images\\elevador.bmp"))
    {
        fechaSDL();
        system("pause");
    }
    else
    {
        tex = SDL_CreateTextureFromSurface(rend,imagem);
        SDL_QueryTexture(tex,NULL,NULL, &dest.w, &dest.h);
        dest.w /= 4;
        dest.h /= 4;
        dest.x = (TELA_LARGURA - dest.w)/ 2;
        float pos_y = TELA_ALTURA;
        while(!querSair)
        {
            SDL_Event evento;
            while(SDL_PollEvent(&evento))
                if(evento.type == SDL_QUIT)
                    querSair = true;
            SDL_RenderClear(rend);
            dest.y = (int) pos_y;
            SDL_RenderCopy(rend,tex,NULL,&dest);
            SDL_SetRenderDrawColor(rend,255,255,255,255);
            SDL_RenderPresent(rend);
            pos_y -= (float) VEL_SCROLL / 60;
            if(pos_y <= -dest.h)
                pos_y = TELA_ALTURA;
            SDL_Delay(1000/60);
        }
    }
    fechaSDL();
}


// Cria a janela e o renderizador

bool inicializaSDL(const char * titulo)
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Não pode inicializar %s", SDL_GetError());
        printf( "SDL não pode ser inicializado! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    janela = SDL_CreateWindow(titulo, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TELA_LARGURA, TELA_ALTURA, 0);
    if(!janela)
    {
        printf("Janela não pode ser criada! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    rend = SDL_CreateRenderer(janela,-1,FLAGS_REND);
    if(!rend)
    {
        printf("Não foi possível criar o renderizador! SDL_Error: %s\n",SDL_GetError());
        return false;
    }
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf("Não foi possível inicializar o SDL_image! SDL_Error: %s",SDL_GetError());
        return false;
    }
    return true;
}

// Carrega imagem na Surface imagem

bool carregaImagem(const char * nome)
{
    imagem = IMG_Load(nome);
    if(!imagem)
    {
        printf("Não foi possível carregar a imagem %s! SDL_Error: %s\n", nome, SDL_GetError() );
        return false;
    }
    return true;
}

// Encerra a biblioteca SDL

void fechaSDL()
{
    SDL_FreeSurface(imagem);
    SDL_DestroyWindow(janela);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(rend);
    SDL_Quit();
}
/*------------------------------------------------------------------------------------------------*/
