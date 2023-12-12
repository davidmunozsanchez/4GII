package tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.PriorityQueue;

import core.game.Observation;
import core.game.StateObservation;
import core.player.AbstractPlayer;
import ontology.Types;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tools.Vector2d;

public class AgenteDijkstra extends AbstractPlayer {
	Vector2d fescala;
	Nodo portal;
	boolean hayPlan;
	ArrayList<ACTIONS> plan;
	int[][] g; // Matriz para guardar el menor valor de g encontrado, se inicializa con el máximo valor
				//que puede tomar un entero en Java
	int nExpandidos; //Numero de nodos expandidos
	int tamRuta; // Numero de nodos que el agente recorre
	double runTime; // Tiempo, en milisegundos, usado para calcular el plan,
					//se calcula de la forma que se indica en las transparencias

	/**
	 * initialize all variables for the agent
	 * 
	 * @param stateObs     Observation of the current state.
	 * @param elapsedTimer Timer when the action returned is due.
	 */
	public AgenteDijkstra(StateObservation state, ElapsedCpuTimer timer) {
		// Calculo el factor de escala pixeles -> grid)
		fescala = new Vector2d(state.getWorldDimension().width / state.getObservationGrid().length,
				state.getWorldDimension().height / state.getObservationGrid()[0].length);

		// Se obtiene la lista de objetivos
		ArrayList<Observation>[] posiciones = state.getPortalsPositions(state.getAvatarPosition());
		
		//Se selecciona el portal más próximo a nuestra ubicación
		portal = new Nodo();
		portal.x = (int) (Math.floor(posiciones[0].get(0).position.x / fescala.x));
		portal.y = (int) (Math.floor(posiciones[0].get(0).position.y / fescala.y));

		//Inicialmente, no hay plan y la lista de acciones es vacía
		hayPlan = false;
		plan = new ArrayList<ACTIONS>();
		//Se inicializan las variables de control a 0
		nExpandidos = 0;
		tamRuta = 0;

		//Se crea la matriz encargada de mapear la g y se inicializa a infinito, que en este caso
		//es el máximo valor que puede tomar un int en Java
		g = new int[state.getObservationGrid().length][state.getObservationGrid()[0].length];
		for (int i = 0; i < g.length; i++) {
			for (int j = 0; j < g[i].length; j++) {
				g[i][j] = Integer.MAX_VALUE;
			}
		}
		
		//Comprobación para ver la correcta inicialización de la matriz
		//System.out.println(g[0][0]);

	}

	/**
	 * Sigue un camino generado por A*
	 * 
	 * @param stateObs     Observation of the current state.
	 * @param elapsedTimer Timer when the action returned is due.
	 * @return la proxima accion a realizar
	 */
	@Override
	public ACTIONS act(StateObservation state, ElapsedCpuTimer timer) {

		//Acción que el algoritmo de Dijkstra devolverá
		ACTIONS accion = ACTIONS.ACTION_NIL;

		//Si no hay plan, hay que calcularlo
		if (!hayPlan) {
			
			//Se genera el nodo inicial a partir de la posición
			Vector2d pos_avatar = new Vector2d(state.getAvatarPosition().x / fescala.x,
					state.getAvatarPosition().y / fescala.y);
			Nodo avatar = new Nodo(pos_avatar);
			
			avatar.g = 0; // Por ser nodo inicial
			avatar.h = 0; //La h siempre será 0 en el algoritmo de Dijkstra
			avatar.contador = 0; //Esta variable se encargará de mantener el orden en la
								//cola con prioridad en base a la antigüedad de los nodos
			avatar.calcular_f(); //Se calcula la f del nodo inicial, que en el caso de Dijkstra será la g

			//Se llama al algoritmo de Dijkstra, que devolverá el último nodo
			long tInicio = System.nanoTime();
			Nodo ultimo = Dijkstra(avatar, portal, state);
			runTime = (System.nanoTime() - tInicio) / 1000000.0;
			hayPlan = true; //Ya tenemos plan

			//El plan se forma recorriendo desde el nodo final los padres hasta el nodo inicial
			while (ultimo != avatar) {
				plan.add(0, ultimo.accion); //Se va metiendo al principio
				ultimo = ultimo.padre;
			}
			tamRuta = plan.size(); //El tamaño de la ruta será el tamaño del ArrayList que guarda
									//las acciones del plan

			//Se imprimen los datos pedidos en la práctica
			System.out.print(" Runtime(ms): " + runTime + ",\n Tamaño de la ruta calculada: " + tamRuta
					+ ",\n Número de nodos expandidos: " + nExpandidos  + "\n");
		}
		// Si hay plan, se selecciona la siguiente accion a ejecutar y se saca del plan
		else {
			accion = plan.get(0);
			plan.remove(0);
		}

		return accion;
	}

	//Este método ejecuta el algoritmo de Dijkstra dado un nodo incial y un objetivo
	public Nodo Dijkstra(Nodo inicial, Nodo objetivo, StateObservation state) {
		//Cola con prioridad donde se van guardando los nodos no visitados.
		//El criterio de ordenación será comentado más adelante, cuando se hable acerca del
		//NodeComparator
		PriorityQueue<Nodo> no_visitados = new PriorityQueue<Nodo>(NodeComparator);

		//ArrayList para guardar los nodos que ya han sido visitados
		ArrayList<Nodo> visitados = new ArrayList<Nodo>();
		Nodo actual; //Nodo que se está explorando
		Nodo hijo = new Nodo(); //Nodo para ir guardando los diferentes nodos generados por la expansion
								//del actual
		
		//El coste g del nodo inicial es 0
		g[inicial.x][inicial.y] = 0;
		//Se añade a la cola de no_visitados el nodo inicial
		no_visitados.add(inicial);
		
		//El bucle parará cuando se llegue al objetivo
		//Dada las características y restricciones del juego que se usa para la práctica
		//tenemos asegurado que siempre es posible llegar al objetivo
		while (true) {
			//El siguiente código comentado se hizo con el fin de observar que el orden en la cola con prioridad
			//mantenía los diferentes criterios para seleccionar un nodo
//			PriorityQueue<Nodo> prueba = new PriorityQueue<Nodo>(no_visitados);
//			System.out.println("Nueva iteracion");
//			for (int i = 0; i < no_visitados.size(); i++) {
//				Nodo n = prueba.poll();
//
//				System.out.println(n.f + " " + n.g + " " + n.accion + " " + n.contador);
//			}
			//El nodo siguiente a visitar y expandir será el primero de la cola con prioridad
			//que ya está ordenada según los criterios de selección que se piden
			actual = no_visitados.poll();
			//System.out.println(actual.x + " " + actual.y);
			//System.out.println(actual.accion + " " + actual.contador);
			//Si es el objetivo, se devuelve y se para de ejecutar el bucle
			if (actual.x == objetivo.x && actual.y == objetivo.y)
				return actual;
			//Se aumenta el número de expandidos
			nExpandidos++;
			
			//Como el coste g es constante de casilla a casilla, cada vez que se genere un hijo se aumenta en 1 la g.
			//Cada uno de los 4 hijos que se expanden desde el nodo actual tendrán 1 más de coste
			hijo.g = actual.g + 1;
			
			// hijo.contador = actual.contador;
			
			//Se expande UP
			hijo.x = actual.x;
			hijo.y = actual.y - 1;
			//La variable contador se va incrementando a partir de cada expansión
			//así, además de controlar la antigüedad de los nodos, comprobamos que se ordenen también
			//según el orden de expansión. A igualdad de f y g, el nodo que se haya generado por UP, por ejemplo
			//llevará más tiempo en la cola que el generado por RIGHT, así que tiene más prioridad, un contador más bajo
			hijo.contador += 1;
			//Hijo no hereda el contador de actual por el hecho de que debemos asignar un identificador unico a cada nuevo hijo que se añada
			visitados.add(actual);
			//Si la posicion en el grid está vacía, es que se puede avanzar
			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
					//o si las coordenadas son las mismas que la del objetivo
					//como hay un portal no nos devolverá el grid vacío, pero se puede avanzar
					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
				//Si el nodo hijo todavía no ha sido visitado y su g es mayor que la del actual + 1
				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE && g[hijo.x][hijo.y] > actual.g + 1) {
					g[hijo.x][hijo.y] = hijo.g; // Guardo su coste en la matriz de costes g

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_UP;
					//Este es un constructor especial para Dijkstra, 0 representa el valor de h del hijo
					no_visitados.add(new Nodo(hijo, objetivo, 0));
				}


			}
			
			//Todo lo hecho anteriormente también se hace con DOWN, RIGHT y LEFT

			// Exploro DOWN
			hijo.x = actual.x;
			hijo.y = actual.y + 1;
			hijo.contador += 1;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
					
					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
				
				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE && g[hijo.x][hijo.y] > actual.g + 1) {
					g[hijo.x][hijo.y] = hijo.g;

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_DOWN;
					no_visitados.add(new Nodo(hijo, objetivo, 0));
				}

			}

			// Exploro LEFT
			hijo.x = actual.x - 1;
			hijo.y = actual.y;
			hijo.contador += 1;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
					
					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
				
				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE && g[hijo.x][hijo.y] > actual.g + 1) {
					g[hijo.x][hijo.y] = hijo.g; 

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_LEFT;
					no_visitados.add(new Nodo(hijo, objetivo, 0));
				}

			}

			// Exploro RIGHT
			hijo.x = actual.x + 1;
			hijo.y = actual.y;
			hijo.contador += 1;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
					
					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
				
				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE && g[hijo.x][hijo.y] > actual.g + 1) {
					g[hijo.x][hijo.y] = hijo.g; 

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_RIGHT;
					no_visitados.add(new Nodo(hijo, objetivo, 0));
				}

			}

		}
	}

	// Comparador que se usa para el orden en la cola de prioridad
	// Primero se tiene en cuenta el valor de f, luego el de g (en este caso g y f siempre coinciden porque
	// h siempre es 0, no se usa la valoración heurística
	
	//Si se empata entre dos nodos en ese caso, se pasa a tomar en cuenta el orden en que se introdujeron en la cola
	//para ello se usa la variable contador. Esta variable se va incrementando por cada hijo de un nodo, así que ya tiene en cuenta
	//el orden de expansión de los nodos.
	Comparator<Nodo> NodeComparator = new Comparator<Nodo>() {
		public int compare(Nodo n1, Nodo n2) {
			int diff = n1.f - n2.f;
			if (diff > 0)
				return 1;
			else if (diff < 0)
				return -1;
			else {
				diff = n1.g - n2.g;
				if (diff > 0)
					return 1;
				else if (diff < 0)
					return -1;
//				else return -1; //son iguales
				else {
					diff = n1.contador - n2.contador;
					if (diff > 0)
						return 1; // si el primero es más nuevo tiene menos prioridad
					else if (diff < 0)
						return -1;
					else return 0;
//					else {
//
//						diff = n1.valorAccion() - n2.valorAccion();
//						if (diff > 0)
//							return 1; // si el primero tiene más valoración tiene menos prioridad
//						else if (diff < 0)
//							return -1;
//						else
//							return 0;
//					}

				}
			}
		}

	};
}
