#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include "grafo.hpp"
#include "grupo.hpp"
#include "solucaoInicial.hpp"

using namespace std;

int main() {
    
    string nomeArquivo;
    cin >> nomeArquivo;
    
    SolucaoInicial* solucaoInicial = new SolucaoInicial();
    
    Grafo grafo = solucaoInicial->leArquivo(nomeArquivo);
    
    //~ cout << grafo.getQtdElementos() << endl;
    
    vector<int> vetor = grafo.getElementosSemGrupo();
    
    //~ cout << "ta certo" << endl;
    //~ for (unsigned int i = 0; i < vetor.size(); ++i) {
        //~ cout << vetor[i] << " ";
    //~ }
    
    double **matriz = grafo.matrizAdjacenciaGrafo();
    
    
    //~ cout << "\n\n\n";
    //~ for (int lin = 0; lin < grafo.getQtdElementos(); ++lin) {
        //~ for (int col = 0; col < grafo.getQtdElementos(); ++col) {
            //~ cout << matriz[lin][col] << " ";
        //~ }
        //~ cout << endl;
    //~ }
    
    
    
    vector<int> elementosAleatorios = solucaoInicial->divisaoInicial(grafo);
    
    
    
    //~ cout << "\n\n";
    //~ for (unsigned int i = 0; i < elementosAleatorios.size(); ++i) {
        //~ cout << elementosAleatorios[i] << endl;
    //~ }
    
    //~ cout << "GRUPOOOOS\n\n";
    vector<Grupo> grupos = solucaoInicial->criaGrupos(grafo, elementosAleatorios);
    
    solucaoInicial->limiteInferior(grafo, grupos, matriz);
    solucaoInicial->limiteSuperior(grafo, grupos, matriz);
    
    double somatorioTotal = 0;
    for (unsigned int i = 0; i  < grupos.size(); ++i) {
        //~ cout << "\n\nGRUPO " << (i + 1)
             //~ << "\nlimiteInferior: " << grupos[i].getLimiteInferior()
             //~ << "\nlimiteSuperior: " << grupos[i].getLimiteSuperior()
             //~ << "\nqtdElementos: " << grupos[i].getQtdElementos()
             //~ << "\nelementos: ";
             
        //~ for (unsigned int j = 0; j < grupos[i].getElementos().size(); ++j) {
            //~ cout << grupos[i].getElemento(j) << " ";
        //~ }
        
        //~ cout << "\narestas: ";
        //~ vector<vector<int> >  arestasElementos = grupos[i].getArestasElementos();
        //~ vector<double> arestasValor = grupos[i].getArestasValor();
        //~ for (unsigned int j = 0; j < arestasElementos.size(); ++j) {
            //~ cout << arestasElementos[j][0] << ", "
                 //~ << arestasElementos[j][1] << ", "
                 //~ << arestasValor[j] << "; ";
        //~ }
        //~ cout << "\nsomatorioDistancias: " << grupos[i].getSomatorioDistancias();
        somatorioTotal += grupos[i].getSomatorioDistancias();
    }
    cout << "\nsomatorioTotal: " << somatorioTotal;
    cout << endl;
    
}
