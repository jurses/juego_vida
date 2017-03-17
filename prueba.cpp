#include <iostream>
#include "tablero.hpp"

int main(void){
    int col = 23;
    int fil = 5;

    for(int i = 1; i<=fil; i++)
        for(int j = 1; j<=col; j++)
            std::cout << (i-1)*col+j << std::endl;
}