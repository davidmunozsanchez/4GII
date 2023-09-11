(define (domain dominio4)
    (:requirements :strips :typing :negative-preconditions :adl :equality)
    (:types
        Localizacion
        Creable Recurso - Entidad
        Edificio Unidad - Creable ; Las unidades y los edificios son "creables", mientras que los recursos son explotables
        Estado ; Estado actual de una unidad (acción que está llevando a cabo)
    )
    (:constants
        VCE Marine Soldado - Unidad ; Constantes que representan tipos específicos de unidades
        CentroDeMando Barracones Extractor - Edificio ; Constantes que representan tipos específicos de edificios
        Mineral Gas - Recurso ; Constantes que representan tipos específicos de recursos
        Extrayendo Ocioso - Estado ; Constantes que representan estados específicos de las unidades
    )
    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ; Relaciona un objeto con su tipo (una de las constantes)
        (estaEn ?x - Entidad ?loc - Localizacion) ; Indica que un objeto se encuentra en una ubicación específica
        (conectado ?a - Localizacion ?b - Localizacion) ; Indica que hay una conexión directa desde la ubicación A hasta la ubicación B
        (construido ?x - Edificio) ; Indica si un edificio ha sido construido
        (estado ?x - Unidad ?est - Estado) ; Indica el estado actual de una unidad (Ocioso o Extrayendo)
        (extrayendo ?x - Unidad ?rec - Recurso) ; Indica que una unidad está extrayendo un recurso específico
        (siendoExtraido ?rec - Recurso) ; Indica que un recurso está siendo extraído
        (requiere ?e - Creable ?rec - Recurso) ; Indica que un tipo de objeto creable requiere un recurso específico para su construcción
        (entrena ?e - Edificio ?u - Unidad) ; Relaciona un edificio con la unidad que puede entrenar
    )
    (:action NAVEGAR
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition
            (and
                (estado ?u Ocioso) ; La unidad debe estar en estado "Ocioso"
                (estaEn ?u ?ori) ; La unidad debe estar en la ubicación de origen
                (conectado ?ori ?dest) ; Las ubicaciones de origen y destino deben estar conectadas
            )
        :effect
            (and
                (estaEn ?u ?dest) ; La unidad se mueve a la ubicación de destino
                (not (estaEn ?u ?ori)) ; La unidad ya no se encuentra en la ubicación de origen
            )
    )
    (:action ASIGNAR
        :parameters (?u - Unidad ?loc - Localizacion ?rec - Recurso)
        :precondition (and
            (tipo ?u VCE)
            (estaEn ?rec ?loc)
            (estaEn ?u ?loc)
            (estado ?u Ocioso)
            (imply
                (= ?rec Gas) ;En el caso de que el recurso sea gas:
                (exists
                    (?e - Edificio) ;Es necesario que exista...
                    (and ;Un extractor que esté construido en ese nodo
                        (construido ?e)
                        (estaEn ?e ?loc)
                        (tipo ?e Extractor)
                    )
                )
            )
        )
        :effect (and 
            (extrayendo ?u ?rec) ; La unidad comienza a extraer el recurso especificado
            (siendoExtraido ?rec) ; El recurso está siendo extraído
            (estado ?u Extrayendo) ; El estado de la unidad cambia a "Extrayendo"
            (not (estado ?u Ocioso)) ; La unidad ya no está en estado "Ocioso"
        )
    )
    (:action CONSTRUIR
        :parameters (?u - Unidad ?e - Edificio ?loc - Localizacion)
        :precondition (and 
            (estaEn ?u ?loc) ; La unidad debe estar en la ubicación especificada
            (tipo ?u VCE) ; La unidad debe ser de tipo VCE
            (estado ?u Ocioso) ; La unidad debe estar en estado "Ocioso"
            (not (construido ?e)) ; El edificio no debe haber sido construido previamente
            (exists (?tipo - Edificio) ; Buscamos el tipo de edificio al que pertenece el edificio que queremos construir
                (and 
                    (tipo ?e ?tipo)
                    (forall (?rec - Recurso) ; Para todos los recursos...
                        (imply (requiere ?tipo ?rec) ; Si ese tipo de edificio los requiere,
                            (siendoExtraido ?rec) ; Comprobamos que se estén extrayendo
                        )
                    )
                )
            )
        )
        :effect (and 
            (construido ?e) ; El edificio se marca como construido
            (estaEn ?e ?loc) ; El edificio se encuentra en la ubicación especificada
        )
    )

    (:action RECLUTAR
        :parameters (?e - Edificio ?u - Unidad ?loc - Localizacion)
        :precondition (and 
            (construido ?e) ; El edificio debe haber sido construido previamente
            (estaEn ?e ?loc) ; El edificio debe estar en la ubicación especificada
            (not (exists (?estado - Estado) (estado ?u ?estado))) ; Si la unidad no tiene estado, significa que aún no ha sido reclutada
            (exists (?tipo_edificio - Edificio) ; Buscamos el tipo de edificio seleccionado
                (exists (?tipo_unidad - Unidad) ; Y el tipo de unidad que queremos reclutar
                    (and
                        (tipo ?e ?tipo_edificio)
                        (tipo ?u ?tipo_unidad)
                        (entrena ?tipo_edificio ?tipo_unidad) ; Ese tipo de edificio puede entrenar ese tipo de unidad
                        (forall (?rec - Recurso) ; Y estamos extrayendo todos los recursos necesarios
                            (imply (requiere ?tipo_unidad ?rec)
                                (siendoExtraido ?rec)
                            )
                        )                
                    )
                )
            )
        )
        :effect (and 
            (estado ?u Ocioso) ; Cuando reclutamos la unidad, la establecemos en estado "Ocioso", esperando órdenes
            (estaEn ?u ?loc) ; La unidad se encuentra en la ubicación especificada
        )
    )          
)

