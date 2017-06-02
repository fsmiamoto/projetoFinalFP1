 /* Código de teste
    p->elevadores[0].andarSel[5] = true;
    p->elevadores[0].andarAtual = 10;
    p->elevadores[0].qntdPassageiros = 1;
    p->elevadores[0].temPassageiros = true;
    p->elevadores[0].totalPassageiros += 1;
    p->elevadores[0].sentido = DESCENDO;
    for(i = 0; i < p -> numAndares + 1; i++)
        if(p->elevadores[0].sentido == DESCENDO && p->elevadores[0].andarSel[i] == true)
            p->elevadores[0].andarDestino = i;
    tempoDecorrido += moveElevador(&(p->elevadores[0]));
    printf("Tempo decorrido: %d s \nE0 - Andar atual: %d",tempoDecorrido,p->elevadores[0].andarAtual);
    */
