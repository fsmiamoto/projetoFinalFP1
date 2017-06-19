#include "source.h"

// Vari�veis globais
int tempo, idChamada, cont, numAndares, numElevadores, capElevador;
int * distancias, * id;
Elevador * elevadores = NULL;
Andar * andares = NULL;
Chamada call;
Vetor_Chamada chamadasConcluidas;
FILE * arqChamadas;
FILE * arqLog;
FILE * arqStat;
const char * nomeArqChamadas = ".\\arquivos\\chamadas.txt";
const char * nomeArqLog = ".\\arquivos\\log.txt";
const char * nomeArqStat = ".\\arquivos\\estatisticas.txt";

/* Fun��es do Menu */

// Printa o menu no console e retorna a op��o selecionada
int mostraMenu(void)
{
    char escolha;
    bool valido = false;
    system("cls");
    printf("Simulador para controle de elevadores\n\n");
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
void instrucoes()
{
    system("cls");
    printf("Bem-vindo ao Simulador para Controle de Elevadores!\n");
    pausa();
}
// Fun��o de defini��o dos par�metros de simula��o
void defineParametros(int * elevadores, int * andares, int * cap)
{
    char c = 0;
    bool flag = false;
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
            flag = false;
            do
            {
                printf("N�mero de andares: ");
                scanf("%d",&buf);
                if(buf > 0 && buf <= ANDARES_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            * andares = buf;
            mostraCursor(false);
            break;
        case '2':
            system("cls");
            mostraCursor(true);
            flag = false;
            do
            {
                printf("N�mero de elevadores: ");
                scanf("%d",&buf);
                if(buf > 0 && buf <= ELEVADORES_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
            * elevadores = buf;
            mostraCursor(false);
            break;
        case '3':
            system("cls");
            mostraCursor(true);
            flag = false;
            do
            {
                printf("Capacidade m�xima por elevador: ");
                scanf("%d",&buf);
                if(buf > 0 && buf <= CAP_MAX)
                    flag = true;
                else
                    printf("Entrada inv�lida!\n");
            }
            while(!flag);
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
    FILE * arq = fopen(nomeArqStat,"r");;
    float tEsperaMed,tEsperaMax,tViagemMed,tViagemMax,numPassageirosMed;
    if(arq == NULL)
    {
        perror("\nArquivo de estat�sticas - O seguinte erro ocorreu: ");
        exit(1);
    }
    if(fscanf(arq,"%f|%f|%f|%f|%f",&tEsperaMed,&tEsperaMax,&tViagemMed,&tViagemMax,&numPassageirosMed))
    {
        system("cls");
        printf("Tempo m�dio de espera: %.1f\nTempo m�ximo de espera: %.1f",tEsperaMed,tEsperaMax);
        printf("\nTempo m�dio de viagem: %.1f\nTempo m�ximo de viagem: %.1f",tViagemMed,tViagemMax);
        printf("\nN�mero m�dio de passageiros por segundo: %.5f\n",numPassageirosMed);
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
void simula(int numE, int numA,int cap)
{
    // Globaliza os par�metros
    numAndares = numA;
    numElevadores = numE;
    capElevador = cap;
    // Seta tempo e contador de identifica��o de chamadas para zero
    tempo = 0;
    idChamada = 0;
    cont = 0;
    // Aloca um vetor com o n�mero de elevadores da struct Elevadorgene
    alocaEA();
    // Abre os arquivos necess�rios
    abreArquivos();
    // Limpa a tela
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
            }

            defineElevador(call);
            posicionaChamada();
        }
        else
        {
            moveElevadores();
        }

    }
    geraEstatisticas();
    //animacao();
    // Libera mem�ria utilizada e fechar arquivos utilizados
    fechaSimulacao();
    pausa();
}

/*------------------------------------------------------------------------------------------------*/

/* Fun��es do Simulador */

// Inicializa��o do vetor de elevadores
void setupElevadores()
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
void setupAndares()
{
    int i;
    for(i = 0; i < (numAndares+1); i++)
    {
        iniciaVetor(&andares[i].vCall,5);
        andares[i].temChamada = false;
    }
}

// Desaloca vetores e fecha arquivos
void fechaSimulacao()
{
    int i;
    for(i = 0; i < numElevadores; i++)
        limpaVetor(&elevadores[i].vCall);
    for(i = 0; i <= numAndares; i++)
        limpaVetor(&andares[i].vCall);
    free(elevadores);
    free(andares);
    free(distancias);
    free(id);
    andares = NULL;
    elevadores = NULL;
    limpaVetor(&chamadasConcluidas);
    fclose(arqChamadas);
    fclose(arqLog);
    fclose(arqStat);
}

// Obtem chamadas da origem desejada -- Revisar
bool pegaChamadas(int origem)
{
    bool status = true;
    bool flag = false;
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
        // Verifica por fim de arquivo
        if(fscanf(arqChamadas,"%d,%d,%d",&tempoCall,&andarOrigem,&andarDestino) != EOF)
            status = true;
        else
            status = false;
        break;
    // Teclado
    case 3:
        // Loopa at� obter entrada v�lida
        mostraCursor(true);
        do
        {
            printf("Insira o tempo da chamada: ");
            scanf("%d",&tempoCall);
            if(tempoCall <= TEMPO_MAX && tempoCall > tempo)
                flag = true;
            else
                printf("Entrada inv�lida\n");
        }
        while(!flag);
        flag = false;
        do
        {
            printf("Insira o andar de origem: ");
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
    if(status)
    {
        // Atributos da chamada s�o setados
        call.andarOrigem = andarOrigem;
        call.andarDestino = andarDestino;
        call.sentido = andarOrigem > andarDestino? DESCENDO : SUBINDO; // Seta o sentido da chamada.
        call.tempoInicial = tempoCall;
        call.ID = idChamada++;
    }
    return status;
}

// Locomove os elevadores
int moveElevadores()
{
    int i;
    bool flag;
    // Define destino de elevadores ociosos
    defineSentido();
    for(i = 0; i < numElevadores; i++)
    {
        flag = false;
        //printf("T: %d | Elevador #%d  - Andar Atual: %d\n",tempo,i,elevadores[i].andarAtual);
        // Se o elevador estiver atendendo alguma chamada:
        if(elevadores[i].estaAtendendo == true)
        {
            // Verifica se o tempo por andar foi decorrido e ent�o altera o andar atual
            if(++elevadores[i].entreAndares == TEMPO_POR_ANDAR)
            {
                if(elevadores[i].sentido == SUBINDO)
                    elevadores[i].andarAtual++;
                else
                    elevadores[i].andarAtual--;
                elevadores[i].entreAndares = 0;
            }
        }
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
            flag = true;
        }
        // Flag indica que elevador deve abrir a porta para entrada ou sa�da de passageiros
        if(flag)
        {
            entraSai(i);
            defineSentido();
        }
    }
    // Incremente o tempo em um segundo
    tempo++;
    return 0;
}

// Insere a chamada no andar de origem correspondente
void posicionaChamada()
{
    insereChamada(&andares[call.andarOrigem].vCall, call);
    andares[call.andarOrigem].temChamada = true;
}

// Define o sentido de deslocamento de elevadores ociosos
void defineSentido()
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
                }
                else if(inf >= 0 && elevadores[i].andar_sel_ext[inf])
                {
                    elevadores[i].sentido = DESCENDO;
                    elevadores[i].estaAtendendo = true;
                }
            }
        }
}

// Define o elevador que atender� a chamada
void defineElevador(Chamada c)
{
    int i, escolhido;
    // Aloca vetores para uso no c�lculo das distancias
    distancias = (int *) calloc(numElevadores,sizeof(int));
    if(distancias == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de distancias!\n");
        exit(1);
    }
    // Vetor auxiliar para identificar as dist�ncias
    id = (int *) calloc(numElevadores,sizeof(int));
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
    ordena(distancias,id,numElevadores);
    // Para chamada de subir
    if(c.sentido == SUBINDO)
    {
        int i;
        bool setado = false;
        // Verifica por elevadores subindo que ainda n�o chegaram no andar de origem ou parados
        for(i = 0; i < numElevadores; i++)
            if(distancias[i] < 0 && elevadores[id[i]].sentido == SUBINDO)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                escolhido = id[i];
                setado = true;
                break;
            }
            else if(elevadores[id[i]].estaAtendendo == false)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                escolhido = id[i];
                setado = true;
                break;
            }
        // Caso n�o haja elevadores "ideais", aloca para o mais pr�ximo;
        if(!setado)
        {
            elevadores[id[0]].andar_sel_ext[c.andarOrigem] = true;
            escolhido = id[0];
        }

    }
    // Para chamada de descer
    else
    {
        int i;
        bool setado = false;
        for(i = 0; i < numElevadores; i++)
            if(distancias[i] > 0 && elevadores[id[i]].sentido == DESCENDO)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                escolhido = id[i];
                setado = true;
                break;
            }
            else if(elevadores[id[i]].estaAtendendo == false)
            {
                elevadores[id[i]].andar_sel_ext[c.andarOrigem] = true;
                escolhido = id[i];
                setado = true;
                break;
            }
        if(!setado)
        {
            elevadores[id[0]].andar_sel_ext[c.andarOrigem] = true;
            escolhido = id[0];
        }
    }
    fprintf(arqLog,"T: %d | Chamada no andar %d, atendido pelo elevador #%d\n",call.tempoInicial,call.andarOrigem,escolhido);
    printf("T: %d | Chamada no andar %d, atendido pelo elevador #%d\n",call.tempoInicial,call.andarOrigem,escolhido);
}

// Define entrada e sa�da de passageiros
void entraSai(int ID)
{
    int i;
    fprintf(arqLog,"T: %d | Elevador #%d - Porta abrindo no andar %d...\n",tempo,ID,elevadores[ID].andarAtual);
    printf("T: %d | Elevador #%d - Porta abrindo no andar %d...\n",tempo,ID,elevadores[ID].andarAtual);
    tempo += TEMPO_ABERTURA;
    //Passsageiros descem primeiro:
    for(i = 0; i < elevadores[ID].vCall.qntd; i++)
        //Checa se o andar atual � o andar de destino do passageiro no interior do elevador
        if(elevadores[ID].vCall.chamadas[i].andarDestino == elevadores[ID].andarAtual)
        {
            fprintf(arqLog,"T: %d | Elevador #%d - Passageiro %d desceu no andar %d\n",tempo, ID, elevadores[ID].vCall.chamadas[i].ID, elevadores[ID].andarAtual);
            printf("T: %d | Elevador #%d - Passageiro %d desceu no andar %d\n",tempo, ID, elevadores[ID].vCall.chamadas[i].ID, elevadores[ID].andarAtual);
            // Insere o tempo de sa�da do elevador
            elevadores[ID].vCall.chamadas[i].tempoSaida = tempo;
            // Remove chamada do elevador e coloca no vetor de chamadas conclu�das
            insereChamada(&chamadasConcluidas,elevadores[ID].vCall.chamadas[i]);
            removeChamada(&elevadores[ID].vCall,i);
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
                fprintf(arqLog,"T: %d | Elevador #%d - Passageiro %d entrou com destino ao andar %d\n", tempo, ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                printf("T: %d | Elevador #%d - Passageiro %d entrou com destino ao andar %d\n", tempo, ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                // Insere o tempo de entrada no elevador
                andares[elevadores[ID].andarAtual].vCall.chamadas[i].tempoEntrada = tempo;
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
            }
            // Checa se o sentido da chamada � o mesmo do elevador e se a capacidade do elevador n�o ser� excedida.
            else if((andares[elevadores[ID].andarAtual].vCall.chamadas[i].sentido == elevadores[ID].sentido) &&  (elevadores[ID].numP+1) <= capElevador)
            {
                fprintf(arqLog,"T: %d | Elevador #%d - Passageiro %d entrou com destino ao andar %d\n", tempo, ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                printf("T: %d | Elevador #%d - Passageiro %d entrou com destino ao andar %d\n", tempo, ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].ID, andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino);
                // Insere o tempo de entrada no elevador
                andares[elevadores[ID].andarAtual].vCall.chamadas[i].tempoEntrada = tempo;
                // Incrementa o total de passageiros transportados e o n�mero atual de passageiros.
                elevadores[ID].numP++;
                elevadores[ID].totalP++;
                // Adiciona andar de destino do passageiro que entrou
                elevadores[ID].andar_sel_int[andares[elevadores[ID].andarAtual].vCall.chamadas[i].andarDestino] = true;
                // Insere chamada no elevador e a remove do andar.
                insereChamada(&elevadores[ID].vCall,andares[elevadores[ID].andarAtual].vCall.chamadas[i]);
                removeChamada(&andares[elevadores[ID].andarAtual].vCall,i);
                // Se o n�mero de chamadas no elevador for zero ent�o seta temChamada como falso
                if(andares[elevadores[ID].andarAtual].vCall.qntd == 0)
                    andares[elevadores[ID].andarAtual].temChamada = false;
            }
            else
                // Se por algum motivo, o passageiro n�o entrou no elevador, ent�o designa outro elevador para atender a chamada.
                defineElevador(andares[elevadores[ID].andarAtual].vCall.chamadas[i]);
        }
    }
    // Se o n�mero de passageiros apos a entrada e sa�da for igual a zero ent�o o elevador est� apto a receber uma chamada externa
    if(elevadores[ID].numP == 0)
        elevadores[ID].estaAtendendo = false;
    fprintf(arqLog,"T: %d | Elevador #%d - Porta fechando no andar %d...\n",tempo,ID,elevadores[ID].andarAtual);
    printf("T: %d | Elevador #%d - Porta fechando no andar %d...\n",tempo,ID,elevadores[ID].andarAtual);
    tempo += TEMPO_FECHAMENTO;
}

// Calcula as estat�sticas baseado no vetor de chamadas conclu�das
void geraEstatisticas()
{
    int i;
    float tEsperaMed = 0, tEsperaMax = 0, tViagemMed = 0, tViagemMax = 0, numPassageirosMed = 0;
    for(i = 0; i < chamadasConcluidas.qntd; i++)
    {
        // C�lculos do tempo de espera
        float tEspera = chamadasConcluidas.chamadas[i].tempoEntrada - chamadasConcluidas.chamadas[i].tempoInicial;
        tEsperaMed += tEspera;
        if(tEspera > tEsperaMax)
            tEsperaMax = tEspera;
        // C�lculos do tempo de viagem
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
    printf("\nEstat�sticas de simula��o: \nTempo m�dio de espera: %.1f\nTempo m�ximo de espera: %.1f",tEsperaMed,tEsperaMax);
    printf("\nTempo m�dio de viagem: %.1f\nTempo m�ximo de viagem: %.1f",tViagemMed,tViagemMax);
    printf("\nN�mero m�dio de passageiros por segundo: %.5f\n\n",numPassageirosMed);
    fprintf(arqStat,"%.1f|%.1f|%.1f|%.1f|%.5f",tEsperaMed,tEsperaMax,tViagemMed,tViagemMax,numPassageirosMed);
}

// Fun��o para abertura dos arquivos necess�rios que checa por erros
void abreArquivos()
{
    arqChamadas = fopen(nomeArqChamadas,"r"); // Arquivo de leitura de chamadas
    if(arqChamadas == NULL)
    {
        perror("\nArquivo de chamadas - O seguinte erro ocorreu: ");
        exit(1);
    }
    arqLog = fopen(nomeArqLog,"w");
    if(arqLog == NULL)
    {
        perror("\nArquivo de logs - O seguinte erro ocorreu: ");
        exit(1);
    }
    fseek(arqChamadas,47,SEEK_SET); // Ignora primeira linha do arquivo
    arqStat = fopen(nomeArqStat,"w");
    if(arqStat == NULL)
    {
        perror("\nArquivo de estat�sticas - O seguinte erro ocorreu: ");
        exit(1);
    }
}

// Fun��o de aloca��o dos vetores de andares e elevadores
void alocaEA()
{
    elevadores = (Elevador *) malloc(numElevadores * sizeof(Elevador)); // Aloca vetor de struct Elevador
    if(elevadores == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de elevadores\n");
        exit(1);
    }
    andares = (Andar *) malloc((numAndares+1) * sizeof(Andar)); // Aloca vetor de struct Andar
    if(andares == NULL)
    {
        fprintf(stderr, "Falha ao alocar vetor de elevadores\n");
        exit(1);
    }
}

void animacao()
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

void printaEspacos(int qntd)
{
    int i = 0;
    for(i = 0; i < qntd; i++)
        putchar(' ');
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
void pausa()
{
    fflush(stdin);
    printf("\nPressione qualquer tecla para voltar ao menu....\n");
    getch();
}

void ordena(int * vec, int * index, int tam)
{
    int temp = 0, j, i;
    for (i = 1; i < tam; i++)
    {
        for (j = 0; j < tam - i; j++)
        {
            if (abs(vec[j]) > abs(vec[j + 1]))
            {
                temp = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = temp;
                temp = index[j];
                index[j] = index[j + 1];
                index[j + 1] = index[j];
            }
        }
    }
}

/*------------------------------------------------------------------------------------------------*/
