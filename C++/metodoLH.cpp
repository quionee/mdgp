#include <iostream>
#include <fstream>
#include "metodoLH.hpp"
#include "buscaLocal.hpp"

using namespace std;

MetodoLH::MetodoLH(int qtdSolucoesFactiveis) : SolucaoInicial() {
    this->qtdSolucoesFactiveis = qtdSolucoesFactiveis;
}

MetodoLH::~MetodoLH() {
    
}

std::vector<Grupo> MetodoLH::gerarSolucao(Grafo* grafo) {
    srand(time(0));
    
    double** matriz = grafo->getMatrizAdjacencia();
    
    int** limites = grafo->getLimites();
    std::vector<vector<Grupo> > solucoes;

    for (int i = 0; i < qtdSolucoesFactiveis; ++i) {
        int limPosicoesDisponiveis = grafo->getQtdElementos();
        std::vector<Grupo> grupos;
        std::vector<int> gruposNaoCheios;
        
        for (int j = 0; j < grafo->getQtdGrupos(); ++j) {
            grupos.push_back(Grupo(limites[j][0], limites[j][1]));
            limiteInferior(grafo, grupos[j], limPosicoesDisponiveis, matriz);
            
            if (grupos[j].getQtdElementos() < grupos[j].getLimiteSuperior()) {
                gruposNaoCheios.push_back(j);
            }
        }
        
        limiteSuperior(grafo, grupos, gruposNaoCheios, matriz);
        solucoes.push_back(grupos);
        grafo->setReiniciaElementosSemGrupo();
    }
    
    //~ for (unsigned int i = 0; i < solucoes.size(); ++i) {
        //~ cout << "\n\nSolucao " << i << ": ";
        //~ double somatorio = 0;
        //~ for (unsigned int j = 0; j < solucoes[i].size(); ++j) {
            //~ cout << "\n\nGRUPO " << j
             //~ << "\nlimiteInferior: " << solucoes[i][j].getLimiteInferior()
             //~ << "\nlimiteSuperior: " << solucoes[i][j].getLimiteSuperior()
             //~ << "\nqtdElementos: " << solucoes[i][j].getQtdElementos()
             //~ << "\nelementos: ";
             
            //~ for (unsigned int k = 0; k < solucoes[i][j].getElementos().size(); ++k) {
                //~ cout << solucoes[i][j].getElemento(k) << " ";
            //~ }
            
            //~ cout << "\narestas: ";
            //~ vector<vector<int> >  arestasElementos = solucoes[i][j].getArestasElementos();
            //~ vector<double> arestasValor = solucoes[i][j].getArestasValor();
            //~ for (unsigned int k = 0; k < arestasElementos.size(); ++k) {
                //~ cout << arestasElementos[k][0] << ", "
                     //~ << arestasElementos[k][1] << ", "
                     //~ << arestasValor[k] << "; ";
            //~ }
            //~ cout << "\nsomatorioDistancias: " << solucoes[i][j].getSomatorioDistancias();
            //~ somatorio += solucoes[i][j].getSomatorioDistancias();
        //~ }
        //~ cout << "\n\nSOMATORIO: " << somatorio << endl;
    //~ }
    
    std::vector<int> vetorY;
    std::vector<int> vetorZ;
    for (int l = 0; l < qtdSolucoesFactiveis; ++l) {
        // vetor de coordenadas dos elementos, indica a qual grupo cada elemento pertence
        vetorY.resize(grafo->getQtdElementos());
        
        // vetor de tamanhos dos grupos, indica qual o tamanho de cada grupo
        vetorZ.resize(grafo->getQtdGrupos());
        
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            vetorZ[i] = solucoes[l][i].getQtdElementos();
            
            vector<int> elementosGrupoI = solucoes[l][i].getElementos();
            for (int j = 0; j < solucoes[l][i].getQtdElementos(); ++j) {
                vetorY[elementosGrupoI[j]] = i;
            }
        }
        
        //~ cout << "\n\nANTES MODIFICACOES";
        //~ cout << "\n\nvetorY: ";
        //~ for (unsigned int i = 0; i < vetorY.size(); ++i) {
            //~ cout << vetorY[i] << " ";
        //~ }
        //~ cout << endl;
        
        //~ cout << "\n\nvetorZ: ";
        //~ for (unsigned int i = 0; i < vetorZ.size(); ++i) {
            //~ cout << vetorZ[i] << " ";
        //~ }
        //~ cout << endl;
        
        // matriz que armazena o valor do movimento
        vector<vector<double> > matrizGama;
        for (int lin = 0; lin < grafo->getQtdElementos(); ++lin) {
            vector<double> vetorAux;
            
            for (int col = 0; col < grafo->getQtdGrupos(); ++col) {
                vetorAux.push_back(0);
            }
            
            matrizGama.push_back(vetorAux);
        }

        //~ // andar pelos grupos realizando o somatorio
        for (int i = 0; i < grafo->getQtdElementos(); ++i) {
            for (int j = 0; j < grafo->getQtdGrupos(); ++j) {
                vector<int> elementosGrupoJ = solucoes[l][j].getElementos();
                for (int k = 0; k < solucoes[l][j].getQtdElementos(); ++k) {
                    matrizGama[i][j] += matriz[i][elementosGrupoJ[k]];
                }
            }
        }
        
        double f = 0;
        bool melhorou = true;
        BuscaLocal buscaLocal;
        while (melhorou) {
            melhorou = false;
            buscaLocal.insercao(solucoes[l], vetorY, vetorZ, matrizGama, melhorou, f, matriz);
            
            buscaLocal.swap(vetorY, matrizGama, melhorou, f, matriz);
        }
        
        // zera variaveis dos grupos (exceto os limites)
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            solucoes[l][i].zeraVariaveis();
        }
        
        // atualizando os elementos e as quantidades de elementos
        for (int i = 0; i < grafo->getQtdElementos(); ++i) {
            solucoes[l][vetorY[i]].setElementos(i);
            solucoes[l][vetorY[i]].setQtdElementos();
        }
        
        // atualizando as arestas e os somatorios das distancias dos grupos
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            for (int j = 0; j < solucoes[l][i].getQtdElementos() - 1; ++j) {
                for (int k = j + 1; k < solucoes[l][i].getQtdElementos(); ++k) {
                    solucoes[l][i].setArestasElementos(solucoes[l][i].getElemento(j), solucoes[l][i].getElemento(k));
                    solucoes[l][i].setArestasValor(matriz[solucoes[l][i].getElemento(j)][solucoes[l][i].getElemento(k)]);
                    solucoes[l][i].setSomatorioDistancias(matriz[solucoes[l][i].getElemento(j)][solucoes[l][i].getElemento(k)]);
                }
            }
        }
        
        vetorY.clear();
        vetorZ.clear();
    }
    
    double maiorSomatorio = 0;
    int melhorSolucaoInicial;
    for (unsigned int i = 0; i < solucoes.size(); ++i) {
        double somatorio = 0;
        for (unsigned int j = 0; j < solucoes[i].size(); ++j) {
            somatorio += solucoes[i][j].getSomatorioDistancias();
        }
        //~ cout << "\nsomatorio: " << somatorio << endl;
        if (somatorio > maiorSomatorio) {
            maiorSomatorio = somatorio;
            melhorSolucaoInicial = i;
        }
    }
    
    return solucoes[melhorSolucaoInicial];
}

void MetodoLH::limiteInferior(Grafo* grafo, Grupo &grupo, int &limPosicoesDisponiveis, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();

    while (grupo.getQtdElementos() < grupo.getLimiteInferior()) {
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        atualizaGrupo(grupo, matriz, elemento);
        atualizaGrafo(grafo, elemento, elementosSemGrupo);
        
        --limPosicoesDisponiveis;
    }
    grafo->setElementosSemGrupo(elementosSemGrupo);
}

void MetodoLH::limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupos, vector<int> &gruposNaoCheios, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    int qtdElementosSemGrupo = grafo->getElementosSemGrupo().size();
    int qtdGruposNaoCheios = gruposNaoCheios.size();

    while (qtdElementosSemGrupo != 0) {
        int posGrupo = rand() % qtdGruposNaoCheios;
        int grupoNaoPronto = gruposNaoCheios[posGrupo];

        atualizaGrupo(grupos[grupoNaoPronto], matriz, elementosSemGrupo[0]);
        atualizaGrafo(grafo, elementosSemGrupo[0], elementosSemGrupo);

        if (grupos[grupoNaoPronto].getQtdElementos() == grupos[grupoNaoPronto].getLimiteSuperior()) {
            for (int i = 0; i < qtdGruposNaoCheios; ++i) {
                if (gruposNaoCheios[i] == grupoNaoPronto) {
                    gruposNaoCheios.erase(gruposNaoCheios.begin() + i);
                }
            }
            --qtdGruposNaoCheios;
        }
        --qtdElementosSemGrupo;
    }
    grafo->setElementosSemGrupo(elementosSemGrupo);
}

void MetodoLH::atualizaGrupo(Grupo& grupo, double** matriz, int elemento) {
    grupo.setElementos(elemento);
    grupo.setQtdElementos();
    
    for (int i = 0; i < grupo.getQtdElementos(); ++i) {
        grupo.setSomatorioDistancias(matriz[elemento][grupo.getElementos()[i]]);
        grupo.setArestasElementos(elemento, grupo.getElementos()[i]);
        grupo.setArestasValor(matriz[elemento][grupo.getElementos()[i]]);
    }
}

void MetodoLH::atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo) {
    grafo->setInseridosUm(elemento);
    grafo->setElementosSemGrupoRemove(elementosSemGrupo, elemento);
}

//~ void MetodoLH::pertubacaoFraca() {
    
//~ }
