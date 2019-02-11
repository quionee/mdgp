#ifndef BUSCALOCAL_HPP
#define BUSCALOCAL_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"

class BuscaLocal {
    public:
        BuscaLocal();
        ~BuscaLocal();
        void insercao(std::vector<Grupo> grupos, std::vector<int> &vetorY, std::vector<int> &vetorZ, std::vector<std::vector<double> > &matrizGama, bool &melhorou, double &f, double** matriz);
        void atualizaMatrizGamaInsercao(std::vector<std::vector<double> > &matrizGama, int grupoI, int grupoJ, int elementoV, double** matriz);
        void swap(std::vector<int> &vetorY, std::vector<std::vector<double> > &matrizGama, bool &melhorou, double &f, double** matriz);
        void atualizaMatrizGamaSwap(std::vector<std::vector<double> > &matrizGama, int grupoI, int grupoJ, int elementoV, int elementoU, double** matriz);
        void swapAux(std::vector<int> &vetorY, int elementoV, int elementoU);
        void swapEmCadeia(std::vector<int> &vetorY, std::vector<std::vector<double> > &matrizGama, bool &melhorou, double &f, double** matriz);
        void atualizaMatrizGamaSwapEmCadeia(std::vector<std::vector<double> > &matrizGama, int grupoV, int grupoU, int grupoW, int elementoV, int elementoU, int elementoW, double** matriz);
        void swapEmCadeiaAux(std::vector<int> &vetorY, int elementoV, int elementoU, int elementoW);
};

#endif

