#ifndef METODOLH_HPP
#define METODOLH_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"
#include "solucaoInicial.hpp"

class MetodoLH : public SolucaoInicial {
    private:
        int qtdSolucoesFactiveis;
    public:
        MetodoLH(int qtdSolucoesFactiveis);
        ~MetodoLH();
        std::vector<Grupo> gerarSolucao(Grafo* grafo);
        void limiteInferior(Grafo* grafo, Grupo &grupo, int &limPosicoesDisponiveis, double** matriz);
        void limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupo, std::vector<int> &gruposNaoCheios, double** matriz);
        void atualizaGrupo(Grupo &grupo, double** matriz, int elemento);
        void atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo);
};

#endif
