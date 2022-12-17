//**************************************************************************
// Práctica 3
//**************************************************************************

#include "objetos_B4.h"
#include "file_ply_stl.hpp"
#include <time.h>

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
  //**** usando vertex_array ****
  glPointSize(grosor);
  glColor3f(r, g, b);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glDrawArrays(GL_POINTS, 0, vertices.size());

  /*int i;
  glPointSize(grosor);
  glColor3f(r,g,b);
  glBegin(GL_POINTS);
  for (i=0;i<vertices.size();i++){
    glVertex3fv((GLfloat *) &vertices[i]);
    }
  glEnd();*/
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
  norm_caras = false;
  norm_caras = false;

  //estos son por defecto, hacer metodo set para modificarlo
  ambiente = _vertex4f(1, 0.8, 0, 1.0); // coeficientes ambiente y difuso, mismo color que con color lambert
  difuso = _vertex4f(1, 0.8, 0, 1.0);
  especular = _vertex4f(0.5, 0.5, 0.5, 1.0);       // coeficiente especular, parametros iguales, desde el punto de vista físico solo, luz verde 0, 0.5, 0
  brillo = 120;                                     // exponente del brillo
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
  //**** usando vertex_array ****
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(grosor);
  glColor3f(r, g, b);

  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
  glDrawElements(GL_TRIANGLES, caras.size() * 3, GL_UNSIGNED_INT, &caras[0]);

  /*int i;
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(grosor);
  glColor3f(r,g,b);
  glBegin(GL_TRIANGLES);
  for (i=0;i<caras.size();i++){
    glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
    glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
    }
  glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
  int i;
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
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glColor3f(colores_caras[i].r, colores_caras[i].g, colores_caras[i].b);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
}

void _triangulos3D::draw_solido_plano()
{

  //Esto sería con argumentos en el método
  // ambiente_difusa[0] = r1;
  // ambiente_difusa[1] = g1;
  // ambiente_difusa[2] = b1;

  // int i;
  // if (norm_caras == false)
  //   calcular_normales_caras();

  // glEnable(GL_LIGHTING);
  // glShadeModel(GL_FLAT); // GL_SMOOTH
  // glEnable(GL_NORMALIZE);

  // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, (GLfloat *)&ambiente_difusa);
  // glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  // glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // glBegin(GL_TRIANGLES);
  // for (i = 0; i < caras.size(); i++)
  // {
  //   glNormal3fv((GLfloat *)&normales_caras[i]);
  //   glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
  //   glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
  //   glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  // }
  // glEnd();

  // glDisable(GL_LIGHTING);
 


  int i;
  if (norm_caras == false)
    calcular_normales_caras();
  glEnable(GL_LIGHTING);
  //Ahora tocaría definir las constantes difusas, especular y el brillo para cada material
  glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat *)&ambiente);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat *)&difuso);
  glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT, GL_SHININESS, brillo);
  glEnable(GL_NORMALIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    //En el suave una normal para vértice
    glNormal3f(normales_caras[i].x, normales_caras[i].y, normales_caras[i].z);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();
  glDisable(GL_LIGHTING);
}

void _triangulos3D::draw_solido_suave()
{

  int i;
  if (norm_caras == false)
    calcular_normales_caras();
  if (norm_vertices == false)
    calcular_normales_vertices();

  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);
  

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (GLfloat *)&ambiente);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (GLfloat *)&difuso);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (GLfloat *)&especular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, brillo);
  glEnable(GL_NORMALIZE);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);
  for (i = 0; i < caras.size(); i++)
  {
    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._0]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._0]);

    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._1]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._1]);

    glNormal3fv((GLfloat *)&normales_vertices[caras[i]._2]);
    glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
  }
  glEnd();

  glDisable(GL_LIGHTING);
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
  case SOLID_ILLUMINATED_FLAT:
    draw_solido_plano();
    break;
  case SOLID_ILLUMINATED_GOURAUD:
    draw_solido_suave();
    break;
  }


}

//*************************************************************************
// asignación colores
//*************************************************************************

void _triangulos3D::colors_random()
{
  int i, n_c;
  n_c = caras.size();
  colores_caras.resize(n_c);
  srand(time(NULL));
  for (i = 0; i < n_c; i++)
  {
    colores_caras[i].r = rand() % 1000 / 1000.0;
    colores_caras[i].g = rand() % 1000 / 1000.0;
    colores_caras[i].b = rand() % 1000 / 1000.0;
  }
}

//*************************************************************************

void _triangulos3D::colors_chess(float r1, float g1, float b1, float r2, float g2, float b2)
{
  int i, n_c;
  n_c = caras.size();
  colores_caras.resize(n_c);
  for (i = 0; i < n_c; i++)
  {
    if (i % 2 == 0)
    {
      colores_caras[i].r = r1;
      colores_caras[i].g = g1;
      colores_caras[i].b = b1;
    }
    else
    {
      colores_caras[i].r = r2;
      colores_caras[i].g = g2;
      colores_caras[i].b = b2;
    }
  }
}

void _triangulos3D::color_lambert_c(float l_x, float l_y, float l_z, float r, float g, float b)
{
  int i, n_c;
  _vertex3f luz, aux_luz;
  float modulo, p_escalar;

  n_c = caras.size();
  colores_caras.resize(n_c);

  aux_luz.x = l_x;
  aux_luz.y = l_y;
  aux_luz.z = l_z;

  for (i = 0; i < n_c; i++)
  {
    luz = aux_luz - vertices[caras[i]._0];
    modulo = sqrt(luz.x * luz.x + luz.y * luz.y + luz.z * luz.z);
    luz.x = luz.x / modulo;
    luz.y = luz.y / modulo;
    luz.z = luz.z / modulo;
    p_escalar = luz.x * normales_caras[i].x + luz.y * normales_caras[i].y + luz.z * normales_caras[i].z;
    if (p_escalar <= 0)
      p_escalar = 0.0;
    colores_caras[i].r = r * p_escalar + r * 0.2;
    colores_caras[i].g = g * p_escalar + g * 0.2;
    colores_caras[i].b = b * p_escalar + b * 0.2;
  }
}

/** CALCULO DE LAS NORMALES
 */
void _triangulos3D::calcular_normales_caras()
{
  int i, n_c;
  _vertex3f vector_a, vector_b, aux;
  float modulo;

  n_c = caras.size();
  normales_caras.resize(n_c);

  for (i = 0; i < n_c; i++)
  {
    vector_a = vertices[caras[i]._1] - vertices[caras[i]._0];
    vector_b = vertices[caras[i]._2] - vertices[caras[i]._0];
    aux.x = vector_a.y * vector_b.z - vector_a.z * vector_b.y;
    aux.y = vector_a.z * vector_b.x - vector_a.x * vector_b.z;
    aux.z = vector_a.x * vector_b.y - vector_a.y * vector_b.x;
    modulo = sqrt(aux.x * aux.x + aux.y * aux.y + aux.z * aux.z);
    normales_caras[i].x = aux.x / modulo;
    normales_caras[i].y = aux.y / modulo;
    normales_caras[i].z = aux.z / modulo;
  }

  norm_caras = true;
}

// Sin terminar
void _triangulos3D::calcular_normales_vertices()
{
  int i, n_v;

  n_v = vertices.size();
  normales_vertices.resize(n_v);

  for (i = 0; i < n_v; i++)
  {
    normales_vertices[i].x = 0.0;
    normales_vertices[i].y = 0.0;
    normales_vertices[i].z = 0.0;
  }

  n_v = caras.size();

  for (i = 0; i < n_v; i++)
  {
    normales_vertices[caras[i]._0] += normales_caras[i];
    normales_vertices[caras[i]._1] += normales_caras[i];
    normales_vertices[caras[i]._2] += normales_caras[i];
  }
}
//*************************************************************************
// objetos o modelos
//*************************************************************************

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
  // vertices
  vertices.resize(8);
  vertices[0].x = -tam;
  vertices[0].y = -tam;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = -tam;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = tam;
  vertices[2].z = tam;
  vertices[3].x = -tam;
  vertices[3].y = tam;
  vertices[3].z = tam;
  vertices[4].x = -tam;
  vertices[4].y = -tam;
  vertices[4].z = -tam;
  vertices[5].x = tam;
  vertices[5].y = -tam;
  vertices[5].z = -tam;
  vertices[6].x = tam;
  vertices[6].y = tam;
  vertices[6].z = -tam;
  vertices[7].x = -tam;
  vertices[7].y = tam;
  vertices[7].z = -tam;

  // triangulos
  caras.resize(12);
  caras[0]._0 = 0;
  caras[0]._1 = 1;
  caras[0]._2 = 3;
  caras[1]._0 = 3;
  caras[1]._1 = 1;
  caras[1]._2 = 2;
  caras[2]._0 = 1;
  caras[2]._1 = 5;
  caras[2]._2 = 2;
  caras[3]._0 = 5;
  caras[3]._1 = 6;
  caras[3]._2 = 2;
  caras[4]._0 = 5;
  caras[4]._1 = 4;
  caras[4]._2 = 6;
  caras[5]._0 = 4;
  caras[5]._1 = 7;
  caras[5]._2 = 6;
  caras[6]._0 = 0;
  caras[6]._1 = 7;
  caras[6]._2 = 4;
  caras[7]._0 = 0;
  caras[7]._1 = 3;
  caras[7]._2 = 7;
  caras[8]._0 = 3;
  caras[8]._1 = 2;
  caras[8]._2 = 7;
  caras[9]._0 = 2;
  caras[9]._1 = 6;
  caras[9]._2 = 7;
  caras[10]._0 = 0;
  caras[10]._1 = 1;
  caras[10]._2 = 4;
  caras[11]._0 = 1;
  caras[11]._1 = 5;
  caras[11]._2 = 4;

  // colores de las caras
  colors_random();
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{
  int i;
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

  // colores de las caras
  colors_random();
}

_piramideMitad::_piramideMitad(float tam, float al)
{
  // vertices
  vertices.resize(5);
  vertices[0].x = 0;
  vertices[0].y = 0;
  vertices[0].z = tam;
  vertices[1].x = tam;
  vertices[1].y = 0;
  vertices[1].z = tam;
  vertices[2].x = tam;
  vertices[2].y = 0;
  vertices[2].z = -tam;
  vertices[3].x = 0;
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

  // colores de las caras
  colors_random();
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
  int i, n_ver, n_car;

  vector<float> ver_ply;
  vector<int> car_ply;

  _file_ply::read(archivo, ver_ply, car_ply);

  n_ver = ver_ply.size() / 3;
  n_car = car_ply.size() / 3;

  printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

  vertices.resize(n_ver);
  caras.resize(n_car);
  // vértices

  for (i = 0; i < n_ver; i++)
  {
    vertices[i].x = ver_ply[3 * i];
    vertices[i].y = ver_ply[3 * i + 1];
    vertices[i].z = ver_ply[3 * i + 2];
  }

  // vértices
  for (i = 0; i < n_car; i++)
  {
    caras[i].x = car_ply[3 * i];
    caras[i].y = car_ply[3 * i + 1];
    caras[i].z = car_ply[3 * i + 2];
  }
  //calcular_normales_caras();
  //calcular_normales_vertices();
  //se hacen ya si no se calculan
  // colores
  colores_caras.resize(n_car);
  colors_random();
  //color_lambert_c(0,20,40,1.0,0.8,0.0);
 // color_lambert_c(-20,20,10,1.0,0.8,0.0); // para luz picada
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
  float radio;

  // tratamiento de los vértice
  radio = sqrt(perfil[0].x * perfil[0].x + perfil[0].y * perfil[0].y);

  num_aux = perfil.size();
  if (tipo == 1)
    num_aux = num_aux - 1;
  vertices.resize(num_aux * num + 2);
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

  caras.resize(2 * (num_aux - 1) * num + 2 * num);
  int c = 0;
  for (j = 0; j < num; j++)
  {
    for (i = 0; i < num_aux - 1; i++)
    {
      caras[c]._0 = i + j * num_aux;
      caras[c]._1 = ((j + 1) % num) * num_aux + i;
      caras[c]._2 = 1 + i + j * num_aux;
      c += 1;
      caras[c]._0 = ((j + 1) % num) * num_aux + i;
      caras[c]._1 = ((j + 1) % num) * num_aux + 1 + i;
      caras[c]._2 = 1 + i + j * num_aux;
      c += 1;
    }
  }

  // tapa inferior
  int total = num_aux * num;

  if (tapa_in == 1)
  {
    vertices[total].x = 0.0;
    if (tipo == 2)
      vertices[total].y = -radio;
    else
      vertices[total].y = perfil[0].y;
    vertices[total].z = 0.0;

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

    vertices[total].x = 0.0;
    if (tipo == 1)
      vertices[total].y = perfil[1].y;
    if (tipo == 0)
      vertices[total].y = perfil[num_aux - 1].y;
    if (tipo == 2)
      vertices[total].y = radio;
    vertices[total].z = 0.0;

    for (j = 0; j < num; j++)
    {
      caras[c]._0 = total;
      caras[c]._1 = ((j + 1) % num) * num_aux + num_aux - 1;
      caras[c]._2 = num_aux - 1 + j * num_aux;
      c += 1;
    }
  }

  // colores de las caras
  colors_random();
}

//************************************************************************
// objeto cilindro (caso especial de rotacion)
//************************************************************************

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
  parametros(perfil, num, 0, 0, 0);
}

//************************************************************************
// objeto cono (caso especial de rotacion)
//************************************************************************

_cono::_cono(float radio, float altura, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f aux;

  aux.x = radio;
  aux.y = 0;
  aux.z = 0.0;
  perfil.push_back(aux);
  aux.x = 0.0;
  aux.y = altura;
  aux.z = 0.0;
  perfil.push_back(aux);
  parametros(perfil, num, 1, 1, 1);
}

//************************************************************************
// objeto esfera (caso especial de rotacion)
//************************************************************************

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

  //Calculo normales vertices
  normales_vertices = vertices;
  norm_vertices = true; //asi no se calcula automaticamente
}

//************************************************************************
// rotacion archivo PLY (caso especial de rotacion)
//************************************************************************

_rotacion_PLY::_rotacion_PLY()
{
}

void _rotacion_PLY::parametros_PLY(char *archivo, int num)
{
  int i, n_ver, n_car;

  vector<float> ver_ply;
  vector<int> car_ply;

  _file_ply::read(archivo, ver_ply, car_ply);

  n_ver = ver_ply.size() / 3;
  n_car = car_ply.size() / 3;

  printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

  vertices.resize(n_ver);
  vector<_vertex3f> perfil;

  // En este caso, solo queremos los vértices que forman el perfil
  //  vértices

  for (i = 0; i < n_ver; i++)
  {
    vertices[i].x = ver_ply[3 * i];
    vertices[i].y = ver_ply[3 * i + 1];
    vertices[i].z = ver_ply[3 * i + 2];

    perfil.push_back(vertices[i]);
  }

  parametros(perfil, num, 0, 0, 1);
}

//************************************************************************

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

  // colores de las caras
  colors_random();
}

// practica3, objeto jerarquico David Muñoz Sánchez AT-ST

// PIEZAS-----------------------------
_piernaBaja::_piernaBaja()
{
  //Material mate, brillo pequeño y especular tambien
  cilindro.ambiente = _vertex4f(0.24725, 0.2245, 0.0645, 1.0); 
  cilindro.brillo = 10;
  cilindro.especular = _vertex4f(0.0, 0.0, 0.0, 1.0);
  ancho = 0.1;
  alto = 1.0;
  fondo = 0.1;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

void _piernaBaja::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();

  // glRotatef(45, 1,0,0);
  glTranslatef(0, -1.0, 0);
  glScalef(ancho, alto, fondo);
  cilindro.draw(modo, r, g, b, grosor);

  // glPopMatrix();
  // glPushMatrix();
  // glRotatef(-45, 1,0,0);
  // glTranslatef(1.0, 1.0, 0.0);
  // glTranslatef(0,0, 2*alto);
  // glScalef(ancho, alto, fondo);
  // cilindro2.draw(modo, r, g, b, grosor);

  glPopMatrix();
}

_pie::_pie()
{
  ancho = 0.5;
  alto = 1.0;
  fondo = 0.5;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

void _pie::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();

  glRotatef(90, 1, 0, 0);
  glRotatef(90, 0, 1, 0);
  // glTranslatef(1.0,  1.0, 0);
  glScalef(ancho, alto, fondo);
  base.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glRotatef(-30, 1, 0, 0);
  glTranslatef(0, 0.3, 0.1);
  glScalef(0.1, 0.3, 0.1);
  cilindro.draw(modo, r, g, b, grosor);
  glPopMatrix();
}

_tronco::_tronco()
{
  //Goma negra (material satinado, componente especular intermedia y poco brillo
  cilindro.ambiente = _vertex4f(0.02, 0.02, 0.02, 1.0); 
  cilindro.brillo = 10;
  cilindro.difuso = _vertex4f(0.01, 0.01, 0.01, 1.0);
  cilindro.especular = _vertex4f(0.4, 0.4, 0.4, 1.0);
  base.ambiente = _vertex4f(0.02, 0.02, 0.02, 1.0); 
  base.difuso = _vertex4f(0.01, 0.01, 0.01, 1.0);
  base.brillo = 10;
  base.especular = _vertex4f(0.4, 0.4, 0.4, 1.0);

  ancho = 2.0;
  alto = 0.2;
  fondo = 1.5;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

void _tronco::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glScalef(ancho, alto, fondo);
  base.draw(modo, r, g, b, grosor);
  glPopMatrix();
  glPushMatrix();
  // glRotatef(-30, 1,0,0);
  glTranslatef(0, 0.8 + alto / 2, 0);
  glScalef(0.1, 0.8, 0.1);
  cilindro.draw(modo, r, g, b, grosor);
  glPopMatrix();
}

_cabeza::_cabeza()
{

  //Plata pulida (brillante)
  atras.ambiente = _vertex4f(0.23125, 0.23125, 0.23125, 1.0); 
  atras.brillo = 89.6;
  atras.difuso = _vertex4f(0.2775, 0.2775, 0.2775, 1.0);
  atras.especular = _vertex4f(0.773911, 0.773911, 0.773911, 1.0);
  alante.ambiente = _vertex4f(0.23125, 0.23125, 0.23125, 1.0); 
  alante.brillo = 89.6;
  alante.difuso = _vertex4f(0.2775, 0.2775, 0.2775, 1.0);
  alante.especular = _vertex4f(0.773911, 0.773911, 0.773911, 1.0);
  ancho = 1.0;
  alto = 1.0;
  fondo = 1.0;
  colors_chess(140, 140, 140, 0.0, 0.0, 1.0);
}

void _cabeza::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glTranslatef(0.0, 0.0, fondo / 2);
  glScalef(ancho, alto, fondo);
  atras.draw(modo, r, g, b, grosor);
  glPopMatrix();
  glPushMatrix();
  glRotatef(-90, 1, 0, 0);
  glRotatef(-90, 0, 1, 0);
  alante.draw(modo, r, g, b, grosor);
  glPopMatrix();
}

_atst::_atst()
{

  giro_cabeza_vertical = 0.0;
  giro_sobre_eje_cabeza = 0.0;
  apertura_pies = 0.0;
  flexion_piernas = 0.0;
  factor_pies = 0.0;
  factor_tronco = 0.0;
  factor_tronco2 = 0.0;

  colors_chess(0, 0, 0, 0, 0, 0);
}

void _atst::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glTranslatef(0, factor_tronco, 0);
  tronco.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0, 2 + factor_tronco, -0.1 - (0.5 - 0.1));
  glRotatef(giro_sobre_eje_cabeza, 0, 1, 0);
  glRotatef(giro_cabeza_vertical, 1, 0, 0);
  cabeza.draw(modo, r, g, b, grosor);

  glPopMatrix();

  glPushMatrix();

  glTranslatef(0.5, -0.1 + factor_tronco, 0);
  glRotatef(flexion_piernas, 1, 0, 0);
  glRotatef(45, -1, 0, 0);
  pata.draw(modo, r, g, b, grosor);

  glTranslatef(0, -2.0, 0);
  glRotatef(2 * flexion_piernas, -1, 0, 0);
  glRotatef(90, 1, 0, 0);
  pata.draw(modo, r, g, b, grosor);

  glPopMatrix();

  glPushMatrix();

  glTranslatef(-0.5, -0.1 + factor_tronco, 0);
  glRotatef(flexion_piernas, 1, 0, 0);
  glRotatef(45, -1, 0, 0);
  pata.draw(modo, r, g, b, grosor);

  glTranslatef(0, -2.0, 0);
  glRotatef(2 * flexion_piernas, -1, 0, 0);
  glRotatef(90, 1, 0, 0);
  pata.draw(modo, r, g, b, grosor);

  glPopMatrix();

  glPushMatrix();

  glTranslatef(0.5, -2 * sin(45) * 2 - factor_pies + factor_tronco, 0.2);
  glRotatef(apertura_pies, 0, 1, 0);
  pie.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();

  glTranslatef(-0.5, -2 * sin(45) * 2 - factor_pies + +factor_tronco, 0.2);

  glRotatef(-apertura_pies, 0, 1, 0);
  pie.draw(modo, r, g, b, grosor);
  glPopMatrix();
}
//************************************************************************
// práctica 3, objeto jerárquico articulado
//************************************************************************

//************************************************************************
// piezas
//************************************************************************

//************************************************************************
// pala
//************************************************************************

_pala::_pala(float radio, float ancho, int num)
{
  vector<_vertex3f> perfil;
  _vertex3f vertice_aux;
  _vertex3i cara_aux;
  int i, j;

  vertice_aux.x = radio;
  vertice_aux.y = 0;
  vertice_aux.z = -ancho / 2.0;
  perfil.push_back(vertice_aux);
  vertice_aux.z = ancho / 2.0;
  perfil.push_back(vertice_aux);

  // tratamiento de los vértices

  for (j = 0; j <= num; j++)
  {
    for (i = 0; i < 2; i++)
    {
      vertice_aux.x = perfil[i].x * cos(M_PI * j / (1.0 * num)) -
                      perfil[i].y * sin(M_PI * j / (1.0 * num));
      vertice_aux.y = perfil[i].x * sin(M_PI * j / (1.0 * num)) +
                      perfil[i].y * cos(M_PI * j / (1.0 * num));
      vertice_aux.z = perfil[i].z;
      vertices.push_back(vertice_aux);
    }
  }

  // tratamiento de las caras

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2;
    cara_aux._2 = (j + 1) * 2 + 1;
    caras.push_back(cara_aux);

    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2 + 1;
    cara_aux._2 = j * 2 + 1;
    caras.push_back(cara_aux);
  }

  // tapa inferior
  vertice_aux.x = 0;
  vertice_aux.y = 0;
  vertice_aux.z = -ancho / 2.0;
  vertices.push_back(vertice_aux);

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2;
    cara_aux._1 = (j + 1) * 2;
    cara_aux._2 = vertices.size() - 1;
    caras.push_back(cara_aux);
  }

  // tapa superior
  vertice_aux.x = 0;
  vertice_aux.y = 0;
  vertice_aux.z = ancho / 2.0;
  vertices.push_back(vertice_aux);

  for (j = 0; j < num; j++)
  {
    cara_aux._0 = j * 2 + 1;
    cara_aux._1 = (j + 1) * 2 + 1;
    cara_aux._2 = vertices.size() - 1;
    caras.push_back(cara_aux);
  }

  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

//************************************************************************
// brazo
//************************************************************************

_brazo::_brazo()
{
  ancho = 0.6;
  alto = 0.1;
  fondo = 0.1;
  colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _brazo::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glScalef(ancho, alto, fondo);
  glTranslatef(0.5, 0, 0);
  cubo.draw(modo, r, g, b, grosor);
  glPopMatrix();
};

//************************************************************************
// cabina
//************************************************************************

_cabina::_cabina()
{
  ancho = 0.4;
  alto = 0.6;
  fondo = 0.4;
  cubo.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _cabina::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glScalef(ancho, alto, fondo);
  cubo.draw(modo, r, g, b, grosor);
  glPopMatrix();
};

//************************************************************************
// sustentación
//************************************************************************

_sustentacion::_sustentacion()
{
  ancho = 1.2;
  alto = 0.3;
  fondo = 0.8;
  radio = 0.15;
  base.colors_chess(1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
};

void _sustentacion::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();
  glTranslatef(2 * ancho / 6, -alto / 2.0, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(radio, fondo / 2.2, radio);
  rueda.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-2 * ancho / 6, -alto / 2.0, 0);
  glRotatef(90, 1, 0, 0);
  glScalef(radio, fondo / 2.2, radio);
  rueda.draw(modo, r, g, b, grosor);
  glPopMatrix();

  glPushMatrix();
  glScalef(ancho, alto, fondo);
  base.draw(modo, r, g, b, grosor);
  glPopMatrix();
};

//************************************************************************
// excavadora (montaje del objeto final)
//************************************************************************

_excavadora::_excavadora()
{
  giro_cabina = 0.0;
  giro_primer_brazo = 0.0;
  giro_primer_brazo_max = 0;
  giro_primer_brazo_min = -90;
  giro_segundo_brazo = 0.0;
  giro_segundo_brazo_max = 30;
  giro_segundo_brazo_min = 0;
  giro_pala = 0.0;
  giro_pala_max = 50.0;
  giro_pala_min = -90.0;

  tamanio_pala = 0.15;
};

void _excavadora::draw(_modo modo, float r, float g, float b, float grosor)
{
  glPushMatrix();

  sustentacion.draw(modo, r, g, b, grosor);

  glTranslatef(0, (cabina.alto + sustentacion.alto) / 2.0, 0);
  glRotatef(giro_cabina, 0, 1, 0);
  cabina.draw(modo, r, g, b, grosor);

  glTranslatef(cabina.ancho / 2.0, 0, 0);
  glRotatef(giro_segundo_brazo, 0, 0, 1);
  brazo.draw(modo, r, g, b, grosor);

  glTranslatef(brazo.ancho, 0, 0);
  glRotatef(giro_primer_brazo, 0, 0, 1);
  brazo.draw(modo, r, g, b, grosor);

  glTranslatef(brazo.ancho, 0, 0);
  glRotatef(giro_pala, 0, 0, 1);
  glTranslatef(tamanio_pala, 0, 0);
  glScalef(tamanio_pala, tamanio_pala, tamanio_pala);
  pala.draw(modo, r, g, b, grosor);

  glPopMatrix();
};