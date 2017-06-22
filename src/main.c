#include "source.h"

int main(void)
{
    bool sair = false;
    setlocale(LC_ALL, "Portuguese");
    mostraCursor(false);
    iniciaVariaveis();
    while(!sair)
    {
        switch(mostraMenu())
        {
        case 1:
            instrucoes();
            break;
        case 2:
            defineParametros();
            break;
        case 3:
            simula();
            break;
        case 4:
            mostraEstatisticas();
            break;
        case 5:
            sair = true;
            break;
        }
    }
    return 0;
}
