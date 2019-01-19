#include <iostream>
#include "grupo.hpp"

Grupo::Grupo(int limiteInferior, int limiteSuperior, int elemento) {
    this->limiteInferior = limiteInferior;
    this->limiteSuperior = limiteSuperior;
    qtdElementos = 1;
    elementos.push_back(elemento);
    somatorioDistancias = 0;
    //~ somatorioTotal = 0;
}

Grupo::~Grupo() {

}

int Grupo::getLimiteInferior() {
    return limiteInferior;
}

int Grupo::getLimiteSuperior() {
    return limiteSuperior;
}

int Grupo::getQtdElementos() {
    return qtdElementos;
}

std::vector<int> Grupo::getElementos() {
    return elementos;
}

int Grupo::getElemento(int pos) {
    return elementos[pos];
}

std::vector<std::vector<int> > Grupo::getArestasElementos() {
    return arestasElementos;
}

std::vector<double> Grupo::getArestasValor() {
    return arestasValor;
}

double Grupo::getSomatorioDistancias() {
    return somatorioDistancias;
}

//~ double Grupo::getSomatorioTotal() {
    //~ return somatorioTotal;
//~ }

void Grupo::setQtdElementos() {
    ++qtdElementos;
}

void Grupo::setElementos(int elemento) {
    elementos.push_back(elemento);
}

void Grupo::setArestasElementos(int elemento1, int elemento2) {
    std::vector<int> arestaElementos;
    arestaElementos.push_back(elemento1);
    arestaElementos.push_back(elemento2);
    arestasElementos.push_back(arestaElementos);
}

void Grupo::setArestasValor(double valor) {
    arestasValor.push_back(valor);
}

void Grupo::setSomatorioDistancias(double valor) {
    somatorioDistancias += valor;
}
