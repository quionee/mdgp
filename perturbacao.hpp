#ifndef PERTUBACAO_HPP
#define PERTUBACAO_HPP

#include <vector>
#include <cstdlib>
#include <ctime>
#include "grafo.hpp"
#include "grupo.hpp"
#include "buscaLocal.hpp"

class Perturbacao {
    private:
        std::string buscaLocalPer;
        std::string buscaLocalPerProx;
    public:
        Perturbacao(std::string buscaLocalPer, std::string buscaLocalPerProx);
        ~Perturbacao();
        std::vector<Grupo> perturbacaoFracaUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
        std::vector<Grupo> perturbacaoForteUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal);

        std::vector<Grupo> realizaBuscaLocal(BuscaLocal &busca, std::string &buscaLocalPer, Grafo *&grafo, std::vector<Grupo> solucao);

        std::vector<Grupo> perturbacaoFraca(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
        std::vector<Grupo> perturbacaoForte(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal);
};

#endif
