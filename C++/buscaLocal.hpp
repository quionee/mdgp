#ifndef BUSCALOCAL_HPP
#define BUSCALOCAL_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"

class BuscaLocal {
    private:
        std::vector<int> vetorY;
        std::vector<int> vetorZ;
        std::vector<std::vector<double> > matrizGama;
        double** matriz;

        void atualizaMatrizGamaInsercao(int grupoI, int grupoJ, int elementoV);
        void atualizaMatrizGamaSwap(int grupoI, int grupoJ, int elementoV, int elementoU);
        void swapAux(int elementoV, int elementoU);
        void atualizaMatrizGamaSwapEmCadeia(int grupoV, int grupoU, int grupoW, int elementoV, int elementoU, int elementoW);
        void swapEmCadeiaAux(int elementoV, int elementoU, int elementoW);
    public:
        BuscaLocal(Grafo* grafo);
        ~BuscaLocal();
        
        void reiniciaVariaveis();

        void criaVariaveis(Grafo* grafo, std::vector<Grupo> solucao);

        std::vector<Grupo> buscaLocal(Grafo* grafo, std::vector<Grupo> solucao);

        std::vector<Grupo> insercao(Grafo* grafo, std::vector<Grupo> solucao);
        void insercaoAlgoritmo(std::vector<Grupo> grupos, bool &melhorou);

        std::vector<Grupo> swap(Grafo* grafo, std::vector<Grupo> solucao);
        void swapAlgoritmo(bool &melhorou);

        std::vector<Grupo> swapEmCadeia(Grafo* grafo, std::vector<Grupo> solucao);
        void swapEmCadeiaAlgoritmo(bool &melhorou);

        void atualizaSolucao(Grafo* grafo, std::vector<Grupo> &grupos);
};

#endif

