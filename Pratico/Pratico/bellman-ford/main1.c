/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.c
 * Author: pripyat
 *
 * Created on 3 de Maio de 2016, 10:16
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct Edges {
	int origem;
	int destino;
	float peso;
} Edge;




typedef struct Pilhas {
    int noh;
    struct Pilhas * prox;
} Pilha;


void pushPilha (Pilha ** s, int noh) {
	Pilha * noh_pilha = calloc(1, sizeof(Pilha));

	noh_pilha->noh = noh;
	noh_pilha->prox = *s;

	*s = noh_pilha;
}

int popPilha (Pilha ** s) {
	int retorno;
	Pilha * proximo;

	if (s != NULL) {
		retorno = (*s)->noh;
		proximo = (*s)->prox;

		free(*s);

		*s = proximo;

		return retorno;

	} else {
		return -1;
	}
}

void imprimePilha(Pilha * s, FILE * f) {

	if (s == NULL) {
		return;
	} else {

		while (s != NULL) {
			fprintf(f, " %d", s->noh);
			popPilha(&s);
		}
	}
	fprintf(f, "\n");
}


void esvaziaPilha(Pilha * s) {

	Pilha * atual, * proximo;

	if (s == NULL) {
		return;
	} else {
		atual = s;
		proximo = s->prox;

		while (proximo != NULL) {
			free (atual);

			atual = proximo;
			proximo = atual->prox;
		}

		free (atual);
	}
}


void le_arquivo(char * diretorio, Edge ** lista_arestas, int * vertices, int * arestas, int * maior_peso) {

	// Define o ponteiro pro arquivo em modo de leitura
	FILE * bench = fopen(diretorio, "r");

	int i = 0, origem_tmp = 0, destino_tmp = 0, peso_tmp = 0;
	char criou_lista = 0;

	// Contador de quantas arestas foram lidas
	int count_arestas = 0;


	// Lê do arquivo o comando da linha
	char comando = fgetc(bench);

	// Enquando não for final de arquivo
	while (comando != EOF) {

		// Verifica qual comando é o comando
		switch (comando) {
			// Comentários serão ignorados
			case 'c':
				while (fgetc(bench) != '\n');
				fgetc(bench);
				break;

			// Informações iniciais do grafo como número de vértices e
				// arestas
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

				// Le o número de vertices e arestas
				fscanf(bench, "%d %d", vertices, arestas);

				criou_lista = 1;

				* maior_peso = -1;
				
				*lista_arestas = calloc(*arestas, sizeof(Edge));
				break;

			case 'a':

				if (criou_lista == 0)
					exit(-1);

				fscanf(bench, "%d %d %d", &origem_tmp, &destino_tmp, &peso_tmp);

				(*lista_arestas)[count_arestas].origem  = origem_tmp;
				(*lista_arestas)[count_arestas].destino = destino_tmp;
				(*lista_arestas)[count_arestas].peso    = peso_tmp;

				* maior_peso = peso_tmp;

				// Le a aresta e seu valor
				count_arestas++;
				// Quebra a linha
				fgetc(bench);
				break;

			default:
				break;
		}

		// Le o proximo comando
		comando = fgetc(bench);
	} //while

	// Verifica se a contagem de leitura de arestas foi realmente exato
	if ((criou_lista == 0) || (count_arestas != *arestas)) {
		printf("Numero de arestas está incorreto em relação ao arquivo.\n");
		exit(-1);
	}

	*maior_peso = *maior_peso * *maior_peso;

	// Fecha o arquivo aberto
	fclose(bench);
}

void imprimeDistancias(int * distancia, int vertices) {
	int i;
	printf("\n\n\n");
	for (i = 0; i < vertices; i++)  {
		printf("%4d ", i);
	}

	printf("\n");

	for (i = 0; i < vertices; i++)  {
		printf("%4d ", distancia[i]);
	}

}

void imprimePredecessores(int * predecessor, int vertices) {
	int i;
	printf("\n\n");
	for (i = 0; i < vertices; i++)  {
		printf("%4d ", i);
	}

	printf("\n");

	for (i = 0; i < vertices; i++)  {
		printf("%4d ", predecessor[i]);
	}
}

void bellmanFord(Edge * lista_arestas, int ** distancia, int ** predecessor, int vertices, int arestas, int origem, int maior_peso) {
	int i, j, peso;

	int * distancia_temp   = calloc(vertices, sizeof(int));
	int * predecessor_temp = calloc(vertices, sizeof(int));

	for (i = 0; i < vertices; i++) {
		distancia_temp[i] = maior_peso;
		predecessor_temp[i] = -1;
	}

	distancia_temp[origem - 1] = 0;

	/*imprimeDistancias(distancia_temp, vertices);
	imprimePredecessores(predecessor_temp, vertices);
	fflush(stdout); */


	for (i = 0; i < vertices; i++) {
		for (j = 0; j < arestas; j++) {
			peso = lista_arestas[j].peso;

			//printf("\n\n%d:\t%d + %d < %d", j, distancia_temp[lista_arestas[j].origem - 1], peso, distancia_temp[lista_arestas[j].destino - 1]);
			//fflush(stdout);

			if (//(distancia_temp[lista_arestas[j].origem - 1] != -1) &&
					(distancia_temp[lista_arestas[j].origem - 1] + peso < distancia_temp[lista_arestas[j].destino - 1] )){
					//||
					//distancia_temp[lista_arestas[j].destino - 1] == -1)) {

					//distancia_temp[lista_arestas[j].origem - 1] + peso < distancia_temp[lista_arestas[j].destino - 1]) {

				distancia_temp[lista_arestas[j].destino - 1] = distancia_temp[lista_arestas[j].origem - 1] + peso;

				predecessor_temp[lista_arestas[j].destino - 1] = lista_arestas[j].origem - 1;


				//imprimeDistancias(distancia_temp, vertices);
				//imprimePredecessores(predecessor_temp, vertices);
				fflush(stdout);
			}
		}
	}

	for (i = 0; i < vertices; i++) {
		peso = lista_arestas[i].peso;

		if (distancia_temp[lista_arestas[j].origem - 1] + peso < distancia_temp[lista_arestas[j].destino - 1]) {
			printf("Graph contains a negative-weight cycle\n");
			exit(-1);
		}
	}

	*distancia = distancia_temp;
	*predecessor = predecessor_temp;
}



/*
 * Procedimento final que imprime o caminho para melhor visualização do usuário
 * bem como o valor total da distância.
 */
void imprimeTodosCaminhos(FILE * file, int * distancia, int * predecessor, int vertices, int origem) {

	int anterior, i;

	Pilha * stack;

	for(i = 0; i < vertices; i++) {
		if (i != origem -1) {
			if (predecessor[i] != -1) {

				fprintf(file, "[%d,%d](%d)", origem, i + 1, distancia[i]);

				stack = NULL;

				//printf("empilha%d ", i + 1);
				pushPilha(&stack, i + 1);

				anterior = predecessor[i];
				while(anterior != origem - 1) {
					//printf("empilha%d ", anterior);
					fflush(stdout);
					pushPilha(&stack, anterior);

					anterior = predecessor[anterior];
				}

				fprintf(file, " %d", origem);

				imprimePilha(stack, file);
				fflush(file);
			}
		}
	}

	/*
	fflush(stdout);

	while (vertice_anterior[anterior - 1] != origem) {
		printf(" <- %d", vertice_anterior[anterior - 1]);
		fflush(stdout);

		anterior = vertice_anterior[anterior - 1];
	}

	printf(" <- %d.", vertice_anterior[anterior - 1]);

	printf("\nDistância pecorrida: %f\n.", lista_adjacencia[destino - 1].peso_atual);
	 * */
}


void desaloca(Edge ** lista_arestas, int ** distancia, int ** predecessor) {

	free(*lista_arestas);
	free(*distancia);
	free(*predecessor);
}


/*
 *
 */
int main(int argc, char** argv) {

	if(argc == 2) {
		clock_t tempo_inicio, tempo_final;
		double intervalo_real;
		
		Edge * lista_arestas = NULL;
		int i, vertices = 0, arestas = 0, * distancia = 0, * predecessor = 0, maior_peso = 0;
		FILE * file = fopen("saida.txt", "w+");
		FILE * tempos = fopen("tempos.txt", "a");

		le_arquivo(argv[1], &lista_arestas, &vertices, &arestas, &maior_peso);

		for (i = 0; i < vertices; i++) {

			tempo_inicio = clock();
				bellmanFord(lista_arestas, &distancia, &predecessor, vertices, arestas, i + 1, maior_peso);
			tempo_final = clock();

			intervalo_real = (double)(tempo_final - tempo_inicio) / CLOCKS_PER_SEC;

			fprintf(tempos, "%f\n", intervalo_real);

			//imprimeCaminho(vertices, vertice_anterior, lista_adjacencia, origem, destino);
			imprimeTodosCaminhos(file, distancia, predecessor, vertices, i + 1);

		}

		fclose(file);
		fclose(tempos);

		desaloca(&lista_arestas, &distancia, &predecessor);

	} 
	else {
		printf("Argumentos Inválidos!\n");
		exit(-1);
	}
	
	return (EXIT_SUCCESS);
}

