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
        //~ static double somatorioTotal;
    public:
        Grupo(int limiteInferior, int limiteSuperior, int elemento);
        ~Grupo();
        int getLimiteInferior();
        int getLimiteSuperior();
        int getQtdElementos();
        std::vector<int> getElementos();
        int getElemento(int pos);
        std::vector<std::vector<int> > getArestasElementos();
        std::vector<double> getArestasValor();
        double getSomatorioDistancias();
        //~ double getSomatorioTotal();
        void setQtdElementos();
        void setElementos(int elemento);
        void setArestasElementos(int elemento1, int elemento2);
        void setArestasValor(double valor);
        void setSomatorioDistancias(double valor);

};

#endif
