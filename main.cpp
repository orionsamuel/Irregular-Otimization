#include "systemc.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "router.h"
#include "router_2.h"
#include "caminho_min.h"
#include "parameters.h"
#include "genetico.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	int coreNumbers;
	int prioridade;
	int router_num;
	int r1, r2;

	trafego_rede trafego;

	conexoes conexoes_rede;

	router *rt[25];
	router_2 *rt2[25];
	routing_table table[25];
	int router_count[25];

	void chaveamento_externo(){
		for(int i = 0; i < (router_num + (router_num / 4)); i++){
			router_count[i] =  0;
		}

		for(int i = 0; i < (router_num + (router_num / 4)); i++){
			if((router_count[(conexoes_rede[i].primeiro)-1] == 0) && (router_count[(conexoes_rede[i].segundo)-1] == 0)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[0] = rt[(conexoes_rede[i].segundo)-1]->out_port[0];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[0].write(rt[(conexoes_rede[i].segundo)-1]->out_val[0]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[0] = rt[(conexoes_rede[i].primeiro)-1]->out_port[0];
				rt[(conexoes_rede[i].segundo)-1]->in_val[0].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[0]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 0) && (router_count[(conexoes_rede[i].segundo)-1] == 1)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[0] = rt[(conexoes_rede[i].segundo)-1]->out_port[1];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[0].write(rt[(conexoes_rede[i].segundo)-1]->out_val[1]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[1] = rt[(conexoes_rede[i].primeiro)-1]->out_port[0];
				rt[(conexoes_rede[i].segundo)-1]->in_val[1].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[0]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 0) && (router_count[(conexoes_rede[i].segundo)-1] == 2)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[0] = rt[(conexoes_rede[i].segundo)-1]->out_port[2];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[0].write(rt[(conexoes_rede[i].segundo)-1]->out_val[2]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[2] = rt[(conexoes_rede[i].primeiro)-1]->out_port[0];
				rt[(conexoes_rede[i].segundo)-1]->in_val[2].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[0]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}/*else if((router_count[(conexoes_rede[i].primeiro)-1] == 0) && (router_count[(conexoes_rede[i].segundo)-1] == 3)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[0] = rt[(conexoes_rede[i].segundo)-1]->out_port[3];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[0].write(rt[(conexoes_rede[i].segundo)-1]->out_val[3]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[3] = rt[(conexoes_rede[i].primeiro)-1]->out_port[0];
				rt[(conexoes_rede[i].segundo)-1]->in_val[3].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[0]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}*/else if((router_count[(conexoes_rede[i].primeiro)-1] == 1) && (router_count[(conexoes_rede[i].segundo)-1] == 0)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[1] = rt[(conexoes_rede[i].segundo)-1]->out_port[0];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[1].write(rt[(conexoes_rede[i].segundo)-1]->out_val[0]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[0] = rt[(conexoes_rede[i].primeiro)-1]->out_port[1];
				rt[(conexoes_rede[i].segundo)-1]->in_val[0].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[1]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 1) && (router_count[(conexoes_rede[i].segundo)-1] == 1)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[1] = rt[(conexoes_rede[i].segundo)-1]->out_port[1];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[1].write(rt[(conexoes_rede[i].segundo)-1]->out_val[1]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[1] = rt[(conexoes_rede[i].primeiro)-1]->out_port[1];
				rt[(conexoes_rede[i].segundo)-1]->in_val[1].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[1]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 1) && (router_count[(conexoes_rede[i].segundo)-1] == 2)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[1] = rt[(conexoes_rede[i].segundo)-1]->out_port[2];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[1].write(rt[(conexoes_rede[i].segundo)-1]->out_val[2]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[2] = rt[(conexoes_rede[i].primeiro)-1]->out_port[1];
				rt[(conexoes_rede[i].segundo)-1]->in_val[2].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[1]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}/*else if((router_count[(conexoes_rede[i].primeiro)-1] == 1) && (router_count[(conexoes_rede[i].segundo)-1] == 3)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[1] = rt[(conexoes_rede[i].segundo)-1]->out_port[3];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[1].write(rt[(conexoes_rede[i].segundo)-1]->out_val[3]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[3] = rt[(conexoes_rede[i].primeiro)-1]->out_port[1];
				rt[(conexoes_rede[i].segundo)-1]->in_val[3].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[1]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}*/else if((router_count[(conexoes_rede[i].primeiro)-1] == 2) && (router_count[(conexoes_rede[i].segundo)-1] == 0)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[2] = rt[(conexoes_rede[i].segundo)-1]->out_port[0];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[2].write(rt[(conexoes_rede[i].segundo)-1]->out_val[0]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[0] = rt[(conexoes_rede[i].primeiro)-1]->out_port[2];
				rt[(conexoes_rede[i].segundo)-1]->in_val[0].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[2]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 2) && (router_count[(conexoes_rede[i].segundo)-1] == 1)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[2] = rt[(conexoes_rede[i].segundo)-1]->out_port[1];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[2].write(rt[(conexoes_rede[i].segundo)-1]->out_val[1]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[1] = rt[(conexoes_rede[i].primeiro)-1]->out_port[2];
				rt[(conexoes_rede[i].segundo)-1]->in_val[1].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[2]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 2) && (router_count[(conexoes_rede[i].segundo)-1] == 2)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[2] = rt[(conexoes_rede[i].segundo)-1]->out_port[2];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[2].write(rt[(conexoes_rede[i].segundo)-1]->out_val[2]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[2] = rt[(conexoes_rede[i].primeiro)-1]->out_port[2];
				rt[(conexoes_rede[i].segundo)-1]->in_val[2].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[2]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}/*else if((router_count[(conexoes_rede[i].primeiro)-1] == 2) && (router_count[(conexoes_rede[i].segundo)-1] == 3)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[2] = rt[(conexoes_rede[i].segundo)-1]->out_port[3];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[2].write(rt[(conexoes_rede[i].segundo)-1]->out_val[3]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[3] = rt[(conexoes_rede[i].primeiro)-1]->out_port[2];
				rt[(conexoes_rede[i].segundo)-1]->in_val[3].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[2]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 3) && (router_count[(conexoes_rede[i].segundo)-1] == 0)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[3] = rt[(conexoes_rede[i].segundo)-1]->out_port[0];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[3].write(rt[(conexoes_rede[i].segundo)-1]->out_val[0]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[0] = rt[(conexoes_rede[i].primeiro)-1]->out_port[3];
				rt[(conexoes_rede[i].segundo)-1]->in_val[0].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[3]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 3) && (router_count[(conexoes_rede[i].segundo)-1] == 1)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[3] = rt[(conexoes_rede[i].segundo)-1]->out_port[1];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[3].write(rt[(conexoes_rede[i].segundo)-1]->out_val[1]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[1] = rt[(conexoes_rede[i].primeiro)-1]->out_port[3];
				rt[(conexoes_rede[i].segundo)-1]->in_val[1].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[3]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 3) && (router_count[(conexoes_rede[i].segundo)-1] == 2)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[3] = rt[(conexoes_rede[i].segundo)-1]->out_port[2];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[3].write(rt[(conexoes_rede[i].segundo)-1]->out_val[2]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[2] = rt[(conexoes_rede[i].primeiro)-1]->out_port[3];
				rt[(conexoes_rede[i].segundo)-1]->in_val[2].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[3]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}else if((router_count[(conexoes_rede[i].primeiro)-1] == 3) && (router_count[(conexoes_rede[i].segundo)-1] == 3)){
				rt[(conexoes_rede[i].primeiro)-1]->in_port[3] = rt[(conexoes_rede[i].segundo)-1]->out_port[3];
				rt[(conexoes_rede[i].primeiro)-1]->in_val[3].write(rt[(conexoes_rede[i].segundo)-1]->out_val[3]);
				rt[(conexoes_rede[i].segundo)-1]->in_port[3] = rt[(conexoes_rede[i].primeiro)-1]->out_port[3];
				rt[(conexoes_rede[i].segundo)-1]->in_val[3].write(rt[(conexoes_rede[i].primeiro)-1]->out_val[3]);
				router_count[(conexoes_rede[i].primeiro)-1]++;
				router_count[(conexoes_rede[i].segundo)-1]++;
			}*/
		}
	}

	void map_rede(){
		if((r1 == 10) && (r2 == 0)){
			for (int i = 0; i < router_num; i++){
				rt[i]->tabela = table[i];
			}
		}else if((r1 == 9) && (r2 == 1)){
			for (int i = 0; i < router_num; i++){
				if(i < 9){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 8) && (r2 == 2)){
			for (int i = 0; i < router_num; i++){
				if(i < 8){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 7) && (r2 == 3)){
			for (int i = 0; i < router_num; i++){
				if(i < 7){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 6) && (r2 == 4)){
			for (int i = 0; i < router_num; i++){
				if(i < 6){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 5) && (r2 == 5)){
			for (int i = 0; i < router_num; i++){
				if(i < 5){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 4) && (r2 == 6)){
			for (int i = 0; i < router_num; i++){
				if(i < 4){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 3) && (r2 == 7)){
			for (int i = 0; i < router_num; i++){
				if(i < 3){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 2) && (r2 == 8)){
			for (int i = 0; i < router_num; i++){
				if(i < 2){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 1) && (r2 == 9)){
			for (int i = 0; i < router_num; i++){
				if(i < 9){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}else if((r1 == 0) && (r2 == 10)){
			for (int i = 0; i < router_num; i++){
				if(i < 10){
					rt[i]->tabela = table[i];
				}else{
					rt2[i]->tabela = table[i];
				}				
			}
		}
	}

	void set_packets(){
		for(int i = 0; i < router_num; i++){
			for(int j = 0; j < trafego.size(); j++){
				if(rt[i]->position == trafego[j].origem){
					rt[i]->in_val[4].write(1);
					for(int k = 0; k < trafego[j].pacotes; k++){
						prioridade = 1 + (rand() % 2);
						for(int l = 0; l < 5; l++){
							if(l == 0){
								rt[i]->in_port[4].type = BEGIN_PACKET;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								rt[i]->in_port[4].priority = prioridade;
								wait(3);
								rt[i]->in_port[4].type = 0;
							}else if(l == 4){
								rt[i]->in_port[4].type = END_PACKET;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								rt[i]->in_port[4].priority = prioridade;
								wait(3);
								rt[i]->in_port[4].type = 0;
							}else{
								rt[i]->in_port[4].type = INSTRUCTION;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								rt[i]->in_port[4].priority = prioridade;
								wait(3);
								rt[i]->in_port[4].type = 0;
							}
						}
					}
				}
				if(rt[i]->position2 == trafego[j].origem){
					rt[i]->in_val[3].write(1);
					for(int k = 0; k < trafego[j].pacotes; k++){
						prioridade = 1 + (rand() % 2);
						for(int l = 0; l < 5; l++){
							if(l == 0){
								rt[i]->in_port[3].type = BEGIN_PACKET;
								rt[i]->in_port[3].payload = (8 + i);
								rt[i]->in_port[3].destiny = trafego[j].destino;
								rt[i]->in_port[3].priority = prioridade;
								wait(3);
								rt[i]->in_port[3].type = 0;
							}else if(l == 4){
								rt[i]->in_port[3].type = END_PACKET;
								rt[i]->in_port[3].payload = (8 + i);
								rt[i]->in_port[3].destiny = trafego[j].destino;
								rt[i]->in_port[3].priority = prioridade;
								wait(3);
								rt[i]->in_port[3].type = 0;
							}else{
								rt[i]->in_port[3].type = INSTRUCTION;
								rt[i]->in_port[3].payload = (8 + i);
								rt[i]->in_port[3].destiny = trafego[j].destino;
								rt[i]->in_port[3].priority = prioridade;
								wait(3);
								rt[i]->in_port[3].type = 0;
							}
						}
					}
				}
			}
		}
	}


	SC_CTOR(NoC){
		for(int i = 0; i < 25; i++){
			rt[i] = new router("rt");
			rt[i]->clk(clk);
		}

		for(int i = 0; i < 25; i++){
			rt2[i] = new router_2("rt");
			rt2[i]->clk(clk);
		}

		SC_METHOD(chaveamento_externo);
		sensitive << clk.pos();
		SC_METHOD(map_rede);
		sensitive << clk.pos();
		SC_CTHREAD(set_packets, clk.pos());
	}
};

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);

	NoC rede("NoC");
	rede.clk(clock);

	genetico gene;

	int coreNumbers;
	char linha[100];
	string temp0;
	string temp1;
	string temp2;
	string temp3;
	int temp00;
	int temp01;
	int temp02;
	int temp03;
	int count;
	int dist[10][10];
	int caminho[10][10];
	caminho_min cam;


	trafego_rede trafego;

	float deadline_parcial, deadline;
	int total_packets;
	sc_time latencia_parcial, latencia_media;
	sc_time conste(0, SC_NS);

	ifstream arquivoTrafego;
	ofstream saidaDados;


	//Instanciamento do arquivo de trafego
	arquivoTrafego.open("01.txt", ios_base::in);
	if (arquivoTrafego.is_open()){
		arquivoTrafego.getline(linha, 100);
		coreNumbers = atoi(linha);
		while(arquivoTrafego.getline(linha, 100)){
			temp0 = "";
			temp1 = "";
			temp2 = "";
			temp3 = "";
			for(count = 0; count < 100; count++){
				if(linha[count] != ' '){
					temp0 = temp0 + linha[count];
				}else{
					break;
				}
			}
			temp00 = atoi(temp0.c_str());
			count = count + 1;

			for(; count < 100; count ++){
				if(linha[count] != ' '){
					temp1 = temp1 + linha[count];
				}else{
					break;
				}
			}
			temp01 = atoi(temp1.c_str());
			count = count + 1;

			for(; count < 100; count ++){
				if(linha[count] != ' '){
					temp2 = temp2 + linha[count];
				}else{
					break;
				}
			}
			temp02 = atoi(temp2.c_str());
			count = count + 1;

			for(; count < 100; count ++){
				if(linha[count] != ' '){
					temp3 = temp3 + linha[count];
				}else{
					break;
				}
			}
			temp03 = atoi(temp3.c_str());

			trafego.push_back({temp00, temp01, temp02, temp03});
		}
	}

	arquivoTrafego.close();

	int router_num = coreNumbers;
	int r1 = router_num;
	int r2 = 0;


	gene.coreNumbers = coreNumbers;
	gene.router_num = router_num;

	//gene.firstPopulation();

	rede.coreNumbers = coreNumbers;
	rede.trafego = trafego;
	

	for(int c = 0; c < (coreNumbers/2); c++){
		gene.firstPopulation();

		for(int d = 0; d <= router_num; d++){
			rede.r1 = r1;
			rede.r2 = r2;


			//Inicio do genético
			for(int b = 0; b < 50; b++){
				rede.router_num = router_num;
				for(int a = 0; a < SIZE_POPULATION; a++){
					cout << b << endl;
					rede.conexoes_rede = gene.conexoes_rede[a];

					//Mapeamento dos roteadores
					if(router_num == coreNumbers){
						for(int j = 0; j < router_num; j++){
							rede.rt[j]->position = j + 1;
						}						
					}else if(router_num == (coreNumbers - 1)){
						for(int j = 0; j < router_num; j++){
							if(j == 0){
								rede.rt[j]->position = j+1;
								rede.rt[j]->position2 = j+2;
							}else{
								rede.rt[j]->position = j + 2;
							}
						}
					}else if(router_num == (coreNumbers - 2)){
						for(int j = 0; j < router_num; j++){
							if(j == 0){
								rede.rt[j]->position = j+1;
								rede.rt[j]->position2 = j+2;
							}else if(j == 1){
								rede.rt[j]->position = j+2;
								rede.rt[j]->position2 = j+3;
							}else{
								rede.rt[j]->position = j + 3;
							}
						}
					}else if(router_num == (coreNumbers - 3)){
						for(int j = 0; j < router_num; j++){
							if(j == 0){
								rede.rt[j]->position = j+1;
								rede.rt[j]->position2 = j+2;
							}else if(j == 1){
								rede.rt[j]->position = j+2;
								rede.rt[j]->position2 = j+3;
							}else if(j == 2){
								rede.rt[j]->position = j+3;
								rede.rt[j]->position2 = j+4;
							}else{
								rede.rt[j]->position = j + 4;
							}
						}
					}else if(router_num == (coreNumbers - 4)){
						for(int j = 0; j < router_num; j++){
							if(j == 0){
								rede.rt[j]->position = j+1;
								rede.rt[j]->position2 = j+2;
							}else if(j == 1){
								rede.rt[j]->position = j+2;
								rede.rt[j]->position2 = j+3;
							}else if(j == 2){
								rede.rt[j]->position = j+3;
								rede.rt[j]->position2 = j+4;
							}else if(j == 3){
								rede.rt[j]->position = j+4;
								rede.rt[j]->position2 = j+5;
							}else{
								rede.rt[j]->position = j + 5;
							}
						}
					}else if(router_num == (coreNumbers - 5)){
						for(int j = 0; j < router_num; j++){
							if(j == 0){
								rede.rt[j]->position = j+1;
								rede.rt[j]->position2 = j+2;
							}else if(j == 1){
								rede.rt[j]->position = j+2;
								rede.rt[j]->position2 = j+3;
							}else if(j == 2){
								rede.rt[j]->position = j+3;
								rede.rt[j]->position2 = j+4;
							}else if(j == 3){
								rede.rt[j]->position = j+4;
								rede.rt[j]->position2 = j+5;
							}else if(j == 4){
								rede.rt[j]->position = j+5;
								rede.rt[j]->position2 = j+6;
							}else{
								rede.rt[j]->position = j + 6;
							}
						}
					}

					//Preenchimento da matriz de adjacência
					for(int i = 0; i < router_num; i++){
						for(int j = 0; j < router_num; j++){
							if (i == j){
								dist[i][j] = 0;
							}else{
								for(int k = 0; k < (router_num + (router_num / 4)); k++){
									if((rede.conexoes_rede[k].primeiro - 1) == i){
										if((rede.conexoes_rede[k].segundo - 1) == j){
											dist[i][j] = 1;
											dist[j][i] = 1;
										}
									}
								}
							}						
						}
					}

					for(int i = 0; i < router_num; i++){
						for(int j = 0; j < router_num; j++){
							if((i != j) && (dist[i][j] != 1)){
								dist[i][j] = 100;
							}
						}
					}

					//Passagem dos valores das matrizes pro caminho mínimo	
					cam.floyd(dist, caminho);


					//Preenchimento das tabelas de roteamento
					int router_count[(router_num + (router_num / 4))];
					int tab_aux[router_num][router_num];

					for(int i = 0; i < (router_num + (router_num / 4)); i++){
							router_count[i] =  0;
					}

					for(int i = 0; i < router_num; i++){
						for(int j = 0; j < router_num; j++){
							tab_aux[i][j] = 100;
						}
					}

					for(int i = 0; i < router_num; i++){
						for(int j = 0; j < router_num; j++){
							if(i == j){
								tab_aux[i][j] = 4;
							}
						}
					}

					for(int i = 0; i < (router_num + (router_num / 4)); i++){
						if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 0) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 0)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 0;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 0;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 0) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 1)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 0;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 1;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 0) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 2)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 0;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 2;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}/*else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 0) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 3)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 0;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 3;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}*/else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 1) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 0)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 1;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 0;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 1) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 1)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 1;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 1;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 1) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 2)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 1;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 2;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}/*else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 1) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 3)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 1;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 3;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}*/else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 2) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 0)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 2;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 0;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 2) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 1)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 2;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 1;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 2) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 2)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 2;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 2;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}/*else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 2) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 3)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 2;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 3;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 3) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 0)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 3;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 0;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 3) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 1)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 3;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 1;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 3) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 2)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 3;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 2;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}else if((router_count[(rede.conexoes_rede[i].primeiro)-1] == 3) && (router_count[(rede.conexoes_rede[i].segundo)-1] == 3)){
							tab_aux[(rede.conexoes_rede[i].primeiro)-1][(rede.conexoes_rede[i].segundo)-1] = 3;
							tab_aux[(rede.conexoes_rede[i].segundo)-1][(rede.conexoes_rede[i].primeiro)-1] = 3;
							router_count[(rede.conexoes_rede[i].primeiro)-1]++;
							router_count[(rede.conexoes_rede[i].segundo)-1]++;
						}*/
					}

					//Tabelas de roteamentp
					if(router_num == coreNumbers){
						for(int i = 0; i < router_num; i++){	
							for(int j = 0; j < coreNumbers; j++){
								rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
							}
						}
					}else if(router_num == (coreNumbers - 1)){
						for(int i = 0; i < router_num; i++){	
							for(int j = 0; j < coreNumbers; j++){
								if(j == 0){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
							}
						}
					}else if(router_num == (coreNumbers - 2)){
						for(int i = 0; i < router_num; i++){	
							for(int j = 0; j < coreNumbers; j++){
								if(j == 0){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 2){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
							}
						}
					}else if(router_num == (coreNumbers - 3)){
						for(int i = 0; i < router_num; i++){	
							for(int j = 0; j < coreNumbers; j++){
								if(j == 0){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 2){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 3){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
							}
						}
					}else if(router_num == (coreNumbers - 4)){
						for(int i = 0; i < router_num; i++){	
							for(int j = 0; j < coreNumbers; j++){
								if(j == 0){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 2){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 3){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 4){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
							}
						}
					}
					else if(router_num == (coreNumbers - 5)){
						for(int i = 0; i < router_num; i++){	
							for(int j = 0; j < coreNumbers; j++){
								if(j == 0){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 2){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 3){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 4){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
								if(j == 5){
									if(j == i){
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), WEST, dist[i][j]});
										j = j + 1;
									}else{
										rede.table[i].push_back({j, tab_aux[i][caminho[i][j]], dist[i][j]});
										rede.table[i].push_back({(j+1), tab_aux[i][caminho[i][j]], dist[i][j]});
										j = j + 1;
									}					
								}
							}
						}
					}
					
					


					sc_start(trafego[0].deadline, SC_NS);



					//Calculo dos pacotes que chegaram ao destino
					for(int i = 0; i < trafego.size(); i++){
						total_packets = total_packets + trafego[i].pacotes;
					}

					for(int i = 0; i < coreNumbers; i++){
						deadline_parcial = deadline_parcial + rede.rt[i]-> count;
					}
					
					for(int i = 0; i < coreNumbers; i++){
						latencia_parcial = latencia_parcial + rede.rt[i]->latencia_rt;
					}

					string simulacao = "_saida_simulacao.txt";

					deadline = (deadline_parcial * 100) / total_packets;
					latencia_media = (latencia_parcial / total_packets);
					if(deadline > 100){
						deadline = 100;
					}

					string nome_pasta = std::to_string(router_num)+"_Roteadores/"+std::to_string(r1)+"_"+std::to_string(r2)+"/";
					const char* pasta = (char*) nome_pasta.c_str();
					mkdir(pasta, 777);
					saidaDados.open(pasta+std::to_string(a)+simulacao);
					saidaDados << deadline << endl;
					saidaDados << latencia_media;// << endl;
					//saidaDados << router_num;
					saidaDados.close();

					//latencia_parcial = latencia_parcial - latencia_parcial;
					//cout << latencia_media << " " << deadline << endl;
					/*for(int i = 0; i < coreNumbers; i++){
						cout << rede.rt[0]->tabela[i].destiny << " " << rede.rt[0]->tabela[i].port << " " << rede.rt[0]->tabela[i].hops << endl;
					}*/
					/*for (int i = 0; i < coreNumbers; i++){
			        	for (int j = 0; j < coreNumbers; j++){
			            	cout << dist[i][j] << " ";
			        	}
			        	cout << endl;
			   		}*/

					}

					gene.fitness();

					gene.crossover();


				}
			string nome_pasta2 = std::to_string(router_num)+"_Conexoes/"+std::to_string(r1)+"_"+std::to_string(r2)+"/";
			const char* pasta2 = (char*) nome_pasta2.c_str();
			mkdir(pasta2, 777);		
			string conex = "_conexoes.txt";
			for(int i = 0; i < SIZE_POPULATION; i++){
				saidaDados.open(pasta2+std::to_string(i)+conex);
				for(int j = 0; j < (router_num + (router_num / 4)); j++){
					saidaDados << gene.conexoes_rede[i][j].primeiro << " " << gene.conexoes_rede[i][j].segundo << endl;
				}
				saidaDados.close();
			}	


			r1 = r1 - 1;
			r2 = r2 + 1;
		}

		


		router_num = router_num - 1;
	}


	


}