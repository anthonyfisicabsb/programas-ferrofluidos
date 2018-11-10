#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "funcoes.h"

int main (void)
{
    srand(time(NULL));

    FILE *config; // arquivo para guardar resultados
    FILE *config2; // arquivo para guardar configurações da caixa
    FILE *config3; // arquivo para guardar energias
    FILE *variaveis; // arquivo para ler variaveis

    double dangmax; // variação angular máxima
    double vetRot[SIZE]; // vetor de rotação
    double matrixRot[SIZE][SIZE];
    double Lx, Ly, Lz; // lados da caixa
    double diam; // diâmetro da partícula
    double drmax; // constante de deslocamento
    double C_MAG; // magnetização da partícula

    int NUM; // numero de particulas
    int qtdIter; // número de iterações

    config = fopen("resultados.txt", "w"); 
    config2 = fopen("caixa.txt", "w");
    config3 = fopen("energias.dat", "w");
    variaveis = fopen("variaveis.txt", "r");

    fscanf(variaveis, "%lf", &drmax);
    fscanf(variaveis, "%lf", &dangmax);
    fscanf(variaveis, "%lf", &diam);
    fscanf(variaveis, "%lf", &Lx);
    fscanf(variaveis, "%lf", &Ly);
    fscanf(variaveis, "%lf", &Lz); 
    fscanf(variaveis, "%d", &NUM);
    fscanf(variaveis, "%lf", &C_MAG);
    fscanf(variaveis, "%d", &qtdIter);

    fclose(variaveis);

    struct particula particulas[NUM]; // criando array de partículas

    fprintf(config2, "%lf %lf %lf\n", Lx, Ly, Lz);
    fclose(config2);

    /* Declarando posições iniciais das coordenadas */
    for(int i=0; i<NUM; i++) {
        createVetPos(particulas[i].r, Lx, diam/2.0);

        for(int j=0; j<i; j++) {
            while(distanciaParticula(particulas[i].r, particulas[j].r) <= diam/2.0)
                createVetPos(particulas[i].r, Lx, diam/2.0);
        }
    }

    /* Declarando as variáveis iniciais do versor momento magnético */
    for(int i=0; i<NUM; i++) {
        for(int j=0; j<3; j++) {
            particulas[i].m[j] = (double)rand()/RAND_MAX;
        }
    }

    for(int i=0; i<NUM; i++) {
        normalizaVet(particulas[i].m);
    }

    /* Definindo o volume e o vetor momento magnético da partícula */
    const double VOLUME = (diam*diam*diam)* (M_PI/6.0) * 1e-27;
    const double MAGZ = VOLUME * C_MAG;

    for(int i=0; i<NUM; i++){
        particulas[i].magz[0] = particulas[i].m[0] * MAGZ;
        particulas[i].magz[1] = particulas[i].m[1] * MAGZ;
        particulas[i].magz[2] = particulas[i].m[2] * MAGZ;
    }

    double energia = sumEnergia(particulas, NUM);
    printf("%g\n", energia);

    fprintf(config, "%d\n", NUM); // adicionar número de partículas ao arquivo

    int count = 0;

    do {
        for(int i=0; i<NUM; i++){
            createVetRot(vetRot);
            normalizaVet(vetRot);

            createMatrix(matrixRot, dangmax*2*((double)rand()/RAND_MAX -0.5f), vetRot);
            rotMat(matrixRot, particulas[i].m);

            normalizaVet(particulas[i].m);

            particulas[i].r[0] += drmax*2.0f*((double)rand()/RAND_MAX -0.5f);
            particulas[i].r[1] += drmax*2.0f*((double)rand()/RAND_MAX -0.5f);
            particulas[i].r[2] += drmax*2.0f*((double)rand()/RAND_MAX -0.5f);

            for(int k=0; k<NUM; k++) {

                if(k == i)
                    continue;

                /* Verifica se não há sobreposição de partículas*/
                if(distanciaParticula(particulas[i].r, particulas[k].r) <= diam/2.0){
                    particulas[i].r[0] = particulas[k].r[0] + diam/2.0;
                    particulas[i].r[1] = particulas[k].r[1] + diam/2.0;
                    particulas[i].r[2] = particulas[k].r[2] + diam/2.0;
                }
            }   

            /* Verifica se partícula não saiu da caixa */
            for(int l=0; l<3; l++) {
                if(particulas[i].r[l] + diam/2.0 > Lx){
                    particulas[i].r[l] = 2*Lx - diam -particulas[i].r[l];
                }else if(particulas[i].r[l] - diam/2.0 < 0){
                    particulas[i].r[l] = diam - particulas[i].r[l];
                }
            }
        }

        double energiaAtual = sumEnergia(particulas, NUM);

        /* Verifica se iteração é válida */
        if(aceitaIteracao(energia, energiaAtual))
            energia = energiaAtual;
        else{
            continue;
        }

        fprintf(config3,"%d\t%g\n", count, energia);
        printParticula(config, particulas, diam, NUM);

        printf("%i\n", count);

        count++;
    }while(count <= qtdIter);

    /* fechando os arquivos */
    fclose(config);
    fclose(config3);

    return EXIT_SUCCESS;
}
