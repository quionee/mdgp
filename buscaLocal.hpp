#ifndef BUSCALOCAL_HPP
#define BUSCALOCAL_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"

class BuscaLocal {
    private:
        std::string buscaLocal;
        std::string buscaLocalProx;
        std::vector<int> vetorY;
        std::vector<int> vetorZ;
        std::vector<std::vector<double> > matrizGama;
        double** matriz;

        void atualizaMatrizGamaInsercao(int &grupoI, int &grupoJ, int &elementoV);
        void atualizaMatrizGamaSwap(int &grupoI, int &grupoJ, int &elementoV, int &elementoU);
        void swapAux(int &elementoV, int &elementoU);
        void atualizaMatrizGamaSwapEmCadeia(int &grupoV, int &grupoU, int &grupoW, int &elementoV, int &elementoU, int &elementoW);
        void swapEmCadeiaAux(int &elementoV, int &elementoU, int &elementoW);
    public:
        BuscaLocal(Grafo* grafo, std::string buscaLocal, std::string buscaLocalProx);
        BuscaLocal(Grafo* grafo);
        ~BuscaLocal();
        
        std::vector<int> getVetorY();
        std::vector<int> getVetorZ();
        std::vector<std::vector<double> > getMatrizGama();

        void reiniciaVariaveis();
        void criaVariaveis(Grafo* grafo, std::vector<Grupo> &solucao);
        void arrumaGrupos(int &grupo1, int &grupo2, int &grupo3);
        void realizaBuscaLocal(Grafo *&grafo, std::string &buscaLocal, std::vector<Grupo> solucao, 
                               bool &melhorou);

        std::vector<Grupo> busca(Grafo* grafo, std::vector<Grupo> solucao);

        std::vector<Grupo> insercao(Grafo* grafo, std::vector<Grupo> solucao);
        void insercaoAlgoritmo(std::vector<Grupo> grupos, bool &melhorou);
        std::vector<Grupo> insercaoSemValorDoMovimento(Grafo* grafo, std::vector<Grupo> solucao);
        void insercaoSemValorDoMovimentoAlgoritmo(std::vector<Grupo> grupos);

        std::vector<Grupo> swap(Grafo* grafo, std::vector<Grupo> solucao);
        void swapAlgoritmo(bool &melhorou);
        std::vector<Grupo> swapSemValorDoMovimento(Grafo* grafo, std::vector<Grupo> solucao);
        void swapSemValorDoMovimentoAlgoritmo();

        std::vector<Grupo> swapEmCadeia(Grafo* grafo, std::vector<Grupo> solucao);
        void swapEmCadeiaAlgoritmo(bool &melhorou);
        std::vector<Grupo> swapEmCadeiaSemValorDoMovimento(Grafo* grafo, std::vector<Grupo> solucao);
        void swapEmCadeiaSemValorDoMovimentoAlgoritmo();

        void atualizaSolucao(Grafo* grafo, std::vector<Grupo> &grupos);
};

#endif

