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

void imprimeInformacoesDeUmGrupo(vector<Grupo> grupos, int i) {
    cout << "\nGRUPO " << (i + 1)
         << "\nlimiteInferior: " << grupos[i].getLimiteInferior()
         << "\nlimiteSuperior: " << grupos[i].getLimiteSuperior()
         << "\nqtdElementos: " << grupos[i].getQtdElementos()
         << "\nelementos: ";
         
    for (unsigned int j = 0; j < grupos[i].getElementos().size(); ++j) {
        cout << grupos[i].getElemento(j) << " ";
    }
    
    cout << "\narestas: ";
    vector<vector<int> >  arestasElementos = grupos[i].getArestasElementos();
    vector<double> arestasValor = grupos[i].getArestasValor();
    for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
        cout << arestasElementos[j][0] << ", "
             << arestasElementos[j][1] << ", "
             << arestasValor[j] << "; ";
    }
    cout << "\nsomatorioDistancias: " << grupos[i].getSomatorioDistancias() << "\n";
}

void realizaSomatorioDaSolucao(double &somatorioTotal, vector<Grupo> grupos) {
    somatorioTotal = 0;
    for (unsigned int i = 0; i  < grupos.size(); ++i) {
        // para printar todas as informacoes dos grupos, basta descomentar a linha abaixo
        //~ imprimeInformacoesDeUmGrupo(grupos, i);
        somatorioTotal += grupos[i].getSomatorioDistancias();
    }
}

vector<Grupo> buscaMaxima(Grafo* grafo, vector<Grupo> s0, int alfa) {
    int qtdIteracoes = 1;
    // cout << "Forca da perturbacao fraca: ";
    // cin >> qtdIteracoes;;

    vector<Grupo> s = s0;
    BuscaLocal buscaLocal(grafo);

    vector<Grupo> sb = buscaLocal.buscaLocal(grafo, s);

    // double somatorioTeste1 = 0;
    // for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     somatorioTeste1 += sb[i].getSomatorioDistancias();
    // }
    // cout << "\nsomatorioTeste1: " << somatorioTeste1;

    int contador = 0;
    Perturbacao perturbacao;

    while (contador < alfa) {
        s = perturbacao.perturbacaoFraca(grafo, sb, qtdIteracoes, buscaLocal);
        // double somatorioTeste = 0;
        // for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        //     somatorioTeste += s[i].getSomatorioDistancias();
        // }
        // cout << "\nsomatorioTeste: " << somatorioTeste;

        s = buscaLocal.buscaLocal(grafo, s);

        // double somatorioTeste2 = 0;
        // for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        //     somatorioTeste2 += s[i].getSomatorioDistancias();
        // }
        // cout << "\nsomatorioTeste2: " << somatorioTeste2;

        double somatorioS = 0, somatorioSb = 0;
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            somatorioS += s[i].getSomatorioDistancias();
            somatorioSb += sb[i].getSomatorioDistancias();
        }

        // cout << "\nsomatorioS: " << somatorioS
        //      << "\nsomatorioSb: " << somatorioSb << "\n\n";

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

void solucaoInicialLH(string nomeArquivo) {
    cout << "\n\n---------- Metodo LH: ----------\n\n";
    
    int qtdSolucoesFactiveis = 2;
    // cout << "Quantidade de solucoes factiveis a serem testadas: ";
    // cin >> qtdSolucoesFactiveis;
    MetodoLH* solucaoInicial = new MetodoLH(qtdSolucoesFactiveis);
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> grupos = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSolucao inicial: " << somatorioTotal;

    int alfa = 5;
    // cout << "\n\nAlfa: ";
    // cin >> alfa;
    // grupos = buscaMaxima(grafo, grupos, alfa);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n     Somatorio apos busca maxima: " << somatorioTotal;

    // cout << "\n\nForca da perturbacao forte: ";
    int qtdIteracoes = 3;
    // cin >> qtdIteracoes;
    Perturbacao perturbacao;
    BuscaLocal buscaLocal(grafo);
    grupos = perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n";

    // cout << "\n\nAlfa: ";
    // cin >> alfa;
    grupos = buscaMaxima(grafo, grupos, alfa);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "     Somatorio apos busca maxima: " << somatorioTotal;

    grupos = perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n\n";
}

void solucaoInicialGC(string nomeArquivo) {
    cout << "\n\n---------- Metodo GC: ----------\n\n";

    MetodoGC* solucaoInicial = new MetodoGC();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> grupos = solucaoInicial->gerarSolucao(grafo);
    
    // for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     cout << "\nGRUPO " << i
    //         << "\nlimiteInferior: " << grupos[i].getLimiteInferior()
    //         << "\nlimiteSuperior: " << grupos[i].getLimiteSuperior()
    //         << "\nqtdElementos: " << grupos[i].getQtdElementos()
    //         << "\nelementos: ";
            
    //     for (unsigned int j = 0; j < grupos[i].getElementos().size(); ++j) {
    //         cout << grupos[i].getElemento(j) << " ";
    //     }
        
    //     cout << "\narestas: ";
    //     vector<vector<int> >  arestasElementos = grupos[i].getArestasElementos();
    //     vector<double> arestasValor = grupos[i].getArestasValor();
    //     for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
    //         cout << arestasElementos[j][0] << ", "
    //             << arestasElementos[j][1] << ", "
    //             << arestasValor[j] << "; ";
    //     }
    //     cout << "\nsomatorioDistancias: " << grupos[i].getSomatorioDistancias() << "\n";
    // }

    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSolucao inicial: " << somatorioTotal;

    // BuscaLocal busca(grafo);
    // busca.criaVariaveis(grafo, grupos);

    // BUSCA LOCAL

    // grupos = busca.buscaLocal(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n  busca local 1: " << somatorioTotal;

    // grupos = busca.buscaLocal(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n  busca local 2: " << somatorioTotal;

    // grupos = busca.buscaLocal(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n  busca local 3: " << somatorioTotal;

    // grupos = busca.buscaLocal(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n  busca local 4: " << somatorioTotal;

    // cout << "\n\n";

    // INSERÇÃO + SWAP + INSERÇÃO + SWAP

    // grupos = busca.insercao(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n     insercao 1: " << somatorioTotal;

    // grupos = busca.swap(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n         swap 1: " << somatorioTotal;

    // grupos = busca.insercao(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n     insercao 2: " << somatorioTotal;

    // grupos = busca.swap(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n         swap 2: " << somatorioTotal;

    // cout << "\n\n";

    // INSERÇÃO + INSERÇÃO + INSERÇÃO + INSERÇÃO

    // grupos = busca.insercao(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 1: " << somatorioTotal;

    // grupos = busca.insercao(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 2: " << somatorioTotal;

    // grupos = busca.insercao(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 3: " << somatorioTotal;

    // grupos = busca.insercao(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 4: " << somatorioTotal;

    // cout << "\n\n";

    // SWAP + SWAP + SWAP + SWAP

    // grupos = busca.swap(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 1: " << somatorioTotal;

    // grupos = busca.swap(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 2: " << somatorioTotal;

    // grupos = busca.swap(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 3: " << somatorioTotal;

    // grupos = busca.swap(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 4: " << somatorioTotal;

    // cout << "\n\n";

    // SWAP EM CADEIA + SWAP EM CADEIA + SWAP EM CADEIA + SWAP EM CADEIA

    // grupos = busca.swapEmCadeia(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 1: " << somatorioTotal;

    // grupos = busca.swapEmCadeia(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 2: " << somatorioTotal;

    // grupos = busca.swapEmCadeia(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 3: " << somatorioTotal;

    // grupos = busca.swapEmCadeia(grafo, grupos);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao 4: " << somatorioTotal;

    // cout << "\n\n";

    // for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
    //     cout << "\nGRUPO " << i
    //         << "\nlimiteInferior: " << grupos[i].getLimiteInferior()
    //         << "\nlimiteSuperior: " << grupos[i].getLimiteSuperior()
    //         << "\nqtdElementos: " << grupos[i].getQtdElementos()
    //         << "\nelementos: ";
            
    //     for (unsigned int j = 0; j < grupos[i].getElementos().size(); ++j) {
    //         cout << grupos[i].getElemento(j) << " ";
    //     }
        
    //     cout << "\narestas: ";
    //     vector<vector<int> >  arestasElementos = grupos[i].getArestasElementos();
    //     vector<double> arestasValor = grupos[i].getArestasValor();
    //     for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
    //         cout << arestasElementos[j][0] << ", "
    //             << arestasElementos[j][1] << ", "
    //             << arestasValor[j] << "; ";
    //     }
    //     cout << "\nsomatorioDistancias: " << grupos[i].getSomatorioDistancias() << "\n";
    // }

    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\noutra solucao: " << somatorioTotal;
    int alfa = 5;
    // cout << "\n\nAlfa: ";
    // cin >> alfa;
    // grupos = buscaMaxima(grafo, grupos, alfa);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n     Somatorio apos busca maxima: " << somatorioTotal;

    // cout << "\n\nForca da perturbacao forte: ";
    int qtdIteracoes = 3;
    // cin >> qtdIteracoes;
    Perturbacao perturbacao;
    BuscaLocal buscaLocal(grafo);
    grupos = perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n";

    // cout << "\n\nAlfa: ";
    // cin >> alfa;
    grupos = buscaMaxima(grafo, grupos, alfa);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "     Somatorio apos busca maxima: " << somatorioTotal;

    grupos = perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n\n";
}

void solucaoInicialWJ(string nomeArquivo) {
    cout << "\n\n---------- Metodo WJ: ----------\n\n";

    MetodoWJ* solucaoInicial = new MetodoWJ();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> grupos = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\n                 Solucao inicial: " << somatorioTotal;
    
    int alfa = 5;
    // cout << "\n\nAlfa: ";
    // cin >> alfa;
    // grupos = buscaMaxima(grafo, grupos, alfa);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\n     Somatorio apos busca maxima: " << somatorioTotal;

    // cout << "\n\nForca da perturbacao forte: ";
    int qtdIteracoes = 3;
    // cin >> qtdIteracoes;
    Perturbacao perturbacao;
    BuscaLocal buscaLocal(grafo);
    grupos = perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n";

    // cout << "\n\nAlfa: ";
    // cin >> alfa;
    grupos = buscaMaxima(grafo, grupos, alfa);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "     Somatorio apos busca maxima: " << somatorioTotal;

    grupos = perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal << "\n\n";
}

int main() {
    // string nomeArquivo;
    // cout << "Nome do arquivo: ";
    // cin >> nomeArquivo;

    for (int j = 0; j < 10; ++j) {
        // for(int i = 1; i <= 16; ++i) {
           string nomeArquivo = (to_string(6) + "i");
           cout << "\n---- arquivo " << nomeArquivo << " ----";
           solucaoInicialGC(nomeArquivo);
    //     }
    }
    

    // solucaoInicialLH(nomeArquivo);

    // solucaoInicialGC(nomeArquivo);
    
    // solucaoInicialWJ(nomeArquivo);

    return 0;
}