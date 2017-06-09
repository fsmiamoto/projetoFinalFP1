#include "source.h"
#include "sdl_funcoes.h"
#include <conio.h>
#include <windows.h>

// Vari�veis globais
int tempo;
int numAndares,numElevadores,capElevador;
Elevador * elevadores;
Andar * andares;
Chamada call;
FILE * arq ;


/* Fun��es do Menu */

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
void mostraEstatisticas()
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
void simula(int numE, int numA,int cap)
{
    // Aloca um vetor com o n�mero de elevadores da struct Elevador
    elevadores = (Elevador *) malloc(numElevadores * sizeof(Elevador)); // Aloca vetor de struct Elevador
    andares = (Andar *) malloc(numAndares * sizeof(Andar)); // Aloca vetor de struct Andar
    arq = fopen(arqChamadas,"r"); // Arquivo de leitura de chamadas
    tempo = 0;
    // Globaliza os par�metros
    numAndares = numA;
    numElevadores = numE;
    capElevador = cap;
    system("cls");
    // Setup inicial dos elevadores e andares
    setupElevadores();
    setupAndares();
    while(tempo <= TEMPO_MAX)
    {
        if(pegaChamadas(ORIGEM_CHAMADAS))
        {
            while(tempo < call.tempoInicial)
            {
                moveElevadores();
                tempo++;
            }
            posicionaChamada();
        }
        tempo++;
    }

    putchar('\n');
    system("pause");
    //animaSDL();

    // Libera mem�ria utilizada e fechar arquivos utilizados
    fechaSimulacao();
}
/*------------------------------------------------------------------------------------------------*/

/* Fun��es do Simulador */
void setupElevadores()
{
    int i;
    for(i = 0; i < numElevadores; i++)
    {
        elevadores[i].andarAtual = 0; // Come�am no t�rreo
        elevadores[i].andarSelect = (bool *) calloc(numAndares,sizeof(bool)); // Vetor de booleanos que representa as teclas selecionadas dentro do elevador
        elevadores[i].totalP = 0; // Inicializa o total de passageiros de cada elevador
        elevadores[i].numP = 0;  // N�mero atual de passageiros
        elevadores[i].sentido = SUBINDO;
        elevadores[i].entreAndares = 0;
    }
}

void setupAndares()
{
    int i;
    for(i = 0; i < numAndares; i++)
    {
        iniciaVetor(&andares[i].vCall,5);
        andares[i].temChamada = false;
    }
}

void fechaSimulacao()
{
    int i;
    for(i = 0; i < numElevadores; i++)
        limpaVetor(&elevadores[i].vCall);
    free(elevadores);
    elevadores = NULL;
    fclose(arq);
}

bool pegaChamadas(int origem)
{
    bool status = true;
    int andarOrigem,andarDestino,tempoCall;
    switch(origem)
    {
    //Arquivo
    case 2:
        if(fscanf(arq,"%d|%d|%d",&tempoCall,&andarOrigem,&andarDestino) != EOF)
        {
            call.andarOrigem = andarOrigem;
            call.andarDestino = andarDestino;
            call.tempoInicial = tempoCall;
            status = true;
        }
        else
            status = false;
        break;
    }
    return status;
}

int moveElevadores()
{
    int i;
    for(i = 0; i < numElevadores; i++)
    {
        if(elevadores[i].numP > 0)
        {
            if(++elevadores[i].entreAndares == 5)
            {
                if(elevadores[i].sentido == SUBINDO)
                    elevadores[i].andarAtual++;
                else
                    elevadores[i].andarAtual--;
                elevadores[i].entreAndares = 0;
            }
            if(elevadores[i].andarAtual == elevadores[i].andarDestino)
                printf("Chegou!\n");
        }
    }
    return 0;
}

void posicionaChamada()
{
    insereChamada(&andares[call.andarOrigem].vCall, call);
    andares[call.andarOrigem].temChamada = true;
}
/*------------------------------------------------------------------------------------------------*/

/* Fun��es do Vetor_Chamada */
void iniciaVetor(Vetor_Chamada * c, size_t tamInicial)
{
    c -> chamadas = (Chamada *) malloc(sizeof(Chamada) * tamInicial);
    c -> qntd = 0;
    c -> tam = tamInicial;
}

void insereChamada(Vetor_Chamada * c, Chamada call)
{
    if(c -> qntd == c -> tam)
    {
        c -> tam *= 2;
        c -> chamadas = (Chamada *) realloc(c->chamadas,c->tam*sizeof(Chamada));
    }
    c->chamadas[c->qntd++] = call;
}

void limpaVetor(Vetor_Chamada * c)
{
    free(c->chamadas);
    c->chamadas = NULL;
    c->qntd = c->tam = 0;
}

void removeChamada(Vetor_Chamada * c, int index)
{
    int i;
    for(i = index; i < c->qntd - 2; i++)
        c->chamadas[i+1] = c-> chamadas[i];
    c->qntd--;
}

/*------------------------------------------------------------------------------------------------*/

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
