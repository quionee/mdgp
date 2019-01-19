#ifndef SOLUCAOINICIAL_HPP
#define SOLUCAOINICIAL_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"

class SolucaoInicial {
    private:
        double somatorioTotal;
        std::string nomeArquivo;
    public:
        SolucaoInicial();
        ~SolucaoInicial();
        double getSomatorioTotal();
        Grafo leArquivo(std::string nomeArquivo);
        std::vector<int> divisaoInicial(Grafo &grafo);
        std::vector<Grupo> criaGrupos(Grafo grafo, std::vector<int> elementosAleatorios);
        void limiteInferior(Grafo &grafo, std::vector<Grupo> &grupos, double** matriz);
        void limiteSuperior(Grafo &grafo, std::vector<Grupo> &grupos, double** matriz);
};

#endif
