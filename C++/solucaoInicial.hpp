#ifndef SOLUCAOINICIAL_HPP
#define SOLUCAOINICIAL_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"

class SolucaoInicial {
    public:
        SolucaoInicial();
        virtual ~SolucaoInicial();
        double getSomatorioTotal();
        Grafo* leArquivo(std::string nomeArquivo);
        virtual std::vector<Grupo> gerarSolucao(Grafo* grafo) = 0;
};

#endif
