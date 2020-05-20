#include <iostream>
#include <fstream>
#include "solucaoInicial.hpp"

using namespace std;

SolucaoInicial::SolucaoInicial() {
    
}

SolucaoInicial::~SolucaoInicial() {
    
}

// lê o arquivo cujo nome é passado por parâmetro e chama o construtor da classe Grafo com as variáveis
// lidas do arquivo e retorna o grafo.
Grafo* SolucaoInicial::leArquivo(std::string nomeArquivo) {
    std::ifstream arquivo;
    arquivo.open(nomeArquivo.c_str());
    
    int qtdElementos, qtdGrupos;
    arquivo >> qtdElementos;
    arquivo >> qtdGrupos;
    
    std::string tipoDivisao;
    arquivo >> tipoDivisao;
    
    int **limites;
    limites = new int*[qtdGrupos];
    for (int i = 0; i < qtdGrupos; ++i) {
        limites[i] = new int[2];
    }
    
    for (int i = 0; i < qtdGrupos; ++i) {
        arquivo >> limites[i][0]
                >> limites[i][1];
    }
    
    int qtdArestas = (qtdElementos * (qtdElementos - 1)) / 2;
    int **arestasElementos = new int*[qtdArestas];
    double *arestasValor = new double[qtdArestas];
    for (int i = 0; i < qtdArestas; ++i) {
        arestasElementos[i] = new int[2];
    }
    
    for (int i = 0; i < qtdArestas; ++i) {
        arquivo >> arestasElementos[i][0]
                >> arestasElementos[i][1]
                >> arestasValor[i];
    }
    
    arquivo.close();

    return new Grafo(qtdElementos, qtdGrupos, qtdArestas, tipoDivisao, limites, arestasElementos, arestasValor);
}
