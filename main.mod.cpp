#include <iostream>
#include <set>
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

medio_cultivo_t::~medio_cultivo_t(){
	borrarCultivo();
	delete [] cultivo_;
}

celula_base_t* medio_cultivo_t::obtCelula(int i, int j)const{
	return cultivo_[npos(i, j)];
}

class celula_base_t{
	private:
		std::set<celula_base_t*> cjtoCelVecinas_;
		enum celulaTipo{BASE, A, B, C, D, TOTAL};
		int natA_ = 3;
		int natB_[2] = {3, 4};
		int natC_[3] = {3, 4, 6};
	protected:
		int sigEstado_;
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
		//virtual bool comprobarNatalidad(int);
		static celula_base_t* crearCelula(int);

		/////
		virtual int numVecinos(void){ return vecinos_ ;}
};

class celula_A_t: public celula_base_t{
	private:
		const int nNat_ = 1;
		const int nRes_ = 2;
		int res_[2] =  {2, 3};
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

		////////
		int numVecinos(void){ return vecinos_ ;}
};

celula_base_t* celula_base_t::crearCelula(int especie){
	switch(especie){
		case A:
			return new celula_A_t;
			break;
			/*
		case B:
			return new celula_B_t;
			break;
			*/

	}
}

bool medio_cultivo_t::comprobarEstado(int i, int j)const{
	return cultivo_[npos(i, j)]->obtEstado();
}

void medio_cultivo_t::explorarCultivo(void){
	for(int i=1; i<=dim_ancho_; i++)
		for(int j=1; j<=dim_largo_; j++)
			cultivo_[npos(i, j)]->contarVecinos(*this, i, j);
}

celula_A_t::celula_A_t(){
	/*
	natalidad_ = new int[nNat_];
	resistencia_ = new int[nRes_];

	natalidad_[0] = 3;
	resistencia_[1] = 2;
	resistencia_[2] = 3;
	*/
}

celula_A_t::~celula_A_t(){
	/*
	delete natalidad_;
	delete resistencia_;
	*/
}

medio_cultivo_t::medio_cultivo_t(int ancho, int largo):
dim_ancho_(ancho),
dim_largo_(largo)
{
	cultivo_ = new celula_base_t*[dim_ancho_*dim_largo_];

	for(int n=0; n<dim_ancho_*dim_largo_; n++)
		cultivo_[n] = new celula_base_t;

	//mapaDefecto();
}

void medio_cultivo_t::mapaDefecto(void){
	for(int n=0; n<dim_ancho_*dim_largo_;n++)
		if(rand() % 100 < 13){
			delete cultivo_[n];
			cultivo_[n] = new celula_A_t;
		}
}

void medio_cultivo_t::borrarCultivo(void){
	for(int n=0; n<dim_ancho_*dim_largo_; n++)
		delete cultivo_[n];
}

const char celula_base_t::verCelula(void){
	return ' ';
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

	for(int si = i-1; si<=i+1; si++)
    	for(int sj = j-1; sj<=j+1; sj++)
			if(!medio.borde(si, sj))	//si no está en el borde
				if(medio.obtCelula(si, sj)->obtEstado())	// si está vivo
					vecinos_ ++;
	
	if(vecinos_ == natA_ ){
		sigEstado_ = 1;
		return;
	}
	
	for(int n=0; n<2; n++){
		if(vecinos_ == natB_[n])
			sigEstado_ = 2;
			return;
	}
	
	for(int n=0; n<3; n++){
		if(vecinos_ == natC_[n]){
			sigEstado_ = 3;
			return;
		}
	}

	return;
}

void celula_A_t::contarVecinos(const medio_cultivo_t& medio, int i, int j){
	vecinos_ = 0;
	for(int si = i-1; si<=i+1; si++)
    	for(int sj = j-1; sj<=j+1; sj++)
			if(!medio.borde(si, sj))	//si no está en el borde
				if(medio.obtCelula(si, sj)->verCelula() == 'A')
					vecinos_++;

	if(vecinos_>0)
		vecinos_--;

	for(int n=0; n<2; n++)
		if(vecinos_ == res_[n]){
			sigEstado_ = 0;
			return;
		}else{
			sigEstado_ = 1;
			return;
		}

}

int celula_base_t::actualizarEstado(void){
	return sigEstado_;
}

int celula_A_t::actualizarEstado(void){
	return sigEstado_;
}

void medio_cultivo_t::actualizarCultivo(void){
	for(int i=1; i<=dim_ancho_; i++)
		for(int j=1; j<=dim_largo_; j++){
			if(cultivo_[npos(i, j)]->actualizarEstado()>0 && cultivo_[npos(i, j)]->obtEstado()){	// si es una célula viva y su siguiente estado es '1'
				delete cultivo_[npos(i, j)];
				cultivo_[npos(i, j)] = new celula_base_t;
			}else if(cultivo_[npos(i, j)]->actualizarEstado()>0 && !cultivo_[npos(i, j)]->obtEstado()){	//si está muerta y su siguiente estado != '0''
				celula_base_t* cel_aux;
				cel_aux = cultivo_[npos(i, j)]->crearCelula(cultivo_[npos(i, j)]->actualizarEstado());
				delete cultivo_[npos(i, j)];
				cultivo_[npos(i, j)] = cel_aux;
			}
		}
}

/*
bool celula_base_t::comprobarNatalidad(int vecinos){	//en este caso es indiferente el resultado de la natalidad
	return false;
}

bool celula_A_t::comprobarNatalidad(int vecinos){
	for(int n=0; n<nNat_; n++)
		if(vecinos_ == natalidad_[n])
			return true;
	
	return false;
}
*/

void medio_cultivo_t::ponCelula(int i, int j){
	delete cultivo_[npos(i, j)];
	cultivo_[npos(i, j)] = new celula_A_t;
}

std::ostream& medio_cultivo_t::mostrarCultivo(std::ostream& os){
	os << ' ';
	for(int j=0; j<dim_largo_; j++)
		os << '#' << ' ';

	os << std::endl;
	for(int i=1; i<=dim_ancho_; i++){
		os << '#';
		for(int j=1; j<=dim_largo_; j++)
			os << cultivo_[npos(i, j)]->verCelula() << ' ';

		os << '#' << std::endl;
	}
	os << ' ';
	for(int j=0; j<dim_largo_; j++)
		os << '#' << ' ';

	return os;
}

std::ostream& medio_cultivo_t::mostrarVecinos(std::ostream& os){
	for(int j=0; j<dim_largo_+2; j++)
		os << '#';

	os << std::endl;
	for(int i=1; i<=dim_ancho_; i++){
		os << '#';
		for(int j=1; j<=dim_largo_; j++)
			os << cultivo_[npos(i, j)]->numVecinos();

		os << '#' << std::endl;
	}
	for(int j=0; j<dim_largo_+2; j++)
		os << '#';

	return os;
}

int main(void){
	medio_cultivo_t medio(40, 40);

	medio.ponCelula(20,20);
	medio.ponCelula(20,21);
	medio.ponCelula(21,20);
	medio.ponCelula(19,20);
	medio.ponCelula(20,19);


	while(getchar() != 'q' ){
	medio.explorarCultivo();
	medio.actualizarCultivo();

	medio.mostrarCultivo(std::cout);
	std::cout << std::endl;
	}
}