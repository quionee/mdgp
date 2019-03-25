#include <iostream>
#include "grafo.hpp"

using namespace std;

// construtor da classe.
// qtdElementos = inteiro que armazena a quantidade de vértices do grafo.
// qtdGrupos = inteiro que armazena a quantidades de grupos a serem formados.
// qtdArestas = inteiro que armazena a quantidade de arestas do grafo.
// tipoDivisao = string que armazena tipo de divisão dos grupos, "ss" se os grupos forem do mesmo tamanho, "ds" caso contrário.
// limites = ponteiro para ponteiro (matriz[i][j]), quando j = 0: os limites inferiores de cada grupo i,
//           coluna j = 1: os limites superiores de cada grupo i.
// arestasElementos = ponteiro para ponteiro (matriz[i][j]), as colunas são os elementos e as linhas as arestas.
// arestasValor = vetor de ponteiros, os valores de cada aresta de "arestasElementos".
// inseridos = vetor de ponteiros booleano, false se o elemento (posição do vetor) não tem grupo, true caso contrário.
// elementosSemGrupo = vetor de inteiros, com os elementos sem grupo.
// matrizAdjacencia = matriz de adjacência do grafo.
Grafo::Grafo(int qtdElementos, int qtdGrupos, int qtdArestas, std::string tipoDivisao, int **limites, int **arestasElementos, double *arestasValor) {
    this->qtdElementos = qtdElementos;
    this->qtdGrupos = qtdGrupos;
    this->qtdArestas = qtdArestas;
    this->tipoDivisao = tipoDivisao;
    this->limites = limites;
    this->arestasElementos = arestasElementos;
    this->arestasValor = arestasValor;
    inseridos = new bool[qtdElementos];
    for (int i = 0; i < qtdElementos; ++i) {
        inseridos[i] = false;
    }
    for (int i = 0; i < qtdElementos; ++i) {
        elementosSemGrupo.push_back(i);
    }
    matrizAdjacencia = new double*[qtdElementos];
    for (int lin = 0; lin < qtdElementos; ++lin) {
        matrizAdjacencia[lin] = new double[qtdElementos];
        matrizAdjacencia[lin][lin] = 0;
    }
    for (int i = 0; i < qtdArestas; ++i) {
        matrizAdjacencia[arestasElementos[i][0]][arestasElementos[i][1]] = arestasValor[i];
        matrizAdjacencia[arestasElementos[i][1]][arestasElementos[i][0]] = arestasValor[i];
    }
}

Grafo::~Grafo() {
    delete[] inseridos;
    for (int col = 0; col < qtdElementos; ++col) {
        delete[] matrizAdjacencia[col];
    }
    delete[] matrizAdjacencia;
}

int Grafo::getQtdElementos() {
    return qtdElementos;
}

int Grafo::getQtdGrupos() {
    return qtdGrupos;
}

int Grafo::getQtdArestas() {
            return qtdArestas;
        }

std::string Grafo::getTipoDivisao() {
    return tipoDivisao;
}

int **Grafo::getLimites() {
    return limites;
}

int **Grafo::getArestasElementos() {
    return arestasElementos;
    }

double *Grafo::getArestasValor() {
    return arestasValor;
}

bool *Grafo::getInseridos() {
    return inseridos;
}

std::vector<int> Grafo::getElementosSemGrupo() {
    return elementosSemGrupo;
}

double **Grafo::getMatrizAdjacencia() {
    return matrizAdjacencia;
}

// muda a variável inseridos inteira.
void Grafo::setInseridos(bool *inseridos) {
    this->inseridos = inseridos;
}

// muda uma posição da variável inseridos.
void Grafo::setInseridosUm(int elemento) {
    inseridos[elemento] = true;
}

// remove um elemento da variável elementosSemGrupo.
void Grafo::setElementosSemGrupoRemove(std::vector<int> &elementosSemGrupo, int elemento) {
    for (unsigned int i = 0; i < elementosSemGrupo.size(); ++i) {
        if (elementosSemGrupo[i] == elemento) {
            elementosSemGrupo.erase(elementosSemGrupo.begin() + i);
        }
    }
}

// atualiza a variável elementosSemGrupo inteira.
void Grafo::setElementosSemGrupo(std::vector<int> elementosSemGrupo) {
    this->elementosSemGrupo = elementosSemGrupo;
}

// reinicia elementosSemGrupo.
void Grafo::setReiniciaElementosSemGrupo() {
    elementosSemGrupo.clear();
    for (int i = 0; i < qtdElementos; ++i) {
        elementosSemGrupo.push_back(i);
    }
}
