#include <iostream>
#include <cstdio>
#include <cstdlib>

class celula_base_t;
class celula_A_t;

class medio_cultivo_t{
	private:
		celula_base_t** cultivo_;
		int dim_ancho_;
		int dim_largo_;
		void borrarCultivo(void);
		int npos(int, int)const;
		void mapaDefecto(void);

	public:
		medio_cultivo_t(int, int);
		~medio_cultivo_t();
		celula_base_t* obtCelula(int, int)const;
		std::ostream& mostrarCultivo(std::ostream&);
		std::ostream& mostrarVecinos(std::ostream&);
		void explorarCultivo(void);
		void ponCelula(int, int);
		void actualizarCultivo(void);
		int obtDim_ancho(void)const;
		int obtDim_largo(void)const;
		bool comprobarEstado(int, int)const;
		bool borde(int, int)const;
};

int medio_cultivo_t::npos(int i, int j)const{
	return (i - 1) * dim_largo_ + j - 1;
}

bool medio_cultivo_t::borde(int i, int j)const{
	return (i < 1 || j < 1 || i > dim_ancho_ || j > dim_largo_);
}

int medio_cultivo_t::obtDim_ancho(void)const{
	return dim_ancho_;
}

int medio_cultivo_t::obtDim_largo(void)const{
	return dim_largo_;
}

celula_base_t* medio_cultivo_t::obtCelula(int i, int j)const{
	return cultivo_[npos(i, j)];
}