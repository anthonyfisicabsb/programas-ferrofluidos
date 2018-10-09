#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "gl2ps.h"
#include "gl2ps.c"


void init(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}

void display(void)
{
	int np, ip;
	float Lx, Ly, Lz;
	float diam, rx, ry, rz;
	float raio, lado;

	FILE * pFile;
	pFile = fopen("configuration.dat", "r");

	if(pFile) {
		fscanf(pFile, "%f %f %f", &Lx, &Ly, &Lz);
		fscanf(pFile, "%d", &np);
	}

        printf ( "%f %f %f", Lx, Ly, Lz);

	GLdouble eqn0[np][2][4];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Esferas: */ 

	while(!feof(pFile)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                   /* Clean the screen and the depth buffer */
		glLoadIdentity();

		if(pFile) {
			for (ip = 0; ip < np; ip++) {
				fscanf(pFile, "%f %f %f %f ", &diam, &rx, &ry, &rz) ;

                                if(fabs(rx) >= fabs(Lx) || fabs(ry) >= fabs(Ly) || fabs(rz) >= fabs(Lz))
                                        continue;                               
 
                                if(rx < 0.0f || ry < 0.0f || rz < 0.0f)
                                        continue;

				float min = fabs(rx - Lx);
                                
                                if(fabs(rx) < min)
                                        min = fabs(rx);
                                if(fabs(ry - Ly) < min)
                                        min = fabs(ry - Ly);
                                if(fabs(ry) < min)
                                        min = fabs(ry);
                                if(fabs(rz - Lz) < min)
                                        min = fabs(rz - Lz);
                                if(fabs(rz) < min)
                                        min = fabs(rz);

                                raio = min;

				glPushMatrix();
				glTranslated (rx, ry, rz);
				glColor3f(rz/Lz, rz/Lz, rz/Lz);
				glutSolidSphere(raio, 16, 16);
				glPopMatrix();
			}
		}


		glPushMatrix();

		glColor3f(0.3, 0.5, 0.4);

		glNormal3f(1,0,0);
		glBegin(GL_QUADS); 
		glVertex3f (0.0f, 0.0f,  Lz );
		glVertex3f (0.0f,  Ly ,  Lz );
		glVertex3f (0.0f,  Ly , 0.0f);
		glVertex3f (0.0f, 0.0f, 0.0f);
		glEnd();

		glNormal3f(0,1,0);
		glBegin(GL_QUADS); 
		glVertex3f ( Lx , 0.0f, 0.0f);
		glVertex3f ( Lx , 0.0f,  Lz );
		glVertex3f (0.0f, 0.0f,  Lz );
		glVertex3f (0.0f, 0.0f, 0.0f);
		glEnd();

		glNormal3f(0,0,100);
		glBegin(GL_QUADS); 
		glVertex3f ( Lx , 0.0f, 0.0f);
		glVertex3f ( Lx ,  Ly , 0.0f);
		glVertex3f (0.0f,  Ly , 0.0f);
		glVertex3f (0.0f, 0.0f, 0.0f);
		glEnd();

		glPopMatrix();

		glFlush();
	
	}
}


static void keyboard(unsigned char key, int x, int y)
{
	FILE *fp;
	int state = GL2PS_OVERFLOW, buffsize = 0;

	(void) x; (void) y;  /* not used */ 

	switch(key) {
		case 'q' :
 			exit(0);
			break;
		case 's' :
			fp = fopen(" ',arqeps, ' ", "wb");
			printf("Escrevendo saida em ',arqeps,' ");
			while(state == GL2PS_OVERFLOW) {
				buffsize += 1024*1024;

				gl2psBeginPage("test", "gl2psTestSimple",
                                                NULL, GL2PS_EPS, GL2PS_SIMPLE_SORT,
                                                GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT,
                                                GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, "out.eps");

				display();
				state = gl2psEndPage();
			}
			fclose(fp);
			printf("Pronto!\n");
			break;
	}
}

void reshape(int w, int h)
{
	float Lx, Ly, Lz ;

	FILE * pFile;
	pFile = fopen("configuration.dat", "r");

        rewind(pFile);
 	fscanf(pFile, "%f %f %f", &Lx, &Ly, &Lz);

        Lx = 30.0;
	Ly = 30.0;
	Lz = 30.0;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-0.3e0*Lx, 1.3e0*Lx, -0.3e0*Ly, 1.3e0*Ly*h/w, -1.3e0*Lz, 0.3e0*Lz);
	else
		glOrtho(-0.3e0*Lx, 1.3e0*Lx*w/h, -0.3e0*Ly, 1.3e0*Ly, -1.3e0*Lz, 0.3e0*Lz);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0; 
}

