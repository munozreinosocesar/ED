#include "../include/quienesquien.h"
#include <sstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <algorithm>



QuienEsQuien::QuienEsQuien(){
	personajes = vector<string> ();
	atributos = vector<string> ();
	tablero = vector<vector<bool>> ();
	arbol = bintree<Pregunta> ();
	jugada_actual = bintree<Pregunta>::node ();
}
QuienEsQuien::QuienEsQuien(const QuienEsQuien &quienEsQuien){
	personajes = quienEsQuien.personajes;
	atributos = quienEsQuien.atributos;
	tablero = quienEsQuien.tablero;
	arbol = quienEsQuien.arbol;
	jugada_actual = quienEsQuien.jugada_actual;
}
QuienEsQuien& QuienEsQuien::operator= (const QuienEsQuien &quienEsQuien){
	if(this!=&quienEsQuien){
		personajes = quienEsQuien.personajes;
		atributos = quienEsQuien.atributos;
		tablero = quienEsQuien.tablero;
		arbol = quienEsQuien.arbol;
		jugada_actual = quienEsQuien.jugada_actual;
	}
	return *this;
}
QuienEsQuien::~QuienEsQuien(){
	this->limpiar();
}

int QuienEsQuien::num_atributos() const{
	return atributos.size();
}

void QuienEsQuien::limpiar(){
	personajes.clear();
	atributos.clear();
	tablero.clear();
	arbol.clear();
	jugada_actual.remove();
}

template <typename T>
std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
  if ( !v.empty() ) {
    out << '[';
    std::copy (v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
    out << "\b\b]";
  }
  return out;
}

void QuienEsQuien::mostrar_estructuras_leidas(){
	cout << "personajes: "<< (this->personajes) << endl;
	cout << "atributos:  "<< (this->atributos)  << endl;
	cout << "tablero:    "<< endl;


	// Escribe la cabecera del tablero
	for(vector<string>::iterator it_atributos = this->atributos.begin();
		it_atributos != this->atributos.end();
		it_atributos++){
		cout << *it_atributos << "\t";
	}
	cout <<endl;

	int indice_personaje = 0;
	for(vector<vector<bool>>::iterator it_tablero_atributos = tablero.begin();
		it_tablero_atributos!= tablero.end();
		it_tablero_atributos++){

		string personaje = this->personajes[indice_personaje];
		int indice_atributo = 0;
		for(vector<bool>::iterator it_atributos_personaje = (*it_tablero_atributos).begin();
			it_atributos_personaje != (*it_tablero_atributos).end();
			it_atributos_personaje++){

			cout << *it_atributos_personaje<<"\t";

			indice_atributo++;
		}

		cout << personaje << endl;

		indice_personaje++;
	}

}

/**
  * @brief Devuelve una copia de la cadena original sin las subcadenas no deseadas.
  * 
  * @param cadena_original Cadena de la que se eliminan las subcadenas no deseadas.
  * @param cadena_a_eliminar Subcadena que se busca y se elimina.
  *
  * @return Copia de la cadena original sin las subcadenas no deseadas.
  */
string limpiar_string(string cadena_original,string cadena_a_eliminar){
	string linea(cadena_original);

	while(linea.find_first_of(cadena_a_eliminar) != std::string::npos){
		linea.erase(linea.find_first_of(cadena_a_eliminar),cadena_a_eliminar.length());
	}

	return linea;
}

istream& operator >> (istream& is, QuienEsQuien &quienEsQuien) {
	quienEsQuien.limpiar();
	
  	if(is.good()){
  		string linea;
    	getline(is, linea, '\n');

    	linea = limpiar_string(linea,"\r");
    	
    	while(linea.find('\t') != string::npos ){
    		string atributo = linea.substr(0,linea.find('\t'));
			quienEsQuien.atributos.push_back(atributo);
    		linea = linea.erase(0,linea.find('\t')+1);
    	}

		assert(linea ==  "Nombre personaje");
	}
	
    while( is.good() ) {
    	string linea;
    	getline(is, linea, '\n');
    	linea = limpiar_string(linea,"\r");

    	//Si la linea contiene algo extrae el personaje. Si no lo es, la ignora.
    	if(linea != ""){;
	    	vector<bool> atributos_personaje;
	    	
	    	int indice_atributo=0;
	    	while(linea.find('\t') != string::npos){
	    		string valor = linea.substr(0,linea.find('\t'));
	    		
	    		assert(valor == "0" || valor == "1");
	    		
	    		bool valor_atributo = valor == "1";
	    		
	    		atributos_personaje.push_back(valor_atributo);
				
	    		linea = linea.erase(0,linea.find('\t')+1);
	    		indice_atributo++;
			}
			
			string nombre_personaje = linea;
	    	
	    	quienEsQuien.personajes.push_back(nombre_personaje);
	    	quienEsQuien.tablero.push_back(atributos_personaje);
	    }
  	}
  	
	return is;
}

ostream& operator << (ostream& os, const QuienEsQuien &quienEsQuien){

	//Escribimos la cabecera, que contiene los atributos y al final una columna para el nombre
	for(vector<string>::const_iterator it_atributos = quienEsQuien.atributos.begin();
		it_atributos != quienEsQuien.atributos.end();
		it_atributos++){

		os  << *it_atributos << "\t";
	}
	os << "Nombre personaje" << endl;

	//Rellenamos con ceros y unos cada línea y al final ponemos el nombre del personaje.
	for(int indice_personaje=0;indice_personaje<quienEsQuien.personajes.size();indice_personaje++){
		for(int indice_atributo=0;indice_atributo<quienEsQuien.atributos.size();indice_atributo++){

			os  << quienEsQuien.tablero[indice_personaje][indice_atributo] << "\t";
		}
		os  << quienEsQuien.personajes[indice_personaje] << endl;
	}

	return os;
}

/**
  * @brief Convierte un número a un vector de bool que corresponde 
  *        con su representación en binario con un numero de digitos
  *        fijo.
  *
  * @param n Número a convertir en binario.
  * @param digitos Número de dígitos de la representación binaria.
  *
  * @return Vector de booleanos con la representación en binario de @e n 
  *      con el número de elementos especificado por @e digitos. 

  */
vector<bool> convertir_a_vector_bool(int n, int digitos) {
  vector<bool> ret;
  while(n) {
    if (n&1){
      ret.push_back(true);
    } else{
      ret.push_back(false);
    }
    n>>=1;  
  }

  while(ret.size()<digitos){
  	ret.push_back(false);
  }

  reverse(ret.begin(),ret.end());
  return ret;
}

bintree<Pregunta> QuienEsQuien::crear_arbol()
{
	Pregunta raiz(atributos[0], personajes.size());
	bintree<Pregunta> arbol(raiz), a(raiz);
	int contador, i;
	vector<bool>::iterator iterat;
	vector<vector<bool> >::iterator iter;
	bintree<Pregunta>::node aux, nodos[personajes.size()];
	
	for(int j=0; j < personajes.size(); j++){
		aux=arbol.root();
		i=1;
		for(iterat=tablero[j].begin(); iterat!=tablero[j].end(); ++iterat){
			contador=1;
			if(*iterat==true){
				if(!aux.left().null()){
					contador+=(*aux.left()).obtener_num_personajes();
					Pregunta nueva(atributos[i],contador);
					*aux.left()=nueva;
				}
				else{
					Pregunta nueva(atributos[i],1);
					arbol.insert_left(aux,nueva);
				}
	
				
				aux=aux.left();
			}
			else{
				if(!aux.right().null()){
					contador+=(*aux.right()).obtener_num_personajes();
					Pregunta nueva(atributos[i],contador);
					*aux.right()=nueva;
				}
				else{
					Pregunta nueva(atributos[i],1);
					arbol.insert_right(aux,nueva);	
				}
				aux=aux.right();
			}
			i++;
		}
		Pregunta nombre(personajes[j],1);
		*aux=nombre;
		nodos[j]=aux;
	}		
	
	for(int j=0; j < personajes.size(); j++){
		*a.root()=*nodos[j];
		while((*nodos[j].parent()).es_personaje())
			nodos[j]=nodos[j].parent();
		arbol.replace_subtree(nodos[j], a, a.root());
	}

	return arbol;
}

void QuienEsQuien::usar_arbol(bintree<Pregunta> arbol_nuevo){
	arbol = arbol_nuevo;
}

void QuienEsQuien::iniciar_juego(){
	bintree<Pregunta>::node nodo;
	if(!arbol.empty()){
		jugada_actual = arbol.root();
		bool ganado=false;
		string respuesta;
		set<string>::iterator it;	
		set<string> levantados;
		
		while(!ganado){
			if((*jugada_actual).es_pregunta()){
				cout<< "Quedan levantados los siguientes personajes: " << endl;
				levantados=informacion_jugada(jugada_actual);
				for(it=levantados.begin(); it!=levantados.end(); ++it) 
					cout<<"\t- " << *it <<endl;
			
				cout<<endl;
				cout << (*jugada_actual) << endl;
				cout << "(Escribe si/no)" << endl;
				do{
					cin >> respuesta;
				}while(respuesta!="si" && respuesta!="no");
					
				if(respuesta == "si"){
					jugada_actual = jugada_actual.left();
				}
				else{
					jugada_actual = jugada_actual.right();
				}

				if((*jugada_actual).es_pregunta())
					preguntas_formuladas(jugada_actual);
					
			}
			else{
				ganado = true;
				preguntas_formuladas(jugada_actual);
			}
		}
	}
	jugada_actual=nodo;
}

set<string> QuienEsQuien::informacion_jugada(bintree<Pregunta>::node jugada_actual){
	set<string> personajes_levantados;
	bintree<Pregunta> arbol2;
	arbol2.assign_subtree(arbol,jugada_actual);
	bintree<Pregunta>::postorder_iterator it=arbol2.begin_postorder();
	while(it!=arbol2.end_postorder()){
		if((*it).es_personaje()){
			personajes_levantados.insert((*it).obtener_personaje());
		}
		++it;
	}

	return personajes_levantados;
}

void escribir_esquema_arbol(ostream& ss,
					  const bintree<Pregunta>& a, 
		    		  bintree<Pregunta>::node n,
					  string& pre){
	if (n.null()){
	    ss << pre << "-- x" << endl;
	} else {
	    ss << pre << "-- " << (*n) << endl;
	    if ( !n.right().null() || !n.left().null()) {// Si no es una hoja
			pre += "   |";
	    	escribir_esquema_arbol(ss,a, n.right(), pre);
	     	pre.replace(pre.size()-4, 4, "    ");
	      	escribir_esquema_arbol (ss,a, n.left(), pre);
	      	pre.erase(pre.size()-4, 4);    
	    }
  	}
}

void QuienEsQuien::escribir_arbol_completo() const{
	string pre = "";
	escribir_esquema_arbol(cout,this->arbol,this->arbol.root(),pre);
}

void QuienEsQuien::eliminar_nodos_redundantes(){
	bintree<Pregunta>::postorder_iterator it, it2=arbol.begin_postorder();
	vector<bool>::iterator iter;
	int i=1, j=0, contador=0;
	bintree<Pregunta>::node aux;
	vector<bintree<Pregunta>::node> nodos;
	bintree<Pregunta> a(arbol);
	bool pos;

	for(it=++arbol.begin_postorder(); it!=arbol.end_postorder(); ++it){
		if((*it).obtener_num_personajes()==(*it2).obtener_num_personajes() && (*it).es_pregunta())
			contador++;
		++it2;
	}

	if(!arbol.root().left().null() && !arbol.root().right().null())
		nodos.insert(nodos.end(), arbol.root());
	else{
		while(arbol.root().left().null() || arbol.root().right().null()){
			if(arbol.root().left().null())
				arbol.replace_subtree(arbol.root(), a, a.root().right());
			else
				arbol.replace_subtree(arbol.root(), a, a.root().left());
			a=arbol;
			contador--;
		}
		nodos.insert(nodos.end(), arbol.root());
	}

	nodos.insert(nodos.end(), arbol.root().left());
	nodos.insert(nodos.end(), arbol.root().right());

	while(j<contador){
		if((!nodos[i].left().null() && !nodos[i].right().null()) && (*nodos[i]).es_pregunta()){
			nodos.insert(nodos.end(), nodos[i].left());
			nodos.insert(nodos.end(), nodos[i].right());
		}
		else{
			if((*nodos[i]).es_pregunta()){
				aux=nodos[i].parent();
				if(nodos[i].left().null())
					arbol.prune_right(nodos[i],a);
				else
					arbol.prune_left(nodos[i],a);

				if(nodos[i]==aux.left())
					pos=true;
				else
					pos=false;
				
				arbol.replace_subtree(nodos[i], a, a.root());
				
				if(pos)
					nodos.insert(nodos.end(), aux.left());
				else
					nodos.insert(nodos.end(), aux.right());
				j++;
			}
		}
		i++;	
	}
}

float QuienEsQuien::profundidad_promedio_hojas(){
	bintree<Pregunta>:: node aux;
	int profundidad_personaje;
	int suma_profundidades = 0;
	float media;
	int j;

	for(int i=0; i<personajes.size(); i++){
		aux=arbol.root();
		profundidad_personaje=1;
		while((*aux).es_pregunta()){
			for(j=0; atributos[j]!=(*aux).obtener_pregunta(); j++);
			if(tablero[i][j])
				aux=aux.left();
			else
				aux=aux.right();
			profundidad_personaje++;
		}		
		suma_profundidades+=profundidad_personaje;
		
	}	

	media=suma_profundidades/(personajes.size()*1.0);

	return media;
}

/**
 * @brief Genera numero enteros positivos aleatorios en el rango [min,max).
**/

int generaEntero(int min, int max){
    int tam= max -min;
    return ((rand()%tam)+min);
}

void QuienEsQuien::tablero_aleatorio(int numero_de_personajes){
	srand(0);

	this->limpiar();

	float log_2_numero_de_personajes = log(numero_de_personajes)/log(2);

	int numero_de_atributos = ceil(log_2_numero_de_personajes);

	cout << "Petición para generar "<< numero_de_personajes<<" personajes ";
	cout << "con "<<numero_de_atributos<< " atributos"<<endl;
	cout << "Paso 1: generar "<< pow(2,numero_de_atributos) << " personajes"<<endl;

	//Fase 1: completar el tablero con todos los personajes posibles
	//Completo el tablero y los nombres de personajes a la vez
	for(int indice_personaje=0;indice_personaje< pow(2,numero_de_atributos);indice_personaje++){
		vector<bool> atributos_personaje = convertir_a_vector_bool(indice_personaje,numero_de_atributos);
		string nombre_personaje = "Personaje_"+to_string(indice_personaje);

		this->personajes.push_back(nombre_personaje);
		this->tablero.push_back(atributos_personaje);
	}

	// Completo los nombres de los atributos.
	for(int indice_atributo=0;indice_atributo<numero_de_atributos;indice_atributo++){
		string nombre_atributo = "Atributo_"+to_string(indice_atributo);
		this->atributos.push_back(nombre_atributo);
	}

	cout << "Paso 2: eliminar "<< (pow(2,numero_de_atributos)-numero_de_personajes) << " personajes"<<endl;
	//Fase 2. Borrar personajes aleatoriamente hasta que quedan solo los que hemos pedido.
	while(personajes.size()>numero_de_personajes){
		int personaje_a_eliminar = generaEntero(0,personajes.size());

		personajes.erase(personajes.begin()+personaje_a_eliminar);
		tablero.erase(tablero.begin()+personaje_a_eliminar);
	}
}

void QuienEsQuien::preguntas_formuladas(bintree<Pregunta>::node jugada){
	bintree<Pregunta>::postorder_iterator it;
	bintree<Pregunta>::node aux=jugada;
	int contador=1;

	while(aux!=arbol.root()){
		aux=aux.parent();
		contador++;
	}
	
	bintree<Pregunta>::node nodos[contador];

	aux=jugada;

	nodos[0]=aux;

	for(int i=1; i<contador; i++)
		nodos[i]=nodos[i-1].parent();
	
	cout<<"\nEl personaje oculto tiene las siguientes características: "<<endl;

	for(int i=contador-1; i>0; --i){
		cout<<(*nodos[i]).obtener_pregunta() << " - ";
		if(nodos[i].left()==nodos[i-1])
			cout<<"si"<<endl;
		else
			cout<<"no"<<endl;
		
	}

	if((*jugada).es_pregunta())
		cout << "pero aún no se cual es."<<endl<<endl;
	else
		cout << "y es " << (*jugada).obtener_personaje()<<"."<<endl<<endl;
}

void QuienEsQuien::aniade_personaje(string nombre, vector<bool> caracteristicas){
	vector<string>::iterator it;
	int i, j, k=0;
	bintree<Pregunta>::node nodo=arbol.root();
	bintree<Pregunta> borrar;
	bool fin=false;

	for(it=personajes.begin(); it!=personajes.end() && *it!=nombre; ++it);

	if(it==personajes.end()){
		tablero.insert(tablero.end(),caracteristicas);
		personajes.insert(personajes.end(), nombre);
		i=personajes.size()-1;

		while((*nodo).es_pregunta()){
			j=0;
			for(it=atributos.begin(); *it!=(*nodo).obtener_pregunta() && it!=atributos.end(); ++it)
				j++;
			Pregunta nueva((*nodo).obtener_pregunta(), (*nodo).obtener_num_personajes()+1);
			*nodo=nueva;
			if(tablero[i][j]){
				if(!nodo.left().null())
					nodo=nodo.left();
				else{
					Pregunta person(nombre,1);
					arbol.insert_left(nodo, person);
					nodo=nodo.left();
					fin=true;
				}
			}
			else{
				if(!nodo.right().null())
					nodo=nodo.right();
				else{
					Pregunta person(nombre,1);
					arbol.insert_right(nodo, person);
					nodo=nodo.right();
					fin=true;
				}
			}
		}
		
		if(!fin){
			for(it=personajes.begin(); *it!=(*nodo).obtener_personaje(); ++it)
				k++;
			
			while(tablero[k][j]==tablero[i][j] && j<atributos.size())
				j++;	

			Pregunta nueva(atributos[j],2);
			bintree <Pregunta> arb(nueva);

			if(tablero[k][j]){
				Pregunta nueva(personajes[k],1);
				arb.insert_left(arb.root(), nueva);
				Pregunta otra(personajes[i],1);
				arb.insert_right(arb.root(), otra);
			}
			else{
				Pregunta nueva(personajes[k],1);
				arb.insert_right(arb.root(), nueva);
				Pregunta otra(personajes[i],1);
				arb.insert_left(arb.root(), otra);
			}
			arbol.replace_subtree(nodo, arb, arb.root());
		}			
		cout<<"\n"<< nombre << " ha sido añadido con exito." << endl;		
	}	
	else
		cout << nombre << " ya se encuentra en el juego." << endl;
}

void QuienEsQuien::elimina_personaje(string nombre){
	vector<string>::iterator it;
	vector<vector<bool> >::iterator it2;
	int i=0, j;	
	bintree<Pregunta>::node nodo=arbol.root(), aux;
	bintree<Pregunta> borrar;	
	
	for(it=personajes.begin(); *it!=nombre && it!=personajes.end(); ++it)
		i++;

	for(it2=tablero.begin(); *it2!=tablero[i]; ++it2);

	if(it!=personajes.end()){
		personajes.erase(it);
		while((*nodo).es_pregunta()){
			j=0;
			for(it=atributos.begin(); *it!=(*nodo).obtener_pregunta() && it!=atributos.end(); ++it)
				j++;

			Pregunta nueva((*nodo).obtener_pregunta(), (*nodo).obtener_num_personajes()-1);
			*nodo=nueva;
			if(tablero[i][j])
				nodo=nodo.left();
			else
				nodo=nodo.right();
		}

		aux=nodo.parent();

		if(nodo==nodo.parent().left()){
			arbol.prune_left(nodo.parent(),borrar);
			bintree<Pregunta> borr(*aux.right());
		}
		else{
			arbol.prune_right(nodo.parent(),borrar);
			bintree<Pregunta> borrar(*aux.left());
		}
		
		if((*aux).es_personaje()){
	        	while(!(*aux.parent()).es_pregunta())
				aux=aux.parent();
			arbol.replace_subtree(aux, borrar, borrar.root());
		}	
		tablero.erase(it2);	
		cout<<"\n"<< nombre << " ha sido eliminado con exito." << endl;			
	}	
	else
		cout << nombre << " no se encuentra en el juego." << endl;
}

bintree<Pregunta> QuienEsQuien::crear_arbol_balanceado()
{
	vector<vector<bool> > tab(tablero);
	pair<int,int> distrib, aux(0,0);
	Pregunta nueva("", personajes.size());
	bintree<Pregunta> arb(nueva);
	int i=0, contador=0, k;
	*arb.root()=mejor_pregunta(arb.root(), arb, distrib.first, distrib.second, tab);
	tab=tablero;
	vector<bintree<Pregunta>::node > nodos;
	nodos.insert(nodos.begin(),arb.root());
	vector<pair<int,int> > dist;
	dist.insert(dist.begin(),distrib);
	bintree<Pregunta>::node vacio, auxiliar;
	
	while(contador<personajes.size()){
		if(nodos[i]!=vacio){
			Pregunta aux_izq("", dist[i].first);
			arb.insert_left(nodos[i],aux_izq);
			dist.insert(dist.end(),aux);
			nodos.insert(nodos.end(),vacio);
			if(dist[i].first!=1){
				*nodos[i].left()=mejor_pregunta(nodos[i].left(), arb, dist[2*i+1].first, dist[2*i+1].second,tab);
				nodos[2*i+1]=nodos[i].left();
				tab=tablero;
			}
			else
				contador++;
			Pregunta aux_der("", dist[i].second);
			arb.insert_right(nodos[i],aux_der);
			dist.insert(dist.end(),aux);
			nodos.insert(nodos.end(),vacio);
			if(dist[i].second!=1){
				*nodos[i].right()=mejor_pregunta(nodos[i].right(), arb, dist[2*i+2].first, dist[2*i+2].second,tab);
				nodos[2*i+2]=nodos[i].right();
				tab=tablero;
			}
			else
				contador++;
		}
		else{
			dist.insert(dist.end(),aux);
			dist.insert(dist.end(),aux);
			nodos.insert(nodos.end(),vacio);
			nodos.insert(nodos.end(),vacio);
		}
		i++;
	}	

	for(int j=0; j<personajes.size(); j++){
		auxiliar=arb.root();
		while((*auxiliar).es_pregunta()){
			for(k=0; atributos[k]!=(*auxiliar).obtener_pregunta(); k++);
				if(tablero[j][k])
					auxiliar=auxiliar.left();
				else
					auxiliar=auxiliar.right();
		}
		Pregunta nueva(personajes[j], 1);
		*auxiliar=nueva;
	}
	
	return arb;
}

Pregunta QuienEsQuien::mejor_pregunta(const bintree<Pregunta>::node &nodo, const bintree<Pregunta> &arb, int &izq, int &der, vector<vector<bool> > &tab){
	bintree<Pregunta>::node aux;
	vector<string>::iterator it;
	vector<string> atrib(atributos);
	bool encontrado;
	vector<bool> localizar; 
	int i, person=personajes.size(), t;
	
	aux=nodo;

	while(aux!=arb.root()){
		t=0;
		if(aux==aux.parent().left())
			localizar.insert(localizar.begin(), 1);
		else
			localizar.insert(localizar.begin(), 0);

		aux=aux.parent();
		encontrado=false;
		for(i=atrib.size()-1; i>=0  && !encontrado; i--)
			if((*aux).obtener_pregunta()==atrib[i]){
				for(int j=person-1; j>=0; j--){
					if(localizar[0]!=tab[j][i]){
						tab.erase(tab.begin()+j);
						t++;
					}
					else
						tab[j].erase(tab[j].begin()+i);
				}
				person-=t;
				encontrado=true;
				atrib.erase(atrib.begin()+i);
			}
	}

	int vect[atrib.size()]={0}, temp;
	string cambio;

	for(int i=0; i<atrib.size(); i++)
		for(int j=0; j<tab.size(); j++)
			if(tab[j][i])
				vect[i]++;
	
	for(int i=1; i<atrib.size(); i++)
		for(int j=0 ; j<atrib.size() - i; j++)
			if (vect[j] > vect[j+1]){
				temp = vect[j];
				cambio = atrib[j];
				vect[j] = vect[j+1];
				atrib[j] = atrib[j+1];	
				vect[j+1] = temp;
				atrib[j+1] = cambio;
			}
	int valor=atrib.size()/2;

	while(vect[valor]==vect[atrib.size()-1] && valor>0)
		valor--;

	while(vect[valor]==0 && valor<atrib.size())
		valor++;
			
	Pregunta mejor(atrib[valor], (*nodo).obtener_num_personajes());
	izq=vect[valor];
	der=(*nodo).obtener_num_personajes()-vect[valor];

	return mejor;
}
