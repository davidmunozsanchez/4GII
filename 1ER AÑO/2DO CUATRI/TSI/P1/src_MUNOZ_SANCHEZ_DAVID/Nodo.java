package tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID;

import tools.Vector2d;
import ontology.Types.ACTIONS;

//Esta clase se crea con el objetivo de hacer menos engorroso el código de los diferentes agentes
//que se nos piden para la práctica.

//Por cada nodo se guardan como variables de clase la posición en el eje x e y del nodo dentro del juego
//su nodo padre para mantenerlo todo enlazado, la acción que ha permitido llegar a ese nodo, el coste g
//(que es el coste desde el nodo inicial al nodo), el coste h (que es la valoración heurística) y la f, que es 
//la suma de las dos anteriores, ademas de un contador, para gestionar la antigüedad y el orden de expansión
public class Nodo {
	int x; // x dentro del juego
	int y; // y dentro del juego
	Nodo padre; // Nodo padre
	ACTIONS accion; // Accion que se realiza para llegar al nodo
	int g; // Coste g
	int h; // Valoracion de la heurística
	int f; // f = g + h
	int contador;

	// Esta clase se basará en el uso de distintos constructores según el agente que
	// tengamos entre manos
	// o las necesidades específicas del algoritmo.

	// El constructor por defecto inicializa todo a 0 o nulo
	public Nodo() {
		x = y = 0;
		padre = null;
		accion = ACTIONS.ACTION_NIL;
		f = 0;
		g = 0;
		h = 0;
		contador = 0;
	}

	// Constructor en base a un vector
	// Se construye un nodo en base a un vector 2D de posicion
	// Los demás elementos se inicializan a nulo o a 0
	public Nodo(Vector2d pos) {
		x = (int) pos.x;
		y = (int) pos.y;
		padre = null;
		accion = ACTIONS.ACTION_NIL;
		f = 0;
		g = 0;
		h = 0;
		contador = 0;
	}

	// Este constructor es igual que el anterior con la única diferencia de que se
	// le pasa otro nodo como
	// parámetro, que pasará a ser el nodo padre del nodo que estamos construyendo
	public Nodo(Vector2d pos, Nodo p) {
		x = (int) pos.x;
		y = (int) pos.y;
		padre = p;
		accion = ACTIONS.ACTION_NIL;
		f = 0;
		g = 0;
		h = 0;
		contador = 0;
	}

	//Constructor de copia para inicializar un nodo exactamente igual que el proporcionado como
	//parámetro
	public Nodo(Nodo original) {
		x = original.x;
		y = original.y;
		padre = original.padre;
		accion = original.accion;
		f = original.f;
		g = original.g;
		h = original.h;
		contador = original.contador;
	}

	// Constructor de copia que tiene la función añadida de recalcular el valor de h y por consiguiente el valor de f
	public Nodo(Nodo original, Nodo objetivo) {
		x = original.x;
		y = original.y;
		padre = original.padre;
		accion = original.accion;
		g = original.g;
		contador = original.contador;
		calcular_h(objetivo);
		calcular_f();
	}
	
	//Este constructor es igual que el anterior pero se le pasa también la h que queramos que tenga
	//se usa solo en Dijkstra ya que se ha construido como un caso particular de A*, siendo una de las características
	//que la valoración huerística siempre es 0, ya que no es un método heurístico
	public Nodo(Nodo original, Nodo objetivo, int heu) {
		x = original.x;
		y = original.y;
		padre = original.padre;
		accion = original.accion;
		g = original.g;
		h = heu;
		contador = original.contador;
		calcular_f();
	}

	//Función que dado un nodo objetivo, establece el coste heurístico para el nodo en base a la distancia Manhattan
	public int calcular_h(Nodo objetivo) {
		h = Math.abs(objetivo.x - x) + Math.abs(objetivo.y - y);
		return h;
	}

	// Función auxiliar para recalcular f
	public int calcular_f() {
		f = h + g;
		return f;
	}

	// Función comparadora de nodos. Dos nodos son iguales si tienen las mismas coordenadas en el juego
	public boolean equals(Object o) {
		Nodo e = (Nodo) o;
		if ((e.x == this.x) && (e.y == this.y))
			return true;
		else
			return false;
	}

	//Función que se usará en los distintos algoritmo para mantener un orden en la cola con prioridad.
	//Como en el guión se dice que en caso de empate en los distintos criterios hay que tomar los nodos según un orden
	//preestablecido de expansión, se le asigna más prioridad a la acción de ir hacia arriba que a la de ir a la derecha
	public int valorAccion() {
		switch (accion) {
		case ACTION_UP:
			return 1;
		case ACTION_DOWN:
			return 2;
		case ACTION_LEFT:
			return 3;
		case ACTION_RIGHT:
			return 4;
		default:
			return 5;
		}
	}

	@Override
	public String toString() {
		return "Nodo{" + "posicion= (" + x + ", " + y + ") " + ", accion= " + accion + ", h= " + h + "conta = " + contador +"}\n";
	}
}
