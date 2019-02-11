#include <iostream>
#include <fstream>
#include "buscaLocal.hpp"

using namespace std;

BuscaLocal::BuscaLocal() {
    
}

BuscaLocal::~BuscaLocal() {
    
}

void BuscaLocal::insercao(std::vector<Grupo> grupos, std::vector<int> &vetorY, std::vector<int> &vetorZ, std::vector<std::vector<double> > &matrizGama, bool &melhorou, double &f, double** matriz) {
    for (int v = 0; v < int(vetorY.size()); ++v) {
        for (int g = 0; g < int(vetorZ.size()); ++g) {
            if ((vetorY[v] != g) and (vetorZ[vetorY[v]] > grupos[vetorY[v]].getLimiteInferior()) and (vetorZ[g] < grupos[g].getLimiteSuperior())) {
                double deltaF = matrizGama[v][g] - matrizGama[v][vetorY[v]];
                if (deltaF > 0) {
                    atualizaMatrizGamaInsercao(matrizGama, vetorY[v], g, v, matriz);
                    
                    vetorZ[vetorY[v]] = vetorZ[vetorY[v]] - 1;
                    vetorZ[g] = vetorZ[g] + 1;
                    vetorY[v] = g;
                    f = f + deltaF;
                    
                    melhorou = true;
                }
            }
        }
    }
}

void BuscaLocal::atualizaMatrizGamaInsercao(std::vector<std::vector<double> > &matrizGama, int grupoV, int grupoU, int elementoV, double** matriz) {
    for (int u = 0; u < int(matrizGama.size()); ++u) {
        if (u != elementoV) {
            matrizGama[u][grupoV] = matrizGama[u][grupoV] - matriz[elementoV][u];
            matrizGama[u][grupoU] = matrizGama[u][grupoU] + matriz[elementoV][u];
        }
    }
    
}

void BuscaLocal::swap(std::vector<int> &vetorY, std::vector<std::vector<double> > &matrizGama, bool &melhorou, double &f, double** matriz) {
    for (int v = 0; v < (int(vetorY.size()) - 1); ++v) {
        for (int u = v + 1; u < int(vetorY.size()); ++u) {
            if (vetorY[v] != vetorY[u]) {
                double deltaF = (matrizGama[v][vetorY[u]] - matrizGama[v][vetorY[v]]) + (matrizGama[u][vetorY[v]] - matrizGama[u][vetorY[u]]) - (2 * matriz[v][u]);
                if (deltaF > 0) {
                    atualizaMatrizGamaSwap(matrizGama, vetorY[v], vetorY[u], v, u, matriz);
                    
                    swapAux(vetorY, v, u);
                    f = f + deltaF;
                    
                    melhorou = true;
                }
            }
        }
    }
}

void BuscaLocal::atualizaMatrizGamaSwap(std::vector<std::vector<double> > &matrizGama, int grupoV, int grupoU, int elementoV, int elementoU, double** matriz) {
    atualizaMatrizGamaInsercao(matrizGama, grupoV, grupoU, elementoV, matriz);
    atualizaMatrizGamaInsercao(matrizGama, grupoU, grupoV, elementoU, matriz);
}

void BuscaLocal::swapAux(std::vector<int> &vetorY, int elementoV, int elementoU) {
    int aux = vetorY[elementoV];
    vetorY[elementoV] = vetorY[elementoU];
    vetorY[elementoU] = aux;
}

void BuscaLocal::swapEmCadeia(std::vector<int> &vetorY, std::vector<std::vector<double> > &matrizGama, bool &melhorou, double &f, double** matriz) {
    for (int v = 0; v < (int(vetorY.size()) - 2); ++v) {
        for (int u = v + 1; u < (int(vetorY.size()) - 1); ++u) {
            for (int w = u + 1; w < int(vetorY.size()); ++w) {
                if ((vetorY[v] != vetorY[u]) and (vetorY[u] != vetorY[w])) {
                    double deltaF = ((matrizGama[v][vetorY[u]] - matrizGama[v][vetorY[v]])
                                    + (matrizGama[u][vetorY[w]] - matrizGama[u][vetorY[u]])
                                    + (matrizGama[w][vetorY[v]] - matrizGama[w][vetorY[w]])
                                    - (matriz[v][u] + matriz[u][w] + matriz[w][v]));
                    if (deltaF > 0) {
                        atualizaMatrizGamaSwapEmCadeia(matrizGama, vetorY[v], vetorY[u], vetorY[w], v, u, w, matriz);
                        
                        swapEmCadeiaAux(vetorY, v, u, w);
                        f = f + deltaF;
                        melhorou = true;
                    }
                }
            }
        }
    }
}

void BuscaLocal::atualizaMatrizGamaSwapEmCadeia(std::vector<std::vector<double> > &matrizGama, int grupoV, int grupoU, int grupoW,
                                                int elementoV, int elementoU, int elementoW, double** matriz) {
    atualizaMatrizGamaInsercao(matrizGama, grupoV, grupoU, elementoV, matriz);
    atualizaMatrizGamaInsercao(matrizGama, grupoU, grupoW, elementoU, matriz);
    atualizaMatrizGamaInsercao(matrizGama, grupoW, grupoV, elementoW, matriz);
}

void BuscaLocal::swapEmCadeiaAux(std::vector<int> &vetorY, int elementoV, int elementoU, int elementoW) {
    int aux = vetorY[elementoV];
    vetorY[elementoV] = vetorY[elementoW];
    vetorY[elementoW] = vetorY[elementoU];
    vetorY[elementoU] = aux;
}
