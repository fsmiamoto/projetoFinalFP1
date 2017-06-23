#include "source.h"

// Constantes utilizadas no simulador
const char * NOME_ARQ_CHAMADAS = ".\\arquivos\\calls.txt";
const char * NOME_ARQ_LOG = ".\\arquivos\\log.txt";
const char * NOME_ARQ_STAT = ".\\arquivos\\estatisticas.txt";
const int TEMPO_ABERTURA = 1;       // Tempo de abertura da porta
const int TEMPO_FECHAMENTO = 1;     // Tempo de fechamento da porta
const int TEMPO_POR_ANDAR = 5;      // Tempo necess�rio para subir/descer um andar
const int TEMPO_MAX = 100000;       // Tempo m�ximo de simula��o
const int ANDARES_MAX = 200;        // N�mero m�ximo de andares
const int CAP_MAX = 20;             // Maior capacidade poss�vel
const int ELEVADORES_MAX = 8000;    // N�mero m�ximo de elevadores
const int NUM_ANDARES_STD = 12;     // N�mero padr�o de andares
const int NUM_ELEVADORES_STD = 2;   // N�mero padr�o de elevadores
const int CAP_ELEVADOR_STD = 8;     // Capacidade m�xima padr�o dos elevadores
const int TEMPO_SIMULACAO_STD = 5000;
const int ORIGEM_STD = ALEATORIO;
const int DELTA_T_MAX = 100;

// Vari�veis do simulador
int tempoAtual, idChamada, numAndares, numElevadores, capElevador, origemChamadas, tempoSimulacao, cargaMax;
Elevador * elevadores;
Andar * andares;
Vetor_Chamada chamadasConcluidas;

/* Fun��es do Menu */

// Printa o menu no console e retorna a op��o selecionada
int mostraMenu(void)
{
    char escolha;
    bool valido = false;
    system("cls");
    printf("1) Instru��es\n2) Defini��o de par�metros\n3) Iniciar simula��o\n4) Estatist�cas da �ltima simula��o\n5) Sair\n");
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
void instrucoes(void)
{
    system("cls");
    printf("Bem-vindo ao Simulador para Controle de Elevadores!\n");
    pausa();
}

// Fun��o de defini��o dos par�metros de simula��o
void defineParametros(void)
{
    char c = 0;
    bool flag = false;
    bool refresh = true;
    while(c != 'v')
    {
        if(refresh)
        {
            system("cls");
            char nomeOrigem[16];
            switch(origemChamadas)
            {
            case ALEATORIO:
                strcpy(nomeOrigem,"Aleat�rio");
                break;
            case ARQUIVO:
                strcpy(nomeOrigem,"Arquivo");
                break;
            case TECLADO:
                strcpy(nomeOrigem,"Teclado");
                break;
            }
            printf("1) Tempo de simula��o: %d s\n2) N�mero de andares: %d",tempoSimulacao,numAndares);
            printf("\n3) N�mero de elevadores: %d\n4) Capacidade m�xima por elevador: %d\n5) Origem das chamadas: %s\n",numElevadores,capElevador,nomeOrigem);
            printf("\nEscolha a op��o para editar ou pressione v para voltar");
            refresh = false;
        }
        fflush(stdin);
        c = getch();
        switch(c)
        {
            int buf;
        case '1':
            system("cls");
            mostraCursor(true);
            flag = false;
            do
            {
                printf("Tempo de simula��o: ");
                fflush(stdin);
                scanf("%d",&buf);
                if(buf > 0 && buf <= TEMPO_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            tempoSimulacao = buf;
            refresh = true;
            mostraCursor(false);
            break;
        case '2':
            system("cls");
            mostraCursor(true);
            flag = false;
            do
            {
                printf("N�mero de andares: ");
                fflush(stdin);
                scanf("%d",&buf);
                if(buf > 0 && buf <= ANDARES_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            numAndares = buf;
            refresh = true;
            mostraCursor(false);
            break;
        case '3':
            system("cls");
            mostraCursor(true);
            flag = false;
            do
            {
                printf("N�mero de elevadores: ");
                fflush(stdin);
                scanf("%d",&buf);
                if(buf > 0 && buf <= ELEVADORES_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            numElevadores = buf;
            refresh = true;
            mostraCursor(false);
            break;
        case '4':
            system("cls");
            mostraCursor(true);
            flag = false;
            do
            {
                printf("Capacidade m�xima por elevador: ");
                fflush(stdin);
                scanf("%d",&buf);
                if(buf > 0 && buf <= CAP_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            capElevador = buf;
            refresh = true;
            mostraCursor(false);
            break;
        case '5':
            system("cls");
            mostraCursor(false);
            flag = false;
            char c;
            printf("Origem das chamadas: \n1) Aleat�rio\n2) Arquivo\n3) Teclado");
            do
            {
                fflush(stdin);
                c = getch();
                if(c > '0' && c < '4')
                    flag = true;
            }
            while(!flag);
            switch(c)
            {
            case '1':
                origemChamadas = ALEATORIO;
                break;
            case '2':
                origemChamadas = ARQUIVO;
                break;
            case '3':
                origemChamadas = TECLADO;
                break;
            }
            refresh = true;
            mostraCursor(false);
            break;
        default:
            break;
        }
    }
}

// Fun��o de apresenta��o das estat�sticas geradas pelo simulador
void mostraEstatisticas(void)
{
    FILE * arq = fopen(NOME_ARQ_STAT,"r");
    int cConcluidas, tempoSimulacao, cRealizadas, cargaMax;
    double tEsperaMed, tEsperaMax, tEsperaMediana, tEsperaDP;
    double tViagemMed,tViagemMax, tViagemMediana, tViagemDP;
    double numPassageirosMed;
    if(arq == NULL)
    {
        perror("\nArquivo de estat�sticas - O seguinte erro ocorreu: ");
        exit(1);
    }
    if(fscanf(arq,"%d|%d|%d|%lf|%lf|%lf|%lf|%lf|%lf|%lf|%lf|%lf|%d",&tempoSimulacao,&cRealizadas,&cConcluidas,&tEsperaMed,&tEsperaMax,&tEsperaMediana,&tEsperaDP,&tViagemMed,&tViagemMax,&tViagemMediana,&tViagemDP,&numPassageirosMed,&cargaMax))
    {
        system("cls");
        printf("Tempo de simula��o: %d s\nChamadas Realizadas: %d\nChamadas conclu�das: %d\n\n",tempoSimulacao,cRealizadas,cConcluidas);
        printf("Tempo m�dio de espera: %.2f s\nTempo m�ximo de espera: %.2f s\nTempo mediano de espera: %.2f s\nDesvio Padr�o de espera: %.2f s\n\n",tEsperaMed,tEsperaMax,tEsperaMediana,tEsperaDP);
        printf("Tempo m�dio de viagem: %.2f s\nTempo m�ximo de viagem: %.2f s\nTempo mediano de viagem: %.2f s\nDesvio Padr�o de viagem: %.2f s\n\n",tViagemMed,tViagemMax,tViagemMediana,tViagemDP);
        printf("N�mero m�dio de passageiros transportados por hora: %.2f\nN�mero m�ximo de passageiros pendentes: %d\n",numPassageirosMed,cargaMax);

    }
    else
    {
        system("cls");
        printf("Erro na leitura do arquivo de estat�sticas!\n");
    }

    pausa();
    fclose(arq);
}

// Fun��o que inicia a simula��o
void simula(void)
{
    // Flag de sa�da da simula��o
    bool sair = false;
    // Chamada que conter� a chamada atual
    Chamada call;
    FILE * arqChamadas;
    FILE * arqLog;
    // Abre os arquivos necess�rios
    abreArquivos(&arqChamadas, &arqLog);
    // Seta tempo e contador de identifica��o de chamadas para zero
    tempoAtual = 0;
    idChamada = 0;
    cargaMax = 0;
    // Aloca um vetor com o n�mero de elevadores da struct Elevadorgene
    alocaEA();
    // Limpa a tela
    system("cls");
    // Setup inicial dos elevadores e andares
    setupElevadores();
    setupAndares();
    iniciaVetor(&chamadasConcluidas,5);
    while(tempoAtual <= tempoSimulacao && !sair)
    {
        if(pegaChamadas(&call, &arqChamadas))
        {
            while(tempoAtual < call.tempoInicial)
            {
                moveElevadores(&arqLog);
            }
            defineElevador(call, &arqLog);
            posicionaChamada(call);
            // Diferen�a entre chamadas recebidas e chamadas terminadas indica as chamadas pendentes
            if((idChamada - chamadasConcluidas.qntd) > cargaMax)
                cargaMax = idChamada - chamadasConcluidas.qntd;
        }
        else
        {
            moveElevadores(&arqLog);
            if((idChamada - chamadasConcluidas.qntd) > cargaMax)
                cargaMax = idChamada - chamadasConcluidas.qntd;
        }
    }
    geraEstatisticas();
    pausa();
    // Libera mem�ria utilizada e fechar arquivos utilizados
    fechaSimulacao(&arqChamadas,&arqLog);
}

// Inicializa as vari�veis de simula��o
void iniciaVariaveis(void)
{
    capElevador = CAP_ELEVADOR_STD;
    numAndares= NUM_ANDARES_STD;
    numElevadores = NUM_ELEVADORES_STD;
    origemChamadas = ORIGEM_STD;
    tempoSimulacao = TEMPO_SIMULACAO_STD;
    srand(time(NULL));
}

/*------------------------------------------------------------------------------------------------*/

/* Fun��es do Simulador */

// Inicializa��o do vetor de elevadores
void setupElevadores(void)
{
    int i;
    for(i = 0; i < numElevadores; i++)
    {
        elevadores[i].andarAtual = 0; // Come�am no t�rreo
        elevadores[i].andar_sel_int = (bool *) calloc(numAndares + 1,sizeof(bool)); // Vetor de booleanos que representa as teclas selecionadas dentro do elevador
        elevadores[i].andar_sel_ext = (bool *) calloc(numAndares + 1,sizeof(bool));
        elevadores[i].estaAtendendo = false;
        elevadores[i].totalP = 0; // Inicializa o total de passageiros de cada elevador
        elevadores[i].numP = 0;  // N�mero atual de passageiros
        elevadores[i].sentido = SUBINDO;
        elevadores[i].entreAndares = 0;
        iniciaVetor(&elevadores[i].vCall,capElevador);
    }
}

// Inicializa��o do vetor de andares
void setupAndares(void)
{
    int i;
    for(i = 0; i < (numAndares+1); i++)
    {
        iniciaVetor(&andares[i].vCall,5);
        andares[i].temChamada = false;
    }
}

// Desaloca vetores e fecha arquivos
void fechaSimulacao(FILE ** arqChamadas, FILE ** arqLog)
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
    fclose(*arqChamadas);
    fclose(*arqLog);
}

// Obtem chamadas da origem desejada -- Revisar
bool pegaChamadas(Chamada * call, FILE ** arqChamadas)
{
    bool status = true;
    bool flag = false;
    int andarOrigem,andarDestino,tempoCall;
    switch(origemChamadas)
    {
    //Random
    default:
    case ALEATORIO:
        // Evita gerar chamadas que n�o ser�o terminadas antes do tempo m�ximo de simula��o
        if(tempoAtual < (tempoSimulacao - numAndares*TEMPO_POR_ANDAR))
        {
            tempoCall = tempoAtual + rand() % DELTA_T_MAX;
            andarOrigem = rand() % numAndares;
            do
            {
                andarDestino = rand() % numAndares;
            }
            while(andarDestino == andarOrigem);
            status = true;
        }
        else
            status = false;
        break;
    //Arquivo
    case ARQUIVO:
        // Verifica por fim de arquivo
        if(fscanf(* arqChamadas,"%d,%d,%d",&tempoCall,&andarOrigem,&andarDestino) != EOF)
        {
            status = true;
            if(tempoCall < 0 || tempoCall > tempoSimulacao || tempoCall < tempoAtual)
                status = false;
            if(andarOrigem < 0 || andarOrigem > numAndares)
                status = false;
            if(andarDestino < 0 || andarDestino > numAndares)
                status = false;
            if(andarDestino == andarOrigem)
                status = false;
        }
        else
            status = false;
        break;
    // Teclado
    case TECLADO:
        // Loopa at� obter entrada v�lida
        if(tempoAtual < tempoSimulacao)
        {
            mostraCursor(true);
            do
            {
                printf("Insira o tempo da chamada: ");
                fflush(stdin);
                scanf("%d",&tempoCall);
                if(tempoCall <= tempoSimulacao && tempoCall > tempoAtual)
                    flag = true;
                else
                    printf("Entrada inv�lida\n");
            }
            while(!flag);
            flag = false;
            do
            {
                printf("Insira o andar de origem: ");
                fflush(stdin);
                scanf("%d",&andarOrigem);
                if(andarOrigem >= 0 && andarOrigem <= numAndares)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            flag = false;
            do
            {
                printf("Insira o andar de destino: ");
                fflush(stdin);
                scanf("%d",&andarDestino);
                if(andarDestino >= 0 && andarDestino <= numAndares && andarDestino != andarOrigem)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            system("cls");
            mostraCursor(false);
            status = true;
            break;
        }
        else
            status = false;
        break;
    }
    if(status)
    {
        // Atributos da chamada s�o setados
        call->andarOrigem = andarOrigem;
        call->andarDestino = andarDestino;
        call->sentido = andarOrigem > andarDestino? DESCENDO : SUBINDO; // Seta o sentido da chamada.
        call->tempoInicial = tempoCall;
        call->ID = idChamada++;
    }
    return status;
}

// Locomove os elevadores
int moveElevadores(FILE ** arqLog)
{
    int i;
    bool flag;
    // Define destino de elevadores ociosos
    defineSentido();
    for(i = 0; i < numElevadores; i++)
    {
        flag = false;
        // Se acabou de chegar no andar...
        if(elevadores[i].entreAndares == 0)
        {
            // Se o andar atual � um andar selecionados internamente:
            if(elevadores[i].andar_sel_int[elevadores[i].andarAtual])
            {
                elevadores[i].andar_sel_int[elevadores[i].andarAtual] = false;
                flag = true;
            }
            // Se o andar atual � um andar selecionados internamente:
            if(elevadores[i].andar_sel_ext[elevadores[i].andarAtual])
            {
                elevadores[i].andar_sel_ext[elevadores[i].andarAtual] = false;
                if(andares[elevadores[i].andarAtual].temChamada)
                    flag = true;
                else if(elevadores[i].numP == 0)
                    elevadores[i].estaAtendendo = false;
            }
            // Flag indica que elevador deve abrir a porta para entrada ou sa�da de passageiros
            if(flag)
                entraSai(i, arqLog);
        }
        // Se o elevador estiver atendendo alguma chamada:
        if(elevadores[i].estaAtendendo == true)
        {
            // Verifica se o tempo por andar foi decorrido e ent�o altera o andar atual
            if(++elevadores[i].entreAndares == TEMPO_POR_ANDAR)
            {
                if(elevadores[i].sentido == SUBINDO && elevadores[i].andarAtual < numAndares)
                    elevadores[i].andarAtual++;
                else if(elevadores[i].sentido == DESCENDO && elevadores[i].andarAtual > 0)
                    elevadores[i].andarAtual--;
                elevadores[i].entreAndares = 0;
            }
        }
    }
    // Incremente o tempo em um segundo
    tempoAtual++;
    return 0;
}

// Insere a chamada no andar de origem correspondente
void posicionaChamada(Chamada  call)
{
    insereChamada(&andares[call.andarOrigem].vCall, call);
    andares[call.andarOrigem].temChamada = true;
}

// Define o sentido de deslocamento de elevadores ociosos
void defineSentido(void)
{
    int i;
    for(i = 0; i < numElevadores; i++)
        if(elevadores[i].estaAtendendo == false)
        {
            int j;
            for(j = 0; j < numAndares; j++)
            {
                int sup = elevadores[i].andarAtual + j;
                int inf = elevadores[i].andarAtual - j;
                if(sup <= numAndares && elevadores[i].andar_sel_ext[sup])
                {
                    elevadores[i].sentido = SUBINDO;
                    elevadores[i].estaAtendendo = true;
                    break;
                }
                else if(inf >= 0 && elevadores[i].andar_sel_ext[inf])
                {
                    elevadores[i].sentido = DESCENDO;
                    elevadores[i].estaAtendendo = true;
                    break;
                }
            }
        }
}

// Define o elevador que atender� a chamada
void defineElevador(Chamada c, FILE ** arqLog)
{
    int i;
    // Aloca vetores para uso no c�lculo das distancias
    int * distancias = (int *) calloc(numElevadores,sizeof(int));
    if(distancias == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de distancias!\n");
        exit(1);
    }
    // Vetor auxiliar para identificar as dist�ncias
    int * id = (int *) calloc(numElevadores,sizeof(int));
    if(id == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de identifica��o!\n");
        exit(1);
    }
    // Calcula a distancia de cada elevador para o andar de origem da chamada
    for(i = 0; i < numElevadores; i++)
    {
        distancias[i] = elevadores[i].andarAtual - c.andarOrigem;
        id[i] = i;
    }
    // Ordena vetor baseado no valor absoluto da dist�ncia
    ordenaDistancias(distancias,id,numElevadores);
    // Para chamada de subir
    if(c.sentido == SUBINDO)
    {
        bool setado = false;
        // Verifica por elevadores subindo que ainda n�o chegaram no andar de origem ou parados
        for(i = 0; i < numElevadores; i++)
        {
            if((distancias[i] < 0 || (distancias[i] == 0 && elevadores[id[i]].entreAndares == 0)) && elevadores[id[i]].sentido == SUBINDO)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                c.elevadorDesignado = id[i];
                setado = true;
                break;
            }
            else if(elevadores[id[i]].estaAtendendo == false)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                c.elevadorDesignado = id[i];
                setado = true;
                break;
            }
        }
        // Caso n�o haja elevadores "ideais", aloca para o mais pr�ximo;
        if(!setado)
        {
            for(i = 0; i < numElevadores; i++)
                // Evita que elevador fique travado caso o mesmo esteja no andar
                if(distancias[i] != 0)
                {
                    elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                    c.elevadorDesignado = id[i];
                    break;
                }
        }

    }
    // Para chamada de descer
    else
    {
        bool setado = false;
        for(i = 0; i < numElevadores; i++)
        {
            if((distancias[i] > 0 || (distancias[i] == 0 && elevadores[id[i]].entreAndares == 0)) && elevadores[id[i]].sentido == DESCENDO)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                c.elevadorDesignado = id[i];
                setado = true;
                break;
            }
            else if(elevadores[id[i]].estaAtendendo == false)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                c.elevadorDesignado = id[i];
                setado = true;
                break;
            }
        }
        if(!setado)
        {
            for(i = 0; i < numElevadores; i++)
            {
                // Evita que elevador fique travado caso o mesmo esteja no andar
                if(distancias[i] != 0)
                {
                    elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                    c.elevadorDesignado = id[i];
                    break;
                }
            }

        }
    }
    fprintf(* arqLog,"T: %d | Chamada do passageiro #%d no andar %d, designada ao elevador #%d\n",tempoAtual,c.ID+1,c.andarOrigem,c.elevadorDesignado+1);
    printf("T: %d | Chamada do passageiro #%d no andar %d, designada ao elevador #%d\n",tempoAtual,c.ID+1,c.andarOrigem,c.elevadorDesignado+1);
    free(distancias);
    free(id);
    distancias = NULL;
    id = NULL;
}

// Define entrada e sa�da de passageiros
void entraSai(int ID, FILE ** arqLog)
{
    int i;
    fprintf(* arqLog,"T: %d | Elevador #%d - Porta abrindo no andar %d...\n",tempoAtual,ID+1,elevadores[ID].andarAtual);
    printf("T: %d | Elevador #%d - Porta abrindo no andar %d...\n",tempoAtual,ID+1,elevadores[ID].andarAtual);
    tempoAtual += TEMPO_ABERTURA;
    //Passsageiros descem primeiro:
    for(i = 0; i < elevadores[ID].vCall.qntd; i++)
        //Checa se o andar atual � o andar de destino do passageiro no interior do elevador
        if(elevadores[ID].vCall.chamadas[i].andarDestino == elevadores[ID].andarAtual)
        {
            fprintf(* arqLog,"T: %d | Elevador #%d - Passageiro #%d desceu no andar %d\n",tempoAtual, ID+1, elevadores[ID].vCall.chamadas[i].ID+1, elevadores[ID].andarAtual);
            printf("T: %d | Elevador #%d - Passageiro #%d desceu no andar %d\n",tempoAtual, ID+1, elevadores[ID].vCall.chamadas[i].ID+1, elevadores[ID].andarAtual);
            // Insere o tempo de sa�da do elevador
            elevadores[ID].vCall.chamadas[i].tempoSaida = tempoAtual;
            // Remove chamada do elevador e coloca no vetor de chamadas conclu�das
            insereChamada(&chamadasConcluidas,elevadores[ID].vCall.chamadas[i]);
            removeChamada(&elevadores[ID].vCall,i);
            // Ajusta o indice caso ainda existam chamadas no elevador
            if(elevadores[ID].vCall.qntd > 0)
                i--;
            //Atualiza o n�mero atual de passageiros
            elevadores[ID].numP--;
        }
    // Entrada de passageiros se o andar os tiver
    if(andares[elevadores[ID].andarAtual].temChamada == true)
    {
        for(i = 0; i < andares[elevadores[ID].andarAtual].vCall.qntd; i++)
        {
            // Se o n�mero de passageiros atual for zero ent�o o primeiro passageiro que entrar ter� controle sobre o elevador
            if(elevadores[ID].numP == 0)
            {
                fprintf(* arqLog,"T: %d | Elevador #%d - Passageiro #%d entrou com destino ao andar %d\n", tempoAtual, ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                printf("T: %d | Elevador #%d - Passageiro #%d entrou com destino ao andar %d\n", tempoAtual, ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                // Insere o tempo de entrada no elevador
                andares[elevadores[ID].andarAtual].vCall.chamadas[i].tempoEntrada = tempoAtual;
                // Incrementa o total de passageiros transportados e o n�mero atual de passageiros.
                elevadores[ID].numP++;
                elevadores[ID].totalP++;
                // Seta o sentido do elevador como o sentido da chamada
                elevadores[ID].sentido = andares[elevadores[ID].andarAtual].vCall.chamadas[i].sentido;
                // Adiciona andar de destino do passageiro que entrou
                elevadores[ID].andar_sel_int[andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino] = true;
                // Insere chamada no elevador e a remove do andar.
                insereChamada(&elevadores[ID].vCall,andares[elevadores[ID].andarAtual].vCall.chamadas[i]);
                removeChamada(&andares[elevadores[ID].andarAtual].vCall,i);
                // Ajusta o indice caso ainda existam chamadas no andar
                if(andares[elevadores[ID].andarAtual].vCall.qntd > 0)
                    i--;
            }
            // Checa se o sentido da chamada � o mesmo do elevador e se a capacidade do elevador n�o ser� excedida.
            else if((andares[elevadores[ID].andarAtual].vCall.chamadas[i].sentido == elevadores[ID].sentido) &&  (elevadores[ID].numP+1) <= capElevador)
            {
                fprintf(* arqLog,"T: %d | Elevador #%d - Passageiro #%d entrou com destino ao andar %d\n", tempoAtual, ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                printf("T: %d | Elevador #%d - Passageiro #%d entrou com destino ao andar %d\n", tempoAtual, ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID+1, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                // Insere o tempo de entrada no elevador
                andares[elevadores[ID].andarAtual].vCall.chamadas[i].tempoEntrada = tempoAtual;
                // Incrementa o total de passageiros transportados e o n�mero atual de passageiros.
                elevadores[ID].numP++;
                elevadores[ID].totalP++;
                // Adiciona andar de destino do passageiro que entrou
                elevadores[ID].andar_sel_int[andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino] = true;
                // Insere chamada no elevador e a remove do andar.
                insereChamada(&elevadores[ID].vCall,andares[elevadores[ID].andarAtual].vCall.chamadas[i]);
                removeChamada(&andares[elevadores[ID].andarAtual].vCall,i);
                if(andares[elevadores[ID].andarAtual].vCall.qntd > 0)
                    i--;
            }
            else
            {
                // Se por algum motivo, o passageiro n�o entrou no elevador, ent�o designa outro elevador para atender a chamada.
                defineElevador(andares[elevadores[ID].andarAtual].vCall.chamadas[i], arqLog);
            }

        }
    }
    // Se o n�mero de chamadas no andar for zero ent�o seta temChamada como falso
    if(andares[elevadores[ID].andarAtual].vCall.qntd == 0)
        andares[elevadores[ID].andarAtual].temChamada = false;
    // Se o n�mero de passageiros apos a entrada e sa�da for igual a zero ent�o o elevador est� apto a receber uma chamada externa
    if(elevadores[ID].numP == 0)
        elevadores[ID].estaAtendendo = false;
    fprintf(* arqLog,"T: %d | Elevador #%d - Porta fechando no andar %d...\n",tempoAtual,ID+1,elevadores[ID].andarAtual);
    printf("T: %d | Elevador #%d - Porta fechando no andar %d...\n",tempoAtual,ID+1,elevadores[ID].andarAtual);
    tempoAtual += TEMPO_FECHAMENTO;
}

// Calcula as estat�sticas baseado no vetor de chamadas conclu�das
void geraEstatisticas(void)
{
    FILE * arqStat = fopen(NOME_ARQ_STAT,"w");
    if(arqStat == NULL)
    {
        perror("\nArquivo de estat�sticas - O seguinte erro ocorreu: ");
        exit(1);
    }
    if(chamadasConcluidas.qntd > 0)
    {
        int i;
        // Quantidade de chamadas concluidas
        int qntd = chamadasConcluidas.qntd;
        // Aloca vetores para tempos de espera e de viagem
        int * tEsperas = (int *) calloc(qntd,sizeof(int));
        if(tEsperas == NULL)
        {
            fprintf(stderr, "Falha ao alocar vetor de tempos de espera\n");
            exit(1);
        }
        int * tViagens = (int *) calloc(qntd,sizeof(int));
        if(tViagens == NULL)
        {
            fprintf(stderr, "Falha ao alocar vetor de tempos de viagem\n");
            exit(1);
        }
        // M�dia, M�ximo, M�nimo, Mediana e Desvio Padr�o
        double tEsperaMed, tEsperaMax, tEsperaMediana, tEsperaDP;
        double tViagemMed, tViagemMax, tViagemMediana, tViagemDP;
        // N�mero m�dio de passageiros por minuto
        double numPassageirosMed = 0;
        // Percorre vetor de chamadasConcluidas calculando os tempos
        for(i = 0; i < qntd; i++)
        {
            tEsperas[i] = chamadasConcluidas.chamadas[i].tempoEntrada - chamadasConcluidas.chamadas[i].tempoInicial;
            tViagens[i] = chamadasConcluidas.chamadas[i].tempoSaida - chamadasConcluidas.chamadas[i].tempoEntrada;
        }
        // Ordena vetores
        ordenaVetor(tEsperas, qntd);
        ordenaVetor(tViagens, qntd);
        // C�lcula m�dia dos vetores e insere nas variaveis correspondentes
        tEsperaMed = media(tEsperas, qntd);
        tViagemMed = media(tViagens, qntd);
        // Pega �ltimo elemento dos vetores
        tEsperaMax = tEsperas[qntd-1];
        tViagemMax = tViagens[qntd-1];
        // Mediana
        tEsperaMediana = mediana(tEsperas, qntd);
        tViagemMediana = mediana(tViagens, qntd);
        // Desvio Padr�o
        tEsperaDP = desvioPadrao(tEsperas, qntd);
        tViagemDP = desvioPadrao(tViagens, qntd);
        // C�lculo da n�mero m�dio de passageiros transportados com tempo em horas
        numPassageirosMed = qntd / (tempoAtual / 3600.0);
        // Imprime resultados.
        printf("\nEstat�sticas da simula��o: \n\nTempo de simula��o: %d s\nChamadas realizadas: %d\nChamadas conclu�das: %d\n\n",tempoAtual,idChamada,qntd);
        printf("Tempo m�dio de espera: %.2f s\nTempo m�ximo de espera: %.2f s\nTempo mediano de espera: %.2f s\nDesvio Padr�o de espera: %.2f s\n\n",tEsperaMed,tEsperaMax,tEsperaMediana,tEsperaDP);
        printf("Tempo m�dio de viagem: %.2f s\nTempo m�ximo de viagem: %.2f s\nTempo mediano de viagem: %.2f s\nDesvio Padr�o de viagem: %.2f s\n\n",tViagemMed,tViagemMax,tViagemMediana,tViagemDP);
        printf("N�mero m�dio de passageiros transportados por hora: %.2lf\nN�mero m�ximo de passageiros pendentes: %d\n",numPassageirosMed,cargaMax);
        // Imprime no arquivo de estat�sticas
        fprintf(arqStat,"%d|%d|%d|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%d",tempoAtual,idChamada,qntd,tEsperaMed,tEsperaMax,tEsperaMediana,tEsperaDP,tViagemMed,tViagemMax,tViagemMediana,tViagemDP,numPassageirosMed,cargaMax);
        // Desaloca vetores
        free(tEsperas);
        free(tViagens);
    }
    else
        printf("\nNenhuma chamada foi conclu�da!\n");
    fclose(arqStat);
}

// Fun��o para abertura dos arquivos necess�rios que checa por erros
void abreArquivos(FILE ** arqChamadas, FILE ** arqLog)
{
    // Abre o arquivo de chamadas
    * arqChamadas = fopen(NOME_ARQ_CHAMADAS,"r");
    if(* arqChamadas == NULL)
    {
        perror("\nArquivo de chamadas - O seguinte erro ocorreu: ");
        exit(1);
    }
    //Ignora primeira linha do arquivo de chamadas
    fscanf(* arqChamadas, "%*[^\n]\n");
    * arqLog = fopen(NOME_ARQ_LOG,"w");
    if(* arqLog == NULL)
    {
        perror("\nArquivo de logs - O seguinte erro ocorreu: ");
        exit(1);
    }
}

// Fun��o de aloca��o dos vetores de andares e elevadores
void alocaEA(void)
{
    elevadores = (Elevador *) calloc(numElevadores,sizeof(Elevador)); // Aloca vetor de struct Elevador
    if(elevadores == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de elevadores\n");
        exit(1);
    }
    andares = (Andar *) calloc((numAndares+1),sizeof(Andar)); // Aloca vetor de struct Andar
    if(andares == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de elevadores\n");
        exit(1);
    }
}

void animacao(void)
{
    int i;
    system("cls");
    printaEspacos(14);
    printf("T");
    for(i = 1; i <= numAndares; i++)
        printf("  %d",i);
    putchar('\n');
    for(i = 0; i < numElevadores; i++)
    {
        int j;
        printf("Elevador #%d - *",i);
        printaEspacos(4*numAndares+1);
        printf("Andares selecionados: ");
        for(j = 0; j <= numAndares; j++)
            if(elevadores[i].andar_sel_int[j])
                printf("%d ",j);
        putchar('\n');
    }
}

/*------------------------------------------------------------------------------------------------*/

/* Fun��es do Vetor_Chamada */

// Fun��o de inicializa��o do vetor din�mico de chamadas
void iniciaVetor(Vetor_Chamada * c, size_t tamInicial)
{
    c -> chamadas = (Chamada *) calloc(tamInicial,sizeof(Chamada));
    if(c->chamadas == NULL)
    {
        fprintf(stderr,"Erro ao iniciar vetor de chamadas! \n");
        exit(1);
    }
    c -> qntd = 0;
    c -> tam = tamInicial;
}

// Fun��o de inser��o no vetor
void insereChamada(Vetor_Chamada * c, Chamada call)
{
    if(c -> qntd == c -> tam)
    {
        c -> tam *= 2;
        c -> chamadas = (Chamada *) realloc(c->chamadas,c->tam*sizeof(Chamada));
        if(c->chamadas == NULL)
        {
            fprintf(stderr,"Erro ao aumentar vetor de chamadas!\n");
            exit(1);
        }
    }
    c->chamadas[c->qntd++] = call;
}

// Desaloca vetor
void limpaVetor(Vetor_Chamada * c)
{
    free(c->chamadas);
    c->chamadas = NULL;
    c->qntd = c->tam = 0;
}

// Remove chamada do vetor
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

// Fun��o substituta para o system("pause");
void pausa(void)
{
    fflush(stdin);
    printf("\nPressione qualquer tecla para voltar ao menu....\n");
    getch();
}

void ordenaDistancias(int * vec, int * index, int tam)
{
    int mem1 = 0, mem2 = 0,j, i;
    for (i = 1; i < tam; i++)
    {
        for (j = 0; j < tam - i; j++)
        {
            if (abs(vec[j]) > abs(vec[j + 1]))
            {
                mem1 = vec[j];
                mem2 = index[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = mem1;
                index[j] = index[j + 1];
                index[j + 1] = mem2;
            }
        }
    }
}

void ordenaVetor(int * vec,int tam)
{
    int mem = 0, i, j;
    for (i = 1; i < tam; i++)
    {
        for (j = 0; j < tam - i; j++)
        {
            if (vec[j] > vec[j + 1])
            {
                mem = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = mem;
            }
        }
    }
}

// Calcula m�dia aritm�tica de um vetor de inteiros
double media(int * vec, int tam)
{
    int i;
    double media = 0;
    for(i = 0; i < tam; i++)
        media += vec[i];
    media /= tam;
    return media;
}

// Calcula a mediana de um vetor de inteiros
double mediana(int * vec, int tam)
{
    int index;
    double mediana;
    // Ordena vetor para encontrar a mediana
    ordenaVetor(vec, tam);
    // Caso seja impar
    if(tam % 2 == 1)
    {
        index = (int) tam / 2 + 1;
        mediana = (double) vec[index];
    }
    else
    {
        index = tam / 2;
        // Caso existam apenas duas chamadas.
        if(index == 1)
            index = 0;
        mediana = (double) (vec[index] + vec[index+1]) / 2;
    }
    return mediana;
}

// Calcula o desvio padr�o de um vetor de inteiros
double desvioPadrao(int * vec, int tam)
{
    int i = 0;
    double md = media(vec,tam);
    double DP = 0;
    for(i = 0; i < tam; i ++)
    {
        DP += pow((vec[i] - md), 2);
    }
    return sqrt(DP/tam);
}

void printaEspacos(int qntd)
{
    int i = 0;
    for(i = 0; i < qntd; i++)
        putchar(' ');
}
/*------------------------------------------------------------------------------------------------*/
