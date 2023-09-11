(define (domain dominio1)
    (:requirements :strips :typing :negative-preconditions)
    (:types
        Localizacion ; Tipo que representa una ubicación en el dominio
        Edificio Unidad Recurso - Entidad ; Tipos de entidades en el dominio
        Estado ; Estado que puede tener una unidad (acción que está ejerciendo en cierto momento)
    )
    (:constants
        VCE - Unidad ; Constante que representa una unidad específica
        CentroDeMando Barracones - Edificio ; Constantes que representan edificios específicos
        Mineral Gas - Recurso ; Constantes que representan recursos específicos
        Extrayendo Ocioso - Estado ; Constantes que representan estados específicos
    )
    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ; Relaciona un objeto con el "tipo" (esto es, alguna de las constantes) al que pertenece
        (estaEn ?x - Entidad ?loc - Localizacion) ; Indica que cierto objeto está en una localización concreta
        (conectado ?a - Localizacion ?b - Localizacion) ; Indica que hay un enlace directo desde la localización a) hasta la localización b)
        (construido ?x - Edificio) ; Indica que un objeto de tipo edificio ha sido construido. Principalmente para evitar que un mismo objeto sea construido múltiples veces
        (estado ?x - Unidad ?est - Estado) ; Indica qué está haciendo una unidad. En principio, una unidad solo puede estar "ociosa" o "extrayendo". Pensado para los VCEs
        (extrayendo ?x - Unidad ?rec - Recurso) ; Indica que una unidad concreta está extrayendo un recurso
    )

    (:action NAVEGAR
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition
            (and
                (estaEn ?u ?ori) ; La unidad está en la ubicación de origen
                (conectado ?ori ?dest) ; Hay una conexión directa desde la ubicación de origen hasta la ubicación de destino
                (estado ?u Ocioso) ; La unidad está en estado ocioso
            )
        :effect
            (and
                (estaEn ?u ?dest) ; La unidad se encuentra en la ubicación de destino
                (not (estaEn ?u ?ori)) ; La unidad ya no está en la ubicación de origen
            )
    )

    (:action ASIGNAR
        :parameters (?u - Unidad ?x - Localizacion ?rec - Recurso)
        :precondition
            (and
                (tipo ?u VCE) ; La unidad es de tipo VCE
                (estaEn ?rec ?x) ; El recurso se encuentra en la ubicación especificada
                (estaEn ?u ?x) ; La unidad se encuentra en la ubicación especificada
                (estado ?u Ocioso) ; La unidad está en estado ocioso
            )
        :effect
            (and
                (not (estado ?u Ocioso)) ; El estado de la unidad deja de ser ocioso
                (estado ?u extrayendo) ; El estado de la unidad se establece como extrayendo
                (extrayendo ?u ?rec) ; La unidad se establece como extrayendo el recurso especificado
            )
    )
)
