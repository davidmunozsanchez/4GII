//**************************************************************************
// Práctica 3 
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B3.h"


using namespace std;

bool subir = true;
bool condicion = true;

// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, CILINDRO, CONO, ESFERA, EXTRUSION, EXCAVADORA, ROTACION_PLY, PRUEBA, PIRAMIDE_MITAD} _tipo_objeto;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=650,Window_high=650;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_piramideMitad piramide_mitad(0.85, 1.3);
_objeto_ply  ply; 
_rotacion rotacion;
_rotacion_PLY r_ply;
_cilindro cilindro(1,2,50); 
_cono cono(1,2,50);
_esfera esfera(1,50,50);
_excavadora excavadora;
_extrusion *extrusion;
_atst prueba1;


// _objeto_ply *ply;


//**************************************************************************
//
//***************************************************************************
void animacion()
{

    if (prueba1.giro_cabeza_vertical < 25 and prueba1.giro_sobre_eje_cabeza<40)
    {
        prueba1.giro_cabeza_vertical += 2.5;
        prueba1.giro_sobre_eje_cabeza += 2.5;
    }

    else if (prueba1.giro_sobre_eje_cabeza > -40){
        prueba1.giro_sobre_eje_cabeza -= 2.5;
    }

    else if(prueba1.apertura_pies < 20 and condicion){
        
        prueba1.apertura_pies += 2.5;

        if (prueba1.apertura_pies == 20){
            condicion = false;
        }
    }

    else if(prueba1.apertura_pies > 0 and !condicion){
         prueba1.apertura_pies -= 2.5;
    }

    else if(prueba1.factor_tronco < 30){
        if (prueba1.flexion_piernas < 40 and subir){
            prueba1.flexion_piernas += 2.5;
            prueba1.factor_pies += 0.07;
            prueba1.factor_tronco += 0.05;

            if (prueba1.flexion_piernas == 40)
            {
                subir = false;
            }
                
        }
        else if (prueba1.flexion_piernas >= 0 and !subir)
        {
            prueba1.flexion_piernas -= 2.5;
            prueba1.factor_pies -= 0.07;

            if (prueba1.flexion_piernas == 0)
            {
                subir = true;
            }
        }
    }
    
    glutPostRedisplay();
}
void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,5);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,5);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,5);break;
        case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,5);break;
        case CILINDRO: cilindro.draw(modo,1.0,0.0,0.0,5);break;
        case CONO: cono.draw(modo,1.0,0.0,0.0,5);break;
        case ESFERA: esfera.draw(modo,1.0,0.0,0.0,5);break;
        case EXCAVADORA: excavadora.draw(modo,1.0,0.0,0.0,5);break;
        case EXTRUSION: extrusion->draw(modo,1.0,0.0,0.0,5);break;
        case ROTACION_PLY: r_ply.draw(modo, 1.0, 0.0, 0.0, 5);break;
        case PRUEBA: prueba1.draw(modo, 1.0, 0.0, 0.0, 5);break;
        case PIRAMIDE_MITAD: piramide_mitad.draw(modo, 1.0, 0.0, 0.0, 5);break;
	}

}


//**************************************************************************
//
//***************************************************************************

void draw(void)
{
clean_window();
change_observer();
draw_axis();
draw_objects();
glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case 'Q':exit(0);
	case '1':modo=POINTS;break;
	case '2':modo=EDGES;break;
	case '3':modo=SOLID;break;
	case '4':modo=SOLID_COLORS;break;
        case 'P':t_objeto=PIRAMIDE;break;
        case 'L':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'R':t_objeto=ROTACION;break;
        case 'V':t_objeto=CILINDRO;break;
        case 'B':t_objeto=CONO;break;
        case 'N':t_objeto=ESFERA;break;
        case 'Z':t_objeto=EXCAVADORA;break;
        case 'X':t_objeto=EXTRUSION;break;
        case 'M':t_objeto=ROTACION_PLY;break;
        case 'A':t_objeto=PRUEBA;break;
        case '9':t_objeto=PIRAMIDE_MITAD;break;
        case 'C':
            prueba1.apertura_pies = 0.0;
            prueba1.giro_cabeza_vertical = 0.0;
            prueba1.giro_sobre_eje_cabeza = 0.0;
            prueba1.flexion_piernas = 0.0;
            prueba1.factor_pies = 0.0;
            prueba1.factor_tronco2 = 0.0;
            prueba1.factor_tronco = 0.0;

            
            glutIdleFunc(animacion);
            
            
        break;
	}
glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{
    
switch (Tecla1){
   case GLUT_KEY_LEFT:Observer_angle_y--;break;
   case GLUT_KEY_RIGHT:Observer_angle_y++;break;
   case GLUT_KEY_UP:Observer_angle_x--;break;
   case GLUT_KEY_DOWN:Observer_angle_x++;break;
   case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
   case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
	
   //case GLUT_KEY_F1:excavadora.giro_cabina+=5;break;
   //case GLUT_KEY_F2:excavadora.giro_cabina-=5;break;
   /*case GLUT_KEY_F3:excavadora.giro_primer_brazo+=1;
        if (excavadora.giro_primer_brazo > excavadora.giro_primer_brazo_max)
            excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_max;break;
   case GLUT_KEY_F4:excavadora.giro_primer_brazo-=1;
        if (excavadora.giro_primer_brazo < excavadora.giro_primer_brazo_min)
            excavadora.giro_primer_brazo = excavadora.giro_primer_brazo_min;break;*/
   /*case GLUT_KEY_F5:excavadora.giro_segundo_brazo+=1;
        if (excavadora.giro_segundo_brazo > excavadora.giro_segundo_brazo_max)
            excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_max;break;
   case GLUT_KEY_F6:excavadora.giro_segundo_brazo-=1;
        if (excavadora.giro_segundo_brazo < excavadora.giro_segundo_brazo_min) 
            excavadora.giro_segundo_brazo = excavadora.giro_segundo_brazo_min;break;
   case GLUT_KEY_F7:excavadora.giro_pala+=1;
        if (excavadora.giro_pala > excavadora.giro_pala_max)
            excavadora.giro_pala = excavadora.giro_pala_max;break;
   case GLUT_KEY_F8:excavadora.giro_pala-=1;
        if (excavadora.giro_pala < excavadora.giro_pala_min)
            excavadora.giro_pala = excavadora.giro_pala_min;break;*/
    
    
    case GLUT_KEY_F1:
    if (prueba1.giro_sobre_eje_cabeza > 0)
        prueba1.giro_sobre_eje_cabeza -= 5;
    else if (static_cast<int>(prueba1.giro_sobre_eje_cabeza) > -40)
        prueba1.giro_sobre_eje_cabeza -= 5;
    
    break;
    case GLUT_KEY_F2:
        if (static_cast<int>(prueba1.giro_sobre_eje_cabeza) < 40)
            prueba1.giro_sobre_eje_cabeza += 5;
    
        break;
    case GLUT_KEY_F3:
        if (static_cast<int>(prueba1.giro_cabeza_vertical) < 25)
            prueba1.giro_cabeza_vertical+= 2.5;
    
        break;
    case GLUT_KEY_F4:
        if (prueba1.giro_cabeza_vertical > 0)
            prueba1.giro_cabeza_vertical-= 2.5;
        else if (static_cast<int>(prueba1.giro_cabeza_vertical) > -25)
            prueba1.giro_cabeza_vertical -= 2.5;
        break;

    case GLUT_KEY_F5:
        if (static_cast<int>(prueba1.apertura_pies) < 20)
            prueba1.apertura_pies += 5;
        break;

    case GLUT_KEY_F6:
        if(static_cast<int>(prueba1.apertura_pies) > 0)
            prueba1.apertura_pies -= 5;
        else if (static_cast<int>(prueba1.apertura_pies) > -20)
            prueba1.apertura_pies -= 5;
        break;
    case GLUT_KEY_F7:
        if (static_cast<int>(prueba1.flexion_piernas) < 40){
            prueba1.flexion_piernas += 5;
            prueba1.factor_pies += 0.15;
            prueba1.factor_tronco += 0.1;
            prueba1.factor_tronco2 = prueba1.factor_tronco;
        }
            
        break;
    case GLUT_KEY_F8:
        if (static_cast<int>(prueba1.flexion_piernas) >= 0){
            prueba1.flexion_piernas -= 5;
            prueba1.factor_pies -= 0.15;
            prueba1.factor_tronco2 -= 0.1;
            prueba1.factor_tronco = prueba1.factor_tronco2;
        }
        break;
              
                
	}
        
    
glutPostRedisplay();
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=4*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection();
glViewport(0,0,Window_width,Window_high);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char *argv[] )
{
 
// perfil 

vector<_vertex3f> perfil, poligono;
_vertex3f aux;

aux.x=1.0; aux.y=-1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=0.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.0; aux.y=1.0; aux.z=0.0;
perfil.push_back(aux);
aux.x=1.5; aux.y=1.2; aux.z=0.0;
perfil.push_back(aux);


rotacion.parametros(perfil,6,0,1,1);

aux.x=1.0; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);
aux.x=1.0; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=-1.0;
poligono.push_back(aux);
aux.x=-0.8; aux.y=0.0; aux.z=0.0;
poligono.push_back(aux);
aux.x=-1.2; aux.y=0.0; aux.z=1.0;
poligono.push_back(aux);

extrusion= new _extrusion(poligono, 0.25, 1.0, 0.25);

// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 3");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);

// funcion de inicialización
initialize();

// creación del objeto ply
ply.parametros(argv[1]);
r_ply.parametros_PLY(argv[2], 50);

//ply = new _objeto_ply(argv[1]);

// inicio del bucle de eventos
glutMainLoop();
return 0;
}
