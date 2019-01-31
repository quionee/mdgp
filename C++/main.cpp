#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "grafo.hpp"
#include "grupo.hpp"
#include "solucaoInicial.hpp"
#include "metodoLH.hpp"
#include "metodoGC.hpp"

using namespace std;

int main() {
    
    string nomeArquivo;
    int qtdSolucoesFactiveis;
    cin >> nomeArquivo >> qtdSolucoesFactiveis;
    
    cout << "\n\nMetodo LH: \n\n";
    MetodoLH* solucaoInicialLH = new MetodoLH(qtdSolucoesFactiveis);
    
    Grafo* grafoLH = solucaoInicialLH->leArquivo(nomeArquivo);
    
    vector<Grupo> gruposLH = solucaoInicialLH->gerarSolucao(grafoLH);
    
    double somatorioTotal = 0;
    for (unsigned int i = 0; i  < gruposLH.size(); ++i) {
        //~ cout << "\n\nGRUPO " << (i + 1)
             //~ << "\nlimiteInferior: " << gruposLH[i].getLimiteInferior()
             //~ << "\nlimiteSuperior: " << gruposLH[i].getLimiteSuperior()
             //~ << "\nqtdElementos: " << gruposLH[i].getQtdElementos()
             //~ << "\nelementos: ";
             
        //~ for (unsigned int j = 0; j < gruposLH[i].getElementos().size(); ++j) {
            //~ cout << gruposLH[i].getElemento(j) << " ";
        //~ }
        
        //~ cout << "\narestas: ";
        //~ vector<vector<int> >  arestasElementos = gruposLH[i].getArestasElementos();
        //~ vector<double> arestasValor = gruposLH[i].getArestasValor();
        //~ for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
            //~ cout << arestasElementos[j][0] << ", "
                 //~ << arestasElementos[j][1] << ", "
                 //~ << arestasValor[j] << "; ";
        //~ }
        //~ cout << "\nsomatorioDistancias: " << gruposLH[i].getSomatorioDistancias();
        somatorioTotal += gruposLH[i].getSomatorioDistancias();
    }
    cout << "\nsomatorioTotal: " << somatorioTotal;
    cout << endl << endl;
        
    cout << "\n\nMetodo GC: \n\n";
    
    MetodoGC* solucaoInicialGC = new MetodoGC();

    Grafo* grafoGC = solucaoInicialGC->leArquivo(nomeArquivo);

    vector<Grupo> gruposGC = solucaoInicialGC->gerarSolucao(grafoGC);
    
    somatorioTotal = 0;
    for (unsigned int i = 0; i  < gruposGC.size(); ++i) {
        //~ cout << "\n\nGRUPO " << (i + 1)
             //~ << "\nlimiteInferior: " << gruposGC[i].getLimiteInferior()
             //~ << "\nlimiteSuperior: " << gruposGC[i].getLimiteSuperior()
             //~ << "\nqtdElementos: " << gruposGC[i].getQtdElementos()
             //~ << "\nelementos: ";
             
        //~ for (unsigned int j = 0; j < gruposGC[i].getElementos().size(); ++j) {
            //~ cout << gruposGC[i].getElemento(j) << " ";
        //~ }
        
        //~ cout << "\narestas: ";
        //~ vector<vector<int> >  arestasElementos = gruposGC[i].getArestasElementos();
        //~ vector<double> arestasValor = gruposGC[i].getArestasValor();
        //~ for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
            //~ cout << arestasElementos[j][0] << ", "
                 //~ << arestasElementos[j][1] << ", "
                 //~ << arestasValor[j] << "; ";
        //~ }
        //~ cout << "\nsomatorioDistancias: " << gruposGC[i].getSomatorioDistancias();
        somatorioTotal += gruposGC[i].getSomatorioDistancias();
    }
    cout << "\nsomatorioTotal: " << somatorioTotal;
    cout << endl << endl;
    
    
}
