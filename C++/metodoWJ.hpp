#ifndef METODOWJ_HPP
#define METODOWJ_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"
#include "solucaoInicial.hpp"

class MetodoWJ : public SolucaoInicial {
    public:
        MetodoWJ();
        ~MetodoWJ();
        std::vector<Grupo> gerarSolucao(Grafo* grafo);
        std::vector<Grupo> criaGrupos(Grafo* grafo, std::vector<int> elementosSemGrupo);
        void atualizaGrupo(Grupo& grupo, double** matriz, int elemento);
        void atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo);
};

#endif

