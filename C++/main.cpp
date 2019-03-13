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

void solucaoInicialLH(string nomeArquivo) {
    cout << "\n\n---------- Metodo LH: ----------\n\n";
    
    int qtdSolucoesFactiveis;
    cout << "Quantidade de solucoes factiveis a serem testadas: ";
    cin >> qtdSolucoesFactiveis;
    MetodoLH* solucaoInicial = new MetodoLH(qtdSolucoesFactiveis);
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> grupos = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSolucao inicial: " << somatorioTotal << endl;
    
    // Comeca perturbacao
    Perturbacao perturbacao;
    int qtdIteracoes;
    
    cout << "\nForca da perturbacao fraca: ";
    cin >> qtdIteracoes;
    BuscaLocal buscaLocal(grafo);
    grupos = perturbacao.perturbacaoFraca(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao fraca: " << somatorioTotal;
    
    cout << "\n\n";
    
    // cout << "\n\nForca da perturbacao forte: ";
    // cin >> qtdIteracoes;
    // perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal;
    
    // cout << "\n\nForca da perturbacao fraca: ";
    // cin >> qtdIteracoes;
    // perturbacao.perturbacaoFraca(grafo, grupos, qtdIteracoes, buscaLocal);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\nSomatorio apos perturbacao fraca: " << somatorioTotal << "\n\n";
}

void buscaMaxima(Grafo* grafo, vector<Grupo> s0, int alfa) {
    int qtdIteracoes;
    cout << "\nqtdIteracoes: ";
    cin >> qtdIteracoes;

    vector<Grupo> s = s0;
    BuscaLocal buscaLocal(grafo);
    vector<Grupo> sb = buscaLocal.buscaLocal(grafo, s);
    int contador = 0;
    Perturbacao perturbacao;
    while (contador < alfa) {
        s = perturbacao.perturbacaoFraca(grafo, sb, qtdIteracoes, buscaLocal);

    }
}

void solucaoInicialGC(string nomeArquivo) {
    cout << "\n\n---------- Metodo GC: ----------\n\n";

    MetodoGC* solucaoInicial = new MetodoGC();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> grupos = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSolucao inicial: " << somatorioTotal << endl;
    
    // int alfa;
    // cout << "\nalfa: ";
    // cin >> alfa;
    // buscaMaxima(grafo, grupos, alfa);

    // Comeca perturbacao
    Perturbacao perturbacao;
    int qtdIteracoes;
    
    cout << "\nForca da perturbacao fraca: ";
    cin >> qtdIteracoes;
    BuscaLocal buscaLocal(grafo);
    grupos = perturbacao.perturbacaoFraca(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao fraca: " << somatorioTotal;
    
    cout << "\n\n";

    // cout << "\n\nForca da perturbacao forte: ";
    // cin >> qtdIteracoes;
    // perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal;
    
    // cout << "\n\nForca da perturbacao fraca: ";
    // cin >> qtdIteracoes;
    // perturbacao.perturbacaoFraca(grafo, grupos, qtdIteracoes, buscaLocal);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\nSomatorio apos perturbacao fraca: " << somatorioTotal << "\n\n";
}

void solucaoInicialWJ(string nomeArquivo) {
    cout << "\n\n---------- Metodo WJ: ----------\n\n";

    MetodoWJ* solucaoInicial = new MetodoWJ();
    
    Grafo* grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    vector<Grupo> grupos = solucaoInicial->gerarSolucao(grafo);
    
    double somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSolucao inicial: " << somatorioTotal << endl;
    
    // Comeca perturbacao
    Perturbacao perturbacao;
    int qtdIteracoes;
    
    cout << "\nForca da perturbacao fraca: ";
    cin >> qtdIteracoes;
    BuscaLocal buscaLocal(grafo);
    grupos = perturbacao.perturbacaoFraca(grafo, grupos, qtdIteracoes, buscaLocal);
    somatorioTotal = 0;
    realizaSomatorioDaSolucao(somatorioTotal, grupos);
    cout << "\nSomatorio apos perturbacao fraca: " << somatorioTotal;
    
    cout << "\n\n";
    
    // cout << "\n\nForca da perturbacao forte: ";
    // cin >> qtdIteracoes;
    // perturbacao.perturbacaoForte(grafo, grupos, qtdIteracoes, buscaLocal);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\nSomatorio apos perturbacao forte: " << somatorioTotal;
    
    // cout << "\n\nForca da perturbacao fraca: ";
    // cin >> qtdIteracoes;
    // perturbacao.perturbacaoFraca(grafo, grupos, qtdIteracoes, buscaLocal);
    // somatorioTotal = 0;
    // realizaSomatorioDaSolucao(somatorioTotal, grupos);
    // cout << "\nSomatorio apos perturbacao fraca: " << somatorioTotal << "\n\n";
}

int main() {
    
    string nomeArquivo;
    cout << "Nome do arquivo: ";
    cin >> nomeArquivo;
    
    // solucaoInicialLH(nomeArquivo);

    // solucaoInicialGC(nomeArquivo);
    
    solucaoInicialWJ(nomeArquivo);


}
