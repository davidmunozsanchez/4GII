package tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.PriorityQueue;
import java.util.Comparator;
import java.util.Random;
import java.util.HashSet;
import java.util.Set;

import core.game.Observation;
import core.game.StateObservation;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tools.Vector2d;

import core.player.AbstractPlayer;

public class AgenteCompeticion extends AbstractPlayer {

	Vector2d fescala;
	Nodo portal;
	boolean hayPlan;
	boolean recalcular;
	ArrayList<ACTIONS> plan;
	ArrayList<Observation> obs_anterior[];
	int[][] g; // Matriz para guardar el menor valor de g encontrado, se inicializa con el
				// máximo valor
				// que puede tomar un entero en Java
	int[][] g_ini;
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
	public AgenteCompeticion(StateObservation state, ElapsedCpuTimer timer) {
		// Calculo el factor de escala pixeles a grid
		fescala = new Vector2d(state.getWorldDimension().width / state.getObservationGrid().length,
				state.getWorldDimension().height / state.getObservationGrid()[0].length);

		// Se obtiene la lista de objetivos
		ArrayList<Observation>[] posiciones = state.getPortalsPositions(state.getAvatarPosition());
		obs_anterior = state.getImmovablePositions();
//		// Se inicializa la lista de obstaculos
//		obstaculos = state.getImmovablePositions();
		// Se selecciona el portal más próximo a nuestra ubicación
		portal = new Nodo();
		portal.x = (int) (Math.floor(posiciones[0].get(0).position.x / fescala.x));
		portal.y = (int) (Math.floor(posiciones[0].get(0).position.y / fescala.y));

		// Inicialmente, no hay plan y la lista de acciones es vacía
		hayPlan = false;
		recalcular = false;
		plan = new ArrayList<ACTIONS>();

		// Se inicializan las variables de control a 0
		nExpandidos = 0;
		tamRuta = 0;

		// Se crea la matriz encargada de mapear la g y se inicializa a infinito, que en
		// este caso
		// es el máximo valor que puede tomar un int en Java
		g = new int[state.getObservationGrid().length][state.getObservationGrid()[0].length];
		g_ini = new int[state.getObservationGrid().length][state.getObservationGrid()[0].length];
		for (int i = 0; i < g.length; i++) {
			for (int j = 0; j < g[i].length; j++) {
				g[i][j] = Integer.MAX_VALUE;
			}
		}

		for (int i = 0; i < g_ini.length; i++) {
			for (int j = 0; j < g_ini[i].length; j++) {
				g_ini[i][j] = Integer.MAX_VALUE;
			}
		}

	}

	/**
	 * Sigue un camino generado por A*
	 * 
	 * @param stateObs     Observation of the current state.
	 * @param elapsedTimer Timer when the action returned is due.
	 * @return la pr�xima acci�n a realizar
	 */
	@Override
	public ACTIONS act(StateObservation state, ElapsedCpuTimer timer) {

		// Acción que el algoritmo A* devolverá
		ACTIONS accion = ACTIONS.ACTION_NIL;

		// Si los muros o las trampas ya no son iguales que en la situación de partida,
		// conviene recalcular
		// en caso de que el paso por una casilla invisible no cambie nada, no se
		// recalcula
		if (!obs_anterior[0].equals(state.getImmovablePositions()[0])
				|| !obs_anterior[1].equals(state.getImmovablePositions()[1])) {
			plan.clear();
			hayPlan = false;
			
			//La matriz de g vuelve a su estado de principio de ejecución
			for (int i = 0; i < g_ini.length; i++) {
			    System.arraycopy(g_ini[i], 0, g[i], 0, g_ini[i].length);
			}

			// g = g_ini.clone();
		}

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
		else if(hayPlan){
			// Esto corresponde a una version anterior en la que se recalculaba cuando el
			// agente no podía
			// avanzar
//			Vector2d pos_avatar = new Vector2d(state.getAvatarPosition().x / fescala.x,
//					state.getAvatarPosition().y / fescala.y);
//			accion = plan.get(0);
//			Vector2d pos = comprobarPos(pos_avatar, accion);
//			if (!state.getObservationGrid()[(int) pos.x][(int) pos.y].isEmpty()
//					&& !((pos.x == portal.x) && (pos.y == portal.y))) {
//				plan.clear();
//				hayPlan = false;
//				accion = ACTIONS.ACTION_NIL;
//				for (int i = 0; i < g.length; i++) {
//					for (int j = 0; j < g[i].length; j++) {
//						g[i][j] = Integer.MAX_VALUE;
//					}
//				}
//			} else
//				plan.remove(0);
			accion = plan.get(0);
			plan.remove(0);
		}
		// System.out.println(accion);
		obs_anterior = state.getImmovablePositions();

		// System.out.println(accion);
		return accion;
	}

	// Algoritmo AStar
	public Nodo AStar(Nodo inicial, Nodo objetivo, StateObservation state) {
		// Cola con prioridad donde se van guardando los nodos en abiertos
		PriorityQueue<Nodo> abiertos = new PriorityQueue<Nodo>(NodeComparator);

		// Lista donde se guardan los nodos ya explorados
		ArrayList<Nodo> cerrados = new ArrayList<Nodo>();
		// Set<Nodo> cerrados = new HashSet<>();
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

			// Como el nodo ya ha sido explorado, se a�ade a cerrados
			cerrados.add(actual);

			// aqui debajo esta el fallo
			// Si es el objetivo, se devuelve y se para de ejecutar el bucle
			if (actual.x == objetivo.x && actual.y == objetivo.y) {
				return actual;
			}

			nExpandidos++;
			// Como el coste g es constante de casilla a casilla, cada vez que se genere un
			// hijo se aumenta en 1 la g.
			// Cada uno de los 4 hijos que se expanden desde el nodo actual tendrán 1 más de
			// coste
			hijo.g = actual.g + 1;
			hijo.padre = actual;
			// hijo.contador += 1; // El descendiente se genera despues que el padre
			// así que se le suma 1 al contador, porque todos sus

			// Exploro UP
			hijo.x = actual.x;
			hijo.y = actual.y - 1;
			hijo.contador += 1;
			hijo.accion = ACTIONS.ACTION_UP;
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
					
					//Esto se puede hacer fuera o dentro, es lo mismo
//					hijo.padre = actual;
//					hijo.accion = ACTIONS.ACTION_UP;
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
				
				//Esta forma de proceder se descarto por los malos tiempos que daba el runtime

//				if (!cerrados.contains(hijo) && !abiertos.contains(hijo)) {
//					g[hijo.x][hijo.y] = hijo.g; 
//
//					abiertos.add(new Nodo(hijo, objetivo));
//				} else if (hijo.g < g[hijo.x][hijo.y]) {
//					if (cerrados.contains(hijo)) {
//						// Se actualiza
//						g[hijo.x][hijo.y] = hijo.g;
//						// Una situación es que el nodo esté en abiertos
//						// Así que se borra y se vuelve a meter con la h y la f actualizados
//
//						cerrados.remove(hijo);
//						abiertos.add(new Nodo(hijo, objetivo));
//					} else if (abiertos.contains(hijo)) {
//						// Se actualiza
//						g[hijo.x][hijo.y] = hijo.g;
//						// Una situación es que el nodo esté en abiertos
//						// Así que se borra y se vuelve a meter con la h y la f actualizados
//
//						abiertos.remove(hijo);
//						abiertos.add(new Nodo(hijo, objetivo));
//					}
//				}

			}

			// Todo este proceso anterior se repite para todos los sucesores, así que los
			// comentarios se toman como iguales
			// Exploro DOWN
			hijo.x = actual.x;
			hijo.y = actual.y + 1;
			hijo.contador += 1;
			hijo.accion = ACTIONS.ACTION_DOWN;
			//
			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())

					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {

				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g; // Guardo su coste en la matriz de costes minimos

//					hijo.padre = actual;
//					hijo.accion = ACTIONS.ACTION_UP;
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

			// Exploro LEFT
			hijo.x = actual.x - 1;
			hijo.y = actual.y;
			hijo.contador += 1;
			hijo.accion = ACTIONS.ACTION_LEFT;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())

					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {

				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g; // Guardo su coste en la matriz de costes minimos

//					hijo.padre = actual;
//					hijo.accion = ACTIONS.ACTION_UP;
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

			// Exploro RIGHT
			hijo.x = actual.x + 1;
			hijo.y = actual.y;
			hijo.contador += 1;
			hijo.accion = ACTIONS.ACTION_RIGHT;

			if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())

					|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {

				if (g[hijo.x][hijo.y] == Integer.MAX_VALUE) {
					g[hijo.x][hijo.y] = hijo.g; // Guardo su coste en la matriz de costes minimos

//					hijo.padre = actual;
//					hijo.accion = ACTIONS.ACTION_UP;
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

	public Vector2d comprobarPos(Vector2d avatar, ACTIONS accion) {

		switch (accion) {
		case ACTION_UP:
			avatar.y--;
			break;
		case ACTION_DOWN:
			avatar.y++;
			break;
		case ACTION_RIGHT:
			avatar.x++;
			break;
		case ACTION_LEFT:
			avatar.x--;
			break;
		default:
			break;
		}

		return avatar;
	}

}
//	Inicializar:
//		  s_start = nodo de inicio
//		  s_goal = nodo objetivo
//		  k_m = 0
//		  g(s) = h(s) = ∞ para cada estado s
//
//		  g(s_goal) = h(s_start, s_goal)
//		  open_list = {s_goal}
//		  while open_list no está vacía:
//		    s = el estado con menor g(s) en open_list
//		    si g(s) > h(s_start, s):
//		      g(s) = h(s_start, s)
//		      actualizar todos los estados en pred(s)
//		    else:
//		      k_old = k_m
//		      k_m = g(s)
//		      actualizar todos los estados en el que el costo ha aumentado
//		      k_m += h(s_start, s)
//		      actualizar todos los estados en open_list con un nuevo valor de g
//
//		    si k_old < k_m:
//	Calcular el camino de s_start a s_goal y actualizar la solución
//	
//	En este pseudocódigo, "g(s)" representa el costo real del camino desde el nodo de inicio hasta el estado "s", "h(s)" representa una estimación del costo de la ruta más corta desde "s" hasta el nodo objetivo, "open_list" es una lista de estados que aún no han sido explorados y "pred(s)" representa los estados predecesores de "s". El valor de "k_m" es una cota inferior del costo real de la ruta más corta desde el nodo de inicio hasta el nodo objetivo.
//
//	El algoritmo comienza inicializando los valores de "g(s)" y "h(s)" para todos los estados con infinito, excepto para el nodo objetivo, para el cual "g(s_goal)" se inicializa con "h(s_start, s_goal)".
//
//	El algoritmo luego selecciona el estado con el valor más bajo de "g(s)" en la "open_list". Si el valor de "g(s)" es mayor que la estimación actual de "h(s_start, s)", se actualiza "g(s)" y se propagan estos cambios a todos los predecesores de "s". De lo contrario, se actualiza "k_m" con el valor de "g(s)" y se propagan los cambios a todos los estados cuyo costo ha aumentado.
//
//	Si el valor de "k_m" ha cambiado desde la última vez que se actualizó, se calcula un nuevo camino desde el nodo de inicio hasta el nodo objetivo y se actualiza la solución.
//
//	El algoritmo continúa hasta que la "open_list" esté vacía o se encuentre una solución.