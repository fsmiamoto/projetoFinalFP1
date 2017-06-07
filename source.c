#include "source.h"
#include "sdl_funcoes.h"
#include <conio.h>
#include <windows.h>

/* Fun��es do simulador */

int mostraMenu(void)
{
    char escolha;
    bool valido = false;
    system("cls");
    printf("SimEle - V 1.0\n\n");
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
// Fun��o de apresenta��o das instru��es
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
// Fun��o de defini��o dos par�metros de simula��o
void defineParametros(int * elevadores, int * andares, int * cap)
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("1) N�mero de andares: %d\n2) N�mero de elevadores: %d\n3) Capacidade m�xima por elevador: %d\n",*andares,*elevadores,*cap);
        printf("\nEscolha a op��o para editar ou pressione v para voltar");
        fflush(stdin);
        c = getch();
        switch(c)
        {
            int buf;
        case '1':
            system("cls");
            mostraCursor(true);
            printf("N�mero de andares: ");
            scanf("%d",&buf);
            * andares = buf;
            mostraCursor(false);
            break;
        case '2':
            system("cls");
            mostraCursor(true);
            printf("N�mero de elevadores: ");
            scanf("%d",&buf);
            * elevadores = buf;
            mostraCursor(false);
            break;
        case '3':
            system("cls");
            mostraCursor(true);
            printf("Capacidade m�xima por elevador: ");
            scanf("%d",&buf);
            * cap = buf;
            mostraCursor(false);
            break;
        default:
            break;
        }
    }
}
// Fun��o de apresenta��o das estat�sticas geradas pelo simulador
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
// Fun��o que inicia a simula��o
void simula(int numElevadores, int numAndares,int capElevador)
{
    int i;
    int tempoDecorrido = 0;
    // Aloca um vetor com o n�mero de elevadores da struct Elevador
    Elevador * elevadores = (Elevador *) malloc(numElevadores * sizeof(Elevador));
    Chamada call;
    FILE * arq = fopen(arqChamadas,"r"); // Arquivo de leitura de chamadas
    // Setup dos elevadores
    for(i = 0; i < numElevadores; i++)
    {
        elevadores[i].andarAtual = 0; // Come�am no t�rreo
        elevadores[i].totalPassageiros = 0; // Inicializa o total de passageiros de cada elevador
        elevadores[i].numPassageiros = 0;
        elevadores[i].sentido = SUBINDO;
        elevadores[i].chamadas = (Chamada *) calloc(capElevador,sizeof(Chamada));
        elevadores[i].capMax = capElevador;
        elevadores[i].entreAndares = 0;
    }
    // Inicio do loop de chamadas

    while (tempoDecorrido <= TEMPO_MAX && pegaChamadas(&call,ORIGEM_CHAMADAS,arq))
    {
        delegaElevador(call, elevadores, numElevadores);
        while(tempoDecorrido < call.tempoInicial)
        {
            printf("\nTempo decorrido: %d Andar Atual: %d\n",tempoDecorrido++,elevadores[0].andarAtual);
            moveElevadores(elevadores, numElevadores);
        }

        //printf("Tempo decorrido: %d\n",tempoDecorrido);
    }

    // Fim do loop
    putchar('\n');
    system("pause");
    //animaSDL();

    // Libera mem�ria utilizada e fechar arquivos utilizados
    for(i = 0; i < numElevadores; i++)
    {
        free(elevadores[i].chamadas);
        free(elevadores);
        fclose(arq);
    }
}

bool pegaChamadas(Chamada * c, int origem, FILE * arq)
{
    bool status = true;
    int andarOrigem,andarDestino,tempoCall;
    switch(origem)
    {
    //Arquivo
    case 2:
        if(fscanf(arq,"%d|%d|%d",&tempoCall,&andarOrigem,&andarDestino) != EOF)
        {
            c->andarOrigem = andarOrigem;
            c->andarDestino = andarDestino;
            c->tempoInicial = tempoCall;
            status = true;
        }
        else
            status = false;
        break;
    }
    return status;
}

void delegaElevador(Chamada c, Elevador * elevadores, int numElevadores)
{
    elevadores[0].andarDestino = 10;
    elevadores[0].numPassageiros = 1;
    elevadores[0].totalPassageiros++;
}

int moveElevadores(Elevador * e, int numElevadores)
{
    int i;
    for(i = 0; i < numElevadores; i++)
    {
        if(e[i].numPassageiros > 0)
        {
            if(++e[i].entreAndares == 5)
            {
                if(e[i].sentido == SUBINDO)
                    e[i].andarAtual++;
                else
                    e[i].andarAtual--;
                e[i].entreAndares = 0;
            }
            if(e[i].andarAtual == e[i].andarDestino)
                printf("\nChegou!, D: %d\n",e[i].andarDestino);
        }
    }
    return 0;
}

/* Fun��es para o console*/

// Desabilita ou mostra o cursor no console
void mostraCursor(bool flag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = flag;
    SetConsoleCursorInfo(out, &cursorInfo);
}
/*------------------------------------------------------------------------------------------------*/
