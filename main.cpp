#include <iostream>
#include <set>
#include <cstdio>

class celula_base_t;

class medio_cultivo_t{
	private:
		enum celulaTipo{BASE, A, B, C, D, TOTAL};
		celula_base_t** cultivo_;
		int dim_ancho_;
		int dim_largo_;
		void borrarCultivo(void);
		int npos(int, int)const;

	public:
		medio_cultivo_t(int, int);
		~medio_cultivo_t();
		celula_base_t* obtCelula(int, int)const;
		std::ostream& mostrarCultivo(std::ostream&);
		void explorarCultivo(void);
		void ponCelula(int, int, int);
		void actualizarCultivo(void);
		int obtDim_ancho(void)const;
		int obtDim_largo(void)const;
};

int medio_cultivo_t::npos(int i, int j)const{
	return (i*dim_ancho_+1)+j-1;
}

int medio_cultivo_t::obtDim_ancho(void)const{
	return dim_ancho_;
}

int medio_cultivo_t::obtDim_largo(void)const{
	return dim_largo_;
}

medio_cultivo_t::~medio_cultivo_t(){
	borrarCultivo();
	delete [] cultivo_;
}

celula_base_t* medio_cultivo_t::obtCelula(int i, int j)const{
	return cultivo_[npos(i, j)];
}

class celula_base_t{
	private:
		int sigEstado;
		std::set<celula_base_t*> cjtoCelVecinas_;
	protected:
		int vecinos_;
		int* natalidad_;
		int* resistencia_;
	public:
		celula_base_t(){}
		virtual ~celula_base_t(){}
		virtual void contarVecinos(const medio_cultivo_t&, int, int);
		virtual const char verCelula(void);
		virtual bool obtEstado(void);
		virtual int actualizarEstado(void);	//devuelve a qué célula tiene que convertirse
		virtual bool comprobarNatalidad(int);
};

class celula_A_t: public celula_base_t{
	private:
		const int nNat_ = 1;
		const int nRes_ = 2;
	protected:
		int vecinos_;
	public:
		celula_A_t();
		virtual ~celula_A_t();
		void contarVecinos(const medio_cultivo_t&, int, int);
		const char verCelula(void);
		bool obtEstado(void);
		bool comprobarNatalidad(int);
		int actualizarEstado(void);	//devuelve 0 si no muere, 1 si muere
};

void medio_cultivo_t::explorarCultivo(void){
	for(int i=0; i<dim_ancho_; i++)
		for(int j=0; j<dim_largo_; j++)
			cultivo_[npos(i, j)]->contarVecinos(*this, i, j);
}

celula_A_t::celula_A_t(){
	std::cout << "nueva A" << std::endl;
	natalidad_ = new int[nNat_];
	resistencia_ = new int[nRes_];

	natalidad_[0] = 3;
	resistencia_[1] = 2;
	resistencia_[2] = 3;
}

celula_A_t::~celula_A_t(){
	std::cout << "borro A" << std::endl;
	delete natalidad_;
	delete resistencia_;
}

medio_cultivo_t::medio_cultivo_t(int ancho, int largo):
dim_ancho_(ancho),
dim_largo_(largo)
{
	cultivo_ = new celula_base_t*[dim_ancho_*dim_largo_];

	for(int n=0; n<dim_ancho_*dim_largo_; n++)
		cultivo_[n] = new celula_base_t;
}

void medio_cultivo_t::borrarCultivo(void){
	for(int n=0; n<dim_ancho_*dim_largo_; n++)
		delete cultivo_[n];
}

const char celula_base_t::verCelula(void){
	return 'M';
}

const char celula_A_t::verCelula(void){
	return 'A';
}

bool celula_base_t::obtEstado(void){
	return false;
}

bool celula_A_t::obtEstado(void){
	return true;
}

void celula_base_t::contarVecinos(const medio_cultivo_t& medio, int i, int j){
	vecinos_ = 0;

	for(int si = i-1; si<=i; si++)
    	for(int sj = j-1; sj<=j; sj++){
			if(si>=0 && sj>=0 && si<medio.obtDim_ancho() && sj<medio.obtDim_largo())	//si no está en el borde
				if(medio.obtCelula(si, sj)->obtEstado())	// si está vivo
					switch(medio.obtCelula(si, sj)->verCelula()){
						case 'A':
							vecinos_++;
							cjtoCelVecinas_.insert(medio.obtCelula(si, sj));
							break;
							/*
						case 'B':
							break;
						case 'C':
							break;
							*/
						default:;
					}
		}
}

void celula_A_t::contarVecinos(const medio_cultivo_t& medio, int i, int j){
	vecinos_ = 0;
	for(int si = i-1; si<=i; si++)
    	for(int sj = j-1; sj<=j; sj++)
			if(si>=0 && sj>=0 && si<medio.obtDim_ancho() && sj<medio.obtDim_largo())	//si no está en el borde
				if(medio.obtCelula(si, sj)->verCelula() == 'A')
					vecinos_++;

	if(vecinos_>0)
		vecinos_--;

	std::cout << "vecinos de A: " << vecinos_ << std::endl;
}

int celula_base_t::actualizarEstado(void){
		for(std::set<celula_base_t*>::iterator it=cjtoCelVecinas_.begin(); it!=cjtoCelVecinas_.end(); it++){
			switch((*it)->verCelula()){
				case 'A':
					if((*it)->comprobarNatalidad(vecinos_))
						return 1;
					break;
					/*
				case 'B':
					break;
					
				case 'C':
					break;
					*/
			}
		}
	
	cjtoCelVecinas_.clear();
	return 0;
}

int celula_A_t::actualizarEstado(void){
	for(int n=0; n<nRes_; n++)
		if(resistencia_[n] == vecinos_)
			return 0;

	return 1;
}

void medio_cultivo_t::actualizarCultivo(void){
	int siguienteCelula_;
	for(int i=0; i<dim_ancho_; i++)
		for(int j=0; j<dim_largo_; j++){
			if(cultivo_[npos(i, j)]->actualizarEstado()>0){	// si es 0 se mantenía el estado, si >0 se actualiza
				siguienteCelula_ = cultivo_[npos(i, j)]->actualizarEstado();
				delete cultivo_[npos(i, j)];
				std::cout << "borré " << cultivo_[npos(i, j)]->verCelula() << std::endl;
				switch(siguienteCelula_){
					case A:	//célula A
						cultivo_[npos(i, j)] = new celula_A_t;
						break;
				}
			}else if(cultivo_[npos(i, j)]->verCelula() == 'A' && cultivo_[npos(i, j)]->obtEstado()>0){
				delete cultivo_[npos(i, j)];
				cultivo_[npos(i, j)] = new celula_base_t;
			}
		}
}

bool celula_base_t::comprobarNatalidad(int vecinos){	//en este caso es indiferente el resultado de la natalidad
	return false;
}

bool celula_A_t::comprobarNatalidad(int vecinos){
	for(int n=0; n<nNat_; n++)
		if(vecinos_ == natalidad_[n])
			return true;
	
	return false;
}

void medio_cultivo_t::ponCelula(int tipo, int i, int j){
	i--;
	j--;
	delete cultivo_[npos(i, j)];
	cultivo_[npos(i, j)] = new celula_A_t;
}

std::ostream& medio_cultivo_t::mostrarCultivo(std::ostream& os){
	for(int i=0; i<dim_ancho_; i++){
		os << std::endl;
		for(int j=0; j<dim_largo_; j++)
			os << cultivo_[npos(i, j)]->verCelula();
	}

		os  << std::endl;
	
	return os;
}

int main(void){
	medio_cultivo_t medio(5, 23);
	medio.mostrarCultivo(std::cout);
	std::cout << std::endl;

	medio.ponCelula(1,5,3);
		
	medio.mostrarCultivo(std::cout);
	std::cout << std::endl;
	//medio.actualizarCultivo();

	medio.explorarCultivo();
	medio.actualizarCultivo();

	medio.mostrarCultivo(std::cout);
	std::cout << std::endl;
}