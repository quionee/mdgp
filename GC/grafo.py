# -*- coding: utf-8 -*-

class Grafo:
    def __init__(self, tipoDivisao, limites, arestas, inseridos):
        self.qtdElementos = len(inseridos)
        self.qtdGrupos = len(limites)
        self.qtdArestas = len(arestas)
        self.tipoDivisao = tipoDivisao
        self.limites = limites
        self.arestas = arestas
        self.inseridos = inseridos
    
    def matrizAdjacenciaGrafo(self):
        matriz = [0] * self.qtdElementos
        
        for lin in range(self.qtdElementos):
            matriz[lin] = [0] * self.qtdElementos
        
        for i in range(self.qtdArestas):
            matriz[self.arestas[i][0]][self.arestas[i][1]] = self.arestas[i][2]
            matriz[self.arestas[i][1]][self.arestas[i][0]] = self.arestas[i][2]
        
        return matriz
