#include <iostream>
#include <fstream>
#include "metodoWJ.hpp"
#include "buscaLocal.hpp"

using namespace std;

MetodoWJ::MetodoWJ() {
    
}

MetodoWJ::~MetodoWJ() {
    
}

std::vector<Grupo> MetodoWJ::gerarSolucao(Grafo* grafo) {
    srand(time(0));
    
    //~ double** matriz = grafo->getMatrizAdjacencia();
    
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    std::vector<Grupo> grupos = criaGrupos(grafo, elementosSemGrupo);
    
    return grupos;
}

std::vector<Grupo> MetodoWJ::criaGrupos(Grafo* grafo, std::vector<int> elementosSemGrupo) {
    int** limites = grafo->getLimites();
    std::vector<Grupo> grupos;
    
    // criando o primeiro grupo;
    int pos = rand() % elementosSemGrupo.size();
    cout << "\npos: " << pos << endl;
    int elemento = elementosSemGrupo[pos];
    cout << "elemento: " << elemento << endl;
    
    cout << "limites: " << limites[0][0] << "  " << limites[0][1] << endl;
    
    grupos.push_back(Grupo(limites[0][0], limites[0][1]));
    
    cout << "\n\nelementosSemGrupo.size()antes: " << elementosSemGrupo.size() << endl;
    
    atualizaGrafo(grafo, elemento, elementosSemGrupo);
    
    while (elementosSemGrupo.size() > 1) {
        pos = rand() % elementosSemGrupo.size();
        elemento = elementosSemGrupo[pos];
        
        double menorDistancia;
        if (pos == (int(elementosSemGrupo.size()) - 1)) {
            menorDistancia = matriz[elemento][elementosSemGrupo[pos - 1]];
        }
        menorDistancia = matriz[elemento][elementosSemGrupo[pos + 1]];
        
        for (int i = 0; i < int(elementosSemGrupo.size()); ++ i) {
            if ((matriz[elemento][elementosSemGrupo[i]] < menorDistancia) and (elemento != elementosSemGrupo[i])) {
                menorDistancia = matriz[elemento][elementosSemGrupo[i]];
            }
            cout << "\nmenorDistancia: " << menorDistancia << endl;
        }
        cout << "\nMENOR DISTANCIA: " << menorDistancia << endl;
        
    }
    grafo->setElementosSemGrupo(elementosSemGrupo);
    
    // LEMBRA DE TRATAR O ULTIMO ELEMENTO, OK

    return grupos;
}

void MetodoWJ::atualizaGrupo(Grupo& grupo, double** matriz, int elemento) {
    grupo.setElementos(elemento);
    grupo.setQtdElementos();
    
    for (int i = 0; i < grupo.getQtdElementos(); ++i) {
        grupo.setSomatorioDistancias(matriz[elemento][grupo.getElementos()[i]]);
        grupo.setArestasElementos(elemento, grupo.getElementos()[i]);
        grupo.setArestasValor(matriz[elemento][grupo.getElementos()[i]]);
    }
}

void MetodoWJ::atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo) {
    grafo->setInseridosUm(elemento);
    grafo->setElementosSemGrupoRemove(elementosSemGrupo, elemento);
}
