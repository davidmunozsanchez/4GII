
set of int: tareas = 1..10; %todas las tareas a realizar
set of int: trabajadores = 1..3; %todos los trabajadores

%array que almacena el tiempo que tarda cada trabajador en realizar una tarea
array [tareas, trabajadores] of int: tiempo = [|5,7,10|
                                                3,5,7|
                                                3,2,4|
                                                2,5,8|
                                                4,3,6|
                                                3,2,1|
                                                2,2,3|
                                                1,3,5|
                                                2,3,4|
                                                3,2,1|];

%arrays que almacenan, para cada tarea, el día que empieza, el día que termina y el trabajador que la realiza
array [tareas] of var 0..100: inicio;
array [tareas] of var 0..100: fin;
array [tareas] of var trabajadores: trabajador;

var int: tiempo_total; %Tiempo que se tarda en hacer toda la construcción

%%Necesario solo en versión 2
array [tareas] of var bool: auxiliar; %Indica, para cada tarea, si ha participado el trabajador auxiliar o no

%Teniendo en cuenta la jerarquía del enunciado, limitamos cuándo puede empezar cada tarea:

%La tarea A es la primera, empieza en el dia 0
constraint inicio[1] == 0;

%El dia que se inicia una tarea debe ser mayor o igual al dia en el que se finalizó cada uno de sus predecesores
constraint inicio[2] >= fin[1];
constraint inicio[3] >= fin[2];
constraint inicio[4] >= fin[1];
constraint inicio[5] >= fin[3];
constraint inicio[5] >= fin[4];
constraint inicio[6] >= fin[3];
constraint inicio[6] >= fin[4];
constraint inicio[6] >= fin[5];
constraint inicio[7] >= fin[3];
constraint inicio[7] >= fin[4];
constraint inicio[8] >= fin[1];
constraint inicio[9] >= fin[6];
constraint inicio[9] >= fin[8];
constraint inicio[10] >= fin[7];
constraint inicio[10] >= fin[9];

%Dadas dos tareas diferentes, si hay un momento en el que se realizan a la vez, entonces tienen que ser realizadas por dos trabajadores diferentes
%Para implementar esto
constraint forall(tarea1 in tareas)(forall(tarea2 in tareas)(
 %Si son la misma, no hay restricciones
  tarea1 == tarea2 \/ 
 %Si el inicio de la primera fue después del fin de la segunda o viceversa, no hay restricciones, ya que no se superponen. Pero en el caso de que ambas cosas sean falsas significará que hubo algún momento en el que se estuvieron realizando a la vez
  fin[tarea1] <= inicio[tarea2] \/ fin[tarea2] <= inicio[tarea1] \/ 
%Si todo lo anterior es falso, es que las tareas se superponen, por lo que deben ser hechas por dos trabajadores diferentes 
 trabajador[tarea1] != trabajador[tarea2] ) );
  
%Para toda tarea, el dia que termina es igual al que empezó más el tiempo que se empleó en hacerla

%Esto es para la versión sin auxiliar
%constraint forall(tarea in tareas)(
  %fin[tarea] == inicio[tarea] + tiempo[tarea, trabajador[tarea]] );
 constraint forall(tarea in tareas)(
 %En el caso de que sea con auxiliar, el tiempo que se ha tardado será el que tarda el trabajador seleccionado - 1
  (auxiliar[tarea] -> fin[tarea] == inicio[tarea] + tiempo[tarea, trabajador[tarea]] - 1) /\ 
 %En caso contrario, será simplemente el tiempo será el que tarde el trabajador en cuestión.
  (not auxiliar[tarea] -> fin[tarea] == inicio[tarea] + tiempo[tarea, trabajador[tarea]] )  );
%Que una tarea tenga al auxiliar solo es posible si el el trabajador solo tarda dos días o más en hacerla. Esto no es necesario para la versión sin auxiliar
constraint forall(tarea in tareas)( (not auxiliar[tarea]) \/ (tiempo[tarea, trabajador[tarea]] >= 2) );

%Si dos tareas se superponen no pueden tener ambas auxiliar. No necesario para la versión sin auxiliar
constraint forall(tarea1 in tareas)(forall(tarea2 in tareas)(
  tarea1 == tarea2 \/ 
  fin[tarea1] <= inicio[tarea2] \/ fin[tarea2] <= inicio[tarea1] \/ 
  (not (auxiliar[tarea1] /\ auxiliar[tarea2])  )  ));

%%La versión sin auxiliar es la misma pero sin tratar nada de lo que aquí se trata con auxiliar
%El tiempo total empleado en hacer la casa será igual al día en el que se finalice la tarea que más tarde se acabó
constraint tiempo_total == max(fin);

solve minimize tiempo_total;

%%Con todos los datos que se muestran, se hace el diagrama de Gantt a partir de un DataFrame de Pandas en Python