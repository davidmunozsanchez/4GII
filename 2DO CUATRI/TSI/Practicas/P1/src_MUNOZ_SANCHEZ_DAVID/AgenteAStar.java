package tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID;

import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Comparator;

import core.game.Observation;
import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tools.Vector2d;

import core.player.AbstractPlayer;

public class AgenteAStar extends AbstractPlayer {

	Vector2d fescala;
	Nodo portal;
	boolean hayPlan;
	ArrayList<ACTIONS> plan;
	ArrayList<Observation> obstaculos[];
	int[][] g; // Matriz para guardar el menor valor de g encontrado, se inicializa con el
				// máximo valor
				// que puede tomar un entero en Java
	int nExpandidos; // Numero de nodos expandidos
	int tamRuta; // Numero de nodos que el agente recorre
	double runTime; // Tiempo, en milisegundos, usado para calcular el plan,
	// se calcula de la forma que se indica en las transparencias

	/**
	 * initialize all variables for the agent
	 * 
	 * @param stateObs     Observation of the current state.
	 * @param elapsedTimer Timer when the action returned is due.
	 */
	public AgenteAStar(StateObservation state, ElapsedCpuTimer timer) {
		// Calculo el factor de escala pixeles a grid
		fescala = new Vector2d(state.getWorldDimension().width / state.getObservationGrid().length,
				state.getWorldDimension().height / state.getObservationGrid()[0].length);

		// Se obtiene la lista de objetivos
		ArrayList<Observation>[] posiciones = state.getPortalsPositions(state.getAvatarPosition());
//		// Se inicializa la lista de obstaculos
//		obstaculos = state.getImmovablePositions();
		// Se selecciona el portal más próximo a nuestra ubicación
		portal = new Nodo();
		portal.x = (int) (Math.floor(posiciones[0].get(0).position.x / fescala.x));
		portal.y = (int) (Math.floor(posiciones[0].get(0).position.y / fescala.y));

		// Inicialmente, no hay plan y la lista de acciones es vacía
		hayPlan = false;
		plan = new ArrayList<ACTIONS>();

		// Se inicializan las variables de control a 0
		nExpandidos = 0;
		tamRuta = 0;

		// Se crea la matriz encargada de mapear la g y se inicializa a infinito, que en
		// este caso
		// es el máximo valor que puede tomar un int en Java
		g = new int[state.getObservationGrid().length][state.getObservationGrid()[0].length];
		for (int i = 0; i < g.length; i++) {
			for (int j = 0; j < g[i].length; j++) {
				g[i][j] = Integer.MAX_VALUE;
			}
		}

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

		// Acción que el algoritmo A* devolverá
		ACTIONS accion = ACTIONS.ACTION_NIL;

		// Si no hay plan, busco uno nuevo
		if (!hayPlan) {

			// Se genera el nodo inicial a partir de la posición
			Vector2d pos_avatar = new Vector2d(state.getAvatarPosition().x / fescala.x,
					state.getAvatarPosition().y / fescala.y);
			Nodo avatar = new Nodo(pos_avatar);
			avatar.g = 0; // Por ser nodo inicial
			avatar.contador = 0; // Esta variable se encargará de mantener el orden en la
									// cola con prioridad en base a la antigüedad de los nodos
			avatar.calcular_h(portal);
			avatar.calcular_f();

			// Se llama al algoritmo A*, que devolverá el último nodo
			long tInicio = System.nanoTime();
			Nodo ultimo = AStar(avatar, portal, state);
			runTime = (System.nanoTime() - tInicio) / 1000000.0;
			hayPlan = true;

			// El plan se forma recorriendo desde el nodo final los padres hasta el nodo
			// inicial
			while (ultimo != avatar) {
				plan.add(0, ultimo.accion);
				ultimo = ultimo.padre;
			}
			tamRuta = plan.size(); // El tamaño de la ruta será el tamaño del ArrayList que guarda
									// las acciones del plan
			
			accion = plan.get(0);
			plan.remove(0);
			// Se imprimen los datos pedidos en la práctica
			System.out.print(" Runtime(ms): " + runTime + ",\n Tamaño de la ruta: " + tamRuta + ",\n Nodos expandidos: "
					+ nExpandidos + "\n");
		}
		// Si hay plan, se selecciona la siguiente accion a ejecutar y se saca del plan
		else if (hayPlan) {
			accion = plan.get(0);
			plan.remove(0);
		}

		return accion;
	}

	// Algoritmo AStar
	public Nodo AStar(Nodo inicial, Nodo objetivo, StateObservation state) {
		// Cola con prioridad donde se van guardando los nodos en abiertos
		PriorityQueue<Nodo> abiertos = new PriorityQueue<Nodo>(NodeComparator);

		// Lista donde se guardan los nodos ya explorados
		ArrayList<Nodo> cerrados = new ArrayList<Nodo>();
		Nodo actual; // Nodo que se está explorando
		Nodo hijo = new Nodo(); // Nodo para ir guardando los diferentes nodos generados por la expansion
								// del actual

		// El coste g del nodo inicial es 0
		g[inicial.x][inicial.y] = 0;
		abiertos.add(inicial);

		// El bucle parará cuando se llegue al objetivo
		// Dada las características y restricciones del juego que se usa para la
		// práctica
		// tenemos asegurado que siempre es posible llegar al objetivo
		while (true) {
			// El nodo siguiente a visitar y expandir será el primero de la cola con
			// prioridad
			// que ya está ordenada según los criterios de selección que se piden
			actual = abiertos.poll();

			// Como el nodo ya ha sido explorado, se añade a cerrados
			cerrados.add(actual);

			// Si es el objetivo, se devuelve y se para de ejecutar el bucle
			if (actual.x == objetivo.x && actual.y == objetivo.y)
				return actual;
			nExpandidos++;
			// Como el coste g es constante de casilla a casilla, cada vez que se genere un
			// hijo se aumenta en 1 la g.
			// Cada uno de los 4 hijos que se expanden desde el nodo actual tendrán 1 más de
			// coste
			hijo.g = actual.g + 1;
			//hijo.contador += 1; // El descendiente se genera despues que el padre

			// Exploro UP
			hijo.x = actual.x;
			hijo.y = actual.y - 1;
			hijo.contador += 1;
			//
			// Si la posicion en el grid está vacía, es que se puede avanzar
			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
					// o si las coordenadas son las mismas que la del objetivo
					// como hay un portal no nos devolverá el grid vacío, pero se puede avanzar
					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
				// Si no esta visitado, es decir, si nunca se ha actualizado su valor de g
				// quiere decir que no está ni en cerrados ni en abiertos, así que se añade a
				// abiertos
				// El constructor al que se le pasa el objetivo y el hijo ya se encarga de
				// calcular la h y la f
				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g; // Guardo su coste en la matriz de costes minimos

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_UP;
					abiertos.add(new Nodo(hijo, objetivo));
				}

				// Si su g es menor que la mínima que se tenía se dan dos situaciones
				else if (hijo.g < g[hijo.x][hijo.y]) {
					// Se actualiza
					g[hijo.x][hijo.y] = hijo.g;
					// Una situación es que el nodo esté en abiertos
					// Así que se borra y se vuelve a meter con la h y la f actualizados
					if (abiertos.contains(hijo)) {
						abiertos.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
					// La otra situación es que esté en cerrados, así que se borra de cerrados
					// y se mete en abiertos con la nueva h y f
					if (cerrados.contains(hijo)) {
						cerrados.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
				}

			}

			// Todo este proceso anterior se repite para todos los sucesores, así que los
			// comentarios se toman como iguales
			// Exploro DOWN
			hijo.x = actual.x;
			hijo.y = actual.y + 1;
			hijo.contador += 1;
			//
			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())

					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {

				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g;

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_DOWN;
					abiertos.add(new Nodo(hijo, objetivo));

				}

				else if (hijo.g < g[hijo.x][hijo.y]) {
					g[hijo.x][hijo.y] = hijo.g;
					if (abiertos.contains(hijo)) {
						abiertos.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
					if (cerrados.contains(hijo)) {
						cerrados.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
				}
			}

			// Exploro LEFT
			hijo.x = actual.x - 1;
			hijo.y = actual.y;
			hijo.contador += 1;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())

					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {

				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g;

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_LEFT;
					abiertos.add(new Nodo(hijo, objetivo));
				}

				else if (hijo.g < g[hijo.x][hijo.y]) {
					g[hijo.x][hijo.y] = hijo.g;
					if (abiertos.contains(hijo)) {
						abiertos.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
					if (cerrados.contains(hijo)) {
						cerrados.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
				}
			}

			// Exploro RIGHT
			hijo.x = actual.x + 1;
			hijo.y = actual.y;
			hijo.contador += 1;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())

					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {

				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g;

					hijo.padre = actual;
					hijo.accion = ACTIONS.ACTION_RIGHT;
					abiertos.add(new Nodo(hijo, objetivo));
				}

				else if (hijo.g < g[hijo.x][hijo.y]) {
					g[hijo.x][hijo.y] = hijo.g;
					if (abiertos.contains(hijo)) {
						abiertos.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
					if (cerrados.contains(hijo)) {
						cerrados.remove(hijo);
						abiertos.add(new Nodo(hijo, objetivo));
					}
				}
			}

		}
	}

	// Comparador que se usa para el orden en la cola de prioridad
	// Primero se tiene en cuenta el valor de f, luego el de g (en este caso g y f
	// siempre coinciden porque
	// h siempre es 0, no se usa la valoración heurística

	// Si se empata entre dos nodos en ese caso, se pasa a tomar en cuenta el orden
	// en que se introdujeron en la cola
	// para ello se usa la variable contador. Esta variable se va incrementando por
	// cada hijo de un nodo, así que ya tiene en cuenta
	// el orden de expansión de los nodos.
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
//					else return -1; //son iguales
				else {
					diff = n1.contador - n2.contador;
					if (diff > 0)
						return 1; // si el primero es más nuevo tiene menos prioridad
					else if (diff < 0)
						return -1;
					else
						return 0;
//						else {
					//
//							diff = n1.valorAccion() - n2.valorAccion();
//							if (diff > 0)
//								return 1; // si el primero tiene más valoración tiene menos prioridad
//							else if (diff < 0)
//								return -1;
//							else
//								return 0;
//						}

				}
			}
		}

	};
}
