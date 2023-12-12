(define (domain dominio6)
    (:requirements :strips :typing :negative-preconditions :adl :equality)
    ;; Se especifican los requisitos del dominio, incluyendo las características que se utilizarán.

    (:types
        Localizacion Creable Requerimiento - Entidad
        Edificio Unidad Investigacion - Creable
        Recurso Investigacion - Requerimiento
        Estado
    )
    ;; Se definen los tipos utilizados en el dominio, como Localizacion, Creable, Requerimiento, Edificio, Unidad, Investigacion y Estado.

    (:constants
        VCE Marine Soldado - Unidad
        CentroDeMando Barracones Extractor Laboratorio - Edificio
        Investigar_Spartan - Investigacion
        Mineral Gas Especia - Recurso
        Extrayendo Ocioso - Estado
    )
    ;; Se definen las constantes utilizadas en el dominio, como las unidades (VCE, Marine, Soldado), los edificios (CentroDeMando, Barracones, Extractor, Laboratorio), las investigaciones (Investigar_Spartan), y los recursos (Mineral, Gas, Especia) y estados (Extrayendo, Ocioso).

    (:functions
        (CostePlan) ;Va a almacenar el número de pasos que requiere nuestro plan
    )

    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ; Relaciona un objeto con su tipo (una de las constantes)
    (conectado ?a - Localizacion ?b - Localizacion) ; Indica que hay una conexión directa desde la ubicación A hasta la ubicación B
    (estaEn ?x - Entidad ?loc - Localizacion) ; Indica que un objeto se encuentra en una ubicación específica
    (construido ?x - Edificio) ; Indica si un edificio ha sido construido
    (estado ?x - Unidad ?est - Estado) ; Indica el estado actual de una unidad (Ocioso o Extrayendo)
    (extrayendo ?x - Unidad ?rec - Recurso) ; Indica que una unidad está extrayendo un recurso específico
    (requiere ?e - Creable ?rec - Requerimiento) ; Indica que un tipo de objeto creable requiere un requerimiento específico para su construcción
    (entrena ?e - Edificio ?u - Unidad) ; Relaciona un edificio con la unidad que puede entrenar
    (adquirido ?req - Requerimiento) ;Se cambian un poco los predicados con respecto al ejercicio anterior
    ;;;;Como antes solo había que lidiar con requerimientos de tipo recurso para para las construcciones, había un predicado llamado siendoExtraido, en referencia
    ;;;;a los recursos, ahora es un predicado más genérico, que engloba tanto recursos, como investigaciones
    )
    ;; Se definen los predicados utilizados en el dominio. Por ejemplo, "tipo" relaciona un objeto con su tipo, "estaEn" indica que un objeto está en una localización determinada, "conectado" indica que hay una conexión directa entre dos localizaciones, "construido" indica que un edificio ha sido construido, "estado" indica el estado de una unidad (Ocioso o Extrayendo), "extrayendo" indica que una unidad está extrayendo un recurso, "requiere" indica que un objeto creable requiere cierto recurso, "entrena" relaciona un edificio con la unidad que entrena, y "adquirido" indica que se ha adquirido un requerimiento.

    (:action NAVEGAR
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        ;; Acción para navegar

        :precondition (and
            (estado ?u Ocioso) ; La unidad está ociosa
            (estaEn ?u ?ori) ; La unidad está en la ubicación de origen
            (conectado ?ori ?dest) ; Hay una conexión directa entre la ubicación de origen y destino
        )

        :effect (and
            (estaEn ?u ?dest) ; La unidad se encuentra en la ubicación de destino
            (not (estaEn ?u ?ori)) ; La unidad ya no está en la ubicación de origen
            (increase (CostePlan) 1)
        )
    )

    (:action ASIGNAR
        :parameters (?u - Unidad ?loc - Localizacion ?rec - Recurso)
        ;; Acción para asignar una unidad a la extracción de un recurso

        :precondition (and
            (tipo ?u VCE) ; La unidad es de tipo VCE
            (estaEn ?rec ?loc) ; El recurso está en la ubicación
            (estaEn ?u ?loc) ; La unidad está en la ubicación
            (estado ?u Ocioso) ; La unidad está ociosa
            (imply
                (= ?rec Gas) ; Si el recurso es Gas
                (exists
                    (?e - Edificio)
                    (and
                        (construido ?e) ; Hay un edificio construido
                        (estaEn ?e ?loc) ; El edificio está en la ubicación
                        (tipo ?e Extractor) ; El edificio es de tipo Extractor
                    )
                )
            )
        )

        :effect (and
            (extrayendo ?u ?rec) ; La unidad está extrayendo el recurso
            (adquirido ?rec) ; Se ha adquirido el requerimiento del recurso
            (estado ?u Extrayendo) ; El estado de la unidad es Extrayendo
            (not (estado ?u Ocioso)) ; La unidad ya no está ociosa
            (increase (CostePlan) 1) ;Se incrementa el coste del plan
        )
    )

    (:action CONSTRUIR
        :parameters (?u - Unidad ?e - Edificio ?loc - Localizacion)
        ;; Acción para construir un edificio

        :precondition (and
            (estaEn ?u ?loc) ; La unidad está en la ubicación
            (tipo ?u VCE) ; La unidad es de tipo VCE
            (estado ?u Ocioso) ; La unidad está ociosa
            (not (construido ?e)) ; El edificio no está construido
            (exists
                (?tipo - Edificio)
                (and
                    (tipo ?e ?tipo) ; El edificio es del tipo especificado
                    (forall
                        (?req - Requerimiento)
                        (imply
                            (requiere ?tipo ?req)
                            (adquirido ?req) ; Se han adquirido todos los requerimientos del edificio
                        )
                    )
                )
            )
        )

        :effect (and
            (construido ?e) ; El edificio está construido
            (estaEn ?e ?loc) ; El edificio está en la ubicación
            (increase (CostePlan) 1) ; Se incrementa el coste del plan
        )
    )

    (:action RECLUTAR
        :parameters (?e - Edificio ?u - Unidad ?loc - Localizacion)
        ;; Acción para reclutar una unidad desde un edificio

        :precondition (and
            (construido ?e) ; El edificio está construido
            (estaEn ?e ?loc) ; El edificio está en la ubicación
            (not (exists
                    (?estado - Estado)
                    (estado ?u ?estado))) ; La unidad no tiene ningún estado
            (exists
                (?tipo_edificio - Edificio)
                (exists
                    (?tipo_unidad - Unidad)
                    (and
                        (tipo ?e ?tipo_edificio) ; El edificio es del tipo especificado
                        (tipo ?u ?tipo_unidad) ; La unidad es del tipo especificado
                        (entrena ?tipo_edificio ?tipo_unidad) ; El edificio entrena la unidad
                        (forall
                            (?req - Requerimiento)
                            (imply
                                (requiere ?tipo_unidad ?req)
                                (adquirido ?req) ; Se han adquirido todos los requerimientos de la unidad
                            )
                        )
                    )
                )
            )
        )

        :effect (and
            (estado ?u Ocioso) ; La unidad está en estado Ocioso
            (estaEn ?u ?loc) ; La unidad está en la ubicación
            (increase (CostePlan) 1) ; Se incrementa el coste del plan
        )
    )

    (:action INVESTIGAR
        :parameters (?e - Edificio ?inv - Investigacion)
        ;; Acción para realizar una investigación en un laboratorio

        :precondition (and
            (tipo ?e Laboratorio) ; El edificio es de tipo Laboratorio
            (construido ?e) ; El edificio está construido
            (not (adquirido ?inv)) ; La investigación no ha sido adquirida
            (forall
                (?req - Requerimiento)
                (imply
                    (requiere ?inv ?req)
                    (adquirido ?req) ; Se han adquirido todos los requerimientos de la investigación
                )
            )
        )

        :effect 
                (and (adquirido ?inv) ; Se ha adquirido la investigación 
                (increase (CostePlan) 1)) ; se incrementa el coste del plan       
           
    )
)
