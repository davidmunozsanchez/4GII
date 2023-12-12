(define (problem problema1)
    (:domain dominio1)
    (:objects
        LOC11 LOC12 LOC13 LOC14 LOC15 LOC21 LOC22 LOC23 LOC24 LOC31 LOC32 LOC33 LOC34 LOC42 LOC43 LOC44 - Localizacion
        CentroDeMando1 - Edificio
        VCE1 - Unidad
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
        (tipo CentroDeMando1 CentroDeMando) (construido centroDeMando) (estaEn CentroDeMando1 LOC11)

        ;Defino las unidades
        (tipo VCE1 VCE) (estado VCE1 Ocioso) (estaEn VCE1 LOC11)

        ;Defino las zonas de extracción de recursos
        (estaEn Mineral LOC24)
        (estaEn Mineral LOC44)

    )
    
    (:goal
        (and
            (extrayendo VCE1 Mineral)
        )
    )
)