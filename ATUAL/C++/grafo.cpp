#include <iostream>
#include "grafo.hpp"

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
    somatorioTotal = 0;
}

Grafo::~Grafo() {
    //~ delete[] this->inseridos;
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

double Grafo::getSomatorioTotal() {
    return somatorioTotal;
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
            //~ cout << "\nAPAGUEI " << elemento;
        }
    }
    //~ cout << "\nMANO DO CEU: ";
    //~ for (unsigned int i = 0; i < elementosSemGrupo.size(); ++i) {
        //~ cout << elementosSemGrupo[i] << " ";
    //~ }
}
        
void Grafo::setElementosSemGrupo(std::vector<int> elementosSemGrupo) {
    this->elementosSemGrupo = elementosSemGrupo;
}
        
double **Grafo::matrizAdjacenciaGrafo() {
    double** matriz = new double*[qtdElementos];
    for (int lin = 0; lin < qtdElementos; ++lin) {
        matriz[lin] = new double[qtdElementos];
        matriz[lin][lin] = 0;
    }
    for (int i = 0; i < qtdArestas; ++i) {
        matriz[arestasElementos[i][0]][arestasElementos[i][1]] = arestasValor[i];
        matriz[arestasElementos[i][1]][arestasElementos[i][0]] = arestasValor[i];
    }
    
    //~ cout << "\n\n\n";
    //~ for (int lin = 0; lin < qtdElementos; ++lin) {
        //~ for (int col = 0; col < qtdElementos; ++col) {
            //~ cout << matriz[lin][col] << " ";
        //~ }
        //~ cout << endl;
    //~ }

    return matriz;
}
