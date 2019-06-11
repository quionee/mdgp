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

void imprimeInformacoesDeUmGrupo(vector<Grupo> &solucao) {
    for (unsigned int i = 0; i < solucao.size(); ++i) {
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
}

void realizaSomatorioDaSolucao(double &somatorioTotal, vector<Grupo> &solucao) {
    somatorioTotal = 0;
    // imprimeInformacoesDeUmGrupo(solucao);
    for (unsigned int i = 0; i  < solucao.size(); ++i) {
        somatorioTotal += solucao[i].getSomatorioDistancias();
    }
}

vector<Grupo> buscaMaxima(Grafo* grafo, vector<Grupo> &s0, int &alfa) {
    int qtdIteracoes = 3;
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

vector<Grupo> melhoria(Grafo* grafo, vector<Grupo> &solucao, time_t &duracao, time_t &tempoMelhorSolucao) {
    int alfa = 3;
    if ((grafo->getQtdElementos() <= 400) or ((grafo->getQtdElementos() / grafo->getQtdGrupos()) <= 10)) {
        alfa = 5;
    }

    Perturbacao perturbacao;
    BuscaLocal buscaLocal(grafo);
    int qtdIteracoes = (grafo->getQtdElementos() / grafo->getQtdGrupos());

    vector<Grupo> solucaoAux = solucao;

    // tempo limite de execução em segundos
    time_t tempo;
    if (grafo->getQtdElementos() == 120) {
        tempo = 3;
    }
    else if (grafo->getQtdElementos() == 240) {
        tempo = 20;
    }
    else if (grafo->getQtdElementos() == 480) {
        tempo = 120;
    }
    else {
        tempo = 600;
    }

    // double melhorSolucao = 0;
    time_t tempoInicial = time(NULL);
    time_t tempoFinal;
    // int contador = 0;
    do {
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

        tempoFinal = time(NULL);
    } while (difftime(tempoFinal, tempoInicial) <= tempo);

    duracao = difftime(tempoFinal, tempoInicial);
    return solucaoAux;
}

double solucaoInicialLH(string &nomeArquivo, time_t &duracao, time_t &tempoMelhorSolucaoLH) {
    cout << "\n\n---------- Metodo LH: ----------\n\n";
    
    int qtdSolucoesFactiveis = 10;

    MetodoLH* solucaoInicial = new MetodoLH(qtdSolucoesFactiveis);
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);
    
    // imprimeInformacoesDeUmGrupo(solucao);

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\nSolucao inicial: " << somatorioTotal;

    solucao = melhoria(grafo, solucao, duracao, tempoMelhorSolucaoLH);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\n\nSomatorio final: " << somatorioTotal << "\n\n";

    // imprimeInformacoesDeUmGrupo(solucao);

    return somatorioTotal;
}

double solucaoInicialGC(string &nomeArquivo, time_t &duracao, time_t &tempoMelhorSolucaoGC) {
    cout << "\n\n---------- Metodo GC: ----------\n\n";

    MetodoGC* solucaoInicial = new MetodoGC();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);

    // imprimeInformacoesDeUmGrupo(solucao);

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\nSolucao inicial: " << somatorioTotal;

    solucao = melhoria(grafo, solucao, duracao, tempoMelhorSolucaoGC);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\n\nSomatorio final: " << somatorioTotal << "\n\n";

    // imprimeInformacoesDeUmGrupo(solucao);

    return somatorioTotal;
}

double solucaoInicialWJ(string &nomeArquivo, time_t &duracao, time_t &tempoMelhorSolucaoWJ) {
    cout << "\n\n---------- Metodo WJ: ----------\n\n";

    MetodoWJ* solucaoInicial = new MetodoWJ();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\nSolucao inicial: " << somatorioTotal;
    
    solucao = melhoria(grafo, solucao, duracao, tempoMelhorSolucaoWJ);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao);
    cout << "\n\nSomatorio final: " << somatorioTotal << "\n\n";

    // imprimeInformacoesDeUmGrupo(solucao);

    return somatorioTotal;
}

void executa(string &nomeArquivo, fstream &arquivoEscrita) {
    time_t duracaoLH = 0, duracaoGC = 0, duracaoWJ = 0,
           tempoMelhorSolucaoLH = 0, tempoMelhorSolucaoGC = 0, tempoMelhorSolucaoWJ = 0;
    double solucaoLH = solucaoInicialLH(nomeArquivo, duracaoLH, tempoMelhorSolucaoLH);
    double solucaoGC = solucaoInicialGC(nomeArquivo, duracaoGC, tempoMelhorSolucaoGC);
    double solucaoWJ = solucaoInicialWJ(nomeArquivo, duracaoWJ, tempoMelhorSolucaoWJ);

    double media = (solucaoLH + solucaoGC + solucaoWJ) / 3;

    arquivoEscrita << "\n     Arquivo " << nomeArquivo << "\n"
                   << "\nsolucao LH: " << solucaoLH << "     duracao LH: " << duracaoLH
                   << "\nsolucao GC: " << solucaoGC << "     duracao GC: " << duracaoGC
                   << "\nsolucao WJ: " << solucaoWJ << "     duracao WJ: " << duracaoWJ
                   << "\n\nMedia: " << media << "\n\n";

    cout << "\n     Arquivo " << nomeArquivo << "\n"
         << "\nsolucao LH: " << solucaoLH << "     duracao LH: " << duracaoLH
         << "\nsolucao GC: " << solucaoGC << "     duracao GC: " << duracaoGC
         << "\nsolucao WJ: " << solucaoWJ << "     duracao WJ: " << duracaoWJ
         << "\n\nMedia: " << media << "\n\n";
}

int main() {
    fstream arquivoEscrita;
    arquivoEscrita.open("arquivo1.txt");
    for(int i = 1; i <= 4; ++i) {
        // conjunto INT
        string nomeArquivoLeitura = (to_string(i) + "_int.txt");
        executa(nomeArquivoLeitura, arquivoEscrita);

        // conjunto REAL
        nomeArquivoLeitura = (to_string(i) + "_real.txt");
        executa(nomeArquivoLeitura, arquivoEscrita);

        // conjunto GEO
        nomeArquivoLeitura = (to_string(i) + "_geo.txt");
        executa(nomeArquivoLeitura, arquivoEscrita);
    }
    arquivoEscrita.close();

    return 0;
}