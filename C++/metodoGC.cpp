#include <iostream>
#include <fstream>
#include "metodoGC.hpp"

MetodoGC::MetodoGC() : SolucaoInicial() {

}

MetodoGC::~MetodoGC() {
    
}

// método que gera a solução inicial utilizando o método GC.
std::vector<Grupo> MetodoGC::gerarSolucao(Grafo* grafo) {
    std::vector<int> elementosAleatorios = divisaoInicial(grafo);
    
    std::vector<Grupo> grupos = criaGrupos(grafo, elementosAleatorios);
    
    double **matriz = grafo->getMatrizAdjacencia();    
    
    limiteInferior(grafo, grupos, matriz);
    limiteSuperior(grafo, grupos, matriz);
    
    return grupos;
}

// método que seleciona e retorna "qtdGrupos" elementos aleatórios para iniciar os grupos.
std::vector<int> MetodoGC::divisaoInicial(Grafo* grafo) {
    srand(time(0));
    std::vector<int> elementosAleatorios;
    int aux = 0;
    int posDisponiveis = grafo->getQtdElementos();
    int pos;
    bool *inseridos = grafo->getInseridos();
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    // while roda até que "qtdGrupos" elementos sejam selecionados.
    while (aux < grafo->getQtdGrupos()) {
        // a posição de um elemento é escolhida aleatoriamente.
        pos = rand() % posDisponiveis;
        
        // se o elemento na posição "pos" ainda não foi escolhido,
        // ele é adicionado ao vetor de "elementosAleatorios" que iniciam os grupos.
        if (!(inseridos[elementosSemGrupo[pos]])) {
            elementosAleatorios.push_back(elementosSemGrupo[pos]);
            inseridos[elementosSemGrupo[pos]] = true;
            grafo->setElementosSemGrupoRemove(elementosSemGrupo, elementosSemGrupo[pos]);
            ++aux;
            --posDisponiveis;
        }
    }

    grafo->setInseridos(inseridos);
    grafo->setElementosSemGrupo(elementosSemGrupo);
    
    return elementosAleatorios;
}

// método que cria os "qtdGrupos" grupos com os elementos aleatórios selecionados no método "divisaoInicial".
std::vector<Grupo> MetodoGC::criaGrupos(Grafo* grafo, std::vector<int> elementosAleatorios) {
    // cria um vetor de grupos.
    std::vector<Grupo> grupos;
    int** limites = grafo->getLimites();
    
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        // inicia cada posição do vetor de grupos, atribuindo os limites e um dos elementos de "elementosAleatorios".
        grupos.push_back(Grupo(limites[i][0], limites[i][1], elementosAleatorios[i]));
    }
    
    return grupos;
}

// método que atualiza os dados do grupo passado por parâmetro quando um elemento é adicionado a ele.
void MetodoGC::atualizaGrupo(Grupo &grupo, int elemento, double** matriz) {
    grupo.setQtdElementos();
    grupo.setElementos(elemento);
    
    for (int j = 0; j < (grupo.getQtdElementos()); ++j) {
        grupo.setSomatorioDistancias(matriz[elemento][grupo.getElementos()[j]]);
        grupo.setArestasElementos(elemento, grupo.getElementos()[j]);
        grupo.setArestasValor(matriz[elemento][grupo.getElementos()[j]]);
    }
}

// método que atualiza os dados do grafo sobre os elementos ainda sem grupo.
void MetodoGC::atualizaGrafo(Grafo* grafo, int elemento, std::vector<int> &elementosSemGrupo) {
    grafo->setInseridosUm(elemento);
    grafo->setElementosSemGrupoRemove(elementosSemGrupo, elemento);
}

// método que atribui elementos aos grupos respeitando o limite inferior de cada grupo individualmente.
void MetodoGC::limiteInferior(Grafo* grafo, std::vector<Grupo> &grupos, double** matriz) {
    srand(time(0));

    int limPosicoesDisponiveis = grafo->getQtdElementos() - grafo->getQtdGrupos();

    // vetor de booleano, "false" se o grupo daquela posição ainda não atingiu seu limitante inferior,
    // "true" caso contrário.
    bool gruposCompletos[grafo->getQtdGrupos()];
    for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
        gruposCompletos[i] = false;
    }
    
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    bool terminou = false;
    // while termina quando todos os grupos atingiram seus limitantes inferiores, considerando que ainda
    // existam elementos sem grupo.
    while ((!terminou) and (elementosSemGrupo.size() > 0)) {
        int qtdGruposProntos = 0;
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        double maiorSomatorio = 0;
        int grupoMaximizado = -1;
        double somatorioGrupoAtual = 0;
        int i = 0;
        // escolhe em qual grupo o elemento escolhido será adicionado, maximizando o somatório das distâncias.
        while (i < grafo->getQtdGrupos()) {
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
        
        // atualiza os dados para inserção do elemento no grupo que maximiza o somatório.
        if (grupoMaximizado > -1) {
            atualizaGrupo(grupos[grupoMaximizado], elemento, matriz);
            atualizaGrafo(grafo, elemento, elementosSemGrupo);
            --limPosicoesDisponiveis;
        }
        
        for (int i = 0; i < grafo->getQtdGrupos(); ++i) {
            if (gruposCompletos[i]) {
                ++qtdGruposProntos;
            }
        }
        
        if (qtdGruposProntos == grafo->getQtdGrupos()) {
            terminou = true;
        }
    }

    grafo->setElementosSemGrupo(elementosSemGrupo);
}

// método que atribui elementos aos grupos respeitando o limite superior de cada grupo individualmente.
void MetodoGC::limiteSuperior(Grafo* grafo, std::vector<Grupo> &grupos, double** matriz) {
    std::vector<int> elementosSemGrupo = grafo->getElementosSemGrupo();
    
    int limPosicoesDisponiveis = elementosSemGrupo.size();

    // while termina quando todos os elementos foram atribuídos a algum grupo.
    while (elementosSemGrupo.size() > 0) {
        srand(time(0));
        int pos = rand() % limPosicoesDisponiveis;
        int elemento = elementosSemGrupo[pos];
        
        int grupoMaximizado = -1;
        
        double maiorSomatorio = 0;
        double somatorioGrupoAtual = 0;
        int i = 0;
        // escolhe em qual grupo o elemento escolhido será adicionado, maximizando o somatório das distâncias.
        while (i < grafo->getQtdGrupos()) {
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
        
        // atualiza os dados para inserção do elemento no grupo que maximiza o somatório. 
        atualizaGrupo(grupos[grupoMaximizado], elemento, matriz);
        atualizaGrafo(grafo, elemento, elementosSemGrupo);
        --limPosicoesDisponiveis;
    }

    grafo->setElementosSemGrupo(elementosSemGrupo);
}