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
/*
     if(ORIGEM_CHAMADAS == 2)
    {
        arq = fopen(arqChamadas,"r");
        for(i = 0; i < 3; i++)
        {
            Chamada c = geraChamadas(3,arq);
        
            elevadores[0].andarSel[c.andar] = true;
            elevadores[0].andarDestino = c.andar;
            elevadores[0].totalPassageiros += c.qntdPassageiros;
            if(elevadores[0].andarAtual < c.andar)
                elevadores[0].sentido = DESCENDO;
            else
                elevadores[0].sentido = SUBINDO;
            tempoDecorrido += moveElevadores(&(elevadores[0]));
            printf("Tempo decorrido: %d s \nE0 - Andar atual: %d\n",tempoDecorrido,elevadores[0].andarAtual);
        }
    }
*/

int j;
            if(elevadores[i].sentido == SUBINDO)
            {
               for(j = elevadores[i].andarAtual; j <= numAndares; j++)
                    if(elevadores[i].andarSelect[j])
                    {
                        elevadores[i].andarDestino = j;
                        elevadores[i].estaMovendo = true;
                        printf("Andar: %d\n",j);
                    }
            }
            else
            {
                for(j = elevadores[i].andarAtual; j >= 0; j--)
                    if(elevadores[i].andarSelect[j])
                    {
                        elevadores[i].andarDestino = j;
                        elevadores[i].estaMovendo = true;
                    }
            }