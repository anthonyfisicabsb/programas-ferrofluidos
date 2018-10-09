#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define SIZE 3 /* Tamanho da matriz */

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

int main (void)
{
    srand(time(NULL)); // semente dos números aleatórios
    float r[SIZE]; // vetor de coordenadas
    float drmax; // variação máxima da coordenada    
    FILE *config;
    float m[3]; // vetor momento magnético
    float dangmax; // variação angular máxima
    float vetRot[SIZE]; // vetor de rotação
    float matrixRot[SIZE][SIZE];
    FILE *config2;

    drmax = 0.4f; // constante de deslocamento

    config = fopen("resultados.txt", "w"); // abrindo arquivo de texto
    config2 = fopen("caixa.txt", "w"); 

    fprintf(config2, "%f %f %f\n", 50.0f, 50.0f, 50.0f); // dimensões da caixa

    /* Declarando posições iniciais das coordenadas */
    r[0] = 3.5f;
    r[1] = 4.2f;
    r[2] = 5.6f;
    

    /* Declarando as variáveis iniciais do momento magnético */
    m[0] = 1.3f;
    m[1] = 0.5f;
    m[2] = 1.12f;

    createVetRot(vetRot); // criando vetor de rotação
 
    dangmax = 0.2f;

    float diam = 5.0f; // diâmetro da partícula

    normalizaVet(m);
    normalizaVet(vetRot);
 
    fprintf(config, "1\n"); // adicionar número de partículas

    for(int i=0; i<100000; i++){
        createMatrix(matrixRot, dangmax*2*((float)rand()/RAND_MAX -0.5f), vetRot);
        rotMat(matrixRot, m);
        createVetRot(vetRot);

        normalizaVet(m);
        normalizaVet(vetRot);

        r[0] += drmax*2.0f*((float)rand()/RAND_MAX -0.5f);
        r[1] += drmax*2.0f*((float)rand()/RAND_MAX -0.5f);       
        r[2] += drmax*2.0f*((float)rand()/RAND_MAX -0.5f); 

        fprintf(config, "%f %f %f %f %f %f %f\n", diam, r[0], r[1], r[2],
                m[0],m[1],m[2]);
    }

    /* fechando os arquivos */
    fclose(config);
    fclose(config2);
    return EXIT_SUCCESS;
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
