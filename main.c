#include "source.h"
#include <locale.h>

int main(int argc, char **argv)
{
    bool sair = false;
    Predio p;
    setlocale(LC_ALL, "Portuguese");
    mostraCursor(false);
    p.capElevador = CAP_ELEVADOR_STD;
    p.numAndares = NUM_ANDARES_STD;
    p.numElevadores = NUM_ELEVADORES_STD;
    while(!sair)
    {
        switch(mostraMenu())
        {
        case 1:
            instrucoes();
            break;
        case 2:
            defineParametros(&p);
            break;
        case 3:
            simula(&p);
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
