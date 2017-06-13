#include "source.h"
#include "sdl_funcoes.h"
#include <conio.h>
#include <windows.h>
#include <time.h>


// Variáveis globais
int tempo;
int idChamada;
int numAndares,numElevadores,capElevador;
Elevador * elevadores;
Andar * andares;
Chamada call;
Vetor_Chamada chamadasConcluidas;
FILE * arqChamadas;
FILE * arqLog;

/* Funções do Menu */

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
// Função de apresentação das instruções
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
// Função de definição dos parâmetros de simulação
void defineParametros(int * elevadores, int * andares, int * cap)
{
    char c = 0;
    while(c != 'v')
    {
        system("cls");
        printf("1) Número de andares: %d\n2) Número de elevadores: %d\n3) Capacidade máxima por elevador: %d\n",*andares,*elevadores,*cap);
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
            * andares = buf;
            mostraCursor(false);
            break;
        case '2':
            system("cls");
            mostraCursor(true);
            printf("Número de elevadores: ");
            scanf("%d",&buf);
            * elevadores = buf;
            mostraCursor(false);
            break;
        case '3':
            system("cls");
            mostraCursor(true);
            printf("Capacidade máxima por elevador: ");
            scanf("%d",&buf);
            * cap = buf;
            mostraCursor(false);
            break;
        default:
            break;
        }
    }
}
// Função de apresentação das estatísticas geradas pelo simulador
void mostraEstatisticas()
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
// Função que inicia a simulação
void simula(int numE, int numA,int cap)
{
    // Aloca um vetor com o número de elevadores da struct Elevadorgene
    elevadores = (Elevador *) malloc(numElevadores * sizeof(Elevador)); // Aloca vetor de struct Elevador
    andares = (Andar *) malloc((numAndares+1) * sizeof(Andar)); // Aloca vetor de struct Andar
    arqChamadas = fopen(nomeArqChamadas,"r"); // Arquivo de leitura de chamadas
    fseek(arqChamadas,47,SEEK_SET); // Ignora primeira linha do arquivo CSV
    arqLog = fopen(nomeArqLog,"w");
    tempo = 0;
    idChamada = 0;
    // Globaliza os parâmetros
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
                printf("T: %d | Elevador #0  - Andar Atual: %d\n",tempo,elevadores[0].andarAtual);
                defineSentido();
                moveElevadores();
                tempo++;
            }
            fprintf(arqLog,"T: %d | Elevador chamado no andar %d\n",call.tempoInicial,call.andarOrigem);
            printf("T: %d | Elevador chamado no andar %d\n",call.tempoInicial,call.andarOrigem);
            defineElevador();
            posicionaChamada();
        }
        else
        {
            printf("T: %d | Elevador #0  - Andar Atual: %d\n",tempo,elevadores[0].andarAtual);
            defineSentido();
            moveElevadores();
            tempo++;
        }
    }
    geraEstatisticas();
    putchar('\n');
    system("pause");
    //animaSDL();
    // Libera memória utilizada e fechar arquivos utilizados
    fechaSimulacao();
}
/*------------------------------------------------------------------------------------------------*/

/* Funções do Simulador */
void setupElevadores()
{
    int i;
    for(i = 0; i < numElevadores; i++)
    {
        elevadores[i].andarAtual = 0; // Começam no térreo
        elevadores[i].andarSelect = (bool *) calloc(numAndares + 1,sizeof(bool)); // Vetor de booleanos que representa as teclas selecionadas dentro do elevador
        elevadores[i].estaMovendo = false;
        elevadores[i].totalP = 0; // Inicializa o total de passageiros de cada elevador
        elevadores[i].numP = 0;  // Número atual de passageiros
        elevadores[i].sentido = SUBINDO;
        elevadores[i].entreAndares = 0;
        iniciaVetor(&elevadores[i].vCall,capElevador);
    }
}

void setupAndares()
{
    int i;
    for(i = 0; i < (numAndares+1); i++)
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
    for(i = 0; i <= numAndares; i++)
        limpaVetor(&andares[i].vCall);
    free(elevadores);
    free(andares);
    andares = NULL;
    elevadores = NULL;
    limpaVetor(&chamadasConcluidas);
    fclose(arqChamadas);
    fclose(arqLog);
}

bool pegaChamadas(int origem)
{
    bool status = true;
    int andarOrigem,andarDestino,tempoCall;
    switch(origem)
    {
    //Random
    case 1:
        srand(time(NULL));
        if(tempo < TEMPO_MAX - 1)
        {
            do
            {
                tempoCall = rand() % TEMPO_MAX;
            }
            while(tempoCall <= tempo);
            andarOrigem = rand() % numAndares;
            do
            {
                andarDestino = rand() % numAndares;
            }
            while(andarDestino == andarOrigem);
            printf("Tempo Random: %d\n",tempoCall);
            status = true;
        }
        else
            status = false;
        break;
    //Arquivo
    case 2:
        if(fscanf(arqChamadas,"%d,%d,%d",&tempoCall,&andarOrigem,&andarDestino) != EOF)
            status = true;
        else
            status = false;
        break;
    // Teclado
    case 3:
        printf("Insira o tempo da chamada: ");
        scanf("%d",&tempoCall);
        printf("Insira o andar de origem: ");
        scanf("%d",&andarOrigem);
        printf("Insira o tempo de destino: ");
        scanf("%d",&andarDestino);
        system("cls");
        status = true;
        break;
    }
    call.andarOrigem = andarOrigem;
    call.andarDestino = andarDestino;
    call.tempoInicial = tempoCall;
    call.ID = idChamada++;
    return status;
}

int moveElevadores()
{
    int i;
    for(i = 0; i < numElevadores; i++)
    {
        if(elevadores[i].estaMovendo == true)
        {
            if(++elevadores[i].entreAndares == TEMPO_POR_ANDAR)
            {
                if(elevadores[i].sentido == SUBINDO)
                    elevadores[i].andarAtual++;
                else
                    elevadores[i].andarAtual--;
                elevadores[i].entreAndares = 0;
            }
        }
        if(elevadores[i].andarSelect[elevadores[i].andarAtual] == true)
        {
            elevadores[i].estaMovendo = false;
            elevadores[i].andarSelect[elevadores[i].andarAtual] = false;
            entraSai(i);
        }
    }
    return 0;
}

void posicionaChamada()
{
    insereChamada(&andares[call.andarOrigem].vCall, call);
    andares[call.andarOrigem].temChamada = true;
}

void defineSentido()
{
    int i;
    for(i = 0; i < numElevadores; i++)
        if(elevadores[i].estaMovendo == false)
        {
            int j;
            for(j = 0; j < (numAndares + 1); j++)
            {
                if(elevadores[i].andarSelect[j] && j > elevadores[i].andarAtual)
                {
                    elevadores[i].sentido = SUBINDO;
                    elevadores[i].estaMovendo = true;
                }

                else if(elevadores[i].andarSelect[j] && j < elevadores[i].andarAtual)
                {
                    elevadores[i].sentido = DESCENDO;
                    elevadores[i].estaMovendo = true;
                }
            }

        }
}

void defineElevador()
{
    elevadores[0].andarSelect[call.andarOrigem] = true;
    call.elevadorDesignado = 0;
}

void entraSai(int ID)
{
    int i;
    printf("T: %d | Elevador #%d - Porta abrindo no andar %d...\n",tempo,ID,elevadores[ID].andarAtual);
    tempo += TEMPO_ABERTURA;
    //Passsageiros descem primeiro:
    for(i = 0; i < elevadores[ID].vCall.qntd; i++)

        //Checa se o andar atual é o andar de destino do passageiro no interior do elevador
        if(elevadores[ID].vCall.chamadas[i].andarDestino == elevadores[ID].andarAtual)
        {
            printf("T: %d | Elevador #%d - Passageiro %d desceu no andar %d\n",tempo, ID, elevadores[ID].vCall.chamadas[i].ID, elevadores[ID].andarAtual);
            // Insere o tempo de saída do elevador
            elevadores[ID].vCall.chamadas[i].tempoSaida = tempo;
            // Remove chamada do elevador e coloca no vetor de chamadas concluídas
            insereChamada(&chamadasConcluidas,elevadores[ID].vCall.chamadas[i]);
            removeChamada(&elevadores[ID].vCall,i);
            //Atualiza o número atual de passageiros
            elevadores[ID].numP--;
        }
    // Entrada de passageiros se o andar os tiver

    if(andares[elevadores[ID].andarAtual].temChamada == true)
    {
        for(i = 0; i < andares[elevadores[ID].andarAtual].vCall.qntd; i++)
            // Checa se o elevador é o designado para atender a chamada e se o mesmo comporta o passageiro.
            if((andares[elevadores[ID].andarAtual].vCall.chamadas[i].elevadorDesignado == ID) && ( elevadores[ID].numP+1) <= capElevador)
            {
                printf("T: %d | Elevador #%d - Passageiro %d entrou com destino ao andar %d\n", tempo, ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                // Insere o tempo de entrada no elevador
                andares[elevadores[ID].andarAtual].vCall.chamadas[i].tempoEntrada = tempo;
                // Incrementa o total de passageiros transportados e o número atual de passageiros.
                elevadores[ID].numP++;
                elevadores[ID].totalP++;
                // Adiciona andar de destino do passageiro que entrou
                elevadores[ID].andarSelect[andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino] = true;
                // Insere chamada no elevador e a remove do andar.
                insereChamada(&elevadores[ID].vCall,andares[elevadores[ID].andarAtual].vCall.chamadas[i]);
                removeChamada(&andares[elevadores[ID].andarAtual].vCall,i);
                if(andares[elevadores[ID].andarAtual].vCall.qntd == 0)
                    andares[elevadores[ID].andarAtual].temChamada = false;
            }
    }
    printf("T: %d | Elevador #%d - Porta fechando no andar %d...\n",tempo,ID,elevadores[ID].andarAtual);
    tempo += TEMPO_FECHAMENTO;
}

void geraEstatisticas()
{
    int i;
    float tEsperaMed = 0, tEsperaMax = 0, tViagemMed = 0, tViagemMax = 0, numPassageirosMed = 0;
    for(i = 0; i < chamadasConcluidas.qntd; i++)
    {
        // Cálculos do tempo de espera
        float tEspera = chamadasConcluidas.chamadas[i].tempoEntrada - chamadasConcluidas.chamadas[i].tempoInicial;
        tEsperaMed += tEspera;
        if(tEspera > tEsperaMax)
            tEsperaMax = tEspera;
        // Cálculos do tempo de viagem
        float tViagem = chamadasConcluidas.chamadas[i].tempoSaida - chamadasConcluidas.chamadas[i].tempoEntrada;
        tViagemMed += tViagem;
        if(tViagem > tViagemMax)
            tViagemMax = tViagem;
    }
    tEsperaMed /= chamadasConcluidas.qntd;
    tViagemMed /= chamadasConcluidas.qntd;
    for(i = 0; i < numElevadores; i++)
        numPassageirosMed += elevadores[i].totalP;
    numPassageirosMed /= tempo;
    printf("\nEstatísticas de simulação: \nTempo médio de espera: %.1f\nTempo máximo de espera: %.1f",tEsperaMed,tEsperaMax);
    printf("\nTempo médio de viagem: %.1f\nTempo máximo de viagem: %.1f",tViagemMed,tViagemMax);
    printf("\nNúmero médio de passageiros por segundo: %.5f\n\n",numPassageirosMed);
}

/*------------------------------------------------------------------------------------------------*/

/* Funções do Vetor_Chamada */
void iniciaVetor(Vetor_Chamada * c, size_t tamInicial)
{
    c -> chamadas = (Chamada *) calloc(tamInicial,sizeof(Chamada));
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
    for(i = index; i < (c->qntd - 1); i++)
    {
        c->chamadas[i] = c->chamadas[i+1];
    }
    c->qntd--;
}

/*------------------------------------------------------------------------------------------------*/

/* Funções para o console*/

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
