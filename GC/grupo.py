# -*- coding: utf-8 -*-

class Grupo:
    idGrupo = 0
    
    def __init__(self, grafo):
        self.limiteInferior = grafo.limites[idGrupo][0]
        self.limiteSuperior = grafo.limites[idGrupo][1]
        self.
        idGrupo += 1

    
    def divisaoInicial(self):
        elementosAleatorios = []
        aux = 0;
        while (aux < qtdGrupos):
            elemento = randint(0, (qtdElementos - 1))

            if not(inseridos[elemento]):
                elementosAleatorios.append(elemento)
                inseridos[elemento] = True
                aux += 1
        
        print(elementosAleatorios)
