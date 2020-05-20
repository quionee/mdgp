#include <iostream>
#include <fstream>
#include "metodoLH.hpp"
#include "buscaLocal.hpp"

using namespace std;

MetodoLH::MetodoLH(int qtdSolucoesFactiveis) : SolucaoInicial() {
    this->qtdSolucoesFactiveis = qtdSolucoesFactiveis;
}

MetodoLH::~MetodoLH() {
    
}

std::vector<Grupo> MetodoLH::gerarSolucao(Grafo* grafo) {
    srand(time(0));
    
    double** matriz = grafo->getMatrizAdjacencia();
    
    int** limites = grafo->getLimites();
    std::vector<vector<Grupo> > solucoes;

    for (int i = 0; i < qtdSolucoesFactiveis; ++i) {
        int limPosicoesDisponiveis = grafo->getQtdElementos();
        std::vector<Grupo> grupos;
        std::vector<int> gruposNaoCheios;
        
        for (int j = 0; j < grafo->getQtdGrupos(); ++j) {
            grupos.push_back(Grupo(limites[j][0], limites[j][1]));
            limiteInferior(grafo, grupos[j], limPosicoesDisponiveis, matriz);
            
            if (grupos[j].getQtdElementos() < grupos[j].getLimiteSuperior()) {
                gruposNaoCheios.push_back(j);
            }
        }
        
        limiteSuperior(grafo, grupos, gruposNaoCheios, matriz);
        solucoes.push_back(grupos);
        grafo->setReiniciaElementosSemGrupo();
    }

    for (int l = 0; l < qtdSolucoesFactiveis; ++l) {
        BuscaLocal buscaLocal(grafo);

        buscaLocal.criaVariaveis(grafo, solucoes[l]);
        bool melhorou = true;
        
        while (melhorou) {
            melhorou = false;
            buscaLocal.insercaoAlgoritmo(solucoes[l], melhorou);
            buscaLocal.swapAlgoritmo(melhorou);
        }
        
        buscaLocal.atualizaSolucao(grafo, solucoes[l]);
    }
    

    double maiorSomatorio = 0;
    int melhorSolucaoInicial;
    for (unsigned int i = 0; i < solucoes.size(); ++i) {
        double somatorio = 0;
        for (unsigned int j = 0; j < solucoes[i].size(); ++j) {
            somatorio += solucoes[i][j].getSomatorioDistancias();
        }
        
        if (somatorio > maiorSomatorio) {
            maiorSomatorio = somatorio;
            melhorSolucaoInicial = i;
        }
    }
    
    return solucoes[melhorSolucaoInicial];
}

void MetodoLH::limiteInferior(Grafo* grafo, Grupo &grupo, int &limPosicoesDisponiveis, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();

    while (grupo.getQtdElementos() < grupo.getLimiteInferior()) {
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        atualizaGrupo(grupo, matriz, elemento);
        atualizaGrafo(grafo, elemento, elementosSemGrupo);
        
        --limPosicoesDisponiveis;
    }
    grafo->setElementosSemGrupo(elementosSemGrupo);
}

void MetodoLH::limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupos, vector<int> &gruposNaoCheios, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    int qtdElementosSemGrupo = grafo->getElementosSemGrupo().size();
    int qtdGruposNaoCheios = gruposNaoCheios.size();

    while (qtdElementosSemGrupo != 0) {
        int posGrupo = rand() % qtdGruposNaoCheios;
        int grupoNaoPronto = gruposNaoCheios[posGrupo];

        atualizaGrupo(grupos[grupoNaoPronto], matriz, elementosSemGrupo[0]);
        atualizaGrafo(grafo, elementosSemGrupo[0], elementosSemGrupo);

        if (grupos[grupoNaoPronto].getQtdElementos() == grupos[grupoNaoPronto].getLimiteSuperior()) {
            for (int i = 0; i < qtdGruposNaoCheios; ++i) {
                if (gruposNaoCheios[i] == grupoNaoPronto) {
                    gruposNaoCheios.erase(gruposNaoCheios.begin() + i);
                }
            }
            --qtdGruposNaoCheios;
        }
        --qtdElementosSemGrupo;
    }
    grafo->setElementosSemGrupo(elementosSemGrupo);
}

void MetodoLH::atualizaGrupo(Grupo& grupo, double** matriz, int &elemento) {
    grupo.setElementos(elemento);
    grupo.setQtdElementos();
    
    for (int i = 0; i < grupo.getQtdElementos(); ++i) {
        grupo.setSomatorioDistancias(matriz[elemento][grupo.getElementos()[i]]);
        grupo.setArestasElementos(elemento, grupo.getElementos()[i]);
        grupo.setArestasValor(matriz[elemento][grupo.getElementos()[i]]);
    }
}

void MetodoLH::atualizaGrafo(Grafo* grafo, int &elemento, std::vector<int> &elementosSemGrupo) {
    grafo->setInseridosUm(elemento);
    grafo->setElementosSemGrupoRemove(elementosSemGrupo, elemento);
}
