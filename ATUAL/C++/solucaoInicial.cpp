#include <iostream>
#include <fstream>
#include "solucaoInicial.hpp"

SolucaoInicial::SolucaoInicial() {
    somatorioTotal = 0;
    this->nomeArquivo = nomeArquivo;
}

SolucaoInicial::~SolucaoInicial() {
    
}

double SolucaoInicial::getSomatorioTotal() {
    return somatorioTotal;
}

Grafo SolucaoInicial::leArquivo(std::string nomeArquivo) {
    std::ifstream arquivo;
    arquivo.open(nomeArquivo.c_str());
    
    int qtdElementos, qtdGrupos;
    arquivo >> qtdElementos;
    arquivo >> qtdGrupos;
    
    std::string tipoDivisao;
    arquivo >> tipoDivisao;
    
    int **limites;
    limites = new int*[qtdGrupos];
    for (int i = 0; i < qtdGrupos; ++i) {
        limites[i] = new int[2];
    }
    
    for (int i = 0; i < qtdGrupos; ++i) {
        arquivo >> limites[i][0]
                >> limites[i][1];
    }
    
    int qtdArestas = (qtdElementos * (qtdElementos - 1)) / 2;
    int **arestasElementos = new int*[qtdArestas];
    double *arestasValor = new double[qtdArestas];
    for (int i = 0; i < qtdArestas; ++i) {
        arestasElementos[i] = new int[2];
    }
    
    for (int i = 0; i < qtdArestas; ++i) {
        arquivo >> arestasElementos[i][0]
                >> arestasElementos[i][1]
                >> arestasValor[i];
    }

    //~ cout << "qtdElementos: " << qtdElementos << endl;
    //~ cout << "qtdGrupos: " << qtdGrupos << endl;
    //~ cout << "tipoDivisao: " << tipoDivisao << endl;
    //~ for (int i = 0; i < qtdGrupos; ++i) {
        //~ cout << limites[i][0] << " " << limites[i][1] << endl;
    //~ }
    //~ cout << "qtdArestas: " << qtdArestas << endl;
    //~ for (int i = 0; i < qtdArestas; ++i) {
        //~ cout << arestasElementos[i][0] << " "
             //~ << arestasElementos[i][1] << " "
             //~ << arestasValor[i] << endl;
    //~ }
    
    Grafo grafo(qtdElementos, qtdGrupos, qtdArestas, tipoDivisao, limites, arestasElementos, arestasValor);
    
    return grafo;
}

std::vector<int> SolucaoInicial::divisaoInicial(Grafo &grafo) {
    std::vector<int> elementosAleatorios;
    int aux = 0;
    int posDisponiveis = grafo.getQtdElementos();
    int pos;
    bool *inseridos = grafo.getInseridos();
    std::vector<int> elementosSemGrupo = grafo.getElementosSemGrupo();
    
    while (aux < grafo.getQtdGrupos()) {
        srand(time(0));
        pos = rand() % posDisponiveis;
        
        if (!(inseridos[elementosSemGrupo[pos]])) {
            elementosAleatorios.push_back(elementosSemGrupo[pos]);
            inseridos[elementosSemGrupo[pos]] = true;
            grafo.setElementosSemGrupoRemove(elementosSemGrupo, elementosSemGrupo[pos]);
            ++aux;
            --posDisponiveis;
        }
    }
    //~ cout << "\nELEMENTOS SEM GRUPO: ";
    //~ for (unsigned int i = 0; i < elementosSemGrupo.size(); ++i) {
        //~ cout << elementosSemGrupo[i] << " ";
    //~ }
    //~ cout << endl;
    grafo.setInseridos(inseridos);
    grafo.setElementosSemGrupo(elementosSemGrupo);
    
    //~ vector<int> ajudaSenhor = grafo.getElementosSemGrupo();
    //~ cout << "\nELEMENTOS SEM GRUPO meo deos: ";
    //~ for (unsigned int i = 0; i < ajudaSenhor.size(); ++i) {
        //~ cout << ajudaSenhor[i] << " ";
    //~ }
    
    return elementosAleatorios;
}

std::vector<Grupo> SolucaoInicial::criaGrupos(Grafo grafo, std::vector<int> elementosAleatorios) {
    std::vector<Grupo> grupos;
    int** limites = grafo.getLimites();
    
    for (int i = 0; i < grafo.getQtdGrupos(); ++i) {
        grupos.push_back(Grupo(limites[i][0], limites[i][1], elementosAleatorios[i]));
    }
    
    return grupos;
}

void SolucaoInicial::limiteInferior(Grafo &grafo, std::vector<Grupo> &grupos, double** matriz) {
    int limPosicoesDisponiveis = grafo.getQtdElementos() - grafo.getQtdGrupos();
    bool gruposCompletos[grafo.getQtdGrupos()];
    for (int i = 0; i < grafo.getQtdGrupos(); ++i) {
        gruposCompletos[i] = false;
    }
    
    std::vector<int> elementosSemGrupo = grafo.getElementosSemGrupo();
    
    //~ cout << "\n\nlimPosicoesDisponiveis: " << limPosicoesDisponiveis
         //~ << "\nelementos sem grupo: ";
    //~ for (unsigned int i = 0; i < elementosSemGrupo.size(); ++i) {
        //~ cout << elementosSemGrupo[i] << " ";
    //~ }
    //~ cout << "\n gruposCompletos: ";
    //~ for (int i = 0; i < grafo.getQtdGrupos(); ++i) {
        //~ cout << gruposCompletos[i] << " ";
    //~ }
    
    bool terminou = false;
    while ((!terminou) and (elementosSemGrupo.size() > 0)) {
        
        int qtdGruposProntos = 0;
        srand(time(0));
        //~ cout << "\n\n AAAAAAlimPosicoesDisponiveis: " << limPosicoesDisponiveis << endl;
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        //~ cout << "\n\npos: " << pos
             //~ << "\nelementos: " << elemento;
        
        double maiorSomatorio = 0;
        int grupoMaximizado = -1;
        double somatorioGrupoAtual = 0;
        int i = 0;
        while (i < grafo.getQtdGrupos()) {
            if (grupos[i].getQtdElementos() < grupos[i].getLimiteInferior()) {
                somatorioGrupoAtual = 0;
                
                for (int j = 0; j < grupos[i].getQtdElementos(); ++j) {
                    somatorioGrupoAtual += matriz[elemento][grupos[i].getElementos()[j]];
                }
                
                if ((somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1)) > maiorSomatorio) {
                    maiorSomatorio = (somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1));
                    grupoMaximizado = i;
                }
            }
            else {
                gruposCompletos[i] = true;
            }
            ++i;
        }
        
        //~ cout << "\n\ngrupoMaximizado: " << grupoMaximizado << endl
             //~ << "somatorio atual: " << somatorioGrupoAtual << endl;
        
        if (grupoMaximizado > -1) {
            //~ cout << "NÃO ENTROU" << endl;
            grupos[grupoMaximizado].setQtdElementos();
            grupos[grupoMaximizado].setElementos(elemento);
            grafo.setInseridosUm(elemento);
            
            for (int j = 0; j < (grupos[grupoMaximizado].getQtdElementos()); ++j) {
                //~ cout << "UAAAAAAAU    " << matriz[elemento][grupos[grupoMaximizado].getElementos()[j]] <<  endl;
                grupos[grupoMaximizado].setSomatorioDistancias(matriz[elemento][grupos[grupoMaximizado].getElementos()[j]]);
                grupos[grupoMaximizado].setArestasElementos(elemento, grupos[grupoMaximizado].getElementos()[j]);
                grupos[grupoMaximizado].setArestasValor(matriz[elemento][grupos[grupoMaximizado].getElementos()[j]]);
                
                this->somatorioTotal += matriz[elemento][grupos[grupoMaximizado].getElementos()[j]];
            }
            grafo.setElementosSemGrupoRemove(elementosSemGrupo, elemento);
            --limPosicoesDisponiveis;
            
        }
        
        for (int i = 0; i < grafo.getQtdGrupos(); ++i) {
            if (gruposCompletos[i]) {
                ++qtdGruposProntos;
            }
        }
        
        if (qtdGruposProntos == grafo.getQtdGrupos()) {
            terminou = true;
        }
    }
    
    grafo.setElementosSemGrupo(elementosSemGrupo);
    
}

void SolucaoInicial::limiteSuperior(Grafo &grafo, std::vector<Grupo> &grupos, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo.getElementosSemGrupo();
    
    int limPosicoesDisponiveis = elementosSemGrupo.size();

    //~ cout << "\n\nlimPosicoesDisponiveis: " << limPosicoesDisponiveis
         //~ << "\nelementos sem grupo: ";
    //~ for (unsigned int i = 0; i < elementosSemGrupo.size(); ++i) {
        //~ cout << elementosSemGrupo[i] << " ";
    //~ }
    //~ cout << "\n gruposCompletos: ";
    //~ for (int i = 0; i < grafo.getQtdGrupos(); ++i) {
        //~ cout << gruposCompletos[i] << " ";
    //~ }

    while (elementosSemGrupo.size() > 0) {
        srand(time(0));
        //~ cout << "\n\n AAAAAAlimPosicoesDisponiveis: " << limPosicoesDisponiveis << endl;
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        double maiorSomatorio = 0;
        int grupoMaximizado = -1;
        double somatorioGrupoAtual = 0;
        int i = 0;
        
        while (i < grafo.getQtdGrupos()) {
            if (grupos[i].getQtdElementos() < grupos[i].getLimiteSuperior()) {
                somatorioGrupoAtual = 0;
                
                for (int j = 0; j < grupos[i].getQtdElementos(); ++j) {
                    somatorioGrupoAtual += matriz[elemento][grupos[i].getElementos()[j]];
                }
                
                if ((somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1)) > maiorSomatorio) {
                    maiorSomatorio = (somatorioGrupoAtual / (grupos[i].getQtdElementos() + 1));
                    grupoMaximizado = i;
                }
            }
            ++i;
        }
        
        grupos[grupoMaximizado].setQtdElementos();
        grupos[grupoMaximizado].setElementos(elemento);
        grafo.setInseridosUm(elemento);
        
        //~ cout << "\n\ngrupoMaximizado: " << grupoMaximizado << endl
             //~ << "somatorio atual: " << somatorioGrupoAtual << endl;
        
        for (int j = 0; j < (grupos[grupoMaximizado].getQtdElementos()); ++j) {
            //~ cout << "UAAAAAAAU    " << matriz[elemento][grupos[grupoMaximizado].getElementos()[j]] <<  endl;
            grupos[grupoMaximizado].setSomatorioDistancias(matriz[elemento][grupos[grupoMaximizado].getElementos()[j]]);
            grupos[grupoMaximizado].setArestasElementos(elemento, grupos[grupoMaximizado].getElementos()[j]);
            grupos[grupoMaximizado].setArestasValor(matriz[elemento][grupos[grupoMaximizado].getElementos()[j]]);
            
            this->somatorioTotal = matriz[elemento][grupos[grupoMaximizado].getElementos()[j]];
        }
        
        grafo.setElementosSemGrupoRemove(elementosSemGrupo, elemento);
        --limPosicoesDisponiveis;
    
        grafo.setElementosSemGrupo(elementosSemGrupo);
    }
}
