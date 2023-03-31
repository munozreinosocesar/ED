#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

#include "../include/quienesquien.h"

using namespace std;

int main(int argc, char * argv[]){

	bool jugar = false;
	bool limpiar = false;
	QuienEsQuien quienEsQuien;
	char menu;
	string nombre;
	
	
	if(argc == 2){
		string parametroAleatorio = "aleatorio";
		if(argv[1]== parametroAleatorio){
			cout << "Creando un QuienEsQuien aleatorio"<< endl;
			int numero_de_personajes;
		   	int numero_de_atributos;

		   	do{
		   		cout << "¿Número de personajes? ";
		   		cin >>numero_de_personajes;
		   	}while(numero_de_personajes<=0);

		   	quienEsQuien.tablero_aleatorio(numero_de_personajes);

		   	string nombreFicheroSalida = string("datos/tablero") +
		   		"-num-per="+to_string(numero_de_personajes);

		   	cout << "Guardando tablero aleatorio en "<<nombreFicheroSalida<<endl;
		   	ofstream archivoDeSalida(nombreFicheroSalida);
		   	archivoDeSalida << quienEsQuien;

		   	cout << "Guardado"<<endl;
		   	return 0;

		}else{
			cout << "Cargando fichero para jugar'"<< argv[1] <<"'"<< endl;
			ifstream f (argv[1]);
			if (!f){
				cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
				return 1;
			}
			f>> quienEsQuien;
			jugar = true;
		}
	} else if(argc == 3 ){
		string parametroLimpiar = "limpiar";
		if(parametroLimpiar== argv[2]){
			cout << "Cargando fichero para limpiar (sin jugar) '"<< argv[1] <<"'"<< endl;
			ifstream f (argv[1]);
			if (!f){
				cout<<"No puedo abrir el fichero "<<argv[1]<<endl;
				return 1;
			}
			f>> quienEsQuien;
			jugar = false;
		} else{
			cout << "Modo '"<<argv[2]<<"' no reconocido"<<endl;
			return 1;
		}

	} else {
		cout << "No se reconocen los argumentos. Ejemplos de uso:" << endl;
		cout << "\tJugar:               ./bin/quienesquien RUTA_FICHERO"<< endl;
		cout << "\tLimpiar sin jugar:   ./bin/quienesquien RUTA_FICHERO limpiar"<< endl;
		cout << "\tGenerar aleatorio:   ./bin/quienesquien aleatorio"<< endl;
		return 1;
	}


	quienEsQuien.mostrar_estructuras_leidas();
	quienEsQuien.usar_arbol(quienEsQuien.crear_arbol());
	
	cout << "=========== Arbol en crudo ===========" << endl;
	quienEsQuien.escribir_arbol_completo();
	cout << "Profundidad promedio de las hojas del arbol: ";
	cout << quienEsQuien.profundidad_promedio_hojas() << endl; 
	cout << "======================================" << endl << endl << endl;

	do{
		cout<<"\nElija una opción: "<<endl;
		cout<<"\t-Insertar un personaje.(I)"<<endl;
		cout<<"\t-Eliminar un personaje.(E)"<<endl;
		cout<<"\t-Ninguna opción de las anteriores.(N)"<<endl;
	
		cin>>menu;

		if(menu=='I'){
			cout << "Introduzca el nombre del personaje: ";
			cin >> nombre;
			cout << "Introduzca sus características (según el orden de los atributos del esquema): ";
			vector<bool> caract;
			bool c;
			for(int i=0; i<quienEsQuien.num_atributos(); i++){
				cin>>c;
				caract.insert(caract.begin()+i,c); 
			}
			quienEsQuien.aniade_personaje(nombre, caract);
		}
		if(menu=='E'){
			cout << "Introduzca el nombre del personaje: ";
			cin >> nombre;
			quienEsQuien.elimina_personaje(nombre);
		}
	}while(menu!='N');
  
	
	quienEsQuien.eliminar_nodos_redundantes();
	
	cout << "=========== Arbol ===================="<<endl;
	quienEsQuien.escribir_arbol_completo();
	cout << "Profundidad promedio de las hojas del arbol: ";
	cout << quienEsQuien.profundidad_promedio_hojas()<<endl; 
	cout << "======================================" << endl << endl << endl;

	quienEsQuien.usar_arbol(quienEsQuien.crear_arbol_balanceado());

	cout << "=========== Arbol balanceado ===================="<<endl;
	quienEsQuien.escribir_arbol_completo();
	cout << "Profundidad promedio de las hojas del arbol: ";
	cout << quienEsQuien.profundidad_promedio_hojas()<<endl; 
	cout << "======================================" << endl << endl << endl;

	if(jugar){
		quienEsQuien.iniciar_juego();
		do{
			cout<<"¿Quiere volver a jugar? Si(s)/No(n)"<<endl;
			cin>>menu;
			
			if(menu=='s')
				quienEsQuien.iniciar_juego();

		}while(menu!='n');
	}
	return 0;
}
