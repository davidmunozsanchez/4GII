include "globals.mzn";
%%%%Datos necesarios para las ejecuciones que se piden.
%%%%Comentar y descomentar 1 de las m's. En caso de dejar 2 o más el
%%%%el programa fallará
%int: m = 3;      % número de marcas
%int: m = 4;  
%int: m = 5;      % número de marcas
%int: m = 6;  
%int: m = 7;      % número de marcas
%int: m = 8;  
%int: m = 9;  
int: m = 10;      % número de marcas
%int: m = 11;  
%int: m = 12;      % número de marcas

array[1..m] of var int: marcas;   % posiciones de las marcas

%Matriz de distancias para comprobar la correcta ejecución del programa
array[1..m,1..m] of var int: distancias = array2d(1..m, 1..m, [abs(marcas[i] - marcas[j]) | i, j in 1..m]) ;

%array[1..m-1] of var int: distancias;
constraint marcas[1] = 0;   % primera marca en 0
constraint forall(i in 2..m)(marcas[i] > marcas[i-1]);  % marcas en orden creciente

% construir matriz de distancias
% Exigimos que todas las combinaciones posibles de pares sean diferentes
% la condición i<j es suficiente para satisfacer esta restricción, porque si comprueba, por ejemplo,
% la marca 1 y 5, después no comparará la 5 y 1. Además, usando all_different, si exigiéramos i = j en vez de i < j
% ningún problema sería satisfacible por el hecho de que la 1 y 5 y la 5 y 1 tendrían que ser diferentes
constraint all_different([abs(marcas[i] - marcas[j]) | i, j in 1..m where i < j]);


% Solución, se busca el mínimo en la última marca que cumpla las restricciones
solve minimize marcas[m];

% salida
%Distancias entre cada par de marcas y las marcas
output ["Distancias:\n"] ++
  [show([distancias[i,j] | j in  1..m]) ++ "\n" | i in 1..m] ++
  ["marcas = \(marcas)"];
