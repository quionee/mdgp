#ifndef PERTUBACAO_HPP
#define PERTUBACAO_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"
#include "buscaLocal.hpp"

class Perturbacao {
    public:
        Perturbacao();
        ~Perturbacao();
        void perturbacaoFracaUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
        void perturbacaoForteUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
        std::vector<Grupo> perturbacaoFraca(Grafo* grafo, std::vector<Grupo> solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
        std::vector<Grupo> perturbacaoForte(Grafo* grafo, std::vector<Grupo> solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
};

#endif
