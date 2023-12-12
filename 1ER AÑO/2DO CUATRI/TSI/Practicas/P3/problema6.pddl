(define (problem problema6)
    (:domain dominio6)
    (:objects
        LOC11 LOC12 LOC13 LOC14 LOC15 LOC21 LOC22 LOC23 LOC24 LOC31 LOC32 LOC33 LOC34 LOC42 LOC43 LOC44 - Localizacion
        CentroDeMando1 Extractor1 Barracones1 Laboratorio1 - Edificio
        VCE1 VCE2 VCE3 Marine1 Marine2 Soldado1 - Unidad
    )
    (:init

        ; Defino el Grid
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

        ; Defino los edificios
        (tipo CentroDeMando1 CentroDeMando) ; Tipo del edificio CentroDeMando1 es CentroDeMando
        (construido CentroDeMando1) ; El edificio CentroDeMando1 ha sido construido
        (estaEn CentroDeMando1 LOC11) ; El edificio CentroDeMando1 se encuentra en LOC11

        (tipo Extractor1 Extractor) ; Tipo del edificio Extractor1 es Extractor

        (tipo Barracones1 Barracones) ; Tipo del edificio Barracones1 es Barracones

        (tipo Laboratorio1 Laboratorio) ; Tipo del edificio Laboratorio es Laboratorio

        ; Defino las unidades
        (tipo VCE1 VCE) ; Tipo de la unidad VCE1 es VCE
        (estado VCE1 Ocioso) ; El estado de la unidad VCE1 es Ocioso
        (estaEn VCE1 LOC11) ; La unidad VCE1 se encuentra en LOC11

        (tipo VCE2 VCE) ; Tipo de la unidad VCE2 es VCE

        (tipo VCE3 VCE) ; Tipo de la unidad VCE3 es VCE

        (tipo Marine1 Marine) ; Tipo de la unidad Marine1 es Marine

        (tipo Marine2 Marine) ; Tipo de la unidad Marine2 es Marine

        (tipo Soldado1 Soldado) ; Tipo de la unidad Soldado1 es Soldado

        ; Defino las zonas de extracción de recursos
        (estaEn Mineral LOC22) ; Hay un recurso Mineral en LOC22
        (estaEn Mineral LOC44) ; Hay un recurso Mineral en LOC44
        (estaEn Gas LOC15) ; Hay un recurso Gas en LOC15
        (estaEn Especia LOC13) ; Hay un recurso Especia en LOC13

        ; Establezco los requerimientos
        (requiere Extractor Mineral) ; El Extractor requiere el recurso Mineral
        (requiere Barracones Mineral) ; Los Barracones requieren el recurso Mineral
        (requiere Barracones Gas) ; Los Barracones requieren el recurso Gas
        (requiere Laboratorio Mineral) ; El Laboratorio requiere el recurso Mineral
        (requiere Laboratorio Gas) ; El Laboratorio requiere el recurso Gas
        (requiere VCE Mineral) ; La unidad VCE requiere el recurso Mineral
        (requiere Marine Mineral) ; La unidad Marine requiere el recurso Mineral
        (requiere Soldado Mineral) ; La unidad Soldado requiere el recurso Mineral
        (requiere Soldado Gas) ; La unidad Soldado requiere el recurso Gas
        (requiere Soldado Investigar_Spartan) ; La unidad Soldado requiere Investigar_Spartan
        (requiere Investigar_Spartan Mineral) ; Investigar_Spartan requiere el recurso Mineral
        (requiere Investigar_Spartan Gas) ; Investigar_Spartan requiere el recurso Gas
        (requiere Investigar_Spartan Especia) ; Investigar_Spartan requiere el recurso Especia

        ; Establezco el edificio que entrena cada unidad
        (entrena CentroDeMando VCE) ; El CentroDeMando entrena la unidad VCE
        (entrena Barracones Soldado) ; Los Barracones entrenan la unidad Soldado
        (entrena Barracones Marine) ; Los Barracones entrenan la unidad Marine

        (= (CostePlan) 0)

    )

    (:goal
        (and
            (estaEn Marine1 LOC14) ; Quiero que la unidad Marine1 se encuentre en LOC14
            (estaEn Marine2 LOC14) ; Quiero que la unidad Marine2 se encuentre en LOC14
            (estaEn Soldado1 LOC14) ; Quiero que la unidad Soldado1 se encuentre en LOC14
            (estaEn Laboratorio1 LOC12) ; Quiero que el edificio Laboratorio se encuentre en LOC12
            (estaEn Barracones1 LOC14) ; Quiero que el edificio Barracones1 se encuentre en LOC14
            ;(< (CostePlan) 28) ;El plan óptimo es de 28 pasos. Con esta restricción el problema es irresoluble

        )
    )

    ;;Indicamos que se quiere minimizar la métrica asociada al Coste del Plan
    (:metric minimize (CostePlan)
        )
)
