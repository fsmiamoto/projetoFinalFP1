#include "source.h"
#include <locale.h>

int main(int argc, char **argv)
{
    bool sair = false;
    setlocale(LC_ALL, "Portuguese");
    mostraCursor(false);
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
            estatisticas();
            break;
        case 5:
            sair = true;
            break;
        }
    }
    return 0;
}
