# -*- coding: utf-8 -*-

class Grupo:
    def __init__(self, limiteInferior, limiteSuperior, elemento):
        self.limiteInferior = limiteInferior
        self.limiteSuperior = limiteSuperior
        self.qtdElementos = 1
        self.elementos = []
        self.elementos.append(elemento)
        self.arestas = []
        self.somatorioDistancias = 0

