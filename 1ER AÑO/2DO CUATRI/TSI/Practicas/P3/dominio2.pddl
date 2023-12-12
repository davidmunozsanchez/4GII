(define (domain dominio2)
    (:requirements :strips :typing :negative-preconditions :adl :equality)

    (:types
        Localizacion  ;; Tipo de objeto: Localizacion
        Edificio Unidad Recurso - Entidad  ;; Subtipos de Entidad: Edificio, Unidad y Recurso
        Estado ;Estado que puede tener una unidad (acción que está ejerciendo en cierto momento)
    )

    (:constants
        VCE - Unidad  ;; Constante: VCE (unidad)
        CentroDeMando Barracones Extractor - Edificio  ;; Constantes: CentroDeMando, Barracones, Extractor (edificios)
        Mineral Gas - Recurso  ;; Constantes: Mineral, Gas (recursos)
        Extrayendo Ocioso - Estado ;; Constantes: Extrayendo, Ocioso (estados)
    )

    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ;Relaciona un objeto con el "tipo" (esto es, alguna de las constantes) al que pertenece
        (estaEn ?x - Entidad ?loc - Localizacion ) ;Indica que cierto objeto está en una localización concreta
        (conectado ?a - Localizacion ?b - Localizacion) ;Indica que hay un enlace directo desde la localización a) hasta la localización b)
        (construido ?x - Edificio) ;Indica que un objeto de tipo edificio ha sido construido. Principalmente para evitar que un mismo objeto sea construido múltiples veces
        (estado ?x - Unidad ?est - Estado) ;Indica qué está haciendo una unidad. En principio, una unidad solo puede estar "ociosa" o "extrayendo". Pensado para los VCEs
        (extrayendo ?x - Unidad ?rec - Recurso) ;Indica que una unidad concreta está extrayendo un recurso
        (siendoExtraido ?rec - Recurso) ;Indica que un recurso está siendo extraído
        (requiere ?e - Edificio ?rec - Recurso) ;Indica que un tipo de edificio requiere cierto recurso para poder ser construido
    )

    (:action NAVEGAR
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)

        :precondition
            (and
                (estaEn ?u ?ori)  ;; La unidad debe estar en la localización de origen
                (conectado ?ori ?dest)  ;; Las localizaciones de origen y destino deben estar conectadas
            )

        :effect
            (and
                (estaEn ?u ?dest)  ;; La unidad pasa a estar en la localización de destino
                (not (estaEn ?u ?ori))  ;; La unidad ya no está en la localización de origen
            )
    )
    ;; Se define la acción "NAVEGAR" que permite que una unidad se mueva de una localización a otra

    (:action ASIGNAR
        :parameters (?u - Unidad ?loc - Localizacion ?rec - Recurso)

        :precondition (and 
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

        :effect (and 
            (extrayendo ?u ?rec)  ;; La unidad comienza a extraer el recurso indicado
            (siendoExtraido ?rec)  ;; El recurso comienza a ser extraído
            (estado ?u Extrayendo)  ;; El estado de la unidad cambia a "Extrayendo"
            (not (estado ?u Ocioso))  ;; El estado "Ocioso" deja de ser válido para la unidad

        )
    )
    ;; Se define la acción "ASIGNAR" que permite asignar una unidad a la extracción de un recurso

    (:action CONSTRUIR
        :parameters (?u - Unidad ?e - Edificio ?loc - Localizacion ?rec - Recurso)

        :precondition (and 
            (estaEn ?u ?loc)  ;; La unidad debe estar en la localización indicada
            (tipo ?u VCE)  ;; La unidad debe ser de tipo VCE
            (estado ?u Ocioso)  ;; El estado de la unidad debe ser "Ocioso"
            (not (construido ?e))  ;; El edificio no debe estar construido aún
            (siendoExtraido ?rec)  ;; El recurso debe estar siendo extraído
            (exists (?tipo - Edificio)  ;; Se busca el tipo de edificio al que pertenece el objeto que se quiere construir
                (and  ;; Si el recurso proporcionado es el requerido por ese tipo de edificio, se puede construir
                    (tipo ?e ?tipo)
                    (requiere ?tipo ?rec)
                )
            )
        )

        :effect (and 
            (construido ?e)  ;; El edificio se marca como construido
            (estaEn ?e ?loc)  ;; El edificio se encuentra en la localización indicada
        )
    )
    ;; Se define la acción "CONSTRUIR" que permite construir un edificio en una localización específica
)
