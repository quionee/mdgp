# -*- coding: utf-8 -*-

from grafoGC import Grafo
from grupoGC import Grupo
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

    #~ print('qtdElementos', qtdElementos)
    #~ print('qtdGrupos', qtdGrupos)
    #~ print('tipoDivisao', tipoDivisao)
    #~ for i in range(qtdGrupos):
        #~ print('limiteInferior', limites[i][0])
        #~ print('limiteSupeior', limites[i][1])

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

def divisaoInicial(grafo):
    elementosAleatorios = []
    aux = 0;
    posDisponiveis = grafo.qtdElementos - 1
    while aux < grafo.qtdGrupos:
        pos = randint(0, posDisponiveis)

        if not(grafo.inseridos[grafo.elementosSemGrupo[pos]]):
            elementosAleatorios.append(grafo.elementosSemGrupo[pos])
            grafo.inseridos[grafo.elementosSemGrupo[pos]] = True
            grafo.elementosSemGrupo.remove(grafo.elementosSemGrupo[pos])
            aux += 1
            posDisponiveis -= 1
    
    return elementosAleatorios
    
def criaGrupos(grafo, elementosAleatorios):
    grupos = [0] * grafo.qtdGrupos
    for i in range(grafo.qtdGrupos):
        grupos[i] = Grupo(grafo.limites[i][0], grafo.limites[i][1], elementosAleatorios[i])
    return grupos

def limiteInferior(grafo, grupos, matriz):
    # limPosicoesDisponiveis - posicoes disponiveis em grafo.elementosSemGrupo
    limPosicoesDisponiveis = (grafo.qtdElementos - grafo.qtdGrupos - 1)
    # gruposComplesto - lista booleana que indica quais grupos atingiram ou nao seu limite inferior
    gruposCompletos = [False] * grafo.qtdGrupos
    
    #~ print 'limPosicoesDisponiveis: ', limPosicoesDisponiveis
    #~ print 'elementos sem grupo: ', grafo.elementosSemGrupo
    #~ print 'gruposCompletos: ', gruposCompletos
    
    terminou = False
    while not(terminou) and (len(grafo.elementosSemGrupo) > 0):
        #~ print '\nlimPosicoesDisponiveis: ', limPosicoesDisponiveis
        
        qtdGruposProntos = 0
        pos = randint(0, limPosicoesDisponiveis)
        elemento = grafo.elementosSemGrupo[pos]
        
        #~ print '\npos: ', pos
        #~ print '\nelemento: ', elemento
        
        maiorSomatorio = 0
        grupoMaximizado = -1
        i = 0
        # para encontrar o grupo que maximiza Dig
        while i < grafo.qtdGrupos:
            if (grupos[i].qtdElementos < grupos[i].limiteInferior):
                somatorioGrupoAtual = 0
                
                for j in range(grupos[i].qtdElementos):
                    somatorioGrupoAtual += matriz[elemento][grupos[i].elementos[j]]
                
                if ((somatorioGrupoAtual / (grupos[i].qtdElementos + 1)) > maiorSomatorio):
                    maiorSomatorio = (somatorioGrupoAtual / (grupos[i].qtdElementos + 1))
                    grupoMaximizado = i
            else:
                gruposCompletos[i] = True
            
            i += 1

        #~ print '\nmaioSomatorio: ', maiorSomatorio
        #~ print 'grupoMaximizado: ', grupoMaximizado
        #~ print 'gruposCompletos: ', gruposCompletos
        
        if (grupoMaximizado > -1):
            grupos[grupoMaximizado].qtdElementos += 1
            grupos[grupoMaximizado].elementos.append(elemento)
            grafo.inseridos[elemento] = True
                
            for j in range(grupos[grupoMaximizado].qtdElementos):
                grupos[grupoMaximizado].somatorioDistancias += matriz[elemento][grupos[grupoMaximizado].elementos[j]]
                aresta = [elemento, grupos[grupoMaximizado].elementos[j], matriz[elemento][grupos[grupoMaximizado].elementos[j]]]
                grupos[grupoMaximizado].arestas.append(aresta)
            
            grafo.elementosSemGrupo.remove(elemento)
            limPosicoesDisponiveis -= 1
        
        for i in range(grafo.qtdGrupos):
            if (gruposCompletos[i]):
                qtdGruposProntos += 1
        
        #~ print('qtdGruposProntos: ', qtdGruposProntos)
        #~ print('qtdGrupos: ', grafo.qtdGrupos)
        
        if (qtdGruposProntos == grafo.qtdGrupos):
            terminou = True
            
        #~ print('gruposCompletos: ', gruposCompletos)
        
        #~ print('elementosSemGrupo: ', grafo.elementosSemGrupo)
        #~ print ''
        #~ print('GRRRRRRRRRRRRRRRRRRRRRRRUPOOOOOOOOOOOOOOOOOOS')
        #~ for i in range(grafo.qtdGrupos):
            #~ print('GRUPO ', i)
            #~ print('limiteInferior: ', grupos[i].limiteInferior)
            #~ print('limiteSuperior: ', grupos[i].limiteSuperior)
            #~ print('qtdElementos: ', grupos[i].qtdElementos)
            #~ print('elementos: ', grupos[i].elementos)
            #~ print('arestas: ', grupos[i].arestas)
            #~ print('somatorioDistancias: ', grupos[i].somatorioDistancias)
        #~ print ''
        #~ print('elementosSemGrupo: ', grafo.elementosSemGrupo)

def limiteSuperior(grafo, grupos, matriz):
    print(len(grafo.elementosSemGrupo))
    print(grafo.elementosSemGrupo)
    limPosicoesDisponiveis = (len(grafo.elementosSemGrupo) - 1)
    gruposCompletos = []
    for i in range(grafo.qtdGrupos):
        gruposCompletos.append(False)
    
    #~ print('limPosicoesDisponiveis: ', limPosicoesDisponiveis)
    #~ print('elementos sem grupo: ', grafo.elementosSemGrupo)
    #~ print('gruposCompletos: ', gruposCompletos)
    terminou = False
    while len(grafo.elementosSemGrupo) > 0:
        #~ print 'len(elementosSemGrupos): ', len(grafo.elementosSemGrupo)
        # posicao aleatoria de um numero
        #~ print('limPosicoesDisponiveis: ', limPosicoesDisponiveis)
        pos = randint(0, limPosicoesDisponiveis)
        elemento = grafo.elementosSemGrupo[pos]
        #~ print()
        #~ print('pos: ', pos)
        #~ print('elemento: ', elemento)
        
        maiorSomatorio = 0
        grupoMaximizado = -1
        i = 0
        while i < grafo.qtdGrupos:
            #~ print('grupo: ', i)
            if (grupos[i].qtdElementos < grupos[i].limiteSuperior):
                somatorioGrupoAtual = 0
                for j in range(grupos[i].qtdElementos):
                    somatorioGrupoAtual += matriz[elemento][grupos[i].elementos[j]]
                
                if ((somatorioGrupoAtual / (grupos[i].qtdElementos + 1)) > maiorSomatorio):
                    maiorSomatorio = (somatorioGrupoAtual / (grupos[i].qtdElementos + 1))
                    grupoMaximizado = i
            i += 1

        #~ print()
        #~ print('maioSomatorio: ', maiorSomatorio)
        #~ print('grupoMaximizado: ', grupoMaximizado)
        #~ print('gruposCompletos: ', gruposCompletos)
        
        if (grupoMaximizado > -1):
            grupos[grupoMaximizado].qtdElementos += 1
            grupos[grupoMaximizado].elementos.append(elemento)
            grafo.inseridos[elemento] = True
                
            for j in range(grupos[grupoMaximizado].qtdElementos):
                grupos[grupoMaximizado].somatorioDistancias += matriz[elemento][grupos[grupoMaximizado].elementos[j]]
                aresta = [elemento, grupos[grupoMaximizado].elementos[j], matriz[elemento][grupos[grupoMaximizado].elementos[j]]]
                grupos[grupoMaximizado].arestas.append(aresta)
            
            grafo.elementosSemGrupo.remove(elemento)
            limPosicoesDisponiveis -= 1
        else:
            print 'MUAHAHAHAHAHAHAHAAHAHHAAHHAHAAHAHAHHAHAHAHAH'
        
        #~ print('elementosSemGrupo: ', grafo.elementosSemGrupo)
        #~ print ''
        #~ print('GRRRRRRRRRRRRRRRRRRRRRRRUPOOOOOOOOOOOOOOOOOOS')
        #~ for i in range(grafo.qtdGrupos):
            #~ print('GRUPO ', i)
            #~ print('limiteInferior: ', grupos[i].limiteInferior)
            #~ print('limiteSuperior: ', grupos[i].limiteSuperior)
            #~ print('qtdElementos: ', grupos[i].qtdElementos)
            #~ print('elementos: ', grupos[i].elementos)
            #~ print('arestas: ', grupos[i].arestas)
            #~ print('somatorioDistancias: ', grupos[i].somatorioDistancias)
        #~ print ''
        #~ print('elementosSemGrupo: ', grafo.elementosSemGrupo)

def main():
    nomeArquivo = raw_input("\nNome do arquivo: ")
    
    grafo = leArquivo(nomeArquivo)
    matrizGrafo = grafo.matrizAdjacenciaGrafo()
    
    grafo.imprimeMatriz(matrizGrafo)

    elementosAleatorios = divisaoInicial(grafo)
    
    grupos = criaGrupos(grafo, elementosAleatorios)
    
    limiteInferior(grafo, grupos, matrizGrafo)
        
    if (len(grafo.elementosSemGrupo) > 0):
        limiteSuperior(grafo, grupos, matrizGrafo)
    
    somatorioTotal = 0
    print 'GC Method'
    for i in range(grafo.qtdGrupos):
        print '\nGRUPO ', (i + 1)
        print 'limiteInferior: ', grupos[i].limiteInferior
        print 'limiteSuperior: ', grupos[i].limiteSuperior
        print 'qtdElementos: ', grupos[i].qtdElementos
        print 'elementos: ', grupos[i].elementos
        print 'arestas: ', grupos[i].arestas
        print 'somatorioDistancias: ', grupos[i].somatorioDistancias
        somatorioTotal += grupos[i].somatorioDistancias
    
    print '\nSomatorio total das distancias: ', somatorioTotal

if __name__ == "__main__":
    main()
