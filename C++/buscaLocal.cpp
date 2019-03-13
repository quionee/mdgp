#include <iostream>
#include <fstream>
#include "buscaLocal.hpp"

BuscaLocal::BuscaLocal(Grafo* grafo) {
    matriz = grafo->getMatrizAdjacencia();
}

BuscaLocal::~BuscaLocal() {
    vetorY.clear();
    vetorZ.clear();
    for (unsigned int lin = 0; lin < matrizGama.size(); lin++) {
        matrizGama[lin].clear();
    }
    matrizGama.clear();
}

std::vector<int> BuscaLocal::getVetorY() {
    return vetorY;
}

std::vector<int> BuscaLocal::getVetorZ() {
    return vetorZ;
}

std::vector<std::vector<double> > BuscaLocal::getMatrizGama() {
    return matrizGama;
}

void BuscaLocal::reiniciaVariaveis() {
    vetorY.clear();
    vetorZ.clear();
    for (unsigned int lin = 0; lin < matrizGama.size(); lin++) {
        matrizGama[lin].clear();
    }
    matrizGama.clear();
}

void BuscaLocal::criaVariaveis(Grafo* grafo, std::vector<Grupo> solucao) {
    reiniciaVariaveis();

    // vetor de coordenadas dos elementos, indica a qual grupo cada elemento pertence
    vetorY.resize(grafo->getQtdElementos());
    
    // vetor de tamanhos dos grupos, indica qual o tamanho de cada grupo
    vetorZ.resize(grafo->getQtdGrupos());
    
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        vetorZ[i] = solucao[i].getQtdElementos();
        
        std::vector<int> elementosGrupoI = solucao[i].getElementos();
        for (int j = 0; j < solucao[i].getQtdElementos(); ++j) {
            vetorY[elementosGrupoI[j]] = i;
        }
    }

    // matriz que armazena o valor do movimento
    for (int lin = 0; lin < grafo->getQtdElementos(); ++lin) {
        std::vector<double> vetorAux;
        
        for (int col = 0; col < grafo->getQtdGrupos(); ++col) {
            vetorAux.push_back(0);
        }
        
        matrizGama.push_back(vetorAux);
    }

    // andar pelos grupos realizando o somatorio das distancias
    for (int i = 0; i < grafo->getQtdElementos(); ++i) {
        for (int j = 0; j < grafo->getQtdGrupos(); ++j) {
            std::vector<int> elementosGrupoJ = solucao[j].getElementos();
            for (int k = 0; k < solucao[j].getQtdElementos(); ++k) {
                matrizGama[i][j] += matriz[i][elementosGrupoJ[k]];
            }
        }
    }
}

std::vector<Grupo> BuscaLocal::buscaLocal(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucao);
    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        insercaoPrincipal(solucaoFinal, melhorou);
        swapPrincipal(melhorou);
        swapEmCadeia(melhorou);
    }
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

std::vector<Grupo> BuscaLocal::insercao(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    bool melhorou = false;
    insercaoPrincipal(solucaoFinal, melhorou);
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::insercaoPrincipal(std::vector<Grupo> grupos, bool &melhorou) {
    for (int v = 0; v < int(vetorY.size()); ++v) {
        for (int g = 0; g < int(vetorZ.size()); ++g) {
            if ((vetorY[v] != g) and (vetorZ[vetorY[v]] > grupos[vetorY[v]].getLimiteInferior()) and (vetorZ[g] < grupos[g].getLimiteSuperior())) {
                double deltaF = matrizGama[v][g] - matrizGama[v][vetorY[v]];
                if (deltaF > 0) {
                    atualizaMatrizGamaInsercao(vetorY[v], g, v);
                    
                    vetorZ[vetorY[v]] = vetorZ[vetorY[v]] - 1;
                    vetorZ[g] = vetorZ[g] + 1;
                    vetorY[v] = g;
                    
                    melhorou = true;
                }
            }
        }
    }
}

void BuscaLocal::atualizaMatrizGamaInsercao(int grupoV, int grupoU, int elementoV) {
    for (int u = 0; u < int(matrizGama.size()); ++u) {
        if (u != elementoV) {
            matrizGama[u][grupoV] = matrizGama[u][grupoV] - matriz[elementoV][u];
            matrizGama[u][grupoU] = matrizGama[u][grupoU] + matriz[elementoV][u];
        }
    }
    
}

void BuscaLocal::swapPrincipal(bool &melhorou) {
    for (int v = 0; v < (int(vetorY.size()) - 1); ++v) {
        for (int u = v + 1; u < int(vetorY.size()); ++u) {
            if (vetorY[v] != vetorY[u]) {
                double deltaF = (matrizGama[v][vetorY[u]] - matrizGama[v][vetorY[v]]) + (matrizGama[u][vetorY[v]] - matrizGama[u][vetorY[u]]) - (2 * matriz[v][u]);
                if (deltaF > 0) {
                    atualizaMatrizGamaSwap(vetorY[v], vetorY[u], v, u);
                    
                    swapAux(v, u);
                    melhorou = true;
                }
            }
        }
    }
}

void BuscaLocal::atualizaMatrizGamaSwap(int grupoV, int grupoU, int elementoV, int elementoU) {
    atualizaMatrizGamaInsercao(grupoV, grupoU, elementoV);
    atualizaMatrizGamaInsercao(grupoU, grupoV, elementoU);
}

void BuscaLocal::swapAux(int elementoV, int elementoU) {
    int aux = vetorY[elementoV];
    vetorY[elementoV] = vetorY[elementoU];
    vetorY[elementoU] = aux;
}

void BuscaLocal::swapEmCadeia(bool &melhorou) {
    for (int v = 0; v < (int(vetorY.size()) - 2); ++v) {
        for (int u = v + 1; u < (int(vetorY.size()) - 1); ++u) {
            for (int w = u + 1; w < int(vetorY.size()); ++w) {
                if ((vetorY[v] != vetorY[u]) and (vetorY[u] != vetorY[w])) {
                    double deltaF = ((matrizGama[v][vetorY[u]] - matrizGama[v][vetorY[v]])
                                    + (matrizGama[u][vetorY[w]] - matrizGama[u][vetorY[u]])
                                    + (matrizGama[w][vetorY[v]] - matrizGama[w][vetorY[w]])
                                    - (matriz[v][u] + matriz[u][w] + matriz[w][v]));
                    if (deltaF > 0) {
                        atualizaMatrizGamaSwapEmCadeia(vetorY[v], vetorY[u], vetorY[w], v, u, w);
                        
                        swapEmCadeiaAux(v, u, w);
                        melhorou = true;
                    }
                }
            }
        }
    }
}

void BuscaLocal::atualizaMatrizGamaSwapEmCadeia(int grupoV, int grupoU, int grupoW, int elementoV, int elementoU, int elementoW) {
    atualizaMatrizGamaInsercao(grupoV, grupoU, elementoV);
    atualizaMatrizGamaInsercao(grupoU, grupoW, elementoU);
    atualizaMatrizGamaInsercao(grupoW, grupoV, elementoW);
}

void BuscaLocal::swapEmCadeiaAux(int elementoV, int elementoU, int elementoW) {
    int aux = vetorY[elementoV];
    vetorY[elementoV] = vetorY[elementoW];
    vetorY[elementoW] = vetorY[elementoU];
    vetorY[elementoU] = aux;
}

void BuscaLocal::atualizaSolucao(Grafo* grafo, std::vector<Grupo> &grupos) {
    // zera variaveis dos grupos (exceto os limites)
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        grupos[i].zeraVariaveis();
    }
    
    // atualizando os elementos e as quantidades de elementos
    for (int i = 0; i < grafo->getQtdElementos(); ++i) {
        grupos[vetorY[i]].setElementos(i);
        grupos[vetorY[i]].setQtdElementos();
    }
    
    // atualizando as arestas e os somatorios das distancias dos grupos
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        for (int j = 0; j < grupos[i].getQtdElementos() - 1; ++j) {
            for (int k = j + 1; k < grupos[i].getQtdElementos(); ++k) {
                grupos[i].setArestasElementos(grupos[i].getElemento(j), grupos[i].getElemento(k));
                grupos[i].setArestasValor(matriz[grupos[i].getElemento(j)][grupos[i].getElemento(k)]);
                grupos[i].setSomatorioDistancias(matriz[grupos[i].getElemento(j)][grupos[i].getElemento(k)]);
            }
        }
    }
}
