(define (problem problema3)
    (:domain dominio3) ; Se especifica el dominio al que pertenece el problema.

    (:objects
        LOC11 LOC12 LOC13 LOC14 LOC15 LOC21 LOC22 LOC23 LOC24 LOC31 LOC32 LOC33 LOC34 LOC42 LOC43 LOC44 - Localizacion
        CentroDeMando1 Extractor1 Barracones1 - Edificio
        VCE1 VCE2 VCE3 - Unidad
    )
    ;; Se definen los objetos utilizados en el problema, como las localizaciones (LOC11, LOC12, etc.),
    ;; los edificios (CentroDeMando1, Extractor1, Barracones1) y las unidades (VCE1, VCE2, VCE3).

    (:init
        ; Defino el Grid (conexiones entre las localizaciones)
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
        (tipo CentroDeMando1 CentroDeMando)
        (construido centroDeMando)
        (estaEn CentroDeMando1 LOC11)

        (tipo Extractor1 Extractor)

        (tipo Barracones1 Barracones)

        ; Defino las unidades
        (tipo VCE1 VCE)
        (estado VCE1 Ocioso)
        (estaEn VCE1 LOC11)

        (tipo VCE2 VCE)
        (estado VCE2 Ocioso)
        (estaEn VCE2 LOC11)

        (tipo VCE3 VCE)
        (estado VCE3 Ocioso)
        (estaEn VCE3 LOC11)

        ; Defino las zonas de extracción de recursos
        (estaEn Mineral LOC24)
        (estaEn Mineral LOC44)
        (estaEn Gas LOC15)

        ; Establezco los requerimientos, independientes de el número de Edificios que haya
        (requiere Extractor Mineral)
        (requiere Barracones Mineral)
        (requiere Barracones Gas)
    )
    ;; Se inicializan los predicados y constantes que definen el estado inicial del problema.
    ;; Por ejemplo, se establecen las conexiones entre localizaciones, se definen los edificios y unidades
    ;; con sus respectivas propiedades y ubicaciones, y se establecen las ubicaciones de los recursos.

    (:goal
        (and
            (exists (?e - Edificio)
                (and
                    (construido ?e)
                    (tipo ?e Barracones)
                    (estaEn ?e LOC34)
                )
            )
        )
    )
    ;; Se define el objetivo del problema. En este caso, el objetivo es que exista un edificio
    ;; de tipo Barracones construido y ubicado en LOC34.

)
