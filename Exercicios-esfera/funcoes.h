#define SIZE 3 /* Tamanho da matriz */
#include "particula.h"
#include <cmath>

double energiaIter(double m1, double m2, double r1, double r2);

double* subtractVet(double vet[], double vet2[]);

double prodEscalar(double vet[], double vet2[]);

void multConstVet(double vet[], double k);

double moduloVetor(double vet[]);

void createVetPos(double r[], double L, double raio);

void rotMat(double mat[][SIZE], double vetor[]);

void createMatrix(double mat[][SIZE], double ang, double vet[]);

void normalizaVet(double vetor[]);

void createVetRot(double vetRot[]);

double distanciaParticula(double d1[], double d2[]);

void sumVect(double vet[], double vet2[]);

double sumEnergia(struct particula vet[], int num);

void printParticula(FILE *arquivo, struct particula vet[], double diam,
                    int num);

double sumEnergiaZema(struct particula part[], int NUM);

bool aceitaIteracao(double energia, double energiaAtual);

double energiaIter(double m1[], double m2[], double r1[], double r2[])
{
    double energia = (prodEscalar(m1, m2)*1e27)/pow(distanciaParticula(r1, r2), 3);

    double vetSubt[3];

    for(int i=0; i<3; i++) {
        vetSubt[i] = r1[i] - r2[i];
    }

    energia -= ((prodEscalar(m1, vetSubt)*prodEscalar(m1, vetSubt))*1e27)/pow(distanciaParticula(r1,r2), 5);

    energia *=  (1.26 * 1e-6)/(4* M_PI);

    return energia;
}

double prodEscalar(double vet[], double vet2[])
{
    double retorno = 0.0f;

    for(int i=0; i<SIZE; i++) {
        retorno += vet[i]*vet2[i];
    }

    return retorno;
}

void multConstVet(double vet[], double k)
{
    for(int i=0; i<SIZE; i++) {
        vet[i] *= k;
    }
}

double moduloVetor(double vet[])
{
    double modulo = 0.0f;

    for(int i=0; i<SIZE; i++){
        modulo += vet[i]*vet[i];
    }

    modulo = sqrt(modulo);

    return modulo;
}

void createVetPos(double r[], double L, double raio)
{
    double size = L - 2*raio;

    for(int i=0; i<3; i++) {
        r[i] = size*((double)rand()/RAND_MAX) + raio;
    }
}
void rotMat(double mat[][SIZE],
            double vetor[])
{
    vetor[0] = mat[0][0]*vetor[0] + mat[0][1]*vetor[1] + mat[0][2]*vetor[2];
    vetor[1] = mat[1][0]*vetor[0] + mat[1][1]*vetor[1] + mat[1][2]*vetor[2];
    vetor[2] = mat[2][0]*vetor[0] + mat[2][1]*vetor[1] + mat[2][2]*vetor[2];
}

void createMatrix(double mat[][SIZE],
                  double ang,
                  double vet[])
{
    mat[0][0] = cos(ang) + (1 - cos(ang))*vet[0]*vet[0];
    mat[0][1] = (1-cos(ang))*vet[0]*vet[1] + sin(ang)*vet[2];
    mat[0][2] = (1-cos(ang))*vet[0]*vet[2] - sin(ang)*vet[1];

    mat[1][0] = (1-cos(ang))*vet[1]*vet[0] - sin(ang)*vet[2];
    mat[1][1] = cos(ang) + (1-cos(ang))*vet[1]*vet[1];
    mat[1][2] = (1-cos(ang))*vet[1]*vet[2] + sin(ang)*vet[0];

    mat[2][0] = (1-cos(ang))*vet[2]*vet[0] + sin(ang)*vet[1];
    mat[2][1] = (1-cos(ang))*vet[2]*vet[1] - sin(ang)*vet[0];
    mat[2][2] = cos(ang) + (1-cos(ang))*vet[2]*vet[2];

    return;
}

void normalizaVet(double vetor[])
{
    double modulo = moduloVetor(vetor);

    for(int i=0; i<SIZE; i++){
        vetor[i] = vetor[i]/modulo;
    }

    return;
}

void createVetRot(double vetRot[])
{
    /* Declarando variáveis iniciais do vetor de rotação */
    vetRot[0] = (double)rand()/RAND_MAX;
    vetRot[1] = (double)rand()/RAND_MAX;
    vetRot[2] = (double)rand()/RAND_MAX;
}

double distanciaParticula(double d1[], 
                         double d2[]) 
{
    double sum = 0.0;

    for(int i=0; i<3; i++) {
        sum += (d1[i]-d2[i])*(d1[i]-d2[i]);
    }

    return sqrt(sum);
}

void sumVect(double vet1[], double vet2[])
{
    for(int i=0; i<SIZE; i++) {
        vet1[i] += vet2[i];
    }
}

double sumEnergia(struct particula part[],
                  int NUM)
{
    double energiaTot = 0.0;

    for(int i=0; i<NUM-1; i++) {
        for(int j=i+1; j<NUM; j++) {
            energiaTot += energiaIter(part[i].magz, part[j].magz, part[i].r, part[j].r);
        }
    }

    energiaTot += sumEnergiaZema(part, NUM);

    return energiaTot;
}
double sumEnergiaZema(struct particula part[],
                      int NUM)
{
    double energia = 0.0;
    double campoMag[] = {0.0f, 0.0f, 0.005f};

    for(int i=0; i<NUM; i++) {
        energia += prodEscalar(part[i].magz, campoMag);
    }

    return -1.0*energia;
}

void printParticula(FILE* arquivo,
                    struct particula particulas[],
                    double diam,
                     int NUM)
{
    for(int i=0; i<NUM; i++) {
        fprintf(arquivo, "%lf %lf %lf %lf %lf %lf %lf\n", diam, particulas[i].r[0], particulas[i].r[1], 
         particulas[i].r[2], particulas[i].m[0], particulas[i].m[1], particulas[i].m[2]);
    }
}

/* Função que aplica o algoritmo de Metropolis */
bool aceitaIteracao(double energia, double energiaAtual)
{
    if(energiaAtual < energia)
        return true;

    double aleatorio = (double)rand()/RAND_MAX;

    double f = exp((energia-energiaAtual)/(1.38*1e-23*300));

    if(f < aleatorio)
        return true;

    return false;
}
