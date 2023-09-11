(define (domain dominio3)
    (:requirements :strips :typing :negative-preconditions :adl :equality)

    (:types
        Localizacion
        Edificio Unidad Recurso - Entidad
        Estado ; Estado que puede tener una unidad (acción que está ejerciendo en cierto momento)
    )

    (:constants
        VCE - Unidad
        CentroDeMando Barracones Extractor - Edificio
        Mineral Gas - Recurso
        Extrayendo Ocioso - Estado
    )
    ;; Se definen las constantes utilizadas en el dominio, como VCE (unidad), CentroDeMando, Barracones, Extractor (edificios),
    ;; Mineral y Gas (recursos), y Extrayendo y Ocioso (estados de una unidad).

    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ; Relaciona un objeto con el "tipo" (esto es, alguna de las constantes) al que pertenece
        (estaEn ?x - Entidad ?loc - Localizacion) ; Indica que cierto objeto está en una localización concreta
        (conectado ?a - Localizacion ?b - Localizacion) ; Indica que hay un enlace directo desde la localización a) hasta la localización b)
        (construido ?x - Edificio) ; Indica que un objeto de tipo edificio ha sido construido. Principalmente para evitar que un mismo objeto sea construido múltiples veces
        (estado ?x - Unidad ?est - Estado) ; Indica qué está haciendo una unidad. En principio, una unidad solo puede estar "ociosa" o "extrayendo". Pensado para los VCEs
        (extrayendo ?x - Unidad ?rec - Recurso) ; Indica que una unidad concreta está extrayendo un recurso
        (siendoExtraido ?rec - Recurso) ; Indica que un recurso está siendo extraído
        (requiere ?e - Edificio ?rec - Recurso) ; Indica que un tipo de edificio requiere cierto recurso para poder ser construido
    )
    ;; Se definen los predicados utilizados en el dominio, como tipo, estaEn, conectado, construido, estado, extrayendo, siendoExtraido y requiere.

    (:action NAVEGAR
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        ;; Acción para que una unidad navegue desde una ubicación de origen a una ubicación de destino.

        :precondition
            (and
                (estaEn ?u ?ori)
                (conectado ?ori ?dest)
            )
        ;; Precondición: La unidad debe estar en la ubicación de origen y debe haber una conexión directa entre la ubicación de origen y la de destino.

        :effect
            (and
                (estaEn ?u ?dest)
                (not (estaEn ?u ?ori))
            )
        ;; Efecto: La unidad se encuentra en la ubicación de destino y ya no está en la ubicación de origen.
    )

    (:action ASIGNAR
        :parameters (?u - Unidad ?loc - Localizacion ?rec - Recurso)
        ;; Acción para asignar una unidad a una ubicación y recurso específicos para comenzar a extraer.

        :precondition
            (and 
            (tipo ?u VCE)  ;; La unidad debe ser de tipo VCE
            (estaEn ?rec ?loc)  ;; El recurso debe estar en la localización indicada
            (estaEn ?u ?loc)  ;; La unidad debe estar en la misma localización
            (estado ?u Ocioso)  ;; El estado de la unidad debe ser "Ocioso"
            (imply (= ?rec Gas)  ;; Si el recurso es gas
                (exists (?e - Edificio)  ;; Debe existir un extractor
                    (and  ;; que esté construido en esa localización
                        (construido ?e)
                        (estaEn ?e ?loc)
                        (tipo ?e Extractor)
                    )
                )
            )     
        )

        :effect
            (and 
            (extrayendo ?u ?rec)  ;; La unidad comienza a extraer el recurso indicado
            (siendoExtraido ?rec)  ;; El recurso comienza a ser extraído
            (estado ?u Extrayendo)  ;; El estado de la unidad cambia a "Extrayendo"
            (not (estado ?u Ocioso))  ;; El estado "Ocioso" deja de ser válido para la unidad
            )
    )

    (:action CONSTRUIR
        :parameters (?u - Unidad ?e - Edificio ?loc - Localizacion)

        :precondition
            (and 
                (estaEn ?u ?loc)  ;; La unidad debe estar en la localización indicada
            (tipo ?u VCE)  ;; La unidad debe ser de tipo VCE
            (estado ?u Ocioso)  ;; El estado de la unidad debe ser "Ocioso"
            (not (construido ?e))  ;; El edificio no debe estar construido aún
                (exists (?tipo - Edificio)  ;; Se busca el tipo de edificio al que pertenece el objeto que se quiere construir
                    (and 
                        (tipo ?e ?tipo)         ;Si todos los recursos que se necesitan están siendo extraídos, se puede construir
                        (forall (?rec - Recurso)
                            (imply (requiere ?tipo ?rec)
                                (siendoExtraido ?rec)
                            )
                        )
                    )
                )
            )
        

        :effect
            (and 
                (construido ?e)
                (estaEn ?e ?loc)
            )
        ;; Efecto: El edificio se marca como construido y se encuentra en la ubicación especificada.
    )
)
