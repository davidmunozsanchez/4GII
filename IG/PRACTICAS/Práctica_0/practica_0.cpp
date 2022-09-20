#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

void Ejes(int width)
{
   glLineWidth(width);
   glBegin(GL_LINES);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(-1.0, 0.0, 0.0);
   glVertex3f(1.0, 0.0, 0.0);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(0.0, -1.0, 0.0);
   glColor3f(1.0, 1.0, 0.0); // En modo flat el eje se queda en amarillo, para que se vea en verde y amarillo hay que poner GL_SMOOTH. FLAT coge el último color activado
   glVertex3f(0.0, 1.0, 0.0);
   glEnd();
}

void Circle(GLfloat radio, GLfloat cx, GLfloat cy, GLint n, GLenum modo)
{
   int i;
   if (modo == GL_LINE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   else if (modo == GL_FILL)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   else
      glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
   glBegin(GL_POLYGON);
   for (i = 0; i < n; i++)
      glVertex2f(cx + radio * cos(2.0 * M_PI * i / n), cy + radio * sin(2.0 * M_PI * i / n));
   glEnd();
}

void Monigote()
{

   // cara
   glLineWidth(1);
   glColor3f(1.0, 0.8, 0.6);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glBegin(GL_POLYGON);
   glVertex3f(-0.2, 0.0, 0.0);
   glVertex3f(0.2, 0.0, 0.0);
   glVertex3f(0.2, 0.55, 0.0);
   glVertex3f(-0.2, 0.55, 0.0);
   glEnd();

   glColor3f(0.0, 0.0, 0.);
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glBegin(GL_POLYGON);
   glVertex3f(-0.2, 0.0, 0.0);
   glVertex3f(0.2, 0.0, 0.0);
   glVertex3f(0.2, 0.55, 0.0);
   glVertex3f(-0.2, 0.55, 0.0);
   glEnd();
}

static void Init()
{

   glShadeModel(GL_FLAT); // Si cambiamos a GL_SMOOTH te hace interpolación de colores
}

static void Reshape(int width, int height)
{
   glViewport(0, 0, (GLint)width, (GLint)height); // Se le va pasando el nuevo valor cuando se cambia el tamaño
   glOrtho(-1.0, 1.0, -1.0, 1.0, -10, 10.0);      // Espacio del mundo, los dos primeros x los siguientes la y y los siguientes la z
}

static void Display() // Es decir, display vuelve a crear lo que había que pintar.
{

   glClearColor(0.5, 0.5, 0.5, 0.0);
   glClear(GL_COLOR_BUFFER_BIT); // Limpiamos el buffer para que no arrastre la imagen anterior

   Ejes(6);
   Monigote();
   glColor3f(0.55, 0.0, 0.0);
   Circle(0.05, -0.1, 0.4, 5, GL_FILL);
   Circle(0.05, 0.1, 0.4, 5, GL_FILL);

   glFlush();
}

static void Keyboard(unsigned char key, int x, int y)
{

   if (key == 27)
      exit(0);
}

int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);

   glutInitWindowPosition(20, 100);
   glutInitWindowSize(500, 500);
   glutCreateWindow("Practica 0 IG");

   Init();

   glutReshapeFunc(Reshape);   // Cambio de tamanio
   glutDisplayFunc(Display);   // Cuando está oculta nuestra pantalla, pinchamos y se vuelve a ver
   glutKeyboardFunc(Keyboard); // En este caso solo está activa la tecla de escape

   glutMainLoop();

   return 0;
}
