#include <iostream>
#include "grafo.hpp"

using namespace std;

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

void Grafo::setInseridos(bool *inseridos) {
    this->inseridos = inseridos;
}

void Grafo::setInseridosUm(int elemento) {
    inseridos[elemento] = true;
}

void Grafo::setElementosSemGrupoRemove(std::vector<int> &elementosSemGrupo, int elemento) {
    for (unsigned int i = 0; i < elementosSemGrupo.size(); ++i) {
        if (elementosSemGrupo[i] == elemento) {
            elementosSemGrupo.erase(elementosSemGrupo.begin() + i);
        }
    }
}
        
void Grafo::setElementosSemGrupo(std::vector<int> elementosSemGrupo) {
    this->elementosSemGrupo = elementosSemGrupo;
}

void Grafo::setReiniciaElementosSemGrupo() {
    for (int i = 0; i < qtdElementos; ++i) {
        elementosSemGrupo.push_back(i);
    }
}
