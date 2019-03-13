#include <iostream>
#include <fstream>
#include "perturbacao.hpp"
#include "buscaLocal.hpp"

Perturbacao::Perturbacao() {
    
}

Perturbacao::~Perturbacao() {
    
}

// void Perturbacao::perturbacaoFracaUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal) {
//     bool melhorou = false;
//     BuscaLocal buscaLocalAux = buscaLocal;

//     std::vector<Grupo> gruposS = solucao;
//     std::vector<Grupo> gruposS1 = solucao;

//     for (int i = 0; i < qtdIteracoes; ++i) {
//         buscaLocal.swap(melhorou);
//         buscaLocal.atualizaSolucao(grafo, gruposS);
        
//         for (int j = 0; j < grafo->getQtdElementos(); ++j) {
//             buscaLocalAux.swap(melhorou);
//             buscaLocalAux.atualizaSolucao(grafo, gruposS1);
            
//             double somatorioS = 0, somatorioS1 = 0;
//             for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
//                 somatorioS += gruposS[i].getSomatorioDistancias();
//                 somatorioS1 += gruposS1[i].getSomatorioDistancias();
//             }
            
//             if (somatorioS1 > somatorioS) {
//                 gruposS = gruposS1;
//                 buscaLocal = buscaLocalAux;
//             }
//         }
        
//         solucao = gruposS;
//     }
// }

// void Perturbacao::perturbacaoForteUmaBuscaLocal(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal) {
//     double somatorio = 0;
//     for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
//         somatorio += solucao[i].getSomatorioDistancias();
//     }
    
//     bool melhorou = false;
//     for (int i = 0; i < qtdIteracoes; ++i) {
//         buscaLocal.swap(melhorou);
//         buscaLocal.atualizaSolucao(grafo, solucao);
        
//         double somatorioS = 0;
//         for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
//             somatorioS += solucao[i].getSomatorioDistancias();
//         }
//     }
//     buscaLocal.criaVariaveis(grafo, solucao);
// }

std::vector<Grupo> Perturbacao::perturbacaoFraca(Grafo* grafo, std::vector<Grupo> s0, int qtdIteracoes, BuscaLocal &buscaLocal) {
    std::vector<Grupo> sp = s0;
    std::vector<Grupo> s;
    std::vector<Grupo> sLinha;
    srand(time(0));

    for (int i = 0; i < qtdIteracoes; ++i) {
        // int numeroAleatorio = rand() % 3;
        // if (numeroAleatorio == 0) {
            s = buscaLocal.insercao(grafo, sp);
        // }
        // else if (numeroAleatorio == 1) {
        //     buscaLocal.swap(melhorou);
        // }
        // else {
        //     buscaLocal.swapEmCadeia(melhorou);
        // }
        // buscaLocal.atualizaSolucao(grafo, gruposS);
        
        for (int j = 0; j < grafo->getQtdElementos(); ++j) {
            // numeroAleatorio = rand() % 3;
            // if (numeroAleatorio == 0) {
                sLinha = buscaLocal.insercao(grafo, sp);
            // }
            // else if (numeroAleatorio == 1) {
            //     buscaLocalAux.swap(melhorou);
            // }
            // else {
            //     buscaLocalAux.swapEmCadeia(melhorou);
            // }
            // buscaLocalAux.atualizaSolucao(grafo, gruposS1);
            
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

// void Perturbacao::perturbacaoForte(Grafo* grafo, std::vector<Grupo> &solucao, int qtdIteracoes, BuscaLocal &buscaLocal) {
//     double somatorio = 0;
//     for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
//         somatorio += solucao[i].getSomatorioDistancias();
//     }
    
//     srand(time(0));
    
//     bool melhorou = false;
//     for (int i = 0; i < qtdIteracoes; ++i) {
//         int numeroAleatorio = rand() % 3;
//         if (numeroAleatorio == 0) {
//             buscaLocal.insercao(solucao, melhorou);
//         }
//         else if (numeroAleatorio == 1) {
//             buscaLocal.swap(melhorou);
//         }
//         else {
//             buscaLocal.swapEmCadeia(melhorou);
//         }
//         buscaLocal.atualizaSolucao(grafo, solucao);
//     }
// }
