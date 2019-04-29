#include <iostream>
#include <fstream>
#include "metodoWJ.hpp"

MetodoWJ::MetodoWJ() {
    
}

MetodoWJ::~MetodoWJ() {
    
}

std::vector<Grupo> MetodoWJ::gerarSolucao(Grafo* grafo) {
    srand(time(0));
    
    double** matriz = grafo->getMatrizAdjacencia();
    
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    std::vector<Grupo> grupos = criaGrupos(grafo, elementosSemGrupo, matriz);
    
    return grupos;
}

std::vector<Grupo> MetodoWJ::criaGrupos(Grafo* grafo, std::vector<int> elementosSemGrupo, double** matriz) {
    int** limites = grafo->getLimites();
    std::vector<Grupo> grupos;
    
    // criando o primeiro grupo com o primeiro elemento;
    int pos = rand() % elementosSemGrupo.size();
    int elemento = elementosSemGrupo[pos];
    
    grupos.push_back(Grupo(limites[0][0], limites[0][1], elemento));    
    atualizaGrafo(grafo, elemento, elementosSemGrupo);
    
    int qtdGruposIniciados = 1;
    double menorDistancia;
    
    while (qtdGruposIniciados < grafo->getQtdGrupos()) {
        if (pos == int(elementosSemGrupo.size())) {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos - 1]];
        }
        else {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos + 1]];
        }
        
        for (int i = 0; i < int(elementosSemGrupo.size()); ++ i) {
            if (matriz[elemento][elementosSemGrupo[i]] <= menorDistancia) {
                menorDistancia = matriz[elemento][elementosSemGrupo[i]];
                pos = i;
            }
        }
        
        elemento = elementosSemGrupo[pos];

        grupos.push_back(Grupo(limites[qtdGruposIniciados][0], limites[qtdGruposIniciados][1], elemento));
        atualizaGrafo(grafo, elemento, elementosSemGrupo);
        
        ++qtdGruposIniciados;
    }
    
    int proximoGrupo = limiteInferior(grafo, grupos, elementosSemGrupo, pos, matriz);
    
    if (elementosSemGrupo.size() > 0) {
        limiteSuperior(grafo, grupos, elementosSemGrupo, pos, matriz, proximoGrupo);
    }
    
    grafo->setElementosSemGrupo(elementosSemGrupo);

    return grupos;
}

int MetodoWJ::limiteInferior(Grafo* grafo, std::vector<Grupo> &grupos, std::vector<int> &elementosSemGrupo, int &pos, double** matriz) {
    double menorDistancia;
    int elemento = elementosSemGrupo[pos];
    int posGrupo = 0;
    
    std::vector<int> gruposNaoProntos;
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        gruposNaoProntos.push_back(i);
    }
    
    int proximoGrupo;
    
    while (int(gruposNaoProntos.size()) > 0) {
        if (pos == int(elementosSemGrupo.size())) {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos - 1]];
        }
        else {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos + 1]];
        }   
        
        for (int i = 0; i < int(elementosSemGrupo.size()); ++i) {
            if (matriz[elemento][elementosSemGrupo[i]] <= menorDistancia) {
                menorDistancia = matriz[elemento][elementosSemGrupo[i]];
                pos = i;
            }
        }
        
        elemento = elementosSemGrupo[pos];
        
        atualizaGrupo(grupos[gruposNaoProntos[posGrupo]], matriz, elemento);
        atualizaGrafo(grafo, elemento, elementosSemGrupo);
        
        proximoGrupo = gruposNaoProntos[posGrupo];
        
        if (grupos[gruposNaoProntos[posGrupo]].getQtdElementos() == grupos[gruposNaoProntos[posGrupo]].getLimiteInferior()) {
            for (int i = 0; i < int(gruposNaoProntos.size()); ++i) {
                if (gruposNaoProntos[i] == gruposNaoProntos[posGrupo]) {
                    gruposNaoProntos.erase(gruposNaoProntos.begin() + i);
                }
            }
            --posGrupo;
        }

        ++posGrupo;
        if (posGrupo == int(gruposNaoProntos.size())) {
            posGrupo = 0;
        }
    }
    
    if ((proximoGrupo + 1) == grafo->getQtdGrupos()) {
        proximoGrupo = 0;
    }
    else {
        ++proximoGrupo;
    }
    
    return proximoGrupo;
}

void MetodoWJ::limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupos, std::vector<int> &elementosSemGrupo, int &pos, double** matriz, int proximoGrupo) {
    double menorDistancia;
    int elemento = elementosSemGrupo[pos];
    
    // para verificar se "proximoGrupo" a ser adicionado um elemento nao esta cheio
    bool posicaoCorreta = false;
    while (!posicaoCorreta) {
        if (grupos[proximoGrupo].getQtdElementos() == grupos[proximoGrupo].getLimiteSuperior()) {
            if ((proximoGrupo + 1) == grafo->getQtdGrupos()) {
                proximoGrupo = 0;
            }
            else {
                ++proximoGrupo;
            }
        }
        else {
            posicaoCorreta = true;
        }
    }
    
    std::vector<int> gruposNaoProntos;
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        if (grupos[i].getQtdElementos() < grupos[i].getLimiteSuperior()) {
            gruposNaoProntos.push_back(i);
        }
    }
    
    // encontrando qual a posicao de "proximoGrupo" em "gruposNaoProntos"
    int posGrupo = 0;
    for (int i = 0; i < int(gruposNaoProntos.size()); ++i) {
        if (proximoGrupo == gruposNaoProntos[i]) {
            posGrupo = i;
        }
    }

    while (elementosSemGrupo.size() > 0) {
        if (pos == int(elementosSemGrupo.size())) {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos - 1]];
        }
        else {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos + 1]];
        }   
        
        for (int i = 0; i < int(elementosSemGrupo.size()); ++i) {
            if (matriz[elemento][elementosSemGrupo[i]] <= menorDistancia) {
                menorDistancia = matriz[elemento][elementosSemGrupo[i]];
                pos = i;
            }
        }
        
        elemento = elementosSemGrupo[pos];
        
        atualizaGrupo(grupos[gruposNaoProntos[posGrupo]], matriz, elemento);
        atualizaGrafo(grafo, elemento, elementosSemGrupo);

        if (grupos[gruposNaoProntos[posGrupo]].getQtdElementos() == grupos[gruposNaoProntos[posGrupo]].getLimiteInferior()) {
            for (int i = 0; i < int(gruposNaoProntos.size()); ++i) {
                if (gruposNaoProntos[i] == gruposNaoProntos[posGrupo]) {
                    gruposNaoProntos.erase(gruposNaoProntos.begin() + i);
                }
            }
            --posGrupo;
        }
        
        ++posGrupo;
        if (posGrupo == int(gruposNaoProntos.size())) {
            posGrupo = 0;
        }
    }
}

void MetodoWJ::atualizaGrupo(Grupo& grupo, double** matriz, int &elemento) {
    grupo.setElementos(elemento);
    grupo.setQtdElementos();
    
    for (int i = 0; i < grupo.getQtdElementos(); ++i) {
        grupo.setSomatorioDistancias(matriz[elemento][grupo.getElementos()[i]]);
        grupo.setArestasElementos(elemento, grupo.getElementos()[i]);
        grupo.setArestasValor(matriz[elemento][grupo.getElementos()[i]]);
    }
}

void MetodoWJ::atualizaGrafo(Grafo* grafo, int &elemento, std::vector<int> &elementosSemGrupo) {
    grafo->setInseridosUm(elemento);
    grafo->setElementosSemGrupoRemove(elementosSemGrupo, elemento);
}
