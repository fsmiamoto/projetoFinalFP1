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
    printf("SimEle - V 1.0\n\n");
    printf("1) Instruções\n2) Definição de parâmetros\n3) Iniciar simulação\n4) Estatistícas de simulação\n5) Sair\n");
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
        printf("Bem-vindo ao Simulador para Controle de Elevadores!\n\nPressione v para voltar\n");
        fflush(stdin);
        c = getch();
    }
}

void defineParametros(Predio * p)
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("1) Número de andares: %d\n2) Número de elevadores: %d\n3) Capacidade máxima por elevador: %d\n",(*p).numAndares,(*p).numElevadores,(*p).capElevador);
        printf("\nEscolha a opção para editar ou pressione v para voltar");
        fflush(stdin);
        c = getch();
        switch(c)
        {
            int buf;
            case '1':
                system("cls");
                mostraCursor(true);
                printf("Número de andares: ");
                scanf("%d",&buf);
                p -> numAndares = buf;
                mostraCursor(false);
                break;
            case '2':
                system("cls");
                mostraCursor(true);
                printf("Número de elevadores: ");
                scanf("%d",&buf);
                p -> numElevadores = buf;
                mostraCursor(false);
                break;
            case '3':
                system("cls");
                mostraCursor(true);
                printf("Capacidade máxima por elevador: ");
                scanf("%d",&buf);
                p -> capElevador = buf;
                mostraCursor(false);
                break;
            default:
                break;
        }
    }
}

void estatisticas()
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("Aqui serão apresentadas as estatísticas do simulador\n\nPressione v para voltar\n");
        fflush(stdin);
        c = getch();
    }
}

void simula(Predio * p)
{
    p->elevadores = (Elevador *) malloc((*p).numElevadores * sizeof(Elevador));
    animaSDL();
    free((*p).elevadores);
}
// Código relacionado a biblioteca SDL

SDL_Window * janela;
SDL_Surface * imagem;
SDL_Renderer * rend;
SDL_Texture * tex;
SDL_Rect dest;


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
        SDL_QueryTexture(tex,NULL,NULL, &dest.w, &dest.h);
        dest.w /= 2;
        dest.h /= 2;
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

    return true;
}

// Carrega imagem BMP na Surface imagem
bool carregaImagem(const char * nome)
{
    imagem = SDL_LoadBMP(nome);
    if(!imagem)
    {
        printf("Não foi possível carregar a imagem %s! SDL_Error: %s\n", nome, SDL_GetError() );
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
