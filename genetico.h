#include "systemc.h"
#include "parameters.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

class genetico{
public:
	conexoes conexoes_rede[SIZE_POPULATION];
	int coreNumbers;
	int router_num;//[SIZE_POPULATION];
	ordena deadline[SIZE_POPULATION];
	ordena latencia[SIZE_POPULATION];
	string grau_latencia[SIZE_POPULATION];
	char linha[15];
	int pai[5], mae[5], sub1[5], sub2[5];

	/*Cria a primeira população de mapeamento*/
	void firstPopulation(){
		Conexoes mapTeste[(coreNumbers + (coreNumbers / 4))];
	    int v;
	    int w;
	    srand(time(NULL));
	    for(int i = 0; i < SIZE_POPULATION; i++){
	       for(int j = 0; j < (coreNumbers + (coreNumbers / 4)); j++){
         		do{
		          	v = 1+(rand()%coreNumbers);
		          	w = 1+(rand()%coreNumbers);
          		}while(v == w);
          		mapTeste[j].primeiro = v;
	            mapTeste[j].segundo = w;
	            conexoes_rede[i].push_back({mapTeste[j].primeiro, mapTeste[j].segundo});
	       }
	   }
	}

	/*Pega os valores de saída da simulação e escolhe os progenitores*/
	void fitness(){
		/*Captura dos valores de saída da simulação*/
		string simulacao = "saida_simulacao.txt";
		int j;
		string aux;
		string aux2;
		ordena aux3;
		int aux_dead, aux_lat;
		int cont = 0;
		for(int i = 0; i < SIZE_POPULATION; i++){
			ifstream leitura;
			leitura.open(std::to_string(i)+simulacao, ios_base::in);
			if(leitura.is_open()){
				leitura.getline(linha, 15);
				deadline[i].valor = atof(linha);
				//leitura.getline(linha, 15);
				while(leitura.getline(linha, 15)){
					aux = "";
					aux2 = "";
					for(j = 0; j < 15; j++){
						if(linha[j] != ' '){
							aux = aux + linha[j];
						}else{
							break;
						}
					}
					latencia[i].valor = atof(aux.c_str());
					j = j + 1;
					for(; j < 15; j++){
						if(linha[j] != ' '){
							aux2 = aux2 + linha[j];
						}else{
							break;
						}
					}

					grau_latencia[i] = aux2;
				}
			}
			leitura.close();
			if((grau_latencia[i][0] == 'p') && (grau_latencia[i][1] == 's')){
				latencia[i].valor = latencia[i].valor / 1000;
			}
		}

		//Seta os valores das posições nos vetors
		for(int i = 0; i < SIZE_POPULATION; i++){
			deadline[i].posicao = i;
			latencia[i].posicao = i;
		}

		/*Seleciona os progenitores*/
		for(int c = 0; c < SIZE_POPULATION; c++){
			for(int k = c+1; k < SIZE_POPULATION; k++){
				if(deadline[c].valor < deadline[k].valor){
					aux3 = deadline[k];
					deadline[k] = deadline[c];
					deadline[c] = aux3;
				}
			}
		}

		for(int c = 0; c < SIZE_POPULATION; c++){
			for(int k = c+1; k < SIZE_POPULATION; k++){
				if(latencia[c].valor < latencia[k].valor){
					aux3 = latencia[k];
					latencia[k] = latencia[c];
					latencia[c] = aux3;
				}
			}
		}


		for(int c = 0; c < 5; c++){
			pai[c] = deadline[c].posicao;
			mae[c] = latencia[c].posicao;
		}

		for(int c = 0; c < SIZE_POPULATION; c++){
			if(c >= 45){
				sub1[cont] = deadline[c].posicao;
				sub2[cont] = latencia[c].posicao;
				cont++;
			}
		}
	}

	void crossover(){
		srand(time(NULL));
		Conexoes filho1[5][(router_num + (router_num / 4))], filho2[5][(router_num + (router_num / 4))], conexoes_aux[5][(router_num + (router_num / 4))];
		conexoes filho3[5], filho4[5];
		
		for(int c = 0; c < 5; c++){
			for(int i = 0; i < (router_num + (router_num / 4)); i++){
				conexoes_aux[c][i].primeiro = (rand()%2);
				conexoes_aux[c][i].segundo = (rand()%2);
			}

			for(int i = 0; i < (router_num + (router_num / 4)); i++){
				filho1[c][i].primeiro = 0;
				filho1[c][i].segundo = 0;
				filho2[c][i].primeiro = 0;
				filho2[c][i].segundo = 0;
			}
		}

		/*Criação dos primeiros 5  filhos*/
		for(int c = 0; c < 5; c++){
			for(int i = 0; i < (router_num + (router_num / 4)); i++){
				if(conexoes_aux[c][i].primeiro == 1){
					filho1[c][i].primeiro = conexoes_rede[pai[c]][i].primeiro;
				}
				if(conexoes_aux[c][i].segundo == 1){
					filho1[c][i].segundo = conexoes_rede[mae[c]][i].segundo;
				}
				if(conexoes_aux[c][i].primeiro == 0){
					filho1[c][i].primeiro = conexoes_rede[mae[c]][i].primeiro;
				}
				if(conexoes_aux[c][i].segundo == 0){
					filho1[c][i].segundo = conexoes_rede[pai[c]][i].segundo;
				}
			}
		}

		


		/*Criação dos ultimos 5 filhos*/
		for(int c = 0; c < 5; c++){
			for(int i = 0; i < (coreNumbers + (coreNumbers / 4)); i++){
				if(conexoes_aux[c][i].primeiro == 1){
					filho2[c][i].primeiro = conexoes_rede[mae[c]][i].primeiro;
				}
				if(conexoes_aux[c][i].segundo == 1){
					filho2[c][i].segundo = conexoes_rede[pai[c]][i].segundo;
				}
				if(conexoes_aux[c][i].primeiro == 0){
					filho2[c][i].primeiro = conexoes_rede[pai[c]][i].primeiro;
				}
				if(conexoes_aux[c][i].segundo == 0){
					filho2[c][i].segundo = conexoes_rede[mae[c]][i].segundo;
				}
			}
		}
		


		/*Mutação*/
		int aux_mut;
		for(int c = 0; c < 5; c++){
			aux_mut = filho1[c][((router_num + (router_num / 4)) - 1)].primeiro;
			filho1[c][((router_num + (router_num / 4)) - 1)].primeiro = filho1[c][0].primeiro;
			filho1[c][0].primeiro = aux_mut;
		}
		
		for(int c = 0; c < 5; c++){
			for(int i = 0; i < (router_num + (router_num / 4)); i++){
				filho3[c].push_back({filho1[c][i].primeiro, filho1[c][i].segundo});
				filho4[c].push_back({filho2[c][i].primeiro, filho2[c][i].segundo});
			}
		}



		/*Troca os filhos pelos indíviduos mais fracos da população*/
		for(int c = 0; c < 5; c++){
			for(int i = 0; i < (router_num + (router_num / 4)); i++){
				conexoes_rede[sub1[c]][i].primeiro = filho1[c][i].primeiro;
				conexoes_rede[sub1[c]][i].segundo = filho1[c][i].segundo;
				conexoes_rede[sub2[c]][i].primeiro = filho2[c][i].primeiro;
				conexoes_rede[sub2[c]][i].segundo = filho2[c][i].segundo;
			}	
		}
	}

	genetico(){

	}
};