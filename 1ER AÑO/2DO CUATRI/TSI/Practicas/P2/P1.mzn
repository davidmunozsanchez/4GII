
%%Todos los datos necesrios para las distintas tablas y ejecuciones
%int: n = 3;              % número de elementos en S
%int: n = 6;              % número de elementos en S
%int: n = 9;              % número de elementos en S
int: n = 20;              % número de elementos en S
set of int: S = 1..n; % índices de los elementos en S
%array[S] of int: nums = [1,1,2]; % valores de los elementos en S
%array[S] of int: nums = [1,2,3,4,5,6]; % valores de los elementos en S
%array[S] of int: nums = [1,1,2,4,4,5,6,7,8]; % valores de los elementos en S
%array[S] of int: nums = [99,14,82,47,82,50,77,45,23,92,52,90,46,57,29,25,74,25,30,70]; % valores de los elementos en S
array[S] of int: nums = [35,82,97,30,38,78,18,39,35,93,49,47,30,57,43,21,92,51,89,80];
%array[S] of int: nums = [60,8,11,2,49,69,87,15,62,72,89,14,76,63,45,92,74,80,11,2];

% Variables de decisión
array[S] of var 0..1: subset1; % indicador de si el elemento i pertenece a S1 (1) o no (0) PERTENECERIA A S2

%La propia codificación del problema ya contempla que la unión de ambos conjuntos sea el conjunto completo y que su intersección sea disjunta

% Restricción: la suma de los elementos en S1 es igual a la suma de los elementos en S2
%%Esto es para la primera versión del algoritmo
%constraint sum([nums[i] | i in S where subset1[i] = 1]) = sum([nums[j] | j in S where subset1[j] = 0]);

% Función objetivo: minimizar la diferencia absoluta entre la suma de S1 y S2
% Esto es para la última versión, la que finalmente se entrega
var int: sum1 = sum([nums[i] | i in S where subset1[i] = 1]);
var int: sum2 = sum([nums[i] | i in S where subset1[i] = 0]);
var int: dif = abs(sum1-sum2);

% Resolver el problema para la última versión
solve minimize dif;

% Resolver el problema para la primera versión
%solve satisfy;

% Imprimir solución
output["PRIMER SUBCONJUNTO: " ++ show([nums[i] | i in  S where subset1[i]=1])] ++ ["\nSEGUNDO SUBCONJUNTO: " ++ show([nums[i] | i in  S where subset1[i]=0])] ++ ["\nSUMA 1: " ++ show(sum1)] ++ ["\nSUMA 2: " ++ show(sum2)] ++ ["\nDIF: " ++ show(dif)];




