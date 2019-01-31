#ifndef GRUPO_HPP
#define GRUPO_HPP

#include <vector>

class Grupo {
    private:
        int limiteInferior;
        int limiteSuperior;
        int qtdElementos;
        std::vector<int> elementos;
        std::vector<std::vector<int> > arestasElementos;
        std::vector<double> arestasValor;
        double somatorioDistancias;
    public:
        Grupo(int limiteInferior, int limiteSuperior, int elemento);
        Grupo(int limiteInferior, int limiteSuperior);
        ~Grupo();
        
        //~ // Desabilita Construtor de Cópia gerado pelo compilador
        //~ Grupo(const Grupo &) = delete;

        //~ // Desabilita Operador de Atribuição gerado pelo compilador
        //~ Grupo &operator=(const Grupo &) = delete;
        
        int getLimiteInferior();
        int getLimiteSuperior();
        int getQtdElementos();
        std::vector<int> getElementos();
        int getElemento(int pos);
        std::vector<std::vector<int> > getArestasElementos();
        std::vector<double> getArestasValor();
        double getSomatorioDistancias();
        void setQtdElementos();
        void setElementos(int elemento);
        void setArestasElementos(int elemento1, int elemento2);
        void setArestasValor(double valor);
        void setSomatorioDistancias(double valor);
        void zeraVariaveis();
};

#endif
