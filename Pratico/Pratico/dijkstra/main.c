/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: pripyat
 *
 * Created on 27 de Abril de 2016, 14:15
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct Nohs {
		int noh_id;
		float peso;
		struct Nohs * proximo;
} Noh;


typedef struct NohsIndividuais {
		char visitado;
		float peso_atual;
} NohIndividual;
    
    
int global_vertices = 0, global_arestas = 0;
Noh * global_lista_adjacencia = NULL;
NohIndividual * global_nohs_individuais = NULL;

	
Noh * criaNovoNoh(int destino, int peso) ;
Noh * criaListaAdjacencia() ;
void  criaNovaAdjacencia(int origem, int destino, int peso);
void  imprimeAdjacencia(Noh * lista) ;
void  le_arquivo(char * diretorio) ;
Noh * adicionaNohCabecaLista(Noh * lista, Noh noh) ;
void  adicionaNohFinalVetor(int * lista, int noh_id) ;
void  removeNohLista(Noh * lista, Noh noh);
int   extraiVerticeMenosCustoso() ;
int   extraiVertice(int id) ;
void  imprimeLista(Noh * lista) ;
void  imprimeVertice(Noh v);
Noh * retornaVertice(int id) ;
int * retornaTodosVertices() ;
void  alteraPesoAtual(int id, int novo_peso) ;
float retornaPesoAtual(int id) ;
void  alteraVisitado(int id, char novo_valor);
void  inicializaDijkstra(int fonte, int * vertices_resultantes);
void  relaxamento(int origem_id) ;
void  dijkstra(int fonte, int destino);
int   retornaValorVisitado(int id) ;
Noh * retornaAdjacencia (int origem) ;
	

Noh * criaNovoNoh(int destino, int peso) {

	Noh *noh = calloc(1, sizeof (Noh));

	noh->noh_id     = destino;
	noh->peso       = peso;
	noh->proximo    = NULL;

	return noh;
}


Noh * criaListaAdjacencia() {

	Noh * lista_adjacencia = calloc(global_vertices, sizeof (Noh));

	for (int i = 0; i < global_vertices; i++) {
		lista_adjacencia[i].noh_id     = i + 1;
		lista_adjacencia[i].peso       = 0;

		lista_adjacencia[i].proximo    = NULL;
	}

	return lista_adjacencia;
}


void criaNovaAdjacencia(int origem, int destino, int peso) {

	Noh * atual    = retornaAdjacencia(origem);
	Noh * seguinte = atual->proximo;
	
	atual->proximo = criaNovoNoh(destino, peso);
	
	atual->proximo->proximo = seguinte;
}


void imprimeAdjacencia(Noh * lista) {
	Noh * atual = NULL;

	printf("\nVértices: %d, Arestas: %d:\n", global_vertices, global_arestas);

	for (int i = 0; i < global_vertices; i++) {
		printf("%d ->", lista[i].noh_id);
		atual = lista[i].proximo;

		while (atual != NULL) {
			printf(" %d", atual->noh_id);

			atual = atual->proximo;
		}
		printf("\n");
	}
}


void le_arquivo(char * diretorio) {

	FILE * bench = fopen(diretorio, "r");
	int i, origem, destino, peso;

	int count_arestas = 0;
	char comando = fgetc(bench);

	while (comando != EOF) {
		switch (comando) {
			case 'c':
				while (fgetc(bench) != '\n');
				fgetc(bench);
				break;

			case 'p':

				if (!(fgetc(bench) == ' ')) {
					printf("Erro na inicializacao!\n");
					exit(2);
				}
				if (!(fgetc(bench) == 's')) {
					printf("Erro na inicializacao!\n");
					exit(2);
				}
				if (!(fgetc(bench) == 'p')) {
					printf("Erro na inicializacao!\n");
					exit(2);
				}

				fscanf(bench, "%d %d", &global_vertices, &global_arestas);

				global_lista_adjacencia = criaListaAdjacencia();
				break;

			case 'a':
				count_arestas++;

				fscanf(bench, "%d %d %d", &origem, &destino, &peso);

				criaNovaAdjacencia(origem, destino, peso);

				// Pula linha
				fgetc(bench);
				break;

			default:
				break;

		}

		comando = fgetc(bench);
	} //while

	if (count_arestas != global_arestas) {
		printf("Numero de arestas está incorreto.\n");
	}
	
	imprimeAdjacencia(global_lista_adjacencia);

	fclose(bench);
}


Noh * adicionaNohCabecaLista(Noh * lista, Noh noh) {
	Noh * cabeca = NULL;
	
	if (lista == NULL) {
		cabeca = calloc(1, sizeof(Noh));
			
		cabeca->noh_id     = noh.noh_id;
		cabeca->peso       = noh.peso;
		
		cabeca->proximo    = NULL;
		
	} else {
		
		cabeca = calloc(1, sizeof(Noh));
	
		cabeca->noh_id     = noh.noh_id;
		cabeca->peso       = noh.peso;
		
		cabeca->proximo    = lista;
	} 
	
	return cabeca;
}


void adicionaNohFinalVetor(int * lista, int noh_id) {
	int i = 0;
	
	while(lista[i] != -1) i++;
	
	lista[i] = noh_id;

}


void removeNohLista(Noh * lista, Noh noh) {
	Noh * anterior, * atual, * proximo, * remover = NULL;

	// Verifica se e' o primeiro
	if (lista->noh_id == noh.noh_id) {
		remover = lista;

		lista   = lista->proximo;

		free (remover);

	} else {
		anterior = lista;

		while (anterior->proximo != NULL || anterior->proximo->noh_id != noh.noh_id) {
			anterior = anterior->proximo;
		}

		atual = anterior->proximo;

		if (atual->noh_id == noh.noh_id) {
			proximo = atual->proximo;

			if (proximo != NULL) {
				anterior->proximo = proximo;

			} else {
				anterior->proximo = NULL;
			}

			free(atual);
		}
	}
}


int extraiVerticeMenosCustoso() {

	NohIndividual * lista = global_nohs_individuais;
	
	/*
	for (int i = 0; i < global_vertices; i++) {
		printf("%d:%f ", (int) lista[i].visitado, lista[i].peso_atual);
	}
	printf("\n");
	 */
	
	int i = 0;
	while (lista[i].visitado == 1 || lista[i].peso_atual < 1) 
		i++;
	
	int menor = i;
	
	for (++i; i < global_vertices; i++) {
		
		if (lista[i].visitado == 0 &&
				lista[i].peso_atual > 0 &&
			lista[i].peso_atual < lista[menor].peso_atual) {
				menor = i;
		}
	}
	
	menor++;
	
	alteraVisitado(menor, 1);
	
	return menor;
}

int extraiVertice(int id) {

	alteraVisitado(id, 1);
	
	return id;
}


void imprimeLista(Noh * lista) {

	Noh * atual = lista;
	
	if (atual == NULL) {
		printf("Lista Vazia.\n");
	} else {
			while (atual != NULL) {
			printf("%d ", atual->noh_id);
			atual = atual->proximo;
		}
		printf("\n");
	}
}


void imprimeVertice(Noh v) {
	printf("ID:%d; PesoAtual:%f; Visitado:%d;\n", v.noh_id, retornaPesoAtual(v.noh_id), (int) global_nohs_individuais[v.noh_id -1].visitado);
}


Noh * retornaVertice(int id) {

	Noh * atual = global_lista_adjacencia;
	
	while (atual != NULL || atual->noh_id != id) {
		atual = atual->proximo;
	}
	
	if (atual->noh_id == id) {
		Noh * retorno = calloc(1, sizeof(Noh));

		retorno->noh_id     = atual->noh_id;
		retorno->peso       = atual->peso;
		retorno->proximo    = NULL;
		
		return retorno;
	} else {
		return NULL;
	}
	
}


int * retornaTodosVertices() {

	int * primeiro = calloc(global_vertices, sizeof(int));

	for (int i = 0; i < global_vertices; i++) {
		primeiro[i] = i + 1;
	}

	return primeiro;
}


void alteraPesoAtual(int id, int novo_peso) {
	
	global_nohs_individuais[id - 1].peso_atual = novo_peso;
}

float retornaPesoAtual(int id) {
	
	return global_nohs_individuais[id - 1].peso_atual;
}

int retornaValorVisitado(int id) {
	
	return (int) global_nohs_individuais[id - 1].visitado;
}


void alteraVisitado(int id, char novo_valor) {
	
	global_nohs_individuais[id - 1].visitado = novo_valor;
}


void inicializaDijkstra(int fonte, int * vertices_resultantes) {
	global_nohs_individuais = calloc(global_vertices, sizeof(NohIndividual));
	
	for (int i = 0; i < global_vertices; i++) {
		vertices_resultantes[i] = -1;
		//vertices_nao_utilizados[i] = 1;
		
		alteraPesoAtual(i + 1, -1);
		alteraVisitado(i + 1, 0);
	}
	
	alteraPesoAtual(fonte, 0);
	alteraVisitado(fonte, 1);
}


Noh * retornaAdjacencia (int origem) {
	return &(global_lista_adjacencia[origem - 1]);
}


void relaxamento(int origem_id) {
	Noh * origem = retornaAdjacencia(origem_id);
	Noh * atual  = origem->proximo;
	
	float peso_atual_temp;
	
	while (atual != NULL) {
		
		peso_atual_temp = retornaPesoAtual(atual->noh_id);
		
		//if (atual->peso_atual < 0 || origem->peso_atual + atual->peso < atual->peso_atual) {
 		if (peso_atual_temp < 0 ||
				retornaPesoAtual(origem->noh_id) + atual->peso < peso_atual_temp) {
			
			//atual->peso_atual = origem->peso_atual + atual->peso;
			alteraPesoAtual(atual->noh_id, retornaPesoAtual(origem->noh_id) + atual->peso);
			
			//printf("%d alterado de %f para %f.\n", atual->noh_id, peso_atual_temp, retornaPesoAtual(atual->noh_id));
		}
	
		atual = atual->proximo;	
	}
}


void dijkstra(int fonte, int destino) {

	int vertices_resultantes[global_vertices];
	int quantidade_resultantes = 0;
	
	inicializaDijkstra(fonte, vertices_resultantes);

	
	int vertice_mais_proximo = extraiVertice(fonte);
	adicionaNohFinalVetor(vertices_resultantes, vertice_mais_proximo);
	
	// Enquanto tiver vertice pra analizar
	while (destino != vertice_mais_proximo && quantidade_resultantes++ < global_vertices) {
		
		relaxamento(vertice_mais_proximo);
		
		vertice_mais_proximo = extraiVerticeMenosCustoso();
		
		adicionaNohFinalVetor(vertices_resultantes, vertice_mais_proximo);
		
	}
	
	printf("\n");
	printf("\n");
	for (int j = 0; j <= quantidade_resultantes; j++)
		printf("%d -> ", vertices_resultantes[j]);
	
	printf(".\tTotal: %f\n", retornaPesoAtual(vertice_mais_proximo));
	
	
}


int main(int argc, char** argv) {

	le_arquivo("bench.txt");
	//le_arquivo("cormen.txt");
	
	dijkstra(1, 2);

	return (EXIT_SUCCESS);

}

