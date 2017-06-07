#include "source.h"
#include <locale.h>

int main(int argc, char **argv)
{
    bool sair = false;
    setlocale(LC_ALL, "Portuguese");
    mostraCursor(false);
    int numElevadores = NUM_ELEVADORES_STD;
    int numAndares = NUM_ANDARES_STD;
    int capElevador = CAP_ELEVADOR_STD;
    while(!sair)
    {
        switch(mostraMenu())
        {
        case 1:
            instrucoes();
            break;
        case 2:
            defineParametros(&numElevadores,&numAndares,&capElevador);
            break;
        case 3:
            simula(numElevadores,numAndares,capElevador);
            break;
        case 4:
            estatisticas();
            break;
        case 5:
            sair = true;
            break;
        }
    }
    return 0;
}
