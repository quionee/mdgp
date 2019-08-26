#include <iostream>
#include <iomanip>
#include <random>
#include <fstream>

using namespace std;

mt19937_64 mt(17);

void escreveNoArquivoInt(string nomeArquivo, int qtdElementos, int qtdGrupos, string tipo,                                   vector<int> limitesInferiores, vector<int> limitesSuperiores, int inicio,                           int fim) {
    uniform_int_distribution<int> numeroInteiro(inicio, fim);
    ofstream arquivo("instanciasGeradas/int/" + nomeArquivo);

    arquivo << qtdElementos << " "
            << qtdGrupos << " "
            << tipo;
    for (int i = 0; i < qtdGrupos; ++i) {
        arquivo << " " << limitesInferiores[i]
                << " " << limitesSuperiores[i];
    }
    for (int i = 0; i < qtdElementos - 1; ++i) {
        for (int j = i + 1; j < qtdElementos; ++j) {
            arquivo << endl << i
                    << " " << j
                    << " " << numeroInteiro(mt);
        }
    }
}

void escreveNoArquivoReal(string nomeArquivo, int qtdElementos, int qtdGrupos, string tipo,                                   vector<int> limitesInferiores, vector<int> limitesSuperiores, int inicio,                           int fim) {
    uniform_real_distribution<double> numeroReal(inicio, fim);
    ofstream arquivo("instanciasGeradas/real/" + nomeArquivo);

    arquivo << qtdElementos << " "
            << qtdGrupos << " "
            << tipo;
    for (int i = 0; i < qtdGrupos; ++i) {
        arquivo << " " << limitesInferiores[i]
                << " " << limitesSuperiores[i];
    }
    for (int i = 0; i < qtdElementos - 1; ++i) {
        for (int j = i + 1; j < qtdElementos; ++j) {
            arquivo << endl << i
                    << " " << j
                    << " " << numeroReal(mt);
        }
    }
}

void escreveNoArquivoGeo(string nomeArquivo, int qtdElementos, int qtdGrupos, string tipo,                                   vector<int> limitesInferiores, vector<int> limitesSuperiores,                                       int **coordenadas) {
    ifstream arquivoCoordenadas("arquivoCoordenadas.txt");
    ofstream arquivo("instanciasGeradas/geo/" + nomeArquivo);

    arquivoCoordenadas >> qtdElementos;
    
    arquivo << qtdElementos << " "
            << qtdGrupos << " "
            << tipo;
    for (int i = 0; i < qtdGrupos; ++i) {
        arquivo << " " << limitesInferiores[i]
                << " " << limitesSuperiores[i];
    }

    int pos = 0;
    for (int i = 0; i < qtdElementos - 1; ++i) {
        for (int j = i + 1; j < qtdElementos; ++j) {
            arquivo << endl << i
                    << " " << j;
            double somatorioPotencias = 0;
            for (int k = 0; k < 21; ++k) {
                somatorioPotencias += pow((coordenadas[j][k] - coordenadas[i][k]), 2);
            }
            arquivo << " " << sqrt(somatorioPotencias);
        }
    }
}

void reiniciaLimites(int qtdGrupos, vector<int> &limitesInferiores, vector<int> &limitesSuperiores,                      int limite = 0) {
    limitesInferiores.clear(), limitesSuperiores.clear();
    limitesInferiores.resize(qtdGrupos, limite);
    limitesSuperiores.resize(qtdGrupos, limite);
}

void preencheLimitesDif(vector<int> &limitesInferiores, vector<int> &limitesSuperiores, int iniInf,                         int fimInf, int iniSup, int fimSup, int qtdGrupos) {
    uniform_int_distribution<int> inferior(iniInf, fimInf);
    uniform_int_distribution<int> superior(iniSup, fimSup);
    for (int i = 0; i < qtdGrupos; ++i) {
        limitesInferiores[i] = inferior(mt);
        limitesSuperiores[i] = superior(mt);
        if (limitesInferiores[i] == limitesSuperiores[i]) {
            ++limitesSuperiores[i];
        }
    }
}

void criaInstanciaInt(string arquivo, int qtdElementos, int qtdGrupos, string tipo, int limite,                           int iniInf = 0, int fimInf = 0, int iniSup = 0, int fimSup = 0) {
    int inicio = 0, fim = 100; // inicio e fim de 120 e 240 e 480 e 960
    vector<int> limitesInferiores, limitesSuperiores;
    reiniciaLimites(qtdGrupos, limitesInferiores, limitesSuperiores, limite);

    if (tipo == "ds") {
        preencheLimitesDif(limitesInferiores, limitesSuperiores, iniInf, fimInf, iniSup, fimSup,                       qtdGrupos);
    }

    escreveNoArquivoInt(arquivo, qtdElementos, qtdGrupos, tipo, limitesInferiores,                                      limitesSuperiores, inicio, fim);
}

void criaInstanciaReal(string arquivo, int qtdElementos, int qtdGrupos, string tipo, int limite,                           int iniInf = 0, int fimInf = 0, int iniSup = 0, int fimSup = 0) {
    int inicio = 0, fim = 100; // inicio e fim de 120 e 240 e 480 e 960
    vector<int> limitesInferiores, limitesSuperiores;
    reiniciaLimites(qtdGrupos, limitesInferiores, limitesSuperiores, limite);

    if (tipo == "ds") {
        preencheLimitesDif(limitesInferiores, limitesSuperiores, iniInf, fimInf, iniSup, fimSup,                       qtdGrupos);
    }

    escreveNoArquivoReal(arquivo, qtdElementos, qtdGrupos, tipo, limitesInferiores,                                      limitesSuperiores, inicio, fim);
}

void criaInstanciaGeo(string arquivo, int qtdElementos, int qtdGrupos, string tipo, int limite,                           int iniInf = 0, int fimInf = 0, int iniSup = 0, int fimSup = 0) {
    vector<int> limitesInferiores, limitesSuperiores;
    reiniciaLimites(qtdGrupos, limitesInferiores, limitesSuperiores, limite);

    if (tipo == "ds") {
        preencheLimitesDif(limitesInferiores, limitesSuperiores, iniInf, fimInf, iniSup, fimSup,                       qtdGrupos);
    }

    ifstream arquivoCoordenadas("arquivoCoordenadas.txt");

    arquivoCoordenadas >> qtdElementos;
    int aux, qtdCoordenadas;
    int **coordenadas = new int*[qtdElementos];
    for (int i = 0; i < qtdElementos; ++i) {
        arquivoCoordenadas >> aux   
                           >> qtdCoordenadas;
        coordenadas[i] = new int[21];
        for (int j = 0; j < qtdCoordenadas; ++j) {
            arquivoCoordenadas >> coordenadas[i][j];
        }
        for (int j = qtdCoordenadas; j < 21; ++j) {
            coordenadas[i][j] = 0;
        }
    }

    escreveNoArquivoGeo(arquivo, qtdElementos, qtdGrupos, tipo, limitesInferiores,                                      limitesSuperiores, coordenadas);
}

void geraArquivoDeCoordenadas(int qtdElementos) {
    ofstream arquivoCoordenadas("arquivoCoordenadas.txt");
    uniform_int_distribution<int> coordenada(0, 10);
    uniform_int_distribution<int> qtdCoordenadas(2, 21);
    arquivoCoordenadas << qtdElementos;
    int qtdCoordenadasAtual;
    for (int i = 0; i < qtdElementos; ++i) {
        arquivoCoordenadas << "\n" << i;
        qtdCoordenadasAtual = qtdCoordenadas(mt);
        arquivoCoordenadas << " " << qtdCoordenadasAtual;
        for (int j = 0; j < qtdCoordenadasAtual; ++j) {
            arquivoCoordenadas << " " << coordenada(mt);
        }
    }
}

// para instâncias INT
void instanciasIntLimitesIguais(string nomeArquivo) {
    nomeArquivo = "int_ss_";
    string tipo = "ss";

    // 120 10 12 12 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "120.txt", 120, 10, tipo, 12);

    // 240 12 20 20 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "240.txt", 240, 12, tipo, 20);

    // 480 20 24 24 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "480.txt", 480, 20, tipo, 24);

    // // 960 24 40 40 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "960.txt", 960, 24, tipo, 40);
}

void instanciasIntLimitesDiferentes(string nomeArquivo) {
    nomeArquivo = "int_ds_";
    string tipo = "ds";

    // 120 10 12 12 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "120.txt", 120, 10, tipo, 12, 8, 12, 12, 16);

    // 240 12 20 20 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "240.txt", 240, 12, tipo, 20, 15, 20, 20, 25);

    // 480 20 24 24 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "480.txt", 480, 20, tipo, 24, 18, 24, 24, 30);

    // // 960 24 40 40 ss -> 0 a 100
    criaInstanciaInt(nomeArquivo + "960.txt", 960, 24, tipo, 40, 32, 40, 40, 48);
}

// para instâncias REAL
void instanciasRealLimitesIguais(string nomeArquivo) {
    nomeArquivo = "real_ss_";
    string tipo = "ss";

    // 120 10 12 12 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "120.txt", 120, 10, tipo, 12);

    // 240 12 20 20 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "240.txt", 240, 12, tipo, 20);

    // 480 20 24 24 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "480.txt", 480, 20, tipo, 24);

    // // 960 24 40 40 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "960.txt", 960, 24, tipo, 40);
}

void instanciasRealLimitesDiferentes(string nomeArquivo) {
    nomeArquivo = "real_ds_";
    string tipo = "ds";

    // 120 10 12 12 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "120.txt", 120, 10, tipo, 12, 8, 12, 12, 16);

    // 240 12 20 20 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "240.txt", 240, 12, tipo, 20, 15, 20, 20, 25);

    // 480 20 24 24 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "480.txt", 480, 20, tipo, 24, 18, 24, 24, 30);

    // // 960 24 40 40 ss -> 0 a 100
    criaInstanciaReal(nomeArquivo + "960.txt", 960, 24, tipo, 40, 32, 40, 40, 48);
}

// para instâncias GEO
void instanciasGeoLimitesIguais(string nomeArquivo) {
    nomeArquivo = "geo_ss_";
    string tipo = "ss";

    // 120 10 12 12 ss -> 0 a 100
    geraArquivoDeCoordenadas(120);
    criaInstanciaGeo(nomeArquivo + "120.txt", 120, 10, tipo, 12);

    // 240 12 20 20 ss -> 0 a 100
    geraArquivoDeCoordenadas(240);
    criaInstanciaGeo(nomeArquivo + "240.txt", 240, 12, tipo, 20);

    // 480 20 24 24 ss -> 0 a 100
    geraArquivoDeCoordenadas(480);
    criaInstanciaGeo(nomeArquivo + "480.txt", 480, 20, tipo, 24);

    // // 960 24 40 40 ss -> 0 a 100
    geraArquivoDeCoordenadas(960);
    criaInstanciaGeo(nomeArquivo + "960.txt", 960, 24, tipo, 40);
}

void instanciasGeoLimitesDiferentes(string nomeArquivo) {
    nomeArquivo = "geo_ds_";
    string tipo = "ds";

    // 120 10 12 12 ss -> 0 a 100
    criaInstanciaGeo(nomeArquivo + "120.txt", 120, 10, tipo, 12, 8, 12, 12, 16);

    // 240 12 20 20 ss -> 0 a 100
    criaInstanciaGeo(nomeArquivo + "240.txt", 240, 12, tipo, 20, 15, 20, 20, 25);

    // 480 20 24 24 ss -> 0 a 100
    criaInstanciaGeo(nomeArquivo + "480.txt", 480, 20, tipo, 24, 18, 24, 24, 30);

    // // 960 24 40 40 ss -> 0 a 100
    criaInstanciaGeo(nomeArquivo + "960.txt", 960, 24, tipo, 40, 32, 40, 40, 48);
}

// Geo set : As the previous two sets of benchmarks, this set con-
// tains 40 EGS instances and 40 DGS instances, but the distances
// are Euclidean distances between pairs of points with random
// coordinates from [], and the number of coordinates for
// each point is generated randomly in [2, 21].

int main() {
    string nomeArquivo;

    instanciasIntLimitesIguais(nomeArquivo);
    instanciasIntLimitesDiferentes(nomeArquivo);
    instanciasRealLimitesIguais(nomeArquivo);
    instanciasRealLimitesDiferentes(nomeArquivo);
    instanciasGeoLimitesIguais(nomeArquivo);
    instanciasGeoLimitesDiferentes(nomeArquivo);
    
    return 0;
}
