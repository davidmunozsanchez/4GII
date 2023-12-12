(define (domain dominio7)
    (:requirements :strips :typing :negative-preconditions :adl :equality)
    (:types
        Localizacion Creable Recurso - Entidad
        Edificio Unidad - Creable ;Ahora, los edificios y las unidades comparten la propiedad de ser entidades "creables", para diferenciarlas de los recursos, que son los explotables
        Estado ;Estado que puede tener una unidad (acción que está ejerciendo en cierto momento)
        )
    (:constants
        VCE Marine Soldado - Unidad ; Constantes que representan tipos específicos de unidades
        CentroDeMando Barracones Extractor - Edificio ; Constantes que representan tipos específicos de edificios
        Mineral Gas - Recurso ; Constantes que representan tipos específicos de recursos
        Extrayendo Ocioso - Estado ; Constantes que representan estados específicos de las unidades
    )
    (:functions
        (CostePlan) ;Va a almacenar el número de pasos que requiere el plan
        (cantidad_VCE ?rec - Recurso ?loc - Localizacion) ;Almacena la cantidad de VCEs que hay explotando cierto recurso en una localización
        (cantidad_almacenada ?rec) ;Cuánta reserva se tiene de un recurso
        (coste ?x - Creable ?rec - Recurso) ;Indica el coste de creación de cierto tipo de creable
        (factorRecurso ?rec - Recurso) ;Indica que cantidad se extrae de cierto tipo de recurso
        (maximoRecurso ?rec - Recurso)     ;Indica la cantidad máxima que se puede almacenar de cierto recurso
    )
    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ; Relaciona un objeto con su tipo (una de las constantes)
        (estaEn ?x - Entidad ?loc - Localizacion) ; Indica que un objeto se encuentra en una ubicación específica
        (conectado ?a - Localizacion ?b - Localizacion) ; Indica que hay una conexión directa desde la ubicación A hasta la ubicación B
        (construido ?x - Edificio) ; Indica si un edificio ha sido construido
        (estado ?x - Unidad ?est - Estado) ; Indica el estado actual de una unidad (Ocioso o Extrayendo)
        (extrayendo ?x - Unidad ?rec - Recurso) ; Indica que una unidad está extrayendo un recurso específico
        (siendoExtraido ?rec - Recurso) ; Indica que un recurso está siendo extraído
        (entrena ?e - Edificio ?u - Unidad) ; Relaciona un edificio con la unidad que puede entrenar
        ;;;; Ya no es necesario un predicado para requiere, ya que con la función de coste, si algún creable no requeire un recurso, tendrá ese valor a 0
    )

    (:action NAVEGAR
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition (and
            (estado ?u Ocioso) ; La unidad debe estar en estado "Ocioso"
            (estaEn ?u ?ori) ; La unidad debe estar en la ubicación de origen
            (conectado ?ori ?dest) ; Las ubicaciones de origen y destino deben estar conectadas
        )
        :effect (and
            (estaEn ?u ?dest) ; La unidad se mueve a la ubicación de destino
            (not (estaEn ?u ?ori)) ; La unidad ya no se encuentra en la ubicación de origen
            (increase (CostePlan) 1) ;Se incrementa en 1 el coste del plan
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
            (increase (cantidad_VCE ?rec ?loc) 1) ;Se incrementa la cantidad de VCE que hay extrayendo
            (increase (CostePlan) 1) ;Se incrementa el coste del plan

        )
    )
    (:action CONSTRUIR
        :parameters (?u - Unidad ?e - Edificio ?loc - Localizacion)
        :precondition (and 
            (estaEn ?u ?loc) ; La unidad debe estar en la ubicación especificada
            (tipo ?u VCE) ; La unidad debe ser de tipo VCE
            (estado ?u Ocioso) ; La unidad debe estar en estado "Ocioso"
            (not (construido ?e)) ; El edificio no debe haber sido construido previamente
            (exists (?tipo - Edificio) ; Buscamos el tipo de edificio al que pertenece el edificio que queremos construir ;(Buscamos el tipo de edificio al que pertenece el objeto que queremos construir)
                (and
                    (tipo ?e ?tipo)
                    (forall
                        (?rec - Recurso) ;Para todos los recursos.
                        (>= (cantidad_almacenada ?rec) (coste ?tipo ?rec)) ;Se ve que haya suficiente cantidad almacenada para el coste de cada tipo de recurso
                    )
                )
            )
        )
        :effect (and
            (construido ?e) ; El edificio se marca como construido
            (estaEn ?e ?loc) ; El edificio se encuentra en la ubicación especificada
            
            (forall
                (?tipo - Edificio)
                (forall
                    (?rec - Recurso)
                    (when
                        (tipo ?e ?tipo) ;Se busca el tipo de edificio al que pertenece el objeto que queremos construir
                        (decrease
                            (cantidad_almacenada ?rec)
                            (coste ?tipo ?rec))  ;Reducimos la cantidad que consumimos de cada recurso para hacer la construcción

                    )
                )
            )
            (increase (CostePlan) 1) ;Se incrementa el coste del plan
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
                        (forall
                            (?rec - Recurso) ;Y hay recursos suficientes
                            (>= (cantidad_almacenada ?rec) (coste ?tipo_unidad ?rec))
                        )
                    )
                )
            )
        )
        :effect (and
            (estado ?u Ocioso) ; Cuando reclutamos la unidad, la establecemos en estado "Ocioso", esperando órdenes
            (estaEn ?u ?loc) ; La unidad se encuentra en la ubicación especificada
            ;Reducimos del almacen la cantidad que consumimos de cada recurso para reclutar la tropa
            (forall
                (?tipo - Unidad)
                (forall
                    (?rec - Recurso)
                    (when
                        (tipo ?u ?tipo)
                        (decrease
                            (cantidad_almacenada ?rec)
                            (coste ?tipo ?rec))
                    )
                )
            )
            (increase (CostePlan) 1) ;Se incrementa el coste del plan
        )
    )

   
    (:action RECOLECTAR
        :parameters (?rec - Recurso ?loc - Localizacion)
        :precondition (and
            (estaEn ?rec ?loc) ;El recurso está en la localización

            ;Su recolección no puede superar la cantidad que se almacenaría
            ;es decir, maximoPermitido >= (cantidad_almacenada + (factorRecurso)*nº de VCE para ese recurso)
            (>= (maximoRecurso ?rec) (+ (cantidad_almacenada ?rec) (* (factorRecurso ?rec) (cantidad_VCE ?rec ?loc))))) ;Solo si caben todos los recursos que se van a recoger

        :effect (and 
                ;Se suma la recolección a la cantidad almacenada
                (increase (cantidad_almacenada ?rec) (* (factorRecurso ?rec) (cantidad_VCE ?rec ?loc))) ;Se aumenta la reserva del correspondiente recurso, 
                (increase (CostePlan) 1) ;Se incrementa el coste del plan en 1                                        ;proporcionalmente al número de VCEs que hay recolectando ese recurso en esa localización
            )

        )


    )
