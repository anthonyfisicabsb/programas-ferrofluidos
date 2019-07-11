#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "funcoes.h"

int main (void) {

    // choose a different seed each time that simulation runs
    srand(time(NULL));

    FILE *positionResults; // arquivo para guardar resultados
    FILE *boxConfig; // arquivo para guardar positionResultsurações da caixa
    FILE *energyResults; // arquivo para guardar energias
    FILE *variaveis; // arquivo para ler variaveis

    double dangmax; // variação angular máxima
    double vetRot[3]; // vetor de rotação
    double matrixRot[3][3];
    double Lx, Ly, Lz; // lados da caixa
    double diam; // diâmetro da partícula
    double drmax; // constante de deslocamento
    double C_MAG; // magnetização da partícula
    double temperatura; // temperatura em Kelvin da simulacao

    int NUM; // numero de particulas
    int qtdIter; // número de iterações
    int aceito = 0; // funcao para calcular totais de iterações aceitas em um período
    int periodo = 1; // contador do periodo

    positionResults = fopen("resultados.txt", "w"); 
    boxConfig = fopen("caixa.txt", "w");
    energyResults = fopen("energias.dat", "w");
    variaveis = fopen("variaveis.txt", "r");

    fscanf(variaveis, "%lf", &drmax);
    fscanf(variaveis, "%lf", &dangmax);
    fscanf(variaveis, "%lf", &diam);
    fscanf(variaveis, "%lf", &Lx);
    fscanf(variaveis, "%lf", &Ly);
    fscanf(variaveis, "%lf", &Lz); 
    fscanf(variaveis, "%d", &NUM);
    fscanf(variaveis, "%lf", &C_MAG);
    fscanf(variaveis, "%lf", &temperatura);
    fscanf(variaveis, "%d", &qtdIter);

    if(variaveis) {
        fclose(variaveis);
    }

    fprintf(boxConfig, "%lf %lf %lf\n", Lx, Ly, Lz);
    
    if(boxConfig) {
        fclose(boxConfig);
    }

    struct particula particulas[NUM]; // criando array de partículas
    struct particula particulasAux[NUM]; //array auxiliar para o algoritmo de metropolis

    /* Declare initial coordinates for particles */
    for(int i=0; i<NUM; i++) {
        createVetPos(particulas[i].r, Lx, diam/2.0);

        for(int j=0; j<i; j++) {
            while(distanciaParticula(particulas[i].r, particulas[j].r) <= diam/2.0) {
                createVetPos(particulas[i].r, Lx, diam/2.0);
            }
        }
    }

    /* Defines the initial components for particles' magnetic momentum unit vector*/
    for(int i=0; i<NUM; i++) {
        for(int j=0; j<3; j++) {
            particulas[i].m[j] = (double)rand()/RAND_MAX;
        }
    }

    for(int i=0; i<NUM; i++) {
        normalizaVet(particulas[i].m);
    }

    /* Defines volume and magnetic momemtum vector for particles */
    const double VOLUME = (diam*diam*diam)* (M_PI/6.0) * 1e-27;
    const double MAGZ = VOLUME * C_MAG;

    for(int i=0; i<NUM; i++){
        particulas[i].magz[0] = particulas[i].m[0] * MAGZ;
        particulas[i].magz[1] = particulas[i].m[1] * MAGZ;
        particulas[i].magz[2] = particulas[i].m[2] * MAGZ;
    }

    double energia = sumEnergia(particulas, NUM);

    fprintf(positionResults, "%d\n", NUM); // adicionar número de partículas ao arquivo

    int count = 1;

    copiaStruct(particulas, particulasAux, NUM);

    fprintf(energyResults,"%d\t%g\n", count, energia);
    printParticula(positionResults, particulas, diam, NUM);
    
    do {
	
	if(periodo > 20){
	    const float aceitaRate = (float)aceito / periodo;

	    if(aceitaRate <= 0.15) {
	        drmax = (drmax*1.2) > Lx*0.33 ? drmax : drmax*1.2;
	    } else if(aceitaRate >= 0.7)
                drmax = (drmax*0.8) < 1e-2 ? drmax : drmax*0.8;
            }

	    periodo = 0;
	    aceito = 0;
	}

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

                if(k == i) {
                    continue;
                }

                /* Check if did not happen particle superposition */
                if(distanciaParticula(particulas[i].r, particulas[k].r) <= diam/2.0){
                    particulas[i].r[0] = particulas[k].r[0] + diam/2.0;
                    particulas[i].r[1] = particulas[k].r[1] + diam/2.0;
                    particulas[i].r[2] = particulas[k].r[2] + diam/2.0;
                }
            }   

            /* Check if particle did not go off the box */
            for(int l=0; l<3; l++) {
                if(particulas[i].r[l] + diam/2.0 > Lx){
                    particulas[i].r[l] = 2*Lx - diam -particulas[i].r[l];
                }else if(particulas[i].r[l] - diam/2.0 < 0){
                    particulas[i].r[l] = diam - particulas[i].r[l];
                }
            }
        }

        double energiaAtual = sumEnergia(particulas, NUM);

	periodo++;

        /* Check if iteration is valid */
        if(aceitaIteracao(energia, energiaAtual, temperatura)){
            energia = energiaAtual;
	    copiaStruct(particulas, particulasAux, NUM);
	    aceito++;
	}else{
	    copiaStruct(particulasAux, particulas, NUM);
	    continue;
	}

        fprintf(energyResults,"%d\t%g\n", count, energia);
        printParticula(positionResults, particulas, diam, NUM);

        printf("%i\n", count);

        count++;
    }while(count < qtdIter);

    if(positionResults) {
        fclose(positionResults);
    }

    if(energyResults) {
        fclose(energyResults);
    }

    return EXIT_SUCCESS;
}
