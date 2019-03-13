#ifndef METODOGC_HPP
#define METODOGC_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"
#include "solucaoInicial.hpp"

class MetodoGC : public SolucaoInicial {
    public:
        MetodoGC();
        ~MetodoGC();
        std::vector<Grupo> gerarSolucao(Grafo* grafo);
        void atualizaGrupo(Grupo &grupo, int elemento, double** matriz);
        void atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo);
        bool decideQualGrupoMaximizar(int elemento, double &somatorioGrupoAtual, int &i, std::vector<Grupo> grupos, int limite, double** matriz, int &grupoMaximizado, double &maiorSomatorio);
        std::vector<int> divisaoInicial(Grafo* grafo);
        std::vector<Grupo> criaGrupos(Grafo* grafo, std::vector<int> elementosAleatorios);
        void limiteInferior(Grafo* grafo, std::vector<Grupo> &grupos, double** matriz);
        void limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupos, double** matriz);
};

#endif

