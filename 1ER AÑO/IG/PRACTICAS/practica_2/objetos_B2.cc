//**************************************************************************
// Práctica 2
//**************************************************************************

#include "objetos_B2.h"
#include "file_ply_stl.hpp"

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
  int i;
  glPointSize(grosor);
  glColor3f(r, g, b);
  glBegin(GL_POINTS);
  for (i = 0; i < vertices.size(); i++)
  {
    glVertex3fv((GLfloat *)&vertices[i]);
  }
  glEnd();
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
  int i;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(grosor);
  glColor3f(r, g, b);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
  int i;
  // Modo GLFILL para rellenar la cara entera del objeto
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(r, g, b);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::draw_solido_colores()
{
  int i;
  int cantidad_colores = colores.size();
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  // El codigo es muy parecido al de antes con la diferencia de que no tenemos el color
  // como argumento de la función, si no que lo vamos cogiendo del vector colores
  // y se va cambiando el color de cada cara en cada iteración del bucle.
  // Dado que no tiene por qué haber los mismos colores que caras (es decir, se puede repetir un color),
  // la posición del vector de colores que vamos a tomar será modulo la cantidad de colores que haya en el vector,
  // Si hay colores de más, no importa y si hay colores de menos, nunca intentará acceder a una posición del vector en
  // que no existe.
  for (i = 0; i < caras.size(); i++)
  {
    glColor3f(colores[i % cantidad_colores].r, colores[i % cantidad_colores].g, colores[i % cantidad_colores].b);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
  switch (modo)
  {
  case POINTS:
    draw_puntos(r, g, b, grosor);
    break;
  case EDGES:
    draw_aristas(r, g, b, grosor);
    break;
  case SOLID:
    draw_solido(r, g, b);
    break;
  case SOLID_COLORS:
    draw_solido_colores();
    break;
  }
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
  // Se creará un cubo cuyo tamaño de arista se nos indica en tam.
  //  Los puntos van desde los que están más abajo a los que están más arriba siguiendo
  //  sentido antihorario.

  // vertices
  vertices.resize(8);
  vertices[0].x = -tam / 2;
  vertices[0].y = -tam / 2;
  vertices[0].z = tam / 2;
  vertices[1].x = tam / 2;
  vertices[1].y = -tam / 2;
  vertices[1].z = tam / 2;
  vertices[2].x = tam / 2;
  vertices[2].y = -tam / 2;
  vertices[2].z = -tam / 2;
  vertices[3].x = -tam / 2;
  vertices[3].y = -tam / 2;
  vertices[3].z = -tam / 2;
  vertices[4].x = -tam / 2;
  vertices[4].y = tam / 2;
  vertices[4].z = tam / 2;
  vertices[5].x = tam / 2;
  vertices[5].y = tam / 2;
  vertices[5].z = tam / 2;
  vertices[6].x = tam / 2;
  vertices[6].y = tam / 2;
  vertices[6].z = -tam / 2;
  vertices[7].x = -tam / 2;
  vertices[7].y = tam / 2;
  vertices[7].z = -tam / 2;

  // triangulos

  caras.resize(12);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 4;
  caras[1]._0 = 1;
  caras[1]._1 = 5;
  caras[1]._2 = 4;
  caras[2]._0 = 1;
  caras[2]._1 = 2;
  caras[2]._2 = 5;
  caras[3]._0 = 2;
  caras[3]._1 = 6;
  caras[3]._2 = 5;
  caras[4]._0 = 2;
  caras[4]._1 = 3;
  caras[4]._2 = 6;
  caras[5]._0 = 3;
  caras[5]._1 = 7;
  caras[5]._2 = 6;
  caras[6]._0 = 3;
  caras[6]._1 = 0;
  caras[6]._2 = 7;
  caras[7]._0 = 0;
  caras[7]._1 = 4;
  caras[7]._2 = 7;
  caras[8]._0 = 4;
  caras[8]._1 = 5;
  caras[8]._2 = 6;
  caras[9]._0 = 4;
  caras[9]._1 = 6;
  caras[9]._2 = 7;
  caras[10]._0 = 3;
  caras[10]._1 = 1;
  caras[10]._2 = 0;
  caras[11]._0 = 3;
  caras[11]._1 = 2;
  caras[11]._2 = 1;

  // Añadido para probar el modo para pintar cada cara de un color diferente
  // Se podría hacer con un bucle para rellenar los colores que queramos.
  rgb color1;
  color1.r = 1.0;
  rgb color2;
  color2.b = 1.0;
  rgb color3;
  color3.g = 1.0;
  rgb color4;
  color4.r = 0.5;
  color4.g = 0.5;

  colores.push_back(color1);
  colores.push_back(color2);
  colores.push_back(color3);
  colores.push_back(color4);
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

  // vertices
  vertices.resize(5);
  vertices[0].x = -tam;
  vertices[0].y = 0;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = 0;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = 0;
  vertices[2].z = -tam;
  vertices[3].x = -tam;
  vertices[3].y = 0;
  vertices[3].z = -tam;
  vertices[4].x = 0;
  vertices[4].y = al;
  vertices[4].z = 0;

  caras.resize(6);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 4;
  caras[1]._0 = 1;
  caras[1]._1 = 2;
  caras[1]._2 = 4;
  caras[2]._0 = 2;
  caras[2]._1 = 3;
  caras[2]._2 = 4;
  caras[3]._0 = 3;
  caras[3]._1 = 0;
  caras[3]._2 = 4;
  caras[4]._0 = 3;
  caras[4]._1 = 1;
  caras[4]._2 = 0;
  caras[5]._0 = 3;
  caras[5]._1 = 2;
  caras[5]._2 = 1;

  rgb color1;
  color1.r = 1.0;
  rgb color2;
  color2.b = 1.0;
  rgb color3;
  color3.g = 1.0;
  rgb color4;
  color4.r = 0.5;
  color4.g = 0.5;

  colores.push_back(color1);
  colores.push_back(color2);
  colores.push_back(color3);
  colores.push_back(color4);
}

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply()
{
  // leer lista de coordenadas de vértices y lista de indices de vértices
}

void _objeto_ply::parametros(char *archivo)
{
  int n_ver, n_car;

  vector<float> ver_ply;
  vector<int> car_ply;

  _file_ply::read(archivo, ver_ply, car_ply);

  n_ver = ver_ply.size() / 3;
  n_car = car_ply.size() / 3;

  printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

  vertices.resize(n_ver);
  caras.resize(n_car);

  // Rellenamos el vector de vertices
  for (int i = 0; i < vertices.size(); i++)
  {
    int j = i * 3;
    vertices[i].x = ver_ply[j];
    vertices[i].y = ver_ply[j + 1];
    vertices[i].z = ver_ply[j + 2];
  }

  // Rellenamos el vector de caras
  for (int i = 0; i < caras.size(); i++)
  {
    int j = i * 3;
    caras[i]._0 = car_ply[j];
    caras[i]._1 = car_ply[j + 1];
    caras[i]._2 = car_ply[j + 2];
  }

  rgb color1;
  color1.r = 1.0;
  rgb color2;
  color2.b = 1.0;
  rgb color3;
  color3.g = 1.0;
  rgb color4;
  color4.r = 0.5;
  color4.g = 0.5;

  colores.push_back(color1);
  colores.push_back(color2);
  colores.push_back(color3);
  colores.push_back(color4);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{
}

void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tipo, int tapa_in, int tapa_su)
{
  int i, j;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int num_aux;

  float radio; // Esto para la esfera

  // tratamiento de los vértices

  radio = sqrt(perfil[0].x * perfil[0].x + perfil[0].y * perfil[0].y); // Dist euclidea

  num_aux = perfil.size();

  if (tipo == 1) // Seria un cono
    num_aux -= 1;

  // Por lo general, se añaden dos vertices mas
  int num_vertices = num_aux * num + 2;

  vertices.resize(num_vertices);

  for (j = 0; j < num; j++)
  {
    for (i = 0; i < num_aux; i++)
    {
      vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
                      perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
      vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
                      perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
      vertice_aux.y = perfil[i].y;
      vertices[i + j * num_aux] = vertice_aux;
    }
  }

  // tratamiento de las caras
  int num_caras = 2 * (num_aux - 1) * num + 2 * num;
  int c = 0;
  caras.resize(num_caras);
  // El numero de caras es el numero de caras de una tira por el numero de tiras.
  //(2 porque hay dos caras por trozo, num_aux - 1 porque si hay 11 puntos, estan unidos por 10 segmentos)

  // Una vez que tenemos una tira, repetimos para que se  haga en todas
  for (j = 0; j < num; j++)
  {
    for (i = 0; i < num_aux - 1; i++)
    {
      caras[c]._0 = i + j * num_aux;
      caras[c]._1 = ((j + 1) % num) * num_aux + i;
      caras[c]._2 = 1 + i + j * num_aux;
      c += 1;
      caras[c]._0 = ((j + 1) % num) * num_aux + i;
      caras[c]._1 = ((j + 1) % num) * num_aux + i + 1;
      caras[c]._2 = 1 + i + j * num_aux;
      c += 1;
    }
  }

  int total = num_aux * num;
  if (tapa_in == 1)
  {
    // tapa inferior
    
    // Punto central
    vertices[total].x = 0.0;

    if (tipo == 2) // esfera
      vertices[total].y = -radio;
    else
      vertices[total].y = perfil[0].y;

    vertices[total].z = 0.0;

    // Formamos la cara
    for (j = 0; j < num; j++)
    {
      caras[c]._0 = j * num_aux;
      caras[c]._1 = ((j + 1) % num) * num_aux;
      caras[c]._2 = total;
      c += 1;
    }
  }

  // tapa superior
  if (tapa_su == 1)
  {
    if (tapa_in == 1)
      total += 1;

    
    // punto central
    vertices[total].x = 0.0;

    if (tipo == 1) // cono
      vertices[total].y = perfil[1].y;
    if (tipo == 0)
      vertices[total].y = perfil[num_aux - 1].y;
    if (tipo == 2) // esfera
      vertices[total].y = radio;

    vertices[total].z = 0.0;

    // formar la cara
    for (j = 0; j < num; j++)
    {
      caras[c]._0 = total;
      caras[c]._1 = ((j + 1) % num) * num_aux + num_aux - 1;
      caras[c]._2 = num_aux - 1 + j * num_aux;
      c += 1;
    }
  }

  // colores
  rgb color1;
  color1.r = 1.0;
  rgb color2;
  color2.b = 1.0;
  rgb color3;
  color3.g = 1.0;
  rgb color4;
  color4.r = 0.5;
  color4.g = 0.5;

  colores.push_back(color1);
  colores.push_back(color2);
  colores.push_back(color3);
  colores.push_back(color4);
}

//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
  int i;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int num_aux;

  // tratamiento de los vértice

  num_aux = poligono.size();
  vertices.resize(num_aux * 2);
  for (i = 0; i < num_aux; i++)
  {
    vertices[2 * i] = poligono[i];
    vertices[2 * i + 1].x = poligono[i].x + x;
    vertices[2 * i + 1].y = poligono[i].y + y;
    vertices[2 * i + 1].z = poligono[i].z + z;
  }

  // tratamiento de las caras

  caras.resize(num_aux * 2);
  int c = 0;
  for (i = 0; i < num_aux; i++)
  {
    caras[c]._0 = i * 2;
    caras[c]._1 = (i * 2 + 2) % (num_aux * 2);
    caras[c]._2 = i * 2 + 1;
    c = c + 1;
    caras[c]._0 = (i * 2 + 2) % (num_aux * 2);
    caras[c]._1 = (i * 2 + 2) % (num_aux * 2) + 1;
    caras[c]._2 = i * 2 + 1;
    c = c + 1;
  }

  // colores
  rgb color1;
  color1.r = 1.0;
  rgb color2;
  color2.b = 0.5;
  rgb color3;
  color3.g = 0.1;
  rgb color4;
  color4.r = 0.5;
  color4.g = 0.5;

  colores.push_back(color1);
  colores.push_back(color2);
  colores.push_back(color3);
  colores.push_back(color4);
}

//*************************************************************************
// clase cilindro
//*************************************************************************
_cilindro::_cilindro(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;

  aux.x = radio;
  aux.y = -altura / 2.0;
  aux.z = 0.0;
  perfil.push_back(aux);

  aux.x = radio;
  aux.y = altura / 2.0;
  aux.z = 0.0;
  perfil.push_back(aux);

  parametros(perfil, num, 0, 1, 1);
};

//*************************************************************************
// clase cono
//*************************************************************************

_cono::_cono(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;

  aux.x = radio;
  aux.y = 0.0;
  aux.z = 0.0;
  perfil.push_back(aux);

  aux.x = 0.0;
  aux.y = altura;
  aux.z = 0.0;
  perfil.push_back(aux);

  parametros(perfil, num, 1, 1, 1);
};

//*************************************************************************
// clase esfera
//*************************************************************************

_esfera::_esfera(float radio, int num1, int num2)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;

  int i;

  for (i = 1; i < num1; i++)
  {
    aux.x = radio * cos(M_PI * i / (num1 * 1.0) - M_PI / 2.0);
    aux.y = radio * sin(M_PI * i / (num1 * 1.0) - M_PI / 2.0);
    aux.z = 0.0;
    perfil.push_back(aux);
  }

  parametros(perfil, num2, 2, 1, 1);
};
