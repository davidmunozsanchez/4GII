(define (problem problema2)
    (:domain dominio2)  ;; Se especifica el dominio al que pertenece el problema

    (:objects
        LOC11 LOC12 LOC13 LOC14 LOC15 LOC21 LOC22 LOC23 LOC24 LOC31 LOC32 LOC33 LOC34 LOC42 LOC43 LOC44 - Localizacion
        ;; Se definen los objetos de tipo Localizacion

        CentroDeMando1 Extractor1 - Edificio
        ;; Se definen los objetos de tipo Edificio: CentroDeMando1 y Extractor1

        VCE1 VCE2 - Unidad
        ;; Se definen los objetos de tipo Unidad: VCE1 y VCE2
    )

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
        (tipo CentroDeMando1 CentroDeMando) (construido centroDeMando) (estaEn CentroDeMando1 LOC11)
        ;; Se define el edificio CentroDeMando1 de tipo CentroDeMando, construido y ubicado en LOC11

        (tipo Extractor1 Extractor) (requiere Extractor Mineral)
        ;; Se define el edificio Extractor1 de tipo Extractor que requiere el recurso Mineral

        ; Defino las unidades
        (tipo VCE1 VCE) (estado VCE1 Ocioso) (estaEn VCE1 LOC11)
        ;; Se define la unidad VCE1 de tipo VCE, en estado Ocioso y ubicada en LOC11

        (tipo VCE2 VCE) (estado VCE2 Ocioso) (estaEn VCE2 LOC11)
        ;; Se define la unidad VCE2 de tipo VCE, en estado Ocioso y ubicada en LOC11

        ; Defino las zonas de extracción de recursos
        (estaEn Mineral LOC24)
        (estaEn Mineral LOC44)
        (estaEn Gas LOC15)
        ;; Se define la ubicación de los recursos: Mineral en LOC24 y LOC44, y Gas en LOC15
    )

    (:goal
        (and
            (siendoExtraido Gas)
        )
    )
    ;; Se define la meta del problema: Que el recurso Gas esté siendo extraído
)
