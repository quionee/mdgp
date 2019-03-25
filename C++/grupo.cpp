#include <iostream>
#include "grupo.hpp"

// construtor da classe que inicia com um elmento.
// limiteInferior = limite inferior do grupo.
// limtieSuperior = limite superior do grupo.
// elemento = primeiro elemento do grupo atual.
Grupo::Grupo(int limiteInferior, int limiteSuperior, int elemento) {
    this->limiteInferior = limiteInferior;
    this->limiteSuperior = limiteSuperior;
    qtdElementos = 1;
    elementos.push_back(elemento);
    somatorioDistancias = 0;
}

// construtor da classe.
// limiteInferior = limite inferior do grupo.
// limtieSuperior = limite superior do grupo.
Grupo::Grupo(int limiteInferior, int limiteSuperior) {
    this->limiteInferior = limiteInferior;
    this->limiteSuperior = limiteSuperior;
    qtdElementos = 0;
    somatorioDistancias = 0;
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

// incrementa a quantidade de elementos do grupo.
void Grupo::setQtdElementos() {
    ++qtdElementos;
}

// adiciona um elemento ao grupo.
void Grupo::setElementos(int elemento) {
    elementos.push_back(elemento);
}

// adiciona os elementos de uma aresta ao arestasElementos do grupo.
void Grupo::setArestasElementos(int elemento1, int elemento2) {
    std::vector<int> arestaElementos;
    arestaElementos.push_back(elemento1);
    arestaElementos.push_back(elemento2);
    arestasElementos.push_back(arestaElementos);
}

// adiciona o valor de uma aresta ao arestasValor do grupo.
void Grupo::setArestasValor(double valor) {
    arestasValor.push_back(valor);
}

// soma a somatorioDistancias um valor.
void Grupo::setSomatorioDistancias(double valor) {
    somatorioDistancias += valor;
}

// zera as variáveis do grupo.
void Grupo::zeraVariaveis() {
    qtdElementos = 0;
    elementos.clear();
    elementos.clear();
    for (unsigned int i = 0; i < arestasElementos.size(); i++) {
        arestasElementos[i].clear();
    }
    arestasElementos.clear();
    arestasValor.clear();
    somatorioDistancias = 0;
}
