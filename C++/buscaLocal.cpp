#include <iostream>
#include <fstream>
#include "buscaLocal.hpp"

using namespace std;

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

void BuscaLocal::criaVariaveis(Grafo* grafo, std::vector<Grupo> &solucao) {
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

    // cout << "\n\nvetorY: ";
    // for(int i = 0; i < grafo->getQtdElementos(); ++i) {
    //     cout << vetorY[i] << " ";
    // }
    
    // cout << "\n\nvetorZ: ";
    // for(int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     cout << vetorZ[i] << " ";
    // }
    
    // cout << "\n\nmatrizGama:\n\n    0   1\n";
    // for(int i = 0; i < grafo->getQtdElementos(); ++i) {
    //     cout << i << ": ";
    //     for(int j = 0; j < grafo->getQtdGrupos(); ++j) {
    //         cout << matrizGama[i][j] << " ";
    //     }
    //     cout << endl;
    // }

}

void BuscaLocal::arrumaGrupos(int &grupo1, int &grupo2, int &grupo3) {
    if (grupo1 <= 1) {
        if (grupo1 == grupo3) {
            ++grupo2;
            grupo3 += 2;
        }
        else if (grupo3 == (grupo1 + 1)) {
            grupo2 += 2;
        }
        else {
            ++grupo2;
        }
    }
    else {
        if (grupo1 == grupo3) {
            --grupo2;
            grupo3 -= 2;
        }
        else if (grupo3 == (grupo1 - 1)) {
            grupo2 -= 2;
        }
        else {
            --grupo2;
        }
    }
}

std::vector<Grupo> BuscaLocal::buscaLocal(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucao);
    bool melhorou = true;
    while (melhorou) {
        melhorou = false;
        insercaoAlgoritmo(solucaoFinal, melhorou);
        swapAlgoritmo(melhorou);
        swapEmCadeiaAlgoritmo(melhorou);
    }
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

std::vector<Grupo> BuscaLocal::insercao(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    bool melhorou = false;
    insercaoAlgoritmo(solucaoFinal, melhorou);
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::insercaoAlgoritmo(std::vector<Grupo> grupos, bool &melhorou) {
    bool reinicia = false;
    for (int v = 0; v < int(vetorY.size()); ++v) {
        for (int g = 0; (g < int(vetorZ.size())) and !reinicia; ++g) {
            if ((vetorY[v] != g) and (vetorZ[vetorY[v]] > grupos[vetorY[v]].getLimiteInferior()) and (vetorZ[g] < grupos[g].getLimiteSuperior())) {
                double deltaF = matrizGama[v][g] - matrizGama[v][vetorY[v]];
                if (deltaF > 0) {
                    atualizaMatrizGamaInsercao(vetorY[v], g, v);
                    
                    vetorZ[vetorY[v]] = vetorZ[vetorY[v]] - 1;
                    vetorZ[g] = vetorZ[g] + 1;
                    vetorY[v] = g;
                    
                    melhorou = true;
                    reinicia = true;
                }
            }
        }
        if (reinicia) {
            v = 0;
            reinicia = false;
        }
    }
}

std::vector<Grupo> BuscaLocal::insercaoSemValorDoMovimento(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    insercaoSemValorDoMovimentoAlgoritmo(solucaoFinal);
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::insercaoSemValorDoMovimentoAlgoritmo(std::vector<Grupo> grupos) {
    int grupo1 = rand() % int(vetorZ.size());
    int grupo2 = rand() % int(vetorZ.size());

    if (grupo2 == grupo1) {
        if (grupo2 == 0) {
            ++grupo2;
        }
        else {
            --grupo2;
        }
    }

    for (int v = 0; v < int(vetorY.size()); ++v) {
        if ((vetorY[v] != grupo1) and (vetorZ[vetorY[v]] > grupos[vetorY[v]].getLimiteInferior())
            and (vetorZ[grupo1] < grupos[grupo1].getLimiteSuperior())) {
            atualizaMatrizGamaInsercao(vetorY[v], grupo1, v);
            vetorZ[vetorY[v]] = vetorZ[vetorY[v]] - 1;
            vetorZ[grupo1] = vetorZ[grupo1] + 1;
            vetorY[v] = grupo1;
        }
        if ((vetorY[v] != grupo2) and (vetorZ[vetorY[v]] > grupos[vetorY[v]].getLimiteInferior())
            and (vetorZ[grupo2] < grupos[grupo2].getLimiteSuperior())) {
            atualizaMatrizGamaInsercao(vetorY[v], grupo2, v);
            vetorZ[vetorY[v]] = vetorZ[vetorY[v]] - 1;
            vetorZ[grupo2] = vetorZ[grupo2] + 1;
            vetorY[v] = grupo2;
        }
    }
}

void BuscaLocal::atualizaMatrizGamaInsercao(int &grupoV, int &grupoU, int &elementoV) {
    for (int u = 0; u < int(matrizGama.size()); ++u) {
        if (u != elementoV) {
            matrizGama[u][grupoV] = matrizGama[u][grupoV] - matriz[elementoV][u];
            matrizGama[u][grupoU] = matrizGama[u][grupoU] + matriz[elementoV][u];
        }
    }
}

std::vector<Grupo> BuscaLocal::swap(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    bool melhorou = false;
    swapAlgoritmo(melhorou);
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::swapAlgoritmo(bool &melhorou) {
    bool reinicia = false;
    for (int v = 0; v < (int(vetorY.size()) - 1); ++v) {
        for (int u = v + 1; (u < int(vetorY.size())) and !reinicia; ++u) {
            if (vetorY[v] != vetorY[u]) {
                double deltaF = (matrizGama[v][vetorY[u]] - matrizGama[v][vetorY[v]]) + (matrizGama[u][vetorY[v]] - matrizGama[u][vetorY[u]]) - (2 * matriz[v][u]);
                if (deltaF > 0) {
                    atualizaMatrizGamaSwap(vetorY[v], vetorY[u], v, u);
                    swapAux(v, u);
                    melhorou = true;
                    reinicia = true;
                }
            }
        }
        if (reinicia) {
            v = 0;
            reinicia = false;
        }
    }
}

std::vector<Grupo> BuscaLocal::swapSemValorDoMovimento(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    swapSemValorDoMovimentoAlgoritmo();
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::swapSemValorDoMovimentoAlgoritmo() {
    int grupo1 = rand() % int(vetorZ.size());
    int grupo2 = rand() % int(vetorZ.size());

    if (grupo1 == grupo2) {
        if (grupo1 == 0) {
            ++grupo2;
        }
        else {
            --grupo2;
        }
    }

    std::vector<int> vetorElementosGrupo1;
    std::vector<int> vetorElementosGrupo2;

    for (int i = 0; i < int(vetorY.size()); ++i) {
        if (vetorY[i] == grupo1) {
            vetorElementosGrupo1.push_back(i);
        }
        else if (vetorY[i] == grupo2) {
            vetorElementosGrupo2.push_back(i);
        }
    }

    for (int v = 0; v < int(vetorElementosGrupo1.size()) - 1; ++v) {
        for (int u = v + 1; u < int(vetorElementosGrupo2.size()); ++u) {
            atualizaMatrizGamaSwap(grupo1, grupo2, v, u);
            swapAux(v, u);
        }
    }

    vetorElementosGrupo1.clear();
    vetorElementosGrupo2.clear();
}

void BuscaLocal::atualizaMatrizGamaSwap(int &grupoV, int &grupoU, int &elementoV, int &elementoU) {
    atualizaMatrizGamaInsercao(grupoV, grupoU, elementoV);
    atualizaMatrizGamaInsercao(grupoU, grupoV, elementoU);
}

void BuscaLocal::swapAux(int &elementoV, int &elementoU) {
    int aux = vetorY[elementoV];
    vetorY[elementoV] = vetorY[elementoU];
    vetorY[elementoU] = aux;
}

std::vector<Grupo> BuscaLocal::swapEmCadeia(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    bool melhorou = false;
    swapEmCadeiaAlgoritmo(melhorou);
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::swapEmCadeiaAlgoritmo(bool &melhorou) {
    bool reinicia = false;
    for (int v = 0; v < (int(vetorY.size()) - 2); ++v) {
        for (int u = v + 1; (u < (int(vetorY.size()) - 1)) and !reinicia; ++u) {
            for (int w = u + 1; (w < int(vetorY.size())) and !reinicia; ++w) {
                if ((vetorY[v] != vetorY[u]) and (vetorY[u] != vetorY[w]) and (vetorY[w] != vetorY[v])) {
                    double deltaF = (((matrizGama[v][vetorY[u]] - matrizGama[v][vetorY[v]])
                                    + (matrizGama[u][vetorY[w]] - matrizGama[u][vetorY[u]])
                                    + (matrizGama[w][vetorY[v]] - matrizGama[w][vetorY[w]]))
                                    - (matriz[v][u] + matriz[u][w] + matriz[w][v]));
                    if (deltaF > 0) {
                        atualizaMatrizGamaSwapEmCadeia(vetorY[v], vetorY[u], vetorY[w], v, u, w);
                        swapEmCadeiaAux(v, u, w);
                        melhorou = true;
                        reinicia = true;
                    }
                }
            }
        }
        if (reinicia) {
            v = 0;
            reinicia = false;
        }
    }
}

std::vector<Grupo> BuscaLocal::swapEmCadeiaSemValorDoMovimento(Grafo* grafo, std::vector<Grupo> solucao) {
    std::vector<Grupo> solucaoFinal = solucao;
    criaVariaveis(grafo, solucaoFinal);
    swapEmCadeiaSemValorDoMovimentoAlgoritmo();
    atualizaSolucao(grafo, solucaoFinal);
    return solucaoFinal;
}

void BuscaLocal::swapEmCadeiaSemValorDoMovimentoAlgoritmo() {
    int grupo1 = rand() % int(vetorZ.size());
    int grupo2 = rand() % int(vetorZ.size());
    int grupo3 = rand() % int(vetorZ.size());

    if (grupo1 == grupo2) {
        arrumaGrupos(grupo1, grupo2, grupo3);
    }
    else if (grupo1 == grupo3) {
        arrumaGrupos(grupo1, grupo3, grupo2);
    }
    else if (grupo2 == grupo3) {
        arrumaGrupos(grupo2, grupo3, grupo1);
    }

    std::vector<int> vetorElementosGrupo1;
    std::vector<int> vetorElementosGrupo2;
    std::vector<int> vetorElementosGrupo3;

    for (int i = 0; i < int(vetorY.size()); ++i) {
        if (vetorY[i] == grupo1) {
            vetorElementosGrupo1.push_back(i);
        }
        else if (vetorY[i] == grupo2) {
            vetorElementosGrupo2.push_back(i);
        }
        else if (vetorY[i] == grupo3) {
            vetorElementosGrupo3.push_back(i);
        }
    }

    for (int v = 0; v < (int(vetorElementosGrupo1.size()) - 2); ++v) {
        for (int u = v + 1; u < (int(vetorElementosGrupo2.size()) - 1); ++u) {
            for (int w = u + 1; w < int(vetorElementosGrupo3.size()); ++w) {
                atualizaMatrizGamaSwapEmCadeia(grupo1, grupo2, grupo3, v, u, w);
                swapEmCadeiaAux(v, u, w);
            }
        }
    }

    vetorElementosGrupo1.clear();
    vetorElementosGrupo2.clear();
    vetorElementosGrupo3.clear();
}

void BuscaLocal::atualizaMatrizGamaSwapEmCadeia(int &grupoV, int &grupoU, int &grupoW, int &elementoV, int &elementoU, int &elementoW) {
    atualizaMatrizGamaInsercao(grupoV, grupoU, elementoV);
    atualizaMatrizGamaInsercao(grupoU, grupoW, elementoU);
    atualizaMatrizGamaInsercao(grupoW, grupoV, elementoW);
}

void BuscaLocal::swapEmCadeiaAux(int &elementoV, int &elementoU, int &elementoW) {
    int aux = vetorY[elementoV];
    vetorY[elementoV] = vetorY[elementoU];
    vetorY[elementoU] = vetorY[elementoW];
    vetorY[elementoW] = aux;
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
