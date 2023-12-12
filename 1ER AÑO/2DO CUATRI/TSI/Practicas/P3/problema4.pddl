(define (problem problema4)
    (:domain dominio4) ; Especifica el dominio al que pertenece el problema

    (:objects
        LOC11 LOC12 LOC13 LOC14 LOC15 LOC21 LOC22 LOC23 LOC24 LOC31 LOC32 LOC33 LOC34 LOC42 LOC43 LOC44 - Localizacion
        CentroDeMando1 Extractor1 Barracones1 - Edificio ; Definición de edificios
        VCE1 VCE2 VCE3 Marine1 Marine2 Soldado1 - Unidad ; Definición de unidades
    )

    (:init
        ; Definición del Grid y conexiones entre las localizaciones
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

        ; Definición de los edificios
        (tipo CentroDeMando1 CentroDeMando) ; El Centro de Mando 1 es de tipo CentroDeMando
        (construido CentroDeMando1) ; El Centro de Mando 1 ha sido construido
        (estaEn CentroDeMando1 LOC11) ; El Centro de Mando 1 se encuentra en la localización LOC11
        (tipo Extractor1 Extractor) ; El Extractor 1 es de tipo Extractor
        (tipo Barracones1 Barracones) ; Los Barracones 1 son de tipo Barracones

        ; Definición de las unidades
        (tipo VCE1 VCE) ; La unidad VCE1 es de tipo VCE
        (estado VCE1 Ocioso) ; El estado de VCE1 es Ocioso
        (estaEn VCE1 LOC11) ; VCE1 se encuentra en la localización LOC11
        (tipo VCE2 VCE) ; La unidad VCE2 es de tipo VCE
        (tipo VCE3 VCE) ; La unidad VCE3 es de tipo VCE
        (tipo Marine1 Marine) ; La unidad Marine1 es de tipo Marine
        (tipo Marine2 Marine) ; La unidad Marine2 es de tipo Marine
        (tipo Soldado1 Soldado) ; La unidad Soldado1 es de tipo Soldado

        ; Definición de las zonas de extracción de recursos
        (estaEn Mineral LOC22) ; La zona de extracción Mineral se encuentra en LOC22
        (estaEn Mineral LOC44) ; La zona de extracción Mineral se encuentra en LOC44
        (estaEn Gas LOC15) ; La zona de extracción Gas se encuentra en LOC15

        ; Establecimiento de los requerimientos
        (requiere Extractor Mineral) ; El Extractor requiere recursos de tipo Mineral
        (requiere Barracones Mineral) ; Los Barracones requieren recursos de tipo Mineral
        (requiere Barracones Gas) ; Los Barracones requieren recursos de tipo Gas
        (requiere VCE Mineral) ; Las unidades VCE requieren recursos de tipo Mineral
        (requiere Marine Mineral) ; Las unidades Marine requieren recursos de tipo Mineral
        (requiere Soldado Mineral) ; Las unidades Soldado requieren recursos de tipo Mineral
        (requiere Soldado Gas) ; Las unidades Soldado requieren recursos de tipo Gas

        ; Establecimiento del edificio que entrena cada unidad
        (entrena CentroDeMando VCE) ; El Centro de Mando entrena unidades VCE
        (entrena Barracones Soldado) ; Los Barracones entrenan unidades Soldado
        (entrena Barracones Marine) ; Los Barracones entrenan unidades Marine
    )

    (:goal
        (and
            (estaEn Marine1 LOC31) ; La unidad Marine1 se encuentra en LOC31
            (estaEn Marine2 LOC24) ; La unidad Marine2 se encuentra en LOC24
            (estaEn Soldado1 LOC12) ; La unidad Soldado1 se encuentra en LOC12
            (estaEn Barracones1 LOC33) ; Los Barracones1 se encuentran en LOC33
        )
    )
)

