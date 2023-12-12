(define (problem problema8)
    (:domain dominio8)
    (:objects
        LOC11 LOC12 LOC13 LOC14 LOC15 LOC21 LOC22 LOC23 LOC24 LOC31 LOC32 LOC33 LOC34 LOC42 LOC43 LOC44 - Localizacion
        CentroDeMando1 Extractor1 Barracones1 - Edificio
        VCE1 VCE2 VCE3 Marine1 Marine2 Soldado1 - Unidad
    )
    (:init

        ;Defino el Grid
        (conectado LOC11 LOC12)
        (conectado LOC11 LOC21)

        (conectado LOC12 LOC11)
        (conectado LOC12 LOC22)

        (conectado LOC13 LOC23)

        (conectado LOC14 LOC15)
        (conectado LOC14 LOC24)

        (conectado LOC15 LOC14)
        (conectado LOC15 LOC44)

        (conectado LOC21 LOC11)
        (conectado LOC21 LOC31)

        (conectado LOC22 LOC12)
        (conectado LOC22 LOC32)
        (conectado LOC22 LOC23)

        (conectado LOC23 LOC22)
        (conectado LOC23 LOC13)
        (conectado LOC23 LOC33)
        (conectado LOC23 LOC24)

        (conectado LOC24 LOC14)
        (conectado LOC24 LOC23)

        (conectado LOC31 LOC21)
        (conectado LOC31 LOC32)

        (conectado LOC32 LOC31)
        (conectado LOC32 LOC22)
        (conectado LOC32 LOC42)

        (conectado LOC33 LOC23)

        (conectado LOC34 LOC44)

        (conectado LOC42 LOC32)
        (conectado LOC42 LOC43)

        (conectado LOC43 LOC42)
        (conectado LOC43 LOC44)

        (conectado LOC44 LOC43)
        (conectado LOC44 LOC34)
        (conectado LOC44 LOC15)

        ;Defino los edificios
        (tipo CentroDeMando1 CentroDeMando) (construido CentroDeMando1) (estaEn CentroDeMando1 LOC11)
        (tipo Extractor1 Extractor)
        (tipo Barracones1 Barracones)

        ;Defino las unidades
        (tipo VCE1 VCE) (estado VCE1 Ocioso) (estaEn VCE1 LOC11)
        (tipo VCE2 VCE)
        (tipo VCE3 VCE)
        (tipo Marine1 Marine)
        (tipo Marine2 Marine)
        (tipo Soldado1 Soldado)

        ;Defino las zonas de extracción de recursos
        (estaEn Mineral LOC22)  (= (cantidad_VCE Mineral LOC22) 0)
        (estaEn Mineral LOC44)  (= (cantidad_VCE Mineral LOC44) 0)
        (estaEn Gas LOC15)      (= (cantidad_VCE Gas LOC15) 0)

        

        ;Establezco el edificio que entrena cada unidad
        (entrena CentroDeMando VCE)
        (entrena Barracones Soldado)
        (entrena Barracones Marine)

        ;Establezco los coste de creación de los diferentes creables 
        (= (coste Barracones Mineral) 40)
        (= (coste Barracones Gas) 10)
        (= (coste Extractor Mineral) 10)
        (= (coste Extractor Gas) 0)
        (= (coste VCE Mineral) 5)
        (= (coste VCE Gas) 0)
        (= (coste Marine Mineral) 10)
        (= (coste Marine Gas) 15)
        (= (coste Soldado Mineral) 30)
        (= (coste Soldado Gas) 40)

        ;Inicializo los almacenes a 0
        (= (cantidad_almacenada Mineral) 0)
        (= (cantidad_almacenada Gas) 0)

        ;Inicializo el cost P3e del plan a 0
        (= (CostePlan) 0)
        (= (tiempoPlan) 0)

        ;Establezco los tiempos de ejecución de las diferentes creaciones 
        (= (tiempoCreacion Barracones) 50)
        (= (tiempoCreacion Extractor) 20)
        (= (tiempoCreacion VCE) 10)
        (= (tiempoCreacion Marine) 20)
        (= (tiempoCreacion Soldado) 30)

        ;Establezco los tiempos de ejecución de los diferentes desplazamientos 
        ;;Rutas de longitud 10
        (= (tiempoDesplazamiento1 VCE) 10)
        (= (tiempoDesplazamiento1 Marine) 2)
        (= (tiempoDesplazamiento1 Soldado) 1)

        ;Rutas de longitud 20
        (= (tiempoDesplazamiento2 VCE) 20)
        (= (tiempoDesplazamiento2 Marine) 4)
        (= (tiempoDesplazamiento2 Soldado) 2)

        ;Rutas de longitud 40
        (= (tiempoDesplazamiento3 VCE) 40)
        (= (tiempoDesplazamiento3 Marine) 8)
        (= (tiempoDesplazamiento3 Soldado) 4)

        (= (factorRecurso Mineral) 5) ;El mineral se extrae de 5 en 5
        (= (factorRecurso Gas) 10) ; El gas de 10 en 10

        (= (maximoRecurso Mineral) 50) ;Se puede almacenar como máximo 50 de mineral
        (= (maximoRecurso Gas) 60) ; Se puede almacenar como máximo 60 de gas

        (= (tiempoRecurso Mineral) 10) ;Se gasta 10 de tiempo por recolectar mineral
        (= (tiempoRecurso Gas) 5) ;5 en caso del gas

    
    )
    
    
    (:goal
        (and
            (estaEn Barracones1 LOC33)
            (estaEn Marine1 LOC31)
            (estaEn Marine2 LOC24)
            (estaEn Soldado1 LOC15)
            ;(< (tiempoPlan) 558) ;;;;Tarda más de 45 minutos con esta restricción, no se ha garantizado el óptimo

        )
    

    )
    (:metric minimize (tiempoPlan)
        )
)
