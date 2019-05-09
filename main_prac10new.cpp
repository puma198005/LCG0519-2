//Merino García Gilberto Bernardino

//#include <gl/gl.h>     
//#include <windows.h>	
//#include <GL/glut.h>   
//#include <stdlib.h>	
//#include <stdio.h>
//#include <string.h>

//#include <malloc.h>	

#include "texture.h"
#include "figuras.h"
#include "Camera.h"
#include "cmodel/CModel.h"

//$$$$$$$$$$$$$$$$$$$$$$$$$$$ MIS MODELOS $$$$$$$$$$$$$$$$$$$$$$$$$$$
CModel chair1;
CModel chair2;
CModel fan;
CModel mariposa;
CModel balon;

float transZ = 0.0f;	//variables para mover 
float transX = 0.0f;
float transY = 0.0f;

int screenW = 0.0;
int screenH = 0.0;

static GLuint ciudad_display_list;

//NEW// Keyframes
float posX = 0, posY = 2.5, posZ = -3.5, rotRodIzq = 0, rotRodDer = 0, rotJenga = 0, rotdod = 0;
float jengx = 6.5, jengy = -3;
float sim = 0;

#define MAX_FRAMES 90   // limitar cuadros 
int i_max_steps = 30;    // cuantos cuadros intermedios influye en la velocidad
int i_curr_steps = 0;

typedef struct _frame   //caracteristicas que guarda el cuadro clave 
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ

	float angulo1;
	float angulo2;
	float angulo3;
	float angulo4;
	float angulo5;

	float rotJenga;    //Rotacion Jenga con keyframe
	float rotInc3;// Incremento

	float rotdod;
	float incdod;

	float jengx;		//Variable para PosicionX
	float jengy;		//Variable para PosicionY

	float incjx;		//Variable para IncrementoX
	float incjy;		//Variable para IncrementoY


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 9;			//introducir datos
bool play = false;
int playIndex = 0;

void prisma(void)
{
	GLfloat vertice[8][3] = {
				{0.5 ,-0.5, 0.5},    //Coordenadas Vértice 0 V0
				{-0.5 ,-0.5, 0.5},    //Coordenadas Vértice 1 V1
				{-0.5 ,-0.5, -0.5},    //Coordenadas Vértice 2 V2
				{0.5 ,-0.5, -0.5},    //Coordenadas Vértice 3 V3
				{0.5 ,0.5, 0.5},    //Coordenadas Vértice 4 V4
				{0.5 ,0.5, -0.5},    //Coordenadas Vértice 5 V5
				{-0.5 ,0.5, -0.5},    //Coordenadas Vértice 6 V6
				{-0.5 ,0.5, 0.5},    //Coordenadas Vértice 7 V7
	};


	glBegin(GL_POLYGON);	//Front
		//glColor3f(1.0,0.0,0.0);
		//glNormal3f(0.0,0.0,1.0);
	glVertex3fv(vertice[0]);
	glVertex3fv(vertice[4]);
	glVertex3fv(vertice[7]);
	glVertex3fv(vertice[1]);
	glEnd();

	glBegin(GL_POLYGON);	//Right
	glColor3f(0.0, 0.0, 1.0);
	//glNormal3f(1.0,0.0,0.0);
	glVertex3fv(vertice[0]);
	glVertex3fv(vertice[3]);
	glVertex3fv(vertice[5]);
	glVertex3fv(vertice[4]);
	glEnd();

	glBegin(GL_POLYGON);	//Back
	glColor3f(0.0, 1.0, 0.0);
	//glNormal3f(0.0,0.0,-1.0);
	glVertex3fv(vertice[6]);
	glVertex3fv(vertice[5]);
	glVertex3fv(vertice[3]);
	glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Left
	glColor3f(1.0, 1.0, 1.0);
	//glNormal3f(-1.0,0.0,0.0);
	glVertex3fv(vertice[1]);
	glVertex3fv(vertice[7]);
	glVertex3fv(vertice[6]);
	glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Bottom
	glColor3f(0.4, 0.2, 0.6);
	//glNormal3f(0.0,-1.0,0.0);
	glVertex3fv(vertice[0]);
	glVertex3fv(vertice[1]);
	glVertex3fv(vertice[2]);
	glVertex3fv(vertice[3]);
	glEnd();

	glBegin(GL_POLYGON);  //Top
	glColor3f(0.8, 0.2, 0.4);
	//glNormal3f(0.0,1.0,0.0);
	glVertex3fv(vertice[4]);
	glVertex3fv(vertice[5]);
	glVertex3fv(vertice[6]);
	glVertex3fv(vertice[7]);
	glEnd();
}

int w = 500, h = 500;
int frame = 0, time, timebase = 0;
char s[30];

CCamera objCamera;				// Camera
GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW) 

int font=(int)GLUT_BITMAP_HELVETICA_18;

GLfloat Diffuse[]= { 0.5f, 0.5f, 0.5f, 1.0f };				// Diffuse Light Values
GLfloat Specular[] = { 1.0, 1.0, 1.0, 1.0 };				// Specular Light Values
GLfloat Position[]= { 0.0f, 7.0f, -5.0f, 0.0f };			// Light Position
GLfloat Position2[]= { 0.0f, 0.0f, -5.0f, 1.0f };			// Light Position


GLfloat m_dif1[] = { 0.0f, 0.2f, 1.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec1[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb1[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s1[] = { 18 };

GLfloat m_dif2[] = { 0.8f, 0.2f, 0.0f, 1.0f };				// Diffuse Light Values
GLfloat m_spec2[] = { 0.0, 0.0, 0.0, 1.0 };				// Specular Light Values
GLfloat m_amb2[] = { 0.0, 0.0, 0.0, 1.0 };				// Ambiental Light Values
GLfloat m_s2[] = { 22 };

CTexture text1;
CTexture text2;

CTexture text6;	//Casa01

CTexture textMesa;
CTexture textPata;  //pared 1
CTexture textPata2;  //pared 2
CTexture textWall;
CTexture textSilla;
CTexture textTecho;
CTexture textPiso;
CTexture texP3;   // Pared 4
CTexture texP5;   // Pared 5

CTexture textCuadro1;
CTexture t_Jeng;
CTexture t_PzaJeng;
CTexture t_PzaJeng2;
CTexture t_dode;

CTexture textGrass;
CTexture textSky;

CTexture textGW;

//***************** TEXTURAS PARA EL PROYECTO *****************
CTexture text_skybox;
CTexture text_mesa;
//Ctexture text_mesa2;
CTexture text_vader;	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
CTexture text_pool;
CTexture text_mueble;
CTexture text_metal;

//****************************************************
float rotDodecaedro = 0;
float rotDado;
float rotFan;
float rotBalon;

//#define MAX_FRAMES 90	// limitar cuadros 
//int i_max_steps = 30;	// cuantos cuadros intermedios influye en la velocidad
//int i_curr_steps = 0;
//
//int numero_cancion = -1;
//modelos 3D


CFiguras tablaMesa;
CFiguras pata1Mesa;
CFiguras pata2Mesa;
CFiguras pata3Mesa;
CFiguras pata4Mesa;
CFiguras asientoSilla;
CFiguras respaldoSilla1;
CFiguras respaldoSilla2;
CFiguras respaldoSilla3;
CFiguras respaldoSilla4;
CFiguras techoCasa;
CFiguras pisoCasa;
CFiguras cubo;
CFiguras sky;


CFiguras fig1;
CFiguras fig2;
CFiguras fig3;
CFiguras fig6;

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ MIS FIGURAS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
CFiguras fig10;	//vaso
CFiguras fig11;	//prismamesa
CFiguras fig12;	//cuadros
CFiguras fig13;	//mueble
		
//Cosas del cuarto

void circulo(void)
{
	glScalef(0.1, 0.1, 0.1); // escala para que sea de radio 1 (es de r=10)
	glScalef(0.5, 0.5, 0.5); // para que sea de diametro = 1
	glScalef(0.35, 0.35, 0.35);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POLYGON);

	glVertex3f(0.0f, -10.0f, 0.0f);
	glVertex3f(1.0f, -10.0f, 0.0f);
	glVertex3f(2.0f, -10.0f, 0.0f);
	glVertex3f(3.0f, -9.8f, 0.0f);
	glVertex3f(4.0, -9.5f, 0.0f);
	glVertex3f(5.0, -9.0f, 0.0f);
	glVertex3f(6.0f, -8.4f, 0.0f);
	glVertex3f(7.0f, -7.6f, 0.0f);
	glVertex3f(8.0f, -6.4f, 0.0f);
	glVertex3f(8.9f, -5.0f, 0.0f);
	glVertex3f(9.7f, -3.0f, 0.0f);
	glVertex3f(9.9f, -2.0f, 0.0f);
	glVertex3f(10.0f, -1.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);

	glVertex3f(10.0f, 1.0f, 0.0f);
	glVertex3f(9.9f, 2.0f, 0.0f);
	glVertex3f(9.7f, 3.0f, 0.0f);
	glVertex3f(8.9f, 5.0f, 0.0f);
	glVertex3f(8.0f, 6.4f, 0.0f);
	glVertex3f(7.0f, 7.6f, 0.0f);
	glVertex3f(6.0f, 8.4f, 0.0f);
	glVertex3f(5.0, 9.0f, 0.0f);
	glVertex3f(4.0, 9.5f, 0.0f);
	glVertex3f(3.0f, 9.8f, 0.0f);
	glVertex3f(2.0f, 10.0f, 0.0f);
	glVertex3f(1.0f, 10.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);

	glVertex3f(-1.0f, 10.0f, 0.0f);
	glVertex3f(-2.0f, 10.0f, 0.0f);
	glVertex3f(-3.0f, 9.8f, 0.0f);
	glVertex3f(-4.0, 9.5f, 0.0f);
	glVertex3f(-5.0, 9.0f, 0.0f);
	glVertex3f(-6.0f, 8.4f, 0.0f);
	glVertex3f(-7.0f, 7.6f, 0.0f);
	glVertex3f(-8.0f, 6.4f, 0.0f);
	glVertex3f(-8.9f, 5.0f, 0.0f);
	glVertex3f(-9.7f, 3.0f, 0.0f);
	glVertex3f(-9.9f, 2.0f, 0.0f);
	glVertex3f(-10.0f, 1.0f, 0.0f);

	glVertex3f(-10.0f, 0.0f, 0.0f);
	glVertex3f(-10.0f, -1.0f, 0.0f);
	glVertex3f(-9.9f, -2.0f, 0.0f);
	glVertex3f(-9.7f, -3.0f, 0.0f);
	glVertex3f(-8.9f, -5.0f, 0.0f);
	glVertex3f(-8.0f, -6.4f, 0.0f);
	glVertex3f(-7.0f, -7.6f, 0.0f);
	glVertex3f(-6.0f, -8.4f, 0.0f);
	glVertex3f(-5.0, -9.0f, 0.0f);
	glVertex3f(-4.0, -9.5f, 0.0f);
	glVertex3f(-3.0f, -9.8f, 0.0f);
	glVertex3f(-2.0f, -10.0f, 0.0f);
	glVertex3f(-1.0f, -10.0f, 0.0f);
	glVertex3f(0.0f, -10.0f, 0.0f);

	glEnd();
}
void esfera(GLfloat radio, int meridianos, int paralelos)
{
	GLdouble theta, phi;

	float ctext_s = 1.0 / meridianos;
	float ctext_t = 1.0 / paralelos;
	float v1[] = { 0.0, 0.0, 0.0 };
	float v2[] = { 0.0, 0.0, 0.0 };
	float v3[] = { 0.0, 0.0, 0.0 };
	float v4[] = { 0.0, 0.0, 0.0 };
	int i, j;
	float angulom, angulop;
	angulom = 2 * 3.141592654 / meridianos;
	angulop = 3.141592654 / paralelos;

	for (i = 0; i < meridianos; i++)
	{
		for (j = 0; j < paralelos; j++)
		{
			v1[0] = radio * cos(angulom*i)*sin(angulop*j);
			v1[1] = radio * cos(angulop*j);
			v1[2] = radio * sin(angulom*i)*sin(angulop*j);

			v2[0] = radio * cos(angulom*i)*sin(angulop*(j + 1));
			v2[1] = radio * cos(angulop*(j + 1));
			v2[2] = radio * sin(angulom*i)*sin(angulop*(j + 1));

			v3[0] = radio * cos(angulom*(i + 1))*sin(angulop*(j + 1));
			v3[1] = radio * cos(angulop*(j + 1));
			v3[2] = radio * sin(angulom*(i + 1))*sin(angulop*(j + 1));

			v4[0] = radio * cos(angulom*(i + 1))*sin(angulop*j);
			v4[1] = radio * cos(angulop*j);
			v4[2] = radio * sin(angulom*(i + 1))*sin(angulop*j);

			glBegin(GL_POLYGON);
			glColor3f(0.0, 1.0, 1.0);
			glVertex3fv(v1);
			glVertex3fv(v2);
			glVertex3fv(v3);
			glVertex3fv(v4);
			glEnd();
		}
	}
}

void mueble()
{
	glPushMatrix();

	fig11.prismamesa(50.0, 30.0, 1.0, text_mueble.GLindex);	//Back
	glPushMatrix();
	glTranslatef(-14.5, 0.0, -5.5);
	fig11.prismamesa(50.0, 1.0, 10.0, text_mueble.GLindex);	//left
	glPopMatrix();
	glPushMatrix();
	glTranslatef(14.5, 0.0, -5.5);
	fig11.prismamesa(50.0, 1.0, 10.0, text_mueble.GLindex);	//right
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 24.5, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//top
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 12.25, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//top center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 18.25, -5.5);
	fig11.prismamesa(11.0, 1.0, 10.0, text_mueble.GLindex);	//top center center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -24.5, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//bottom
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -12.25, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//bottom center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -6.25, -5.5);
	fig11.prismamesa(11.0, 1.0, 10.0, text_mueble.GLindex);	//bottom center center
	glPopMatrix();
	glPopMatrix();
}

void muebledoble()
{
	glPushMatrix();

	fig11.prismamesa(50.0, 30.0, 1.0, text_mueble.GLindex);	//Back
	glPushMatrix();
	glTranslatef(-14.5, 0.0, -5.5);
	fig11.prismamesa(25.0, 1.0, 10.0, text_mueble.GLindex);	//left
	glPopMatrix();
	glPushMatrix();
	glTranslatef(14.5, 0.0, -5.5);
	fig11.prismamesa(50.0, 1.0, 10.0, text_mueble.GLindex);	//right
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.0, 24.5, -5.5);
	fig11.prismamesa(1.0, 29.5, 10.0, text_mueble.GLindex);	//top
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 12.25, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//top center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 18.25, -5.5);
	fig11.prismamesa(11.0, 1.0, 10.0, text_mueble.GLindex);	//top center center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -24.5, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//bottom
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -12.25, -5.5);
	fig11.prismamesa(1.0, 28.0, 10.0, text_mueble.GLindex);	//bottom center
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -6.25, -5.5);
	fig11.prismamesa(11.0, 1.0, 10.0, text_mueble.GLindex);	//bottom center center
	glPopMatrix();
	glPopMatrix();
}
void mesa()
{
	glPushMatrix();
	fig11.prismamesa(3.0, 30.0, 30.0, text_mesa.GLindex); //mesa

	glPushMatrix();//pata1
	glTranslatef(-13.0, -9.5, 13.0);
	fig11.prismamesa(16.0, 4.0, 4.0, text_metal.GLindex);
	glPopMatrix();

	glPushMatrix();//pata2
	glTranslatef(13.0, -9.5, 13.0);
	fig11.prismamesa(16.0, 4.0, 4.0, text_metal.GLindex);
	glPopMatrix();

	glPushMatrix();//pata3
	glTranslatef(-13.0, -9.5, -13.0);
	fig11.prismamesa(16.0, 4.0, 4.0, text_metal.GLindex);
	glPopMatrix();

	glPushMatrix();//pata4
	glTranslatef(13.0, -9.5, -13.0);
	fig11.prismamesa(16.0, 4.0, 4.0, text_metal.GLindex);
	glPopMatrix();
	glPopMatrix();
}
void base_mesa(GLuint textura1, GLuint textura2)  //Funcion creacion mesa ajedrez
{

	GLfloat vertice[8][3] = {
				{0.5 ,-0.5, 0.5},    //Coordenadas Vértice 0 V0
				{-0.5 ,-0.5, 0.5},    //Coordenadas Vértice 1 V1
				{-0.5 ,-0.5, -0.5},    //Coordenadas Vértice 2 V2
				{0.5 ,-0.5, -0.5},    //Coordenadas Vértice 3 V3
				{0.5 ,0.5, 0.5},    //Coordenadas Vértice 4 V4
				{0.5 ,0.5, -0.5},    //Coordenadas Vértice 5 V5
				{-0.5 ,0.5, -0.5},    //Coordenadas Vértice 6 V6
				{-0.5 ,0.5, 0.5},    //Coordenadas Vértice 7 V7
	};


	glBindTexture(GL_TEXTURE_2D, textura2);   // choose the texture to use. 
	//glBindTexture(GL_TEXTURE_2D, 0);   // choose the texture to use.
	glBegin(GL_POLYGON);	//Front
		//glColor3f(0.0,0.0,1.0);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(4.0f, 0.0f); glVertex3fv(vertice[4]);
	glTexCoord2f(4.0f, 4.0f); glVertex3fv(vertice[7]);
	glTexCoord2f(0.0f, 4.0f); glVertex3fv(vertice[1]);
	glEnd();

	glBegin(GL_POLYGON);	//Right
		//glColor3f(0.0,0.0,1.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[3]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[4]);
	glEnd();

	glBegin(GL_POLYGON);	//Back
		//glColor3f(0.0,1.0,0.0);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[3]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Left
		//glColor3f(1.0,1.0,1.0);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[1]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[7]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Bottom
		//glColor3f(0.4,0.2,0.6);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[1]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[2]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[3]);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textura1);   // choose the texture to use.
	glBegin(GL_POLYGON);  //Top
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(4.0, 0.0f); glVertex3fv(vertice[4]);
	glTexCoord2f(4.0, 4.0); glVertex3fv(vertice[5]);
	glTexCoord2f(0.0, 4.0); glVertex3fv(vertice[6]);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[7]);
	glEnd();
}
void prisma(GLuint textura1, GLuint textura2)  //Funcion creacion prisma
{

	GLfloat vertice[8][3] = {
				{0.5 ,-0.5, 0.5},    //Coordenadas Vértice 0 V0
				{-0.5 ,-0.5, 0.5},    //Coordenadas Vértice 1 V1
				{-0.5 ,-0.5, -0.5},    //Coordenadas Vértice 2 V2
				{0.5 ,-0.5, -0.5},    //Coordenadas Vértice 3 V3
				{0.5 ,0.5, 0.5},    //Coordenadas Vértice 4 V4
				{0.5 ,0.5, -0.5},    //Coordenadas Vértice 5 V5
				{-0.5 ,0.5, -0.5},    //Coordenadas Vértice 6 V6
				{-0.5 ,0.5, 0.5},    //Coordenadas Vértice 7 V7
	};


	glBindTexture(GL_TEXTURE_2D, textura2);   // choose the texture to use.
	//glBindTexture(GL_TEXTURE_2D, 0);   // choose the texture to use.
	glBegin(GL_POLYGON);	//Front
		//glColor3f(0.0,0.0,1.0);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[4]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[7]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[1]);
	glEnd();

	glBegin(GL_POLYGON);	//Right
		//glColor3f(0.0,0.0,1.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[3]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[4]);
	glEnd();

	glBegin(GL_POLYGON);	//Back
		//glColor3f(0.0,1.0,0.0);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[3]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Left
		//glColor3f(1.0,1.0,1.0);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[1]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[7]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Bottom
		//glColor3f(0.4,0.2,0.6);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[1]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[2]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[3]);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textura1);   // choose the texture to use.
	glBegin(GL_POLYGON);  //Top

	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0f); glVertex3fv(vertice[4]);
	glTexCoord2f(1.0, 1.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(0.0, 1.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[7]);
	glEnd();
}
void refri(GLuint textura1, GLuint textura2)  //Funcion creacion prisma
{

	GLfloat vertice[8][3] = {
				{0.5 ,-0.5, 0.5},    //Coordenadas Vértice 0 V0
				{-0.5 ,-0.5, 0.5},    //Coordenadas Vértice 1 V1
				{-0.5 ,-0.5, -0.5},    //Coordenadas Vértice 2 V2
				{0.5 ,-0.5, -0.5},    //Coordenadas Vértice 3 V3
				{0.5 ,0.5, 0.5},    //Coordenadas Vértice 4 V4
				{0.5 ,0.5, -0.5},    //Coordenadas Vértice 5 V5
				{-0.5 ,0.5, -0.5},    //Coordenadas Vértice 6 V6
				{-0.5 ,0.5, 0.5},    //Coordenadas Vértice 7 V7
	};


	glBindTexture(GL_TEXTURE_2D, textura2);   // choose the texture to use.
	//glBindTexture(GL_TEXTURE_2D, 0);   // choose the texture to use.
	glBegin(GL_POLYGON);	//Front
		//glColor3f(0.0,0.0,1.0);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[4]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[7]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[1]);
	glEnd();

	glBegin(GL_POLYGON);	//Right
		//glColor3f(0.0,0.0,1.0);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[3]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[4]);
	glEnd();

	glBegin(GL_POLYGON);	//Back
		//glColor3f(0.0,1.0,0.0);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[3]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Left
		//glColor3f(1.0,1.0,1.0);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[1]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[7]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[2]);
	glEnd();

	glBegin(GL_POLYGON);  //Bottom
		//glColor3f(0.4,0.2,0.6);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[0]);
	glTexCoord2f(1.0f, 0.0f); glVertex3fv(vertice[1]);
	glTexCoord2f(1.0f, 1.0f); glVertex3fv(vertice[2]);
	glTexCoord2f(0.0f, 1.0f); glVertex3fv(vertice[3]);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, textura1);   // choose the texture to use.
	glBegin(GL_POLYGON);  //Top

	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(1.0, 0.0f); glVertex3fv(vertice[4]);
	glTexCoord2f(1.0, 1.0f); glVertex3fv(vertice[5]);
	glTexCoord2f(0.0, 1.0f); glVertex3fv(vertice[6]);
	glTexCoord2f(0.0f, 0.0f); glVertex3fv(vertice[7]);
	glEnd();
}
void silla(GLfloat xMadera, GLfloat yMadera, GLfloat zMadera, GLfloat xPosSilla, GLfloat yPosSilla, GLfloat zPosSilla) {
	float numBarrotes = -2.9;
	glPushMatrix();
	//posicion y tamaño
	glScalef(xMadera, yMadera, zMadera);
	glTranslatef(xPosSilla, yPosSilla, zPosSilla);
	//figura
	glPushMatrix();
	glScalef(3, .5, 3);
	asientoSilla.prisma(1.0, 1.0, 1.0, textSilla.GLindex);//1
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.35, -3.25, -1.35);
	glScalef(.3, 6, .3);
	pata1Mesa.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	glTranslatef(-9.0, 0, 0);
	pata2Mesa.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	glTranslatef(0, 0, 9);
	pata3Mesa.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	glTranslatef(9, 0, 0);
	pata4Mesa.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1.45, 4.25, -1.45);
	glScalef(0.1, 8.0, 0.1);
	respaldoSilla1.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	for (int numBar = 1; numBar <= 10; numBar++) {
		glTranslatef(numBarrotes, 0, 0);
		respaldoSilla1.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	}
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 1, -1.45);
	glScalef(2.3, 0.1, 0.1);
	respaldoSilla1.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	//
	for (int numBar = 1; numBar <= 30; numBar++) {
		glTranslatef(0, 2, 0);
		respaldoSilla1.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	}
	glTranslatef(0, 12, 0);
	glScalef(1.5, 1, 1);
	respaldoSilla1.prisma(1.0, 1.0, 1.0, textSilla.GLindex);
	glPopMatrix();
	glPopMatrix();

	return;
}
void Paredes(void) {
	//Cuarto en la nada
		//Techo
	glPushMatrix();
	glTranslatef(0, 30, 0);
	pisoCasa.prisma(1, 40, 40, textTecho.GLindex);
	glPopMatrix();

	//piso
	glPushMatrix();
	glTranslatef(0, -10, 0);
	pisoCasa.prisma(1, 40, 40, textPiso.GLindex);
	glPopMatrix();

	//Frente y atras
	glPushMatrix(); //F
	glTranslatef(0.0, 10, -20);
	cubo.prisma(40, 40, 1, textPata2.GLindex);

	glPushMatrix();		//A
	glTranslatef(0, 0, 40);
	cubo.prisma(40, 40, 1, textPata.GLindex);
	glPopMatrix();
	glPopMatrix();

	//derecha e Izquierda 
	glPushMatrix();
	glTranslatef(-20.0, 10, 0);
	glRotatef(90, 0, 1, 0);
	cubo.prisma(40, 40, 1, texP5.GLindex);

	glPushMatrix();
	glTranslatef(0, 0, 40);
	cubo.prisma(40, 40, 1, texP3.GLindex);
	glPopMatrix();
	glPopMatrix();

	return;
}
void Pintura(void) {
	glPushMatrix();
	glScalef(1, 1, -1);
	glTranslatef(7, 10, 0);
	cubo.prisma(15, 7, .2, textCuadro1.GLindex);
	glPopMatrix();
	return;
}
void MesaJuego() {
	glPushMatrix();
	//glDisable(GL_LIGHTING); //Desactiva las luces

	glScalef(1.0, 1.0, 1.0); //para que tenga tamaño adecuado
	glColor3f(1.0, 1.0, 1.0);
	glScalef(3.0, 0.5, 3.0);
	base_mesa(textSilla.GLindex, textSilla.GLindex); //cara de arriba. demás caras

	glScalef(1.0 / 3.0, 1.0 / 0.5, 1.0 / 3.0);
	glPushMatrix(); //abajo
	glTranslatef(0.0, 0.0, 1.75);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(3.0, 0.5, 0.5);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //arriba
	glTranslatef(0.0, 0.0, -1.75);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(3.0, 0.5, 0.5);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //der
	glTranslatef(1.75, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 0.5, 3.0);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //izq
	glTranslatef(-1.75, 0.0, 0.0);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 0.5, 3.0);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //pata derecha arriba
	glTranslatef(1.75, -1.75, -1.75);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 4.0, 0.5);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //pata izq arriba
	glTranslatef(-1.75, -1.75, -1.75);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 4.0, 0.5);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //pata derecha abajo 
	glTranslatef(1.75, -1.75, 1.75);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 4.0, 0.5);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	glPushMatrix(); //pata izq abajo 
	glTranslatef(-1.75, -1.75, 1.75);
	glColor3f(1.0, 1.0, 1.0);
	glScalef(0.5, 4.0, 0.5);
	prisma(textSilla.GLindex, textSilla.GLindex);
	glPopMatrix();

	//glEnable(GL_LIGHTING);
	glPopMatrix();  //fin de la mesa 
}
//END

GLuint createDL()
{
	GLuint ciudadDL;
	//GLuint cieloDL;

	// Create the id for the list
	ciudadDL = glGenLists(1);
	// start list
	glNewList(ciudadDL, GL_COMPILE);

	glEndList();

	return(ciudadDL);
}
void InitGL ( GLvoid )     // Inicializamos parametros
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Negro de fondo	

	glEnable(GL_TEXTURE_2D);

	glShadeModel (GL_SMOOTH);							//Para construir la figura comentar y la luz
	glLightfv(GL_LIGHT1, GL_POSITION, Position);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Diffuse);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/*glEnable ( GL_COLOR_MATERIAL );*/

	glClearDepth(1.0f);									// Configuramos Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Habilitamos Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a realizar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	//Texturas del cuarto y sus elementos

	t_dode.LoadTGA("dodecaedro.tga");
	t_dode.BuildGLTexture();
	t_dode.ReleaseImage();

	textPata.LoadBMP("cuarto/Pared1.bmp");
	textPata.BuildGLTexture();
	textPata.ReleaseImage();

	textPata2.LoadBMP("cuarto/Pared2.bmp");
	textPata2.BuildGLTexture();
	textPata2.ReleaseImage();

	texP3.LoadTGA("cuarto/Pared4.tga");
	texP3.BuildGLTexture();
	texP3.ReleaseImage();

	texP5.LoadTGA("cuarto/Pared5.tga");
	texP5.BuildGLTexture();
	texP5.ReleaseImage();

	textCuadro1.LoadTGA("cuarto/gato.tga");
	textCuadro1.BuildGLTexture();
	textCuadro1.ReleaseImage();


	textPiso.LoadBMP("cuarto/MaderaPiso.bmp");
	textPiso.BuildGLTexture();
	textPiso.ReleaseImage();

	textTecho.LoadTGA("cuarto/Piso2.tga");
	textTecho.BuildGLTexture();
	textTecho.ReleaseImage();

	textGW.LoadBMP("cuarto/MaderaFea.bmp");
	textGW.BuildGLTexture();
	textGW.ReleaseImage();

	textSilla.LoadBMP("cuarto/madera.bmp");
	textSilla.BuildGLTexture();
	textSilla.ReleaseImage();


	t_Jeng.LoadTGA("Jenga_comp.tga");
	t_Jeng.BuildGLTexture();
	t_Jeng.ReleaseImage();


	t_PzaJeng2.LoadTGA("pza_roj.tga");
	t_PzaJeng2.BuildGLTexture();
	t_PzaJeng2.ReleaseImage();

	//**************** TEXTURAS PARA EL PROYECTO *************************

	text_skybox.LoadTGA("texturas/textura_skybox7.tga");
	text_skybox.BuildGLTexture();
	text_skybox.ReleaseImage();

	text_mesa.LoadTGA("texturas/mesa2.tga");
	text_mesa.BuildGLTexture();
	text_mesa.ReleaseImage();

	text_vader.LoadTGA("texturas/vader.tga");	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	text_vader.BuildGLTexture();
	text_vader.ReleaseImage();

	text_pool.LoadTGA("texturas/pool.tga");	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	text_pool.BuildGLTexture();
	text_pool.ReleaseImage();

	text_mueble.LoadTGA("texturas/mueble.tga");	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	text_mueble.BuildGLTexture();
	text_mueble.ReleaseImage();

	text_mueble.LoadTGA("texturas/metal.tga");	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	text_mueble.BuildGLTexture();
	text_mueble.ReleaseImage();

	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ MIS MODELOS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	chair1._3dsLoad("3D/silla1/chair1.3ds");
	chair2._3dsLoad("3D/silla2/chair2.3ds");
	fan._3dsLoad("3D/ventilador/fan.3ds");
	mariposa._3dsLoad("3D/mariposa/Grass in box N220216.3ds");
	balon._3dsLoad("3D/balon/Basketball ball N220909.3ds");
	//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



	




	//objCamera.Position_Camera(10,2.5f,13, 10,2.5f,10, 0, 1, 0);		ORIGINAL
	objCamera.Position_Camera(-2.0, 2.0f, -15.0, -2, 2.0f, 0, 0, 1, 0);


}

void pintaTexto(float x, float y, float z, void *font,char *string)
{
  
  char *c;     //Almacena los caracteres a escribir
  glRasterPos3f(x, y, z);	//Posicion apartir del centro de la ventana
  //glWindowPos2i(150,100);
  for (c=string; *c != '\0'; c++) //Condicion de fin de cadena
  {
    glutBitmapCharacter(font, *c); //imprime
  }
}


void dodecaedro(GLuint textura1) {
	GLfloat vertice[20][3];

	double vertices[20][3];
	double Pi = 3.141592653589793238462643383279502884197;

	double phiaa = 52.62263590;
	double phibb = 10.81231754;
	double r, phia, phib, phic, phid, the72, theb, the;
	int i, j;

	r = 1.0;
	phia = Pi * phiaa / 180.0;
	phib = Pi * phibb / 180.0;
	phic = Pi * (-phibb) / 180.0;
	phid = Pi * (-phiaa) / 180.0;
	the72 = Pi * 72.0 / 180;
	theb = the72 / 2.0;
	the = 0.0;
	for (i = 0; i < 5; i++) {
		vertices[i][0] = r * cos(the)*cos(phia);
		vertices[i][1] = r * sin(the)*cos(phia);
		vertices[i][2] = r * sin(phia);
		the = the + the72;
	}
	the = 0.0;
	for (i = 5; i < 10; i++) {
		vertices[i][0] = r * cos(the)*cos(phib);
		vertices[i][1] = r * sin(the)*cos(phib);
		vertices[i][2] = r * sin(phib);
		the = the + the72;
	}
	the = theb;
	for (i = 10; i < 15; i++) {
		vertices[i][0] = r * cos(the)*cos(phic);
		vertices[i][1] = r * sin(the)*cos(phic);
		vertices[i][2] = r * sin(phic);
		the = the + the72;
	}
	the = theb;
	for (i = 15; i < 20; i++) {
		vertices[i][0] = r * cos(the)*cos(phid);
		vertices[i][1] = r * sin(the)*cos(phid);
		vertices[i][2] = r * sin(phid);
		the = the + the72;
	}

	for (i = 0; i < 20; i++) {
		for (j = 0; j < 3; j++) {
			vertice[i][j] = vertices[i][j];
		}

	}

	glBindTexture(GL_TEXTURE_2D, textura1);

	//12 base frontal
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0.33, 0.21); glVertex3fv(vertice[0]);
	glTexCoord2f(0.44, 0.13); glVertex3fv(vertice[1]);
	glTexCoord2f(0.55, 0.21); glVertex3fv(vertice[2]);
	glTexCoord2f(0.51, 0.33); glVertex3fv(vertice[3]);
	glTexCoord2f(0.37, 0.33); glVertex3fv(vertice[4]);
	glEnd();

	//11
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.33, 0.21); glVertex3fv(vertice[0]);
	glTexCoord2f(0.44, 0.13); glVertex3fv(vertice[1]);
	glTexCoord2f(0.4, 0); glVertex3fv(vertice[6]);
	glTexCoord2f(0.26, 0); glVertex3fv(vertice[10]);
	glTexCoord2f(0.22, 0.13); glVertex3fv(vertice[5]);
	glEnd();
	//7
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.44, 0.13); glVertex3fv(vertice[1]);
	glTexCoord2f(0.55, 0.21); glVertex3fv(vertice[2]);
	glTexCoord2f(0.65, 0.13); glVertex3fv(vertice[7]);
	glTexCoord2f(0.62, 0); glVertex3fv(vertice[11]);
	glTexCoord2f(0.48, 0); glVertex3fv(vertice[6]);
	glEnd();
	//8
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.55, 0.21); glVertex3fv(vertice[2]);
	glTexCoord2f(0.51, 0.33); glVertex3fv(vertice[3]);
	glTexCoord2f(0.62, 0.41); glVertex3fv(vertice[8]);
	glTexCoord2f(0.73, 0.33); glVertex3fv(vertice[12]);
	glTexCoord2f(0.68, 0.21); glVertex3fv(vertice[7]);
	glEnd();
	//2
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.51, 0.33); glVertex3fv(vertice[3]);
	glTexCoord2f(0.37, 0.33); glVertex3fv(vertice[4]);
	glTexCoord2f(0.33, 0.46); glVertex3fv(vertice[9]);
	glTexCoord2f(0.44, 0.54); glVertex3fv(vertice[13]);
	glTexCoord2f(0.55, 0.46); glVertex3fv(vertice[8]);
	glEnd();
	//3
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.37, 0.33); glVertex3fv(vertice[4]);
	glTexCoord2f(0.33, 0.21); glVertex3fv(vertice[0]);
	glTexCoord2f(0.2, 0.21); glVertex3fv(vertice[5]);
	glTexCoord2f(0.16, 0.33); glVertex3fv(vertice[14]);
	glTexCoord2f(0.26, 0.41); glVertex3fv(vertice[9]);
	glEnd();

	//4
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.44, 0.79); glVertex3fv(vertice[15]);
	glTexCoord2f(0.55, 0.87); glVertex3fv(vertice[16]);
	glTexCoord2f(0.51, 1); glVertex3fv(vertice[11]);
	glTexCoord2f(0.38, 1); glVertex3fv(vertice[6]);
	glTexCoord2f(0.33, 0.87); glVertex3fv(vertice[10]);
	glEnd();
	//10
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.55, 0.87); glVertex3fv(vertice[16]);
	glTexCoord2f(0.65, 0.79); glVertex3fv(vertice[17]);
	glTexCoord2f(0.76, 0.87); glVertex3fv(vertice[12]);
	glTexCoord2f(0.72, 1); glVertex3fv(vertice[7]);
	glTexCoord2f(0.59, 1); glVertex3fv(vertice[11]);
	glEnd();
	//9
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.65, 0.79); glVertex3fv(vertice[17]);
	glTexCoord2f(0.62, 0.67); glVertex3fv(vertice[18]);
	glTexCoord2f(0.72, 0.58); glVertex3fv(vertice[13]);
	glTexCoord2f(0.83, 0.67); glVertex3fv(vertice[8]);
	glTexCoord2f(0.79, 0.79); glVertex3fv(vertice[12]);
	glEnd();
	//6
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.62, 0.67); glVertex3fv(vertice[18]);
	glTexCoord2f(0.48, 0.67); glVertex3fv(vertice[19]);
	glTexCoord2f(0.44, 0.54); glVertex3fv(vertice[14]);
	glTexCoord2f(0.55, 0.46); glVertex3fv(vertice[9]);
	glTexCoord2f(0.66, 0.54); glVertex3fv(vertice[13]);
	glEnd();
	//1
	glBegin(GL_POLYGON);
	//glNormal3f(0, -1, 0);
	glTexCoord2f(0.48, 0.67); glVertex3fv(vertice[19]);
	glTexCoord2f(0.44, 0.79); glVertex3fv(vertice[15]);
	glTexCoord2f(0.31, 0.79); glVertex3fv(vertice[10]);
	glTexCoord2f(0.27, 0.67); glVertex3fv(vertice[5]);
	glTexCoord2f(0.37, 0.59); glVertex3fv(vertice[14]);
	glEnd();

	//5 base posterior
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glTexCoord2f(0.44, 0.79); glVertex3fv(vertice[15]);
	glTexCoord2f(0.55, 0.87); glVertex3fv(vertice[16]);
	glTexCoord2f(0.65, 0.79); glVertex3fv(vertice[17]);
	glTexCoord2f(0.62, 0.67); glVertex3fv(vertice[18]);
	glTexCoord2f(0.48, 0.67); glVertex3fv(vertice[19]);
	glEnd();
}

void display ( void )   // Creamos la funcion donde se dibuja
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	glPushMatrix();

		glRotatef(g_lookupdown,1.0f,0,0);

		gluLookAt(	objCamera.mPos.x,  objCamera.mPos.y,  objCamera.mPos.z,	
					objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,	
					objCamera.mUp.x,   objCamera.mUp.y,   objCamera.mUp.z);
		//glRotatef(-90,01,0);

		
	
		/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ AQUI DIBUJAMOS $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/


		glPushMatrix();

		glPushMatrix();
		glDisable(GL_LIGHTING);
		fig1.skybox(100.0, 50.0, 100.0, text_skybox.GLindex);			//AQUI ESTA EL SKYBOX
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(49.49, 5.0, 25.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		fig12.cuadro(30.0, 25.0, 1.0, text_pool.GLindex);				//CUADRO POOL
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(30.0, 5.0, -49.49);
		fig12.cuadro(30.0, 30.0, 1.0, text_vader.GLindex);				//CUADRO VADER
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glDisable(GL_LIGHTING);
		glTranslatef(30.0, -4.99, 49.0);
		glScalef(0.7, 0.8, 0.7);
		mueble();														//MUEBLE
		glEnable(GL_LIGHTING);
		glPopMatrix();


		glPushMatrix();
		glTranslatef(-0.7, -4.99, -8.0);
		glScalef(0.8, 0.8, 0.8);
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(-61.0, 0.0, 55.0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		muebledoble();												//MUEBLE DOBLE
		glPushMatrix();
		glTranslatef(-29.0, 0.0, 0.0);
		glRotatef(180, 0.0, 0.0, 1.0);
		muebledoble();												//MUEBLE DOBLE
		glPopMatrix();
		glPopMatrix();
		glEnable(GL_LIGHTING);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0.0, -7.49, 0.0);
		mesa();															//MESA
		glPopMatrix();



		glPushMatrix();
		glTranslatef(12.0, -8.20, 14.0);
		fig10.vaso();													//Buchaca1
		glPopMatrix();

		glPushMatrix();
		glTranslatef(12.0, -8.20, 0.0);
		fig10.vaso();													//Buchaca2
		glPopMatrix();

		glPushMatrix();
		glTranslatef(12.0, -8.20, -14.0);
		fig10.vaso();													//Buchaca3
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-12.0, -8.20, 14.0);
		fig10.vaso();													//Buchaca1
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-12.0, -8.20, 0.0);
		fig10.vaso();													//Buchaca2
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-12.0, -8.20, -14.0);
		fig10.vaso();													//Buchaca3
		glPopMatrix();

		

		glPushMatrix();
		glTranslatef(25.0, -15.0, 45.0);
		glRotatef(rotFan, 0.0, 1.0, 0.0);
		glScalef(0.1, 0.1, 0.1);
		balon.GLrender(NULL, _SHADED, 1.0);		//BALON
		glPopMatrix();

		glPushMatrix();
					glEnable(GL_ALPHA_TEST);
					glAlphaFunc(GL_GREATER, 0.1);
					glTranslatef(0.0, 11.3855, 0.0);
					glScalef(0.5, 0.5, 0.5);
					glRotatef(rotFan, 0, 1, 0);
					fan.GLrender(NULL, _SHADED, 1.0);			//VENTILADOR 
					glDisable(GL_ALPHA_TEST);
				glPopMatrix();

				glPushMatrix();
					glTranslatef(40.0, -25.0, 2.3);
					glRotatef(0, 0.0, 1.0, 0.0);
					glScalef(0.10, 0.17, 0.15);
					chair1.GLrender(NULL, _SHADED, 1.0);	//SILLA TRACERA
				glPopMatrix();

		glPushMatrix();
		//Pintura 	
		glPushMatrix();
		glTranslatef(0, 0, 19.5);
		Pintura();
		glPopMatrix();

		glPopMatrix();

		//glEnable(GL_LIGHTING);
		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);

		glPopMatrix();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 0.0, 0.0);
		pintaTexto(-11, 12.0, -14.0, (void *)font, "GLYFFOS");
		pintaTexto(-11, 8.5, -14, (void *)font, s);
		glColor3f(1.0, 1.0, 1.0);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);


		glutSwapBuffers();

}
void animacion()
{
	fig3.text_izq -= 0.01;
	fig3.text_der -= 0.01;
	if (fig3.text_izq < -1)
		fig3.text_izq = 0;
	if (fig3.text_der < 0)
		fig3.text_der = 1;

	//Movimiento 

	if (play) //al presionar l para reproducir
	{

		if (i_curr_steps >= i_max_steps) //end of animation between frames?   ver si ya acabo la animacion verificando cuadros intermedios
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation		
				KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotJenga - KeyFrame[playIndex].rotJenga) / i_max_steps;
				KeyFrame[playIndex].incjx = (KeyFrame[playIndex + 1].jengx - KeyFrame[playIndex].jengx) / i_max_steps;
				KeyFrame[playIndex].incjy = (KeyFrame[playIndex + 1].jengy - KeyFrame[playIndex].jengy) / i_max_steps;
			}
		}
		else
		{    //dibujar la informacion
			rotJenga += KeyFrame[playIndex].rotInc3;
			jengx += KeyFrame[playIndex].incjx;
			jengy += KeyFrame[playIndex].incjy;

			i_curr_steps++;
		}

	}

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s, "FPS:%4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

	glutPostRedisplay();
}
void ventilador()
{
	int i = 0;
	for (i = 0; i < 15; i++)
	{
		rotFan++;
	}
	glutPostRedisplay();
}
void reshape(int width, int height)   // Creamos funcion Reshape
{
	if (height == 0)										// Prevenir division entre cero
	{
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);						// Seleccionamos Projection Matrix
	glLoadIdentity();

	// Tipo de Vista

	glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 170.0);

	glMatrixMode(GL_MODELVIEW);							// Seleccionamos Modelview Matrix
	glLoadIdentity();
}
void keyboard(unsigned char key, int x, int y)  // Create Keyboard Function
{
	switch (key) {
	case 'w':   //Movimientos de camara
	case 'W':
		objCamera.Move_Camera(CAMERASPEED + 0.0025);
		break;

	case 's':
	case 'S':
		objCamera.Move_Camera(-(CAMERASPEED + 0.0025));
		break;

	case 'a':
	case 'A':
		objCamera.Strafe_Camera(-(CAMERASPEED + 0.0025));
		break;

	case 'd':
	case 'D':
		objCamera.Strafe_Camera(CAMERASPEED + 0.0025);
		break;

	case 'h':    //Prende y apaga luz 0 adentro y fuera del cuarto
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		//glEnable(GL_LIGHTING);
		break;
	case 'H':
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		//glDisable(GL_LIGHTING);
		break;



	case 'l':
	case 'L':
		if (play == false && (FrameIndex > 1))
		{

			rotJenga = KeyFrame[0].rotJenga;
			jengx = KeyFrame[0].jengx;
			jengy = KeyFrame[0].jengy;

			//First Interpolation

			KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotJenga - KeyFrame[playIndex].rotJenga) / i_max_steps;
			KeyFrame[playIndex].incjx = (KeyFrame[playIndex + 1].jengx - KeyFrame[playIndex].jengx) / i_max_steps;
			KeyFrame[playIndex].incjy = (KeyFrame[playIndex + 1].jengy - KeyFrame[playIndex].jengy) / i_max_steps;


			printf("PzaJenga: %f \n", KeyFrame[playIndex].rotJenga);
			printf("PzaJenga X: %f \n", KeyFrame[playIndex].jengx);
			printf("PzaJenga Y: %f \n", KeyFrame[playIndex].jengy);
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
		break;


	case 'k':		// Para guardar los cuadros
	case 'K':
		if (FrameIndex < MAX_FRAMES)
		{
			printf("frameindex %d\n", FrameIndex);

			FrameIndex++;
		}

		break;

	case 'm':
		sim = sim - 0.3;
		break;

	case 'M':
		sim = sim + 0.3;
		break;



	case 27:        // Cuando Esc es presionado...
		exit(0);   // Salimos del programa
		break;
	default:        // Cualquier otra
		break;
	}

	glutPostRedisplay();
}
void arrow_keys(int a_keys, int x, int y)  // Funcion para manejo de teclas especiales (arrow keys)
{
	switch (a_keys) {
	case GLUT_KEY_PAGE_UP:
		objCamera.UpDown_Camera(CAMERASPEED);
		break;

	case GLUT_KEY_PAGE_DOWN:
		objCamera.UpDown_Camera(-CAMERASPEED);
		break;

	case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
		g_lookupdown -= 1.0f;
		break;

	case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
		g_lookupdown += 1.0f;
		break;

	case GLUT_KEY_LEFT:
		objCamera.Rotate_View(-CAMERASPEED);
		break;

	case GLUT_KEY_RIGHT:
		objCamera.Rotate_View(CAMERASPEED);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}
void menuKeyFrame(int id)
{
	switch (id)
	{
	case 0:
		if (FrameIndex < MAX_FRAMES)
		{
			printf("frameindex %d\n", FrameIndex);


			FrameIndex++;
		}


		break;

	case 1:
		if (play == false && FrameIndex > 1)
		{


			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}
		break;


	}
}
void menu(int id)
{
	/*switch (id)
	{
		case 1:
			g_fanimacion^= true; //Activamos/desactivamos la animacíon
			break;

	}*/
}
int main(int argc, char** argv)   // Main Function
{
	int submenu;
	glutInit(&argc, argv); // Inicializamos OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Doble )
	glutInitWindowSize(500, 500);	// Tamaño de la Ventana
	glutInitWindowPosition(0, 0);	//Posicion de la Ventana
	glutCreateWindow("Computacion Grafica"); // Nombre de la Ventana       
	//glutFullScreen     ( );         // Full Screen																										     		     /* Body Love 3.0*/
	InitGL();						// Parametros iniciales de la aplicacion
	glutDisplayFunc(display);  //Indicamos a Glut función de dibujo
	glutReshapeFunc(reshape);	//Indicamos a Glut función en caso de cambio de tamano
	glutKeyboardFunc(keyboard);	//Indicamos a Glut función de manejo de teclado
	glutSpecialFunc(arrow_keys);	//Otras
	glutIdleFunc(animacion);
	//glutIdleFunc(baloncito);
	glutIdleFunc(ventilador);
					//para usar los botones del mouse (sin mover)
	//glutIdleFunc		  ( animacion );
	submenu = glutCreateMenu(menuKeyFrame);
	glutAddMenuEntry("Guardar KeyFrame", 0);
	glutAddMenuEntry("Reproducir Animacion", 1);
	glutCreateMenu(menu);
	glutAddSubMenu("Animacion", submenu);

	glutAttachMenu(GLUT_RIGHT_BUTTON);


	glutMainLoop();          // 

	return 0;
}

