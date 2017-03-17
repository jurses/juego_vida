#include <iostream>
//#include "tablero.hpp"
#include "celula.hpp"

int main(void){
    medio_cultivo_t medio(10, 10);
	medio.mostrarCultivo(std::cout);
	std::cout << std::endl;

	while(getchar() != 'q' ){
		medio.explorarCultivo();
		medio.actualizarCultivo();

		medio.mostrarCultivo(std::cout);
		std::cout << std::endl;

		medio.mostrarVecinos(std::cout);
		std::cout << std::endl;
	}
}