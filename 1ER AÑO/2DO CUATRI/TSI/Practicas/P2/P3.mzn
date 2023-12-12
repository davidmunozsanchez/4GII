set of int: franjas = 1..6; %Todas las franjas horarias
set of int: dias = 1..5; %Todos los dias que usamos
set of int: asignaturas = 1..9; %Todas las asignaturas que tenemos

%arrays que almacenan, para cada asignatura, el número de horas, número de bloques y tamaño de cada bloque a impartir
array [asignaturas] of int: num_horas = [4, 2, 4, 4, 4, 2, 2, 2, 1];
array [asignaturas] of int: num_bloques = [2, 2, 2, 2, 2, 2, 2, 1, 1];
array [asignaturas] of int: tam_bloque = [2, 1, 2, 2, 2, 1, 1, 2, 1];

%array que almacena, para cada asignatura, qué profesor la imparte.
array [asignaturas] of int: profesor = [1, 4, 1, 2, 2, 3, 4, 4, 3];

%Horario completo. En cada [dia,hora] se indica qué asignatura se está impartiendo
array [dias, franjas] of var 1..10: horario; %El 10 serán las tutorías
% Matriz intercambiada para mejor visualización
array [franjas, dias] of var 1..10: horario_nuevo; %El 10 serán las tutorías

%Hora a la que se imarten las asignaturas. En cada [asignatura, dia] se indica a qué hora empieza el bloque que se imparte ese día. Con esto nos aseguramos de cumplir la restricción de impartir, como máximo, un bloque por día/asignatura
array [asignaturas,dias] of var 0..6: hora_clase; %(0 si no se imparte)

%Si una asignatura tiene clase un dia concreto a una hora concreta, esa asignatura deberá estar apuntada ahí en el horario
constraint(forall(asignatura in asignaturas)(forall(dia in dias)(forall(hora in franjas)(
    (hora_clase[asignatura,dia] == hora) -> (horario[dia,hora] == asignatura) ) ) ) );
    
%Si, además de lo anterior, sus bloques son de dos horas, también deberá estar apuntada en la siguiente hora del horario
constraint(forall(asignatura in asignaturas)(forall(dia in dias)(forall(hora in franjas)(
    (hora_clase[asignatura,dia] == hora) /\ (tam_bloque[asignatura] > 1) -> 
	%Se evita que se pueda comenzar una clase de 2h en la última hora
	(hora < 6 /\ (horario[dia,hora+1] == asignatura)) ) ) ) );

%El número de días que se imparte una asignatura debe ser igual al número de bloques que tiene.
%Así, junto a la restricción anterior, se asegura que cada día se imparta solo un bloque. 
constraint(forall(asignatura in asignaturas)( (sum(dia in dias)(hora_clase[asignatura,dia]!=0) ) == num_bloques[asignatura]) );

%El número de horas que aparece una asignatura en el horario debe ser igual al número de horas que, se supone, se tiene que impartir
constraint(forall(asignatura in asignaturas)( 
  (sum(dia in dias)(sum(hora in franjas)(horario[dia,hora] == asignatura) ) ) == num_horas[asignatura] ) );

%Si dos asignaturas distintas se imparten el mismo dia, deben ser impartidas por diferente profesor (a excepción de que el profesor sea el 4, en cuyo caso da igual)
constraint(forall(dia in dias)(forall(asignatura1 in asignaturas)(forall(asignatura2 in asignaturas)(
 %Si son la misma asignatura, no hay restricción
  asignatura1 == asignatura2 \/
%Si alguna de las asignaturas no se imparte ese día, no hay restricción
  hora_clase[asignatura1, dia] == 0 \/ hora_clase[asignatura2, dia] == 0 \/
%Si las asignaturas tienen el profesor 4, no hay restricción
  profesor[asignatura1] == 4 \/
%Si no se cumple ninguna de las anteriores, significa que no pueden tener el mismo profesor
  profesor[asignatura1] != profesor[asignatura2] ) ) ) );
 
%El recreo es la cuarta asignatura, todos los días
constraint(forall(dia in dias)(horario[dia,4] == 10));

%Aquí se contemplan todas las resticciones de a qué horas puede dar clases un profesor. Para ello usaremos estructuras condicionales:
%Dada una clase (asignatura concreta impartida un dia y una hora concretos)
constraint(forall(asignatura in asignaturas)(forall(dia in dias)(forall(hora in franjas)(
  horario[dia,hora] == asignatura -> (
%Si la imparte el profesor 1, la clase deberá ser en las horas 5 o 6
  (profesor[asignatura] == 1 -> hora >= 5 ) /\
%Si la imparte el proffesor 2, la clase deberá ser en las horas 1, 2 o 3
  (profesor[asignatura] == 2 -> hora <= 3 ) /\
%Si la imparte el proffesor 3, la clase deberá ser en la hora 3
  (profesor[asignatura] == 3 -> hora == 3 ) ) ) ) ) );
  
%Aquí se compactan todas las restricciones de qué días se puede impartir cada asignatura.
%Todas siguen la misma estructura. O no se imparte (hora == 0) o se imparte alguno de los días permitidos (dia == x)
constraint(forall(dia in dias)(
  (  hora_clase[1,dia] == 0 \/ dia == 1 \/ dia == 2 ) /\
  (  hora_clase[3,dia] == 0 \/ dia == 3 \/ dia == 4 ) /\
  (  hora_clase[4,dia] == 0 \/ dia == 1 \/ dia == 2 ) /\
  (  hora_clase[5,dia] == 0 \/ dia == 4 \/ dia == 5 ) /\
  (  hora_clase[6,dia] == 0 \/ dia == 3 \/ dia == 5 ) /\
  (  hora_clase[7,dia] == 0 \/ dia == 2 \/ dia == 5 ) /\
  (  hora_clase[8,dia] == 0 \/ dia == 3  ) /\
  (  hora_clase[9,dia] == 0 \/ dia == 1) ) );


% Intercambiar filas y columnas
constraint(forall(dia in dias, franja in franjas)(
  (horario_nuevo[franja,dia] = horario[dia,franja])
));



solve satisfy;

%output[show(horario)];
output [ "        Lunes Martes Miercoles Jueves Viernes\n" ++
         "08-09" ++ "   " ++show(horario_nuevo[1,1]) ++ " P" ++ show(profesor[horario_nuevo[1,1]])++ "   " ++ show(horario_nuevo[1,2]) ++ " P" ++ show(profesor[horario_nuevo[1,2]]) ++"    " ++show(horario_nuevo[1,3]) ++ " P" ++ show(profesor[horario_nuevo[1,3]]) ++ "     " ++ show(horario_nuevo[1,4]) ++ " P"++ show(profesor[horario_nuevo[1,4]])++"   " ++show(horario_nuevo[1,5]) ++ " P"++ show(profesor[horario_nuevo[1,5]])++ "\n" ++
         "09-10" ++ "   " ++show(horario_nuevo[2,1]) ++ " P" ++ show(profesor[horario_nuevo[2,1]])++ "   " ++ show(horario_nuevo[2,2]) ++ " P" ++ show(profesor[horario_nuevo[2,2]]) ++"    " ++show(horario_nuevo[2,3]) ++ " P" ++ show(profesor[horario_nuevo[2,3]]) ++ "     " ++ show(horario_nuevo[2,4]) ++ " P"++ show(profesor[horario_nuevo[2,4]])++"   " ++show(horario_nuevo[2,5]) ++ " P"++ show(profesor[horario_nuevo[2,5]])++ "\n" ++
         "10-11" ++ "   " ++show(horario_nuevo[3,1]) ++ " P" ++ show(profesor[horario_nuevo[3,1]])++ "   " ++ show(horario_nuevo[3,2]) ++ " P" ++ show(profesor[horario_nuevo[3,2]]) ++"    " ++show(horario_nuevo[3,3]) ++ " P" ++ show(profesor[horario_nuevo[3,3]]) ++ "     " ++ show(horario_nuevo[3,4]) ++ " P"++ show(profesor[horario_nuevo[3,4]])++"   " ++show(horario_nuevo[3,5]) ++ " P"++ show(profesor[horario_nuevo[3,5]])++ "\n" ++
         "11-12" ++ "    " ++show(horario_nuevo[4,1]) ++ "     " ++ show(horario_nuevo[4,2]) ++ "      " ++show(horario_nuevo[4,3]) ++ "       " ++ show(horario_nuevo[4,4]) ++ "     " ++show(horario_nuevo[4,5]) ++ "\n" ++
         "12-13" ++ "   " ++show(horario_nuevo[5,1]) ++ " P" ++ show(profesor[horario_nuevo[5,1]])++ "   " ++ show(horario_nuevo[5,2]) ++ " P" ++ show(profesor[horario_nuevo[5,2]]) ++"    " ++show(horario_nuevo[5,3]) ++ " P" ++ show(profesor[horario_nuevo[5,3]]) ++ "     " ++ show(horario_nuevo[5,4]) ++ " P"++ show(profesor[horario_nuevo[5,4]])++"   " ++show(horario_nuevo[5,5]) ++ " P"++ show(profesor[horario_nuevo[5,5]])++ "\n" ++ 
         "13-14" ++ "   " ++show(horario_nuevo[6,1]) ++ " P" ++ show(profesor[horario_nuevo[6,1]])++ "   " ++ show(horario_nuevo[6,2]) ++ " P" ++ show(profesor[horario_nuevo[6,2]]) ++"    " ++show(horario_nuevo[6,3]) ++ " P" ++ show(profesor[horario_nuevo[6,3]]) ++ "     " ++ show(horario_nuevo[6,4]) ++ " P"++ show(profesor[horario_nuevo[6,4]])++"   " ++show(horario_nuevo[6,5]) ++ " P"++ show(profesor[horario_nuevo[6,5]])++ "\n"];
