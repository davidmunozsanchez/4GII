//**************************************************************************
// Práctica 2 
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>


const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID,SOLID_COLORS} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
vector<_vertex3f> colores_vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_colores();
void 	draw(_modo modo, float r, float g, float b, float grosor);

/* asignación de colores */
void 	colors_random();
void 	colors_chess(float r1, float g1, float b1, float r2, float g2, float b2);


vector<_vertex3i> caras;
vector<_vertex3f> colores_caras;
};

//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=1.0);
};

class _piramideMitad: public _triangulos3D
{
public:
       _piramideMitad(float tam = 0.5, float al = 1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

void  parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************
// tipo indica si es una figura por revolución normal o bien un cono o una esfera
// tipo=0 normal, tipo=1 cono, tipo=2 esfera
// tapa_in=0 sin tapa, tapa_in=1 con tapa
// tapa_su=0 sin tapa, tapa_su=1 con tapa

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
       
void  parametros(vector<_vertex3f> perfil, int num, int tipo, int tapa_in, int tapa_su);
};

 
//************************************************************************
// cilindro
//************************************************************************

class _cilindro: public _rotacion
{
public:
       _cilindro(float radio=1.0, float altura=2.0, int num=12);
};

//************************************************************************
// cono
//************************************************************************

class _cono: public _rotacion
{
public:
       _cono(float radio, float altura, int num);
};

//************************************************************************
// esfera
//************************************************************************

class _esfera: public _rotacion
{
public:
       _esfera(float radio, int num1, int num2);
};


//************************************************************************
// rotacion archivo PLY
//************************************************************************

class _rotacion_PLY: public _rotacion
{
public:
       _rotacion_PLY();
void  parametros_PLY(char *archivo, int num);
};


//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion: public _triangulos3D
{
public:
       _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};

//practica3 objeto David Muñoz

//piezas------------------------------------

class _piernaBaja: public _triangulos3D
{
public:
       _piernaBaja();
float ancho, alto, fondo;
void draw(_modo modo, float r, float g, float b, float grosor);

protected:
       _cilindro cilindro;
       _cilindro cilindro2;

};

class _pie: public _triangulos3D
{
public:
       _pie();
float ancho, alto, fondo;
void draw(_modo modo, float r, float g, float b, float grosor);

protected:
       _cilindro cilindro;
       _piramideMitad base;

};

class _tronco: public _triangulos3D
{
public:
       _tronco();
float ancho, alto, fondo;
void draw(_modo modo, float r, float g, float b, float grosor);

protected:
       _cilindro cilindro;
       _cubo base;

};

class _cabeza: public _triangulos3D
{
public:
       _cabeza();
float ancho, alto, fondo;
void draw(_modo modo, float r, float g, float b, float grosor);

protected:
       _cubo atras;
       _piramideMitad alante;

};

class _atst: public _triangulos3D
{
public:
       _atst();
float ancho_base, alto_base, fondo_base;
float ancho_pierna, alto_pierna, fondo_pierna;

void draw(_modo modo, float r, float g, float b, float grosor);

float giro_cabeza_vertical;
float giro_sobre_eje_cabeza;
float apertura_pies;
float flexion_piernas;
float factor_pies;
float factor_tronco, factor_tronco2;



protected:
       _cabeza cabeza;
       _tronco tronco;
       _piernaBaja pata;
       _pie pie;

};

//************************************************************************
// práctica 3, objeto jerárquico articulado excavadora
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

class _pala: public _triangulos3D
{
public:
      _pala(float radio=1.0, float ancho=2.0, int num=8);
};

//************************************************************************
// brazo
//************************************************************************

class _brazo: public _triangulos3D
{
public:
      _brazo();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// cabina
//************************************************************************

class _cabina: public _triangulos3D
{
public:
       _cabina();
void  draw(_modo modo, float r, float g, float b, float grosor);

float ancho;
float alto;
float fondo;

protected:
_cubo cubo;
};

//************************************************************************
// sustentación
//************************************************************************

class _sustentacion: public _triangulos3D
{
public:
      _sustentacion();
void  draw(_modo modo, float r, float g, float b, float grosor);
       
float ancho;
float alto;
float fondo;



float radio;

protected:
_cilindro rueda;
_cubo base;
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

class _excavadora: public _triangulos3D
{
public:
       _excavadora();
       
void  draw(_modo modo, float r, float g, float b, float grosor);

float giro_cabina;
float giro_primer_brazo;
float giro_segundo_brazo;
float giro_pala;

float giro_primer_brazo_max;
float giro_primer_brazo_min;
float giro_segundo_brazo_max;
float giro_segundo_brazo_min;
float giro_pala_max;
float giro_pala_min;

float tamanio_pala;

protected:
_pala pala;
_brazo brazo;
_cabina cabina;
_sustentacion sustentacion;
};
