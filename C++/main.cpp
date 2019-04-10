#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "grafo.hpp"
#include "grupo.hpp"
#include "solucaoInicial.hpp"
#include "metodoLH.hpp"
#include "metodoGC.hpp"
#include "metodoWJ.hpp"
#include "buscaLocal.hpp"
#include "perturbacao.hpp"

using namespace std;

void imprimeInformacoesDeUmGrupo(vector<Grupo> solucao, int i) {
    cout << "\nGRUPO " << (i + 1)
         << "\nlimiteInferior: " << solucao[i].getLimiteInferior()
         << "\nlimiteSuperior: " << solucao[i].getLimiteSuperior()
         << "\nqtdElementos: " << solucao[i].getQtdElementos()
         << "\nelementos: ";
         
    for (unsigned int j = 0; j < solucao[i].getElementos().size(); ++j) {
        cout << solucao[i].getElemento(j) << " ";
    }
    
    cout << "\narestas: ";
    vector<vector<int> >  arestasElementos = solucao[i].getArestasElementos();
    vector<double> arestasValor = solucao[i].getArestasValor();
    for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
        cout << arestasElementos[j][0] << ", "
             << arestasElementos[j][1] << ", "
             << arestasValor[j] << "; ";
    }
    cout << "\nsomatorioDistancias: " << solucao[i].getSomatorioDistancias() << "\n";
}

void realizaSomatorioDaSolucao(double &somatorioTotal, vector<Grupo> solucao) {
    somatorioTotal = 0;
    for (unsigned int i = 0; i  < solucao.size(); ++i) {
        // para printar todas as informacoes dos solucao, basta descomentar a linha abaixo
        //~ imprimeInformacoesDeUmGrupo(solucao, i);
        somatorioTotal += solucao[i].getSomatorioDistancias();
    }
}

vector<Grupo> buscaMaxima(Grafo* grafo, vector<Grupo> s0, int alfa) {
    int qtdIteracoes = 1;
    vector<Grupo> s = s0;
    BuscaLocal buscaLocal(grafo);

    vector<Grupo> sb = buscaLocal.buscaLocal(grafo, s);

    int contador = 0;
    Perturbacao perturbacao;

    while (contador < alfa) {
        s = perturbacao.perturbacaoFraca(grafo, sb, qtdIteracoes, buscaLocal);
        s = buscaLocal.buscaLocal(grafo, s);

        double somatorioS = 0, somatorioSb = 0;
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            somatorioS += s[i].getSomatorioDistancias();
            somatorioSb += sb[i].getSomatorioDistancias();
        }

        if (somatorioS > somatorioSb) {
            sb = s;
            contador = 0;
        }
        else {
            ++contador;
        }
    }

    return sb;
}

vector<Grupo> melhoria(Grafo* grafo, vector<Grupo> solucao) {
    int alfa = 5;

    Perturbacao perturbacao;
    BuscaLocal buscaLocal(grafo);
    int qtdIteracoes = 2 * (grafo->getQtdElementos() / grafo->getQtdGrupos());

    vector<Grupo> solucaoAux = solucao;

    int tempo = 0;
    while (tempo < 3) {
        solucao = buscaMaxima(grafo, solucao, alfa);

        double somatorioS = 0, somatorioSAux = 0;
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            somatorioS += solucao[i].getSomatorioDistancias();
            somatorioSAux += solucaoAux[i].getSomatorioDistancias();
        }

        if (somatorioS > somatorioSAux) {
            solucaoAux = solucao;
        }
        solucao = perturbacao.perturbacaoForte(grafo, solucao, qtdIteracoes, buscaLocal);
        tempo += 1;
    }
    return solucaoAux;

    // double melhorResultado = 0;
    // for (int i = 0; i < 5; ++i) {
    //     cout << "\n\n---------- Iteração " << i << " ----------\n\n";
    //     solucao = buscaMaxima(grafo, solucao, alfa);
    //     double somatorioTotal = 0;
    //     realizaSomatorioDaSolucao(somatorioTotal, solucao);
    //     cout << "\n     Somatorio apos busca maxima: " << somatorioTotal;

    //     if (somatorioTotal > melhorResultado) {
    //         melhorResultado = somatorioTotal;
    //     }

    //     // cout << "\n\nForca da perturbacao forte: ";
    //     int qtdIteracoes = 2 * (grafo->getQtdElementos() / grafo->getQtdGrupos());
    //     // cin >> qtdIteracoes;
    //     Perturbacao perturbacao;
    //     BuscaLocal buscaLocal(grafo);
    //     solucao = perturbacao.perturbacaoForte(grafo, solucao, qtdIteracoes, buscaLocal);
    //     somatorioTotal = 0;
    //     realizaSomatorioDaSolucao(somatorioTotal, solucao);
    //     cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n";

    //     if (somatorioTotal > melhorResultado) {
    //         melhorResultado = somatorioTotal;
    //     }

    //     // cout << "\n\nAlfa: ";
    //     // cin >> alfa;
    //     solucao = buscaMaxima(grafo, solucao, alfa);
    //     somatorioTotal = 0;
    //     realizaSomatorioDaSolucao(somatorioTotal, solucao);
    //     cout << "     Somatorio apos busca maxima: " << somatorioTotal;

    //     if (somatorioTotal > melhorResultado) {
    //         melhorResultado = somatorioTotal;
    //     }
    // }
    // cout << "\n\nMelhor resultado: " << melhorResultado << "\n\n";
    // return solucao;
}

void solucaoInicialLH(string nomeArquivo) {
    cout << "\n\n---------- Metodo LH: ----------\n\n";
    
    int qtdSolucoesFactiveis = 2;
    // cout << "Quantidade de solucoes factiveis a serem testadas: ";
    // cin >> qtdSolucoesFactiveis;
    MetodoLH* solucaoInicial = new MetodoLH(qtdSolucoesFactiveis);
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\nSolucao inicial: " << somatorioTotal;

    solucao = melhoria(grafo, solucao);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\n\nSomatorio final: " << somatorioTotal << "\n\n";

    // for(int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     imprimeInformacoesDeUmGrupo(solucao, i);
    // }
}

void solucaoInicialGC(string nomeArquivo) {
    cout << "\n\n---------- Metodo GC: ----------\n\n";

    MetodoGC* solucaoInicial = new MetodoGC();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);
    
    // for(int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     imprimeInformacoesDeUmGrupo(solucao, i);
    // }

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\nSolucao inicial: " << somatorioTotal;

    solucao = melhoria(grafo, solucao);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\n\nSomatorio final: " << somatorioTotal << "\n\n";

    // for(int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     imprimeInformacoesDeUmGrupo(solucao, i);
    // }
}

void solucaoInicialWJ(string nomeArquivo) {
    cout << "\n\n---------- Metodo WJ: ----------\n\n";

    MetodoWJ* solucaoInicial = new MetodoWJ();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\nSolucao inicial: " << somatorioTotal;
    
    solucao = melhoria(grafo, solucao);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\n\nSomatorio final: " << somatorioTotal << "\n\n";

    // for(int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     imprimeInformacoesDeUmGrupo(solucao, i);
    // }
}

int main() {
    // string nomeArquivo;
    // cout << "Nome do arquivo: ";
    // cin >> nomeArquivo;

    // for (int j = 0; j < 5; ++j) {
        // for(int i = 9; i <= 16; ++i) {
           string nomeArquivo = (to_string(13) + "r");
           cout << "\n---- arquivo " << nomeArquivo << " ----";
           solucaoInicialLH(nomeArquivo);
           solucaoInicialGC(nomeArquivo);
           solucaoInicialWJ(nomeArquivo);
        // }
    // }

    return 0;
}