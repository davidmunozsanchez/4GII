package tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Random;
import java.util.Scanner;

import javax.swing.JFrame;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import core.game.Observation;
import core.game.StateObservation;
import core.player.AbstractPlayer;
import ontology.Types.ACTIONS;
import tools.ElapsedCpuTimer;
import tools.Utils;
import tools.Vector2d;
import tracks.ArcadeMachine;

import java.awt.Color;
import java.awt.Graphics;

public class Pregunta4 {
	
	//Para los experimentos de RTA y LRTA para cambiar de forma random la salida.
	//No se pone semilla
	static Random rand = new Random();
	
	//Esto es para el experimento de Astar, con fin de recorrer todas las casillas como inicio
	//un valor de true significa que ya se ha tomado
	static boolean[][] control = new boolean[14][16];

	public Pregunta4() {
		for (int i = 0; i < control.length; i++) {
			for (int j = 0; j < control[0].length; j++) {
				control[i][j] = false;
			}
		}
	}

	//Este método simplemente cambia de nombre el mapa 6
	public static void nuevoMapa1() {
		int row = 0, col = 0;
		char[][] matrix = new char[14][16]; // matriz de 13 filas x 16 columnas
		Vector2d pos_a = new Vector2d();
		Vector2d new_pos_a = new Vector2d();
		String fileName = "examples.gridphysics.labyrinth_lvl9.txt";
		File archivo = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl6.txt");
		// Leer el archivo y almacenar su contenido en la matriz
		try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
			String line;
			while ((line = br.readLine()) != null) {

				matrix[row++] = line.toCharArray();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

//		System.out.println(matrix.length);
//		for (int i = 0; i < matrix.length; i++) {
//			for (int j = 0; j < matrix[0].length; j++) {
//				System.out.print(matrix[i][j] + " ");
//			}
//			System.out.println("\n");
//		}

		archivo = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt");
		try {
			// Si el archivo no existe, se crea uno nuevo
			if (!archivo.exists()) {
				archivo.createNewFile();
			}

			// Escribir la matriz actualizada en el archivo original
			BufferedWriter bw = new BufferedWriter(new FileWriter(archivo));
			for (int i = 0; i < matrix.length; i++) {
				for (int j = 0; j < matrix[0].length; j++) {
					bw.write(matrix[i][j]);
				}
				bw.newLine();
			}
			bw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}
	
	//Este metodo toma mapa 9 y cambia la A (inicio) de sitio según un valor random.
	public static void nuevoMapa2() {

		int row = 0, col = 0;
		char[][] matrix = new char[14][16]; // matriz de 13 filas x 16 columnas
		Vector2d pos_a = new Vector2d();
		Vector2d new_pos_a = new Vector2d();
		String fileName = "examples.gridphysics.labyrinth_lvl9.txt";
		File archivo = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt");
		// Leer el archivo y almacenar su contenido en la matriz
		try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
			String line;
			while ((line = br.readLine()) != null) {

				matrix[row++] = line.toCharArray();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

//		System.out.println(matrix.length);
//		for (int i = 0; i < matrix.length; i++) {
//			for (int j = 0; j < matrix[0].length; j++) {
//				System.out.print(matrix[i][j] + " ");
//			}
//			System.out.println("\n");
//		}

		// Buscar la posición de la letra 'A' y cambiar su valor
		for (int i = 0; i < matrix.length; i++) {
			for (int j = 0; j < matrix[0].length; j++) {

				if (matrix[i][j] == 'A') {
					pos_a.x = i;
					pos_a.y = j;
					row = i;
					col = j;
					matrix[row][col] = '.'; // cambiar el valor de la posición a '.'
					break;
				}
			}
		}

		//Nueva posicion random
		new_pos_a.x = (pos_a.x + rand.nextInt(matrix.length)) % matrix.length;
		new_pos_a.y = (pos_a.y + rand.nextInt(matrix[0].length)) % matrix[0].length;

		while (matrix[(int) new_pos_a.x][(int) new_pos_a.y] == 'x'
				|| matrix[(int) new_pos_a.x][(int) new_pos_a.y] == 'w'
				|| matrix[(int) new_pos_a.x][(int) new_pos_a.y] == 't') {
			new_pos_a.x = (pos_a.x + rand.nextInt(matrix.length)) % matrix.length;
			new_pos_a.y = (pos_a.y + rand.nextInt(matrix[0].length)) % matrix[0].length;
		}
		matrix[(int) new_pos_a.x][(int) new_pos_a.y] = 'A';

		archivo = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt");
		try {
			// Si el archivo no existe, se crea uno nuevo
			if (!archivo.exists()) {
				archivo.createNewFile();
			}

			// Escribir la matriz actualizada en el archivo original
			BufferedWriter bw = new BufferedWriter(new FileWriter(archivo));
			for (int i = 0; i < matrix.length; i++) {
				for (int j = 0; j < matrix[0].length; j++) {
					bw.write(matrix[i][j]);
				}
				bw.newLine();
			}
			bw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	//Este método es para Astar, cambia la A de sitio a un lugar que no se haya transitado según la matriz
	//control
	public static boolean nuevoMapa3() {
		boolean retorno = false;
		boolean cambio = true;
		int row = 0, col = 0;
		char[][] matrix = new char[14][16]; // matriz de 13 filas x 16 columnas
		Vector2d pos_a = new Vector2d();
		Vector2d new_pos_a = new Vector2d();
		String fileName = "examples.gridphysics.labyrinth_lvl9.txt";
		File archivo = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt");
		// Leer el archivo y almacenar su contenido en la matriz
		try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
			String line;
			while ((line = br.readLine()) != null) {

				matrix[row++] = line.toCharArray();
			}
		} catch (IOException e) {
			e.printStackTrace();
		}

//		System.out.println(matrix.length);
//		for (int i = 0; i < matrix.length; i++) {
//			for (int j = 0; j < matrix[0].length; j++) {
//				System.out.print(matrix[i][j] + " ");
//			}
//			System.out.println("\n");
//		}
		// Buscar la posición de la letra 'A' y cambiar su valor
		for (int i = 0; i < matrix.length; i++) {
			for (int j = 0; j < matrix[0].length; j++) {
				if (matrix[i][j] == 'A') {
					row = i;
					col = j;
					matrix[row][col] = '.'; // cambiar el valor de la posición a '.'
					control[row][col] = true;
					break;
				}
			}
		}

		for (int i = 0; i < matrix.length; i++) {
			for (int j = 0; j < matrix[0].length; j++) {
				if (control[i][j] == false && matrix[i][j] != 'x' && matrix[i][j] != 'w' && matrix[i][j] != 't' && cambio) {
					row = i;
					col = j;
					matrix[row][col] = 'A'; // cambiar el valor de la posición a '.'
					retorno = true;
					cambio = false;
					break;
				}
			}
		}

		archivo = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt");
		try {
			// Si el archivo no existe, se crea uno nuevo
			if (!archivo.exists()) {
				archivo.createNewFile();
			}

			// Escribir la matriz actualizada en el archivo original
			BufferedWriter bw = new BufferedWriter(new FileWriter(archivo));
			for (int i = 0; i < matrix.length; i++) {
				for (int j = 0; j < matrix[0].length; j++) {
					bw.write(matrix[i][j]);
				}
				bw.newLine();
			}
			bw.close();
		} catch (IOException e) {
			e.printStackTrace();
		}

		return retorno;

	}

	public static void main(String[] args) {

		HeatChart mapa1;
		boolean seguir = true;
		int maximo1, maximo2, maximo3;
//		for (int i = 0; i < 10; i++) {
//			// Available tracks:
//			String prueba6 = "tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID.AgenteRTAStar";
//
//			// Load available games
//			String spGamesCollection = "examples/all_games_sp.csv";
//			String[][] games = Utils.readGames(spGamesCollection);
//
//			// Game settings
//			boolean visuals = true;
//			int seed = new Random().nextInt();
//
//			// Game and level to play
//			int gameIdx = 58; // 58 122
//			// int levelIdx = 9; // level names from 0 to 4 (game_lvlN.txt).
//			String gameName = games[gameIdx][1];
//			String game = games[gameIdx][0];
//			String level1 = game.replace(gameName, gameName + "_lvl" + 9);
//
//			String recordActionsFile = null;// "actions_" + games[gameIdx] + "_lvl"
//			// + levelIdx + "_" + seed + ".txt";
//			// where to record the actions
//			// executed. null if not to save.
//
//			// 1. This starts a game, in a level, played by a human.
//			// ArcadeMachine.playOneGame(game, level1, recordActionsFile, seed);
//
//			// 2. This plays a game in a level by the controller.
//			ArcadeMachine.runOneGame(game, level1, visuals, prueba6, recordActionsFile, seed, 0);
//
//		}

		// Available tracks:
		String preg4_1 = "tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID.AgenteLRTAStar_PREG4";
		String preg4_2 = "tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID.AgenteRTAStar_PREG4";
		String preg4_3 = "tracks.singlePlayer.evaluacion.src_MUNOZ_SANCHEZ_DAVID.AgenteAStar_PREG4";

		// Load available games
		String spGamesCollection = "examples/all_games_sp.csv";
		String[][] games = Utils.readGames(spGamesCollection);

		// Game settings
		boolean visuals = true;
		int seed = new Random().nextInt();

		// Game and level to play
		int gameIdx = 58; // 58 122
		// int levelIdx = 9; // level names from 0 to 4 (game_lvlN.txt).
		String gameName = games[gameIdx][1];
		String game = games[gameIdx][0];
		String level1 = "./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt";

		String recordActionsFile = null;// "actions_" + games[gameIdx] + "_lvl"
		// + levelIdx + "_" + seed + ".txt";
		// where to record the actions
		// executed. null if not to save.

		// 1. This starts a game, in a level, played by a human.
		// ArcadeMachine.playOneGame(game, level1, recordActionsFile, seed);

		// 2. This plays a game in a level by the controller.
		//Se copia el mapa 6 en 9
		nuevoMapa1();
		//Se realizan 400 it para LRTA
		for (int i = 0; i < 400; i++) {
			//Posicion de inicio random
			nuevoMapa2();
			ArcadeMachine.runOneGame(game, level1, false, preg4_1, recordActionsFile, seed, 0);
		}
		
		//Experimento igual que el anterior, pero para RTA
		nuevoMapa1();
		for (int i = 0; i < 400; i++) {
			nuevoMapa2();
			ArcadeMachine.runOneGame(game, level1, false, preg4_2, recordActionsFile, seed, 0);
			
		}

		//Experimento para A*
		nuevoMapa1();
		while (seguir) {
			//Seguir se pondra a false cuando no haya ninguna posición nueva de la que partir
			//En ese caso, el método para cambiar de posicion devuelve false
			ArcadeMachine.runOneGame(game, level1, false, preg4_3, recordActionsFile, seed, 0);
			seguir = nuevoMapa3();
			
		}
		

//		try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
//			String line;
//			while ((line = br.readLine()) != null) {
//				char[] chares = line.toCharArray();
//				
//				for(int i = 0; i < chares.length; i++) {
//					System.out.print(chares[i]);
//				}
//				
//				System.out.println();
//			}
//		} catch (IOException e) {
//			e.printStackTrace();
//		}

		// Se borra el archivo auxiliar de mapa
		File archivo2 = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/labyrinth_lvl9.txt");
		archivo2.delete();

		double[][] h_double = new double[16][14];
		double[][] h_def = new double[14][16];

		// PARA EL EXPERIMENTO LRTA*
		//Se pasa a double el mapeo de h
		try {
			Scanner scanner = new Scanner(
					new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/mapaH_LRTA.txt"));

			int i = 0;
			while (scanner.hasNextLine() && i < h_double.length) {
				String linea = scanner.nextLine();
				String[] elementos = linea.split(" ");
				for (int j = 0; j < h_double[0].length && j < elementos.length; j++) {
					h_double[i][j] = Double.parseDouble(elementos[j]);
					if (h_double[i][j] == Integer.MAX_VALUE) {
						h_double[i][j] = -1;
					}
				}
				i++;
			}

			scanner.close();

		} catch (FileNotFoundException e) {
			System.out.println("Archivo no encontrado");
			e.printStackTrace();
		}
//
//		for (int j = 0; j < h_double.length; j++) {
//			for (int h = 0; h < h_double[0].length; h++) {
//				System.out.print(h_double[j][h] + "c");
//
//			}
//			System.out.println();
//		}
		
		//Se toma una orientación correcta del mapa. En los algoritmos, la x son las filas
		//del mapeo de h y la y las columnas, pero para mostrarlo como el juego, es al revés
		for (int j = 0; j < h_double.length; j++) {
			for (int h = 0; h < h_double[0].length; h++) {
				h_def[h][j] = h_double[j][h];
				//System.out.print(h_def[h][j] + "c");

			}
			//System.out.println();

		}

		//Se crea el HeatChart con la clase auxiliar
		mapa1 = new HeatChart(h_def);
		mapa1.setTitle("Mapeo de h tras 400 it. LRTA*");
		maximo1 = (int) mapa1.getHighValue();

		//Se crea el jpg y se borra el mapeo
		File archivo3 = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/img1.jpg");
		File borrar = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/mapaH_LRTA.txt");
		try {

			archivo3.createNewFile();
			mapa1.saveToFile(archivo3);
			borrar.delete();
		}

		catch (

		IOException e) {
			e.printStackTrace();
		}

		// Para RTA
		//Se procede igual que antes pero con otro nombre de imagen
		try {
			Scanner scanner = new Scanner(
					new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/mapaH_RTA.txt"));

			int i = 0;
			while (scanner.hasNextLine() && i < h_double.length) {
				String linea = scanner.nextLine();
				String[] elementos = linea.split(" ");
				for (int j = 0; j < h_double[0].length && j < elementos.length; j++) {
					h_double[i][j] = Double.parseDouble(elementos[j]);
					if (h_double[i][j] == Integer.MAX_VALUE) {
						h_double[i][j] = -2000;
					}
				}
				i++;
			}

			scanner.close();

		} catch (FileNotFoundException e) {
			System.out.println("Archivo no encontrado");
			e.printStackTrace();
		}

//		for (int j = 0; j < h_double.length; j++) {
//			for (int h = 0; h < h_double[0].length; h++) {
//				System.out.print(h_double[j][h] + "c");
//
//			}
//			System.out.println();
//		}
		for (int j = 0; j < h_double.length; j++) {
			for (int h = 0; h < h_double[0].length; h++) {
				h_def[h][j] = h_double[j][h];
				//System.out.print(h_def[h][j] + "c");

			}
			//System.out.println();

		}

		mapa1 = new HeatChart(h_def);
		mapa1.setTitle("Mapeo de h tras 400 it. RTA*");
		maximo2 = (int) mapa1.getHighValue();

		File archivo4 = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/img2.jpg");
		File borrar2 = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/mapaH_RTA.txt");
		try {

			archivo4.createNewFile();
			mapa1.saveToFile(archivo4);
			borrar2.delete();
		}

		catch (

		IOException e) {
			e.printStackTrace();
		}
		
		//Para Astar
		//Igual que para LRTA pero con otro nombre de imagen
		try {
			Scanner scanner = new Scanner(
					new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/mapaH_A.txt"));

			int i = 0;
			while (scanner.hasNextLine() && i < h_double.length) {
				String linea = scanner.nextLine();
				String[] elementos = linea.split(" ");
				for (int j = 0; j < h_double[0].length && j < elementos.length; j++) {
					h_double[i][j] = Double.parseDouble(elementos[j]);
					if (h_double[i][j] == Integer.MAX_VALUE) {
						h_double[i][j] = -1;
					}
				}
				i++;
			}

			scanner.close();

		} catch (FileNotFoundException e) {
			System.out.println("Archivo no encontrado");
			e.printStackTrace();
		}

		
		
		for (int j = 0; j < h_double.length; j++) {
			for (int h = 0; h < h_double[0].length; h++) {
				h_def[h][j] = h_double[j][h];
			}
		}

		mapa1 = new HeatChart(h_def);
		mapa1.setTitle("Camino óptimo desde cada celda, A*");
		maximo3 = (int) mapa1.getHighValue();

		File archivo5 = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/img3.jpg");
		File borrar3 = new File("./src/tracks/singlePlayer/evaluacion/src_MUNOZ_SANCHEZ_DAVID/mapaH_A.txt");
		try {

			archivo5.createNewFile();
			mapa1.saveToFile(archivo5);
			borrar3.delete();
		}

		catch (

		IOException e) {
			e.printStackTrace();
		}
		
		//Como no encontre manera de poner un codigo de colores en la imagen, se muestra esto para facilitar la comprensión de los heat charts.
		System.out.println("PARA LRTA*: ");
		System.out.println("Valor de h máximo graficado: " + maximo1);
		System.out.println("PARA RTA*: ");
		System.out.println("Valor de h máximo graficado: " + maximo2 );
		System.out.println("PARA A*: ");
		System.out.println("Valor de h* máximo graficado: " + maximo3);
	}
}
