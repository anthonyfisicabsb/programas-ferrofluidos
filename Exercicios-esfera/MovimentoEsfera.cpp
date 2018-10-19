#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

#define SIZE 3 /* Tamanho da matriz */
#define NUM 25 /* Número de partículas  */

/* Função para rotacionar matriz */
void rotMat(float mat[][SIZE],
            float vet[]);

/* Função para normalizar vetor */
void normalizaVet(float vet[]);

/*Função para criar matriz*/
void createMatrix(float mat[][SIZE],
                  float ang,
                  float vet[]);

/* Função para criar vetor de rotação */
void createVetRot(float vet[]);

/* Função que calcula a distancia entre duas partículas  */
float distanciaParticula(float d1[], float d2[]);

void createVetPos(float r[], float L, float raio);

struct particula {
    float r[SIZE]; // posicao da partícula
    float m[3]; // momento magnético da partícula
}; // estrtutura que representa a partícula

int main (void)
{
    srand(time(NULL)); // semente dos números aleatórios
    float dangmax; // variação angular máxima
    FILE *config;
    float vetRot[SIZE]; // vetor de rotação
    float matrixRot[SIZE][SIZE];
    float Lx, Ly, Lz; // lados da caixa
    FILE *config2;
    struct particula particulas[NUM];
    float diam = 5.0f; // diâmetro da partícula
    float drmax;

    Lx = Ly = Lz = 100.0f;

    drmax = 0.6f; // constante de deslocamento

    config = fopen("resultados.txt", "w"); // abrindo arquivo de texto
    config2 = fopen("caixa.txt", "w"); 

    fprintf(config2, "%f %f %f\n", Lx, Ly, Lz); // dimensões da caixa

    /* Declarando posições iniciais das coordenadas */
    for(int i=0; i<NUM; i++) {
        createVetPos(particulas[i].r, Lx, diam/2.0);

        for(int j=0; j<i; j++) {
            while(distanciaParticula(particulas[i].r, particulas[j].r) <= diam/2.0)
                createVetPos(particulas[i].r, Lx, diam/2.0);
        }
    }

    /* Declarando as variáveis iniciais do momento magnético */
    for(int i=0; i<NUM; i++) {
        for(int j=0; j<3; j++) {
            particulas[i].m[j] = (float)rand()/RAND_MAX;
        }
    }

    createVetRot(vetRot); // criando vetor de rotação

    dangmax = 0.4f;

    for(int i=0; i<NUM; i++) {
        normalizaVet(particulas[i].m);
    }

    normalizaVet(vetRot);
 
    fprintf(config, "25\n"); // adicionar número de partículas

    for(int count = 0; count < 6001; count++) {
        for(int i=0; i<NUM; i++){
            createMatrix(matrixRot, dangmax*2*((float)rand()/RAND_MAX -0.5f), vetRot);
            rotMat(matrixRot, particulas[i].m);
            createVetRot(vetRot);

            normalizaVet(particulas[i].m);
            normalizaVet(vetRot);

            for(int k=0; k<NUM; k++) {

                if(k == i)
                    continue;

                particulas[i].r[0] += drmax*2.0f*((float)rand()/RAND_MAX -0.5f);
                particulas[i].r[1] += drmax*2.0f*((float)rand()/RAND_MAX -0.5f);
                particulas[i].r[2] += drmax*2.0f*((float)rand()/RAND_MAX -0.5f);

                if(distanciaParticula(particulas[i].r, particulas[k].r) <= diam/2.0){
                    particulas[i].r[0] = particulas[k].r[0] + diam/2.0;
                    particulas[i].r[1] = particulas[k].r[1] + diam/2.0;
                    particulas[i].r[2] = particulas[k].r[1] + diam/2.0;
                }

                for(int l=0; l<3; l++) {
                    if(particulas[i].r[l] > Lx)
                        particulas[i].r[l] -= Lx;
                    else if(particulas[i].r[l] < 0)
                        particulas[i].r[l] += Lx;
                }
            }

            fprintf(config, "%f %f %f %f %f %f %f\n", diam, particulas[i].r[0], particulas[i].r[1], particulas[i].r[2],
                    particulas[i].m[0], particulas[i].m[1], particulas[i].m[2]);
        }
    }

    /* fechando os arquivos */
    fclose(config);
    fclose(config2);
    return EXIT_SUCCESS;
}
void createVetPos(float r[], float L, float raio)
{
    float size = L - 2*raio;

    for(int i=0; i<3; i++) {
        r[i] = size*((float)rand()/RAND_MAX) + raio;
    }
}
void rotMat(float mat[][SIZE],
            float vetor[])
{
    vetor[0] = mat[0][0]*vetor[0] + mat[0][1]*vetor[1] + mat[0][2]*vetor[2];
    vetor[1] = mat[1][0]*vetor[0] + mat[1][1]*vetor[1] + mat[1][2]*vetor[2];
    vetor[2] = mat[2][0]*vetor[0] + mat[2][1]*vetor[1] + mat[2][2]*vetor[2];
}

void createMatrix(float mat[][SIZE],
                  float ang,
                  float vet[])
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

void normalizaVet(float vetor[])
{
    float modulo = 0.0f;    

    for(int i=0; i<SIZE; i++){
        modulo += vetor[i]*vetor[i];
    }

    modulo = sqrt(modulo);

    for(int i=0; i<SIZE; i++){
        vetor[i] = vetor[i]/modulo;
    }

    return;
}

void createVetRot(float vetRot[])
{
    /* Declarando variáveis iniciais do vetor de rotação */
    vetRot[0] = (float)rand()/RAND_MAX;
    vetRot[1] = (float)rand()/RAND_MAX;
    vetRot[2] = (float)rand()/RAND_MAX;
}
float distanciaParticula(float d1[], 
                         float d2[]) 
{
    float sum = 0.0;

    for(int i=0; i<3; i++) {
        sum += (d1[i]-d2[i])*(d1[i]-d2[i]);
    }

    return sqrt(sum);
}
