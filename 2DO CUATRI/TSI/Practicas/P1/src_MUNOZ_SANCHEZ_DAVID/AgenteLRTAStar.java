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

public class AgenteLRTAStar extends AbstractPlayer {

	Vector2d fescala;
	Nodo portal;
	// boolean hayPlan; //Aqui se va paso a paso en tiempo real
	ArrayList<ACTIONS> plan;
	// ArrayList<Observation> obstaculos[];
	int[][] h; //Matriz para el mapeo de h a lo largo del algoritmo
	int nExpandidos; //Numero de nodos expandido
	int tamRuta; //Nodos que el agente recorre
	double runTime; // Tiempo, en milisegundos, usado para calcular el plan
					// en este caso, se calculara la suma de todas las llamadas al algoritmo
					//dado que el algoritmo no devuelve un plan completo si no una acción

	/**
	 * initialize all variables for the agent
	 * 
	 * @param stateObs     Observation of the current state.
	 * @param elapsedTimer Timer when the action returned is due.
	 */
	public AgenteLRTAStar(StateObservation state, ElapsedCpuTimer timer) {
		// Calculo el factor de escala de pixeles al grid)
		fescala = new Vector2d(state.getWorldDimension().width / state.getObservationGrid().length,
				state.getWorldDimension().height / state.getObservationGrid()[0].length);

		//Lista de objetivos
		ArrayList<Observation>[] posiciones = state.getPortalsPositions(state.getAvatarPosition());
		//Se selecciona el objetivo más próximo
		portal = new Nodo();
		portal.x = (int) (Math.floor(posiciones[0].get(0).position.x / fescala.x));
		portal.y = (int) (Math.floor(posiciones[0].get(0).position.y / fescala.y));

		//Se inicializa el plan como vacío
		// hayPlan = false;
		//plan = new ArrayList<ACTIONS>();

		// Inicializo las variables de control a 0
		nExpandidos = 0;
		tamRuta = 0;

		//Se crea el mapa de h y se inicializa todo a infinito, que en este caso es el máximo valor
		//que puede tomar un entero en Java
		h = new int[state.getObservationGrid().length][state.getObservationGrid()[0].length];

		for (int i = 0; i < h.length; i++) {
			for (int j = 0; j < h[i].length; j++) {
				h[i][j] = Integer.MAX_VALUE;
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

		//Accion que devolverá el algoritmo
		ACTIONS accion = ACTIONS.ACTION_NIL;
		//Se genera el nodo inicial a partir de la posición
		Vector2d pos_avatar = new Vector2d(state.getAvatarPosition().x / fescala.x,
				state.getAvatarPosition().y / fescala.y);
		Nodo avatar = new Nodo(pos_avatar);
		//La g inicialmente es 0, las demás variables se calculan con los métodos creados
		//para ello
		avatar.g = 0;
		avatar.calcular_h(portal);
		avatar.calcular_f();

		// Se llama al algoritmo de búsqueda y se calcula el tiempo
		// se imprimirá información por pantalla siempre y cuando se vaya a devolver
		// la acción que nos lleve a la última casilla
		long tInicio = System.nanoTime();
		accion = RTAStar(avatar, portal, state);
		// Vamos acumulando runtime
		runTime += (System.nanoTime() - tInicio);

		pos_avatar = comprobarPos(pos_avatar, accion);
		if (pos_avatar.x == portal.x && pos_avatar.y == portal.y) {
			System.out.print(" Runtime(ms): " + runTime / 1000000.0 + ",\n Tamaño de la ruta calculada: " + tamRuta
					+ ",\n Número de nodos expandidos: " + nExpandidos + "\n");
			
//			for (int i = 0; i < h.length; i++) {
//				for (int j = 0; j < h[i].length; j++) {
//					System.out.print(portal.x + " " + portal.y);
//				}
//				System.out.println();
//			}
		}

		return accion;
	}

	public ACTIONS RTAStar(Nodo actual, Nodo objetivo, StateObservation state) {
		nExpandidos++;
		tamRuta++;
		// Se inicializa la heurística con la actual si no está inicializada
		if (h[actual.x][actual.y] == Integer.MAX_VALUE) {
			h[actual.x][actual.y] = actual.h;
		}

		PriorityQueue<Nodo> sucesores = new PriorityQueue<Nodo>(NodeComparator);
		Nodo hijo = new Nodo(); //Nodo en el que se ira almacenando los distintos hijos del actual
		
		// Exploro UP
		hijo.x = actual.x;
		hijo.y = actual.y - 1;
		hijo.contador += 1;
		
		// Si esa posición en el grid está vacía, es que es suelo. Se puede avanzar
		if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
				// Si las cordenadas son las mismas que las del objetivo, se puede
				// avanzar
				|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
			// Si no esta visitado, se calcula la heurística y se guarda
			if (h[hijo.x][hijo.y] == Integer.MAX_VALUE) {
				h[hijo.x][hijo.y] = hijo.calcular_h(objetivo);
			}
			// Si ya ha sido visitado, su heurística es la que tiene el algoritmo para esa posición
			else
				hijo.h = h[hijo.x][hijo.y];

			// Se añade a la cola de sucesores
			hijo.accion = ACTIONS.ACTION_UP;
			sucesores.add(new Nodo(hijo));
		}
		
		//Los comentarios anteriores sirve para los demás nodos, lo único que cambia son las coordenadas
		//del hijo calculadas al principio
		
		// Exploro DOWN
		hijo.x = actual.x;
		hijo.y = actual.y + 1;
		hijo.contador += 1;
		if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
				
				|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
			
			if (h[hijo.x][hijo.y] == Integer.MAX_VALUE) {
				h[hijo.x][hijo.y] = hijo.calcular_h(objetivo);
			}
			
			else
				hijo.h = h[hijo.x][hijo.y];

			hijo.accion = ACTIONS.ACTION_DOWN;
			sucesores.add(new Nodo(hijo));
		}

		// Exploro LEFT
		hijo.x = actual.x - 1;
		hijo.y = actual.y;
		hijo.contador += 1;
		if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
				
				|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
			
			if (h[hijo.x][hijo.y] == Integer.MAX_VALUE) {
				h[hijo.x][hijo.y] = hijo.calcular_h(objetivo);
			}
			
			else
				hijo.h = h[hijo.x][hijo.y];

			hijo.accion = ACTIONS.ACTION_LEFT;
			sucesores.add(new Nodo(hijo));
		}

		// Exploro RIGHT
		hijo.x = actual.x + 1;
		hijo.y = actual.y;
		hijo.contador += 1;
		if ((state.getObservationGrid()[hijo.x][hijo.y].isEmpty())
				
				|| ((hijo.x == objetivo.x) && (hijo.y == objetivo.y))) {
			
			if (h[hijo.x][hijo.y] == Integer.MAX_VALUE) {
				h[hijo.x][hijo.y] = hijo.calcular_h(objetivo);
			}
			
			else
				hijo.h = h[hijo.x][hijo.y];

			hijo.accion = ACTIONS.ACTION_RIGHT;
			sucesores.add(new Nodo(hijo));
		}

		// Se extrae el hijo con menor coste según la heurística
		hijo = sucesores.poll();
		ACTIONS accion = hijo.accion;
		int new_h = hijo.h + 1;
		
		
		//Esta parte lo diferencia de RTA*
//		hijo = sucesores.poll(); // El segundo mejor
//
//		// Solo si hay un segundo hijo en la cola de sucesores
//		// se actualiza la nueva h
//		if (hijo != null) {
//			new_h = hijo.h + 1;
//		}
//Esta parte se ha comentado porque en LRTAstar se actualiza en base
//al mejor, no al segundo mejor
		if (h[actual.x][actual.y] < new_h) {
			h[actual.x][actual.y] = new_h;
		}

		return accion;

	}
	
	//Al igual que en RTA*, solo se compara por la h y después por el contador
	Comparator<Nodo> NodeComparator = new Comparator<Nodo>() {
		public int compare(Nodo n1, Nodo n2) {
			int diff = n1.h - n2.h;
			if (diff > 0)
				return 1;
			else if (diff < 0)
				return -1;
			else {
				diff = n1.contador - n2.contador;
				if (diff > 0)
					return 1;
				else if (diff < 0)
					return -1;
				else return 0;
//				else {
//					diff = n1.valorAccion() - n2.valorAccion();
//					if (diff > 0)
//						return 1;
//					else if (diff < 0)
//						return -1;
//					else
//						return 0;
//				}
			}
		}
	};
	
	//Esto sirve para comprobar si vamos a llegar al objetivo con una acción
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
