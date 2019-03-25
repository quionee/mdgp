#include <iostream>
#include <fstream>
#include "perturbacao.hpp"
#include "buscaLocal.hpp"

Perturbacao::Perturbacao() {
    
}

Perturbacao::~Perturbacao() {
    
}

// método de perturbação fraca com uma busca local apenas.
std::vector<Grupo> Perturbacao::perturbacaoFracaUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> s0, int qtdIteracoes, BuscaLocal &buscaLocal) {
    std::vector<Grupo> sp = s0;
    std::vector<Grupo> s;
    std::vector<Grupo> sLinha;

    for (int i = 0; i < qtdIteracoes; ++i) {
        s = buscaLocal.insercao(grafo, sp);
        // s = buscaLocal.swap(grafo, sp);
        // s = buscaLocal.swapEmCadeia(grafo, sp);
        
        for (int j = 0; j < grafo->getQtdElementos(); ++j) {
            sLinha = buscaLocal.insercao(grafo, sp);
            // sLinha = buscaLocal.swap(grafo, sp);
            // sLinha = buscaLocal.swapEmCadeia(grafo, sp);
            
            double somatorioS = 0, somatorioS1 = 0;
            for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
                somatorioS += s[i].getSomatorioDistancias();
                somatorioS1 += sLinha[i].getSomatorioDistancias();
            }
            
            if (somatorioS1 > somatorioS) {
                s = sLinha;
            }
        }
        sp = s;
    }
    return sp;
}

// método de perturbação forte com uma busca local apenas.
std::vector<Grupo> Perturbacao::perturbacaoForteUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> s0, int qtdIteracoes, BuscaLocal &buscaLocal) {
    std::vector<Grupo> sp = s0;
    std::vector<Grupo> s;

    for (int i = 0; i < qtdIteracoes; ++i) {
        s = buscaLocal.insercao(grafo, sp);
        // s = buscaLocal.swap(grafo, sp);
        // s = buscaLocal.swapEmCadeia(grafo, sp);
        sp = s;
    }
    return sp;
}

// método de perturbação fraca com possibilidade das três buscas locais.
std::vector<Grupo> Perturbacao::perturbacaoFraca(Grafo* grafo, std::vector<Grupo> s0, int qtdIteracoes, BuscaLocal &buscaLocal) {
    std::vector<Grupo> sp = s0;
    std::vector<Grupo> s;
    std::vector<Grupo> sLinha;
    srand(time(0));

    for (int i = 0; i < qtdIteracoes; ++i) {
        // int numeroAleatorio = rand() % 3;

        int numeroAleatorio = rand() % 2;
        if (numeroAleatorio == 0) {
            s = buscaLocal.insercao(grafo, sp);
        }
        else {
            s = buscaLocal.swap(grafo, sp);
        }
        // else if (numeroAleatorio == 1) {
        //     s = buscaLocal.swap(grafo, sp);
        // }
        // else {
        //     s = buscaLocal.swapEmCadeia(grafo, sp);
        // }
        
        for (int j = 0; j < grafo->getQtdElementos(); ++j) {
            // numeroAleatorio = rand() % 3;

            numeroAleatorio = rand() % 2;
            if (numeroAleatorio == 0) {
                sLinha = buscaLocal.insercao(grafo, sp);
            }
            else {
                sLinha = buscaLocal.swap(grafo, sp);
            }
            // else if (numeroAleatorio == 1) {
            //     sLinha = buscaLocal.swap(grafo, sp);
            // }
            // else {
            //     sLinha = buscaLocal.swapEmCadeia(grafo, sp);
            // }
            
            double somatorioS = 0, somatorioS1 = 0;
            for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
                somatorioS += s[i].getSomatorioDistancias();
                somatorioS1 += sLinha[i].getSomatorioDistancias();
            }
            
            if (somatorioS1 > somatorioS) {
                s = sLinha;
            }
        }
        sp = s;
    }
    return sp;
}

// método de perturbação forte com possibilidade das três buscas locais.
std::vector<Grupo> Perturbacao::perturbacaoForte(Grafo* grafo, std::vector<Grupo> s0, int qtdIteracoes, BuscaLocal &buscaLocal) {
    srand(time(0));
    
    std::vector<Grupo> sp = s0;
    std::vector<Grupo> s;

    for (int i = 0; i < qtdIteracoes; ++i) {
        // int numeroAleatorio = rand() % 3;

        int numeroAleatorio = rand() % 2;
        if (numeroAleatorio == 0) {
            s = buscaLocal.insercao(grafo, sp);
        }
        else {
            s = buscaLocal.swap(grafo, sp);
        }
        // else if (numeroAleatorio == 1) {
        //     s = buscaLocal.swap(grafo, sp);
        // }
        // else {
        //     s = buscaLocal.swapEmCadeia(grafo, sp);
        // }
        sp = s;
    }
    return sp;
}
