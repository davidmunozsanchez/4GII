"""
Para ejecutarlo:

$ ./Metric-FF/ff -o dominio8.pddl -f problema8.pddl -O -g 1 -h 1 | python3 getCostFromPlanEj8.py

o 

$ ./Metric-FF/ff -o dominio8.pddl -f problema8.pddl -O -g 1 -h 1 > solucion
$ python3 getCostFromPlanEj8.py solucion
"""

import sys
import fileinput

cost = 0
printPlan = False
printPlanAndCost = False

Lines = []

# Leer de fichero (ej: solucion.txt)
if len(sys.argv)>1:
    plan = open(sys.argv[1], 'r')
    Lines = plan.readlines()
# Leer de stdin
else:
    Lines = fileinput.input()


processLine = False
for line in Lines:
    
    # Procesar líneas entre "step" y "time spent"
    if line.startswith("step"):
        processLine = True
    elif line.startswith("time spent"):
        processLine = False
    
    # Procesar líneas que definen una acción: contienen el caracter ":" (ignorar líneas vacías)
    if ":" in line and processLine:
        
        # La acción se define a partir del caracter 11 de la línea
        actionStr = line[11:]
        if printPlan:
            print(actionStr, end='')
        
        # Dividimos los distintos argumentos de la acción
        actionArr = actionStr.split(" ")
        
        # COSTES:
        # for i in range(len(actionArr)):
        #     print(actionArr[i])
        
        # Navegar: Coste = Distancia / Velocidad
        #    - Todas las casillas a distancia 10 salvo:
            # LOC22-LOC23 distancia 20
            # LOC42-LOC43 distancia 20
            # LOC44-LOC15 distancia 40
        #    - velocidad(VCE)=1, vel(MARINE)=5, vel(SOLDADO=10)
        if (actionArr[0] == "NAVEGAR_1" or actionArr[0] == "NAVEGAR_2" or actionArr[0] == "NAVEGAR_3" or actionArr[0] == "NAVEGAR_4") and actionArr[1].startswith("VCE"):   
            if ((actionArr[2].startswith("LOC22") and actionArr[3].startswith("LOC23")) or (actionArr[3].startswith("LOC22") and actionArr[2].startswith("LOC23"))):
                cost+=20
            elif ((actionArr[2].startswith("LOC42") and actionArr[3].startswith("LOC43")) or (actionArr[3].startswith("LOC42") and actionArr[2].startswith("LOC43"))):
                cost+=20
            elif ((actionArr[2].startswith("LOC44") and actionArr[3].startswith("LOC15")) or (actionArr[3].startswith("LOC44") and actionArr[2].startswith("LOC15"))):
                cost+=40
            else:
                cost += 10
        elif (actionArr[0] == "NAVEGAR_1" or actionArr[0] == "NAVEGAR_2" or actionArr[0] == "NAVEGAR_3" or actionArr[0] == "NAVEGAR_4")  and actionArr[1].startswith("MARINE"):
            if ((actionArr[2].startswith("LOC22") and actionArr[3].startswith("LOC23")) or (actionArr[3].startswith("LOC22") and actionArr[2].startswith("LOC23"))):
                cost+=4
            elif ((actionArr[2].startswith("LOC42") and actionArr[3].startswith("LOC43")) or (actionArr[3].startswith("LOC42") and actionArr[2].startswith("LOC43"))):
                cost+=4
            elif ((actionArr[2].startswith("LOC44") and actionArr[3].startswith("LOC15")) or (actionArr[3].startswith("LOC44") and actionArr[2].startswith("LOC15"))):
                cost+=8
            else:
                cost += 2
        elif (actionArr[0] == "NAVEGAR_1" or actionArr[0] == "NAVEGAR_2" or actionArr[0] == "NAVEGAR_3" or actionArr[0] == "NAVEGAR_4")  and actionArr[1].startswith("SOLDADO"):
            if ((actionArr[2].startswith("LOC22") and actionArr[3].startswith("LOC23")) or (actionArr[3].startswith("LOC22") and actionArr[2].startswith("LOC23"))):
                cost+=2
            elif ((actionArr[2].startswith("LOC42") and actionArr[3].startswith("LOC43")) or (actionArr[3].startswith("LOC42") and actionArr[2].startswith("LOC43"))):
                cost+=2
            elif ((actionArr[2].startswith("LOC44") and actionArr[3].startswith("LOC15")) or (actionArr[3].startswith("LOC44") and actionArr[2].startswith("LOC15"))):
                cost+=4
            else:
                cost += 1
            
        # Asignar: No tiene coste    
        elif actionArr[0] == "ASIGNAR":
            cost += 0
            
        # ConstruirEdificio: Coste = tiempo de construcción
        #    - tiempo(Extractor)=20, tiempo(Barracones)=50
        elif actionArr[0]=="CONSTRUIR" and actionArr[2].startswith("EXTRACTOR"):
            cost += 20
        elif actionArr[0]=="CONSTRUIR" and actionArr[2].startswith("BARRACON"):
            cost += 50
            
        # Reclutar:  Coste = tiempo de reclutar
        #    - tiempo(VCE)=10, tiempo(MARINE)=20, tiempo(SOLDADO)=30
        elif actionArr[0]=="RECLUTAR" and actionArr[2].startswith("VCE"):
            cost += 10
        elif actionArr[0]=="RECLUTAR" and actionArr[2].startswith("MARINE"):
            cost += 20
        elif actionArr[0]=="RECLUTAR" and actionArr[2].startswith("SOLDADO"):
            cost += 30
            
        # Recolectar: el coste depende de lo que se recolecte. 
        elif actionArr[0]=="RECOLECTAR":
            if actionArr[1]=="MINERAL":
                cost += 10
            elif actionArr[1]=="GAS":
                cost += 5
            else:
                print('Errorr')
                
        if printPlanAndCost:
            print(actionArr, cost) 
            
print("TOTAL COST = ", cost)