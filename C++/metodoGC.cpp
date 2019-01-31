#include <iostream>
#include <fstream>
#include "metodoGC.hpp"

using namespace std;

MetodoGC::MetodoGC() : SolucaoInicial() {
}

MetodoGC::~MetodoGC() {
    
}

std::vector<Grupo> MetodoGC::gerarSolucao(Grafo* grafo) {
    std::vector<int> elementosAleatorios = divisaoInicial(grafo);
    
    std::vector<Grupo> grupos = criaGrupos(grafo, elementosAleatorios);
    
    double **matriz = grafo->getMatrizAdjacencia();    
    
    limiteInferior(grafo, grupos, matriz);
    limiteSuperior(grafo, grupos, matriz);
    
    return grupos;
}

std::vector<int> MetodoGC::divisaoInicial(Grafo* grafo) {
    std::vector<int> elementosAleatorios;
    int aux = 0;
    int posDisponiveis = grafo->getQtdElementos();
    int pos;
    bool *inseridos = grafo->getInseridos();
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    while (aux < grafo->getQtdGrupos()) {
        srand(time(0));
        pos = rand() % posDisponiveis;
        
        if (!(inseridos[elementosSemGrupo[pos]])) {
            elementosAleatorios.push_back(elementosSemGrupo[pos]);
            inseridos[elementosSemGrupo[pos]] = true;
            grafo->setElementosSemGrupoRemove(elementosSemGrupo, elementosSemGrupo[pos]);
            ++aux;
            --posDisponiveis;
        }
    }

    grafo->setInseridos(inseridos);
    grafo->setElementosSemGrupo(elementosSemGrupo);
    
    return elementosAleatorios;
}

std::vector<Grupo> MetodoGC::criaGrupos(Grafo* grafo, std::vector<int> elementosAleatorios) {
    std::vector<Grupo> grupos;
    int** limites = grafo->getLimites();
    
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        grupos.push_back(Grupo(limites[i][0], limites[i][1], elementosAleatorios[i]));
    }
    
    return grupos;
}

void MetodoGC::atualizaGrupo(Grupo &grupo, int elemento, double** matriz) {
    grupo.setQtdElementos();
    grupo.setElementos(elemento);
    
    for (int j = 0; j < (grupo.getQtdElementos()); ++j) {
        grupo.setSomatorioDistancias(matriz[elemento][grupo.getElementos()[j]]);
        grupo.setArestasElementos(elemento, grupo.getElementos()[j]);
        grupo.setArestasValor(matriz[elemento][grupo.getElementos()[j]]);
    }
    
}

void MetodoGC::atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo) {
    grafo->setInseridosUm(elemento);
    grafo->setElementosSemGrupoRemove(elementosSemGrupo, elemento);
}

void MetodoGC::limiteInferior(Grafo* grafo, std::vector<Grupo> &grupos, double** matriz) {
    int limPosicoesDisponiveis = grafo->getQtdElementos() - grafo->getQtdGrupos();
    bool gruposCompletos[grafo->getQtdGrupos()];
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        gruposCompletos[i] = false;
    }
    
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    bool terminou = false;
    while ((!terminou) and (elementosSemGrupo.size() > 0)) {
        
        int qtdGruposProntos = 0;
        srand(time(0));
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        double maiorSomatorio = 0;
        int grupoMaximizado = -1;
        double somatorioGrupoAtual = 0;
        int i = 0; 
        
        while (i < grafo->getQtdGrupos()) {
            if (grupos[i].getQtdElementos() < grupos[i].getLimiteInferior()) {
                somatorioGrupoAtual = 0;
                
                for (int j = 0; j < grupos[i].getQtdElementos(); ++j) {
                    somatorioGrupoAtual += matriz[elemento][grupos[i].getElementos()[j]];
                }
                
                if ((somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1)) > maiorSomatorio) {
                    maiorSomatorio = (somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1));
                    grupoMaximizado = i;
                }
            }
            else {
                gruposCompletos[i] = true;
            }
            ++i;
        }
        
        if (grupoMaximizado > -1) {
            atualizaGrupo(grupos[grupoMaximizado], elemento, matriz);
            atualizaGrafo(grafo, elemento, elementosSemGrupo);
            --limPosicoesDisponiveis;
        }
        
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            if (gruposCompletos[i]) {
                ++qtdGruposProntos;
            }
        }
        
        if (qtdGruposProntos == grafo->getQtdGrupos()) {
            terminou = true;
        }
    }
    
    grafo->setElementosSemGrupo(elementosSemGrupo);
    
}

void MetodoGC::limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupos, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    int limPosicoesDisponiveis = elementosSemGrupo.size();

    while (elementosSemGrupo.size() > 0) {
        srand(time(0));
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        int grupoMaximizado = -1;
        
        double maiorSomatorio = 0;
        double somatorioGrupoAtual = 0;
        int i = 0;
        
        while (i < grafo->getQtdGrupos()) {
            if (grupos[i].getQtdElementos() < grupos[i].getLimiteSuperior()) {
                somatorioGrupoAtual = 0;
                
                for (int j = 0; j < grupos[i].getQtdElementos(); ++j) {
                    somatorioGrupoAtual += matriz[elemento][grupos[i].getElementos()[j]];
                }
                
                if ((somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1)) > maiorSomatorio) {
                    maiorSomatorio = (somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1));
                    grupoMaximizado = i;
                }
            }
            ++i;
        }
        
        atualizaGrupo(grupos[grupoMaximizado], elemento, matriz);
        atualizaGrafo(grafo, elemento, elementosSemGrupo);
        --limPosicoesDisponiveis;
    }
    grafo->setElementosSemGrupo(elementosSemGrupo);
}

