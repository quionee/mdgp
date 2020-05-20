#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <stdexcept>

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

vector<Grupo> buscaMaxima(Grafo* grafo, vector<Grupo> &s0, int &alfa, BuscaLocal busca, 
                          Perturbacao perturbacao) {
    int qtdIteracoes = 3;
    vector<Grupo> s = s0;

    vector<Grupo> sb = busca.busca(grafo, s);
    // vector<Grupo> sb = buscaLocal(busca, buscaLocalImp, busca)

    int contador = 0;

    while (contador < alfa) {
        s = perturbacao.perturbacaoFraca(grafo, sb, qtdIteracoes, busca);
        s = busca.busca(grafo, s);

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

vector<Grupo> melhoria(Grafo* grafo, vector<Grupo> &solucao, string &buscaLocal, 
                       string &buscaLocalProx, string &buscaLocalPer, string &buscaLocalPerProx, time_t &duracao, time_t &tempoMelhorSolucao) {
    int alfa = 3;
    if ((grafo->getQtdElementos() <= 400) or ((grafo->getQtdElementos() / grafo->getQtdGrupos()) <=     10)) {
        alfa = 5;
    }

    Perturbacao perturbacao(buscaLocalPer, buscaLocalPerProx);
    BuscaLocal busca(grafo, buscaLocal, buscaLocalProx);
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
        solucao = buscaMaxima(grafo, solucao, alfa, busca, perturbacao);

        double somatorioS = 0, somatorioSAux = 0;
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            somatorioS += solucao[i].getSomatorioDistancias();
            somatorioSAux += solucaoAux[i].getSomatorioDistancias();
        }

        if (somatorioS > somatorioSAux) {
            solucaoAux = solucao;
        }
        solucao = perturbacao.perturbacaoForte(grafo, solucao, qtdIteracoes, busca);

        tempoFinal = time(NULL);
    } while (difftime(tempoFinal, tempoInicial) <= tempo);

    duracao = difftime(tempoFinal, tempoInicial);
    return solucaoAux;
}

vector<Grupo> solucaoInicialLH(string &nomeArquivo, Grafo* &grafo) {
    // cout << "\n\n---------- Metodo LH: ----------\n\n";
    
    int qtdSolucoesFactiveis = 10;

    MetodoLH* solucaoInicial = new MetodoLH(qtdSolucoesFactiveis);

    grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);

    // imprimeInformacoesDeUmGrupo(solucao);
    
    return solucao;
}

vector<Grupo> solucaoInicialGC(string &nomeArquivo, Grafo* &grafo) {
    // cout << "\n\n---------- Metodo GC: ----------\n\n";

    MetodoGC* solucaoInicial = new MetodoGC();
    
    grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);

    // imprimeInformacoesDeUmGrupo(solucao);

    return solucao;
}

vector<Grupo> solucaoInicialWJ(string &nomeArquivo, Grafo* &grafo) {
    // cout << "\n\n---------- Metodo WJ: ----------\n\n";

    MetodoWJ* solucaoInicial = new MetodoWJ();
    
    grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> solucao = solucaoInicial->gerarSolucao(grafo);

    // imprimeInformacoesDeUmGrupo(solucao);

    return solucao;
}

void lh(string &nomeArquivo, string &buscaLocal, string &buscaLocalProx, string &buscaLocalPer, 
        string &buscaLocalPerProx, int numMax, vector<Grupo> &solucao) {
    Grafo* grafo;
    solucao = solucaoInicialLH(nomeArquivo, grafo);

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao); 
    // cout << "\nSolucao inicial: " << somatorioTotal << endl;

    time_t duracao, tempoMelhorSolucao;

    solucao = melhoria(grafo, solucao, buscaLocal, buscaLocalProx, buscaLocalPer, buscaLocalPerProx,                   duracao, tempoMelhorSolucao);

    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao); 
    // cout << "\nSolucao melhorada: " << somatorioTotal << endl;
    cout << somatorioTotal;

}

void gc(string &nomeArquivo, string &buscaLocal, string &buscaLocalProx, string &buscaLocalPer, 
        string &buscaLocalPerProx, int numMax, vector<Grupo> &solucao) {
    Grafo* grafo;
    solucao = solucaoInicialGC(nomeArquivo, grafo);

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao); 
    // cout << "\nSolucao inicial: " << somatorioTotal << endl;

    time_t duracao, tempoMelhorSolucao;

    solucao = melhoria(grafo, solucao, buscaLocal, buscaLocalProx, buscaLocalPer, buscaLocalPerProx,                   duracao, tempoMelhorSolucao);

    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao); 
    // cout << "\nSolucao melhorada: " << somatorioTotal << endl;
    cout << somatorioTotal;
}

void wj(string &nomeArquivo, string &buscaLocal, string &buscaLocalProx, string &buscaLocalPer, 
        string &buscaLocalPerProx, int numMax, vector<Grupo> &solucao) {
    Grafo* grafo;
    solucao = solucaoInicialWJ(nomeArquivo, grafo);

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao); 
    // cout << "\nSolucao inicial: " << somatorioTotal << endl;

    time_t duracao, tempoMelhorSolucao;

    solucao = melhoria(grafo, solucao, buscaLocal, buscaLocalProx, buscaLocalPer, buscaLocalPerProx,                   duracao, tempoMelhorSolucao);

    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, solucao); 
    // cout << "\nSolucao melhorada: " << somatorioTotal << endl;
    cout << somatorioTotal;
}

int main(int argc, char *argv[]) {
    string nomeArquivo = argv[2];

    string solucaoInicial(argv[6]);
    string buscaLocal(argv[8]), buscaLocalProx(argv[10]);
    string buscaLocalPer(argv[12]), buscaLocalPerProx(argv[14]);
    string num(argv[16]);

    // string solucaoInicial(argv[6]);
    // string buscaLocal(argv[8]), buscaLocalInsertion(argv[10]), buscaLocalSwap(argv[12]);
    // string buscaLocalThreeChain(argv[14]), buscaLocalPer(argv[16]);
    // string buscaLocalPerInsertion(argv[18]), buscaLocalPerSwap(argv[20]);
    // string buscaLocalPerThreeChain(argv[22]), num(argv[24]);

    // string nomeArquivo = "Instances/int_ds_120.txt";
    // string solucaoInicial = "wj", buscaLocal = "insertionImp", buscaLocalInsertion = "none", buscaLocalSwap = "none", buscaLocalThreeChain = "none", buscaLocalPer = "insertion", buscaLocalPerInsertion = "none", buscaLocalPerSwap = "insertion", buscaLocalPerThreeChain = "none";
    // int numMax = 5;

    // string buscaLocalProx, buscaLocalPerProx;

    // if (buscaLocal == "insertionImp") {
    //     buscaLocalProx = buscaLocalInsertion;
    // }
    // else if (buscaLocal == "swapImp") {
    //     buscaLocalProx = buscaLocalSwap;
    // }
    // else {
    //     buscaLocalProx = buscaLocalThreeChain;
    // }

    // if (buscaLocalPer == "insertion") {
    //     buscaLocalPerProx = buscaLocalPerInsertion;
    // }
    // else if (buscaLocalPer == "swap") {
    //     buscaLocalPerProx = buscaLocalPerSwap;
    // }
    // else {
    //     buscaLocalPerProx = buscaLocalPerThreeChain;
    // }

    int numMax = 0;
    try {
        size_t pos;
        numMax = stoi(num, &pos);
        if (pos < num.size()) {
            cerr << "Trailing characters after number: " << num << '\n';
        }
    } catch (invalid_argument const &ex) {
        cerr << "Invalid number: " << num << '\n';
    } catch (out_of_range const &ex) {
        cerr << "Number out of range: " << num << '\n';
    }

    vector<Grupo> solucao;

    if (solucaoInicial == "lh") {
        lh(nomeArquivo, buscaLocal, buscaLocalProx, buscaLocalPer, buscaLocalPerProx, numMax, 
           solucao);
    }
    else if (solucaoInicial == "gc") {
        gc(nomeArquivo, buscaLocal, buscaLocalProx, buscaLocalPer, buscaLocalPerProx, numMax, 
           solucao);
    }
    else if (solucaoInicial == "wj") {
        wj(nomeArquivo, buscaLocal, buscaLocalProx, buscaLocalPer, buscaLocalPerProx, numMax, 
           solucao);
    }

    return 0;
}