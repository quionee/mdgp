# -*- coding: utf-8 -*-

from grafo import Grafo
#~ from grupo import Grupo
#~ import time
from random import randint

def leArquivo(nomeArquivo):
    arquivo = open(nomeArquivo)
    
    # leitura da primeira linha, qtdVertices e qtdGrupos
    informacoes = arquivo.readline()
    informacoes = informacoes.split(' ')
    qtdElementos = int(informacoes[0])
    qtdGrupos = int(informacoes[1])
    tipoDivisao = informacoes[2]
    limites = [0] * qtdGrupos
    posAux = 3
    for i in range(qtdGrupos):
        limites[i] = [0] * 2
        limites[i][0] = int(informacoes[posAux])
        limites[i][1] = int(informacoes[posAux + 1])
        posAux += 2

    print('qtdElementos', qtdElementos)
    print('qtdGrupos', qtdGrupos)
    print('tipoDivisao', tipoDivisao)
    for i in range(qtdGrupos):
        print('limiteInferior', limites[i][0])
        print('limiteSupeior', limites[i][1])

    qtdArestas = int((qtdElementos * (qtdElementos - 1)) / 2)
    arestas = [0] * qtdArestas
    
    for i in range(qtdArestas):
        arestas[i] = [0] * 3

    for i in range(qtdArestas):
        linha = arquivo.readline()
        valores = linha.split(' ')
        arestas[i][0] = (int(valores[0]))
        arestas[i][1] = (int(valores[1]))
        arestas[i][2] = (float(valores[2]))
    
    #~ for i in range(qtdArestas):
        #~ print(arestas[i][0], arestas[i][1], arestas[i][2])
    
    inseridos = [False] * qtdElementos
    
    grafo = Grafo(tipoDivisao, limites, arestas, inseridos)
    
    return grafo

def main():
    nomeArquivo = raw_input("\nNome do arquivo: ")
    grafo = leArquivo(nomeArquivo);
    grafo.matrizAdjacenciaGrafo();
    

if __name__ == "__main__":
    main()
