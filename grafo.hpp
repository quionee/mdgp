#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>

class Grafo {
    private:
        int qtdElementos;
        int qtdGrupos;
        int qtdArestas;
        std::string tipoDivisao;
        int **limites;
        int **arestasElementos;
        double *arestasValor;
        bool *inseridos;
        std::vector<int> elementosSemGrupo;
        double** matrizAdjacencia;
    public:
        Grafo(int qtdElementos, int qtdGrupos, int qtdArestas, std::string tipoDivisao, int **limites, int **arestasElementos, double *arestasValor);
        ~Grafo();
        
        //~ // Desabilita Construtor de Cópia gerado pelo compilador
        //~ Grafo(const Grafo &) = delete;

        //~ // Desabilita Operador de Atribuição gerado pelo compilador
        //~ Grafo &operator=(const Grafo &) = delete;
        
        int getQtdElementos();
        int getQtdGrupos();
        int getQtdArestas();
        std::string getTipoDivisao();
        int **getLimites();
        int **getArestasElementos();
        double *getArestasValor();
        bool *getInseridos();
        std::vector<int> getElementosSemGrupo();
        double **getMatrizAdjacencia();
        void setInseridos(bool *inseridos);
        void setInseridosUm(int elemento);
        void setElementosSemGrupoRemove(std::vector<int> &elementosSemGrupo, int elemento);
        void setElementosSemGrupo(std::vector<int> elementosSemGrupo);
        void setReiniciaElementosSemGrupo();
};

#endif
