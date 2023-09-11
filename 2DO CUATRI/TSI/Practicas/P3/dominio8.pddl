(define (domain dominio8)
    (:requirements :strips :typing :negative-preconditions :adl :equality)
    (:types
        Localizacion Creable Recurso - Entidad
        Edificio Unidad - Creable ;Ahora, los edificios y las unidades comparten la propiedad de ser entidades "creables", para diferenciarlas de los recursos, que son los explotables
        Estado ;Estado que puede tener una unidad (acción que está ejerciendo en cierto momento)
        )
    (:constants
        VCE Marine Soldado - Unidad
        CentroDeMando Barracones Extractor - Edificio
        Mineral Gas - Recurso
        Extrayendo Ocioso - Estado
    )
    (:functions
        (CostePlan) ;Va a almacenar el número de pasos que requiere nuestro plan
        (cantidad_VCE ?rec - Recurso ?loc - Localizacion) ;Almacena la cantidad de VCEs que hay explotando cierto recurso en cierto nodo
        (cantidad_almacenada ?rec) ;Almacena cuánta reserva tenemos de un recurso en concreto
        (coste ?x - Creable ?rec - Recurso) ;Indica el coste de creación de cierto tipo de creable
        (tiempoPlan) ;Va a almacenar el tiempo (virtual, dentro del juego), que consume nuestro plan
        (tiempoCreacion ?x - Creable) ;Indica el tiempo virtual que se tarda en crear cierto tipo de creable
        (tiempoDesplazamiento1 ?x - Unidad) ;Indica el tiempo virtual que tarda en desplazarse cierto tipo de unidad
        (tiempoDesplazamiento2 ?x - Unidad) ;Indica el tiempo virtual que tarda en desplazarse cierto tipo de unidad
        (tiempoDesplazamiento3 ?x - Unidad) ;Indica el tiempo virtual que tarda en desplazarse cierto tipo de unidad
        (factorRecurso ?rec - Recurso) ;Indica que cantidad se extrae de cierto tipo de recurso
        (maximoRecurso ?rec - Recurso) ;Indica la cantidad máxima que se puede almacenar de cierto recurso
        (tiempoRecurso ?rec - Recurso)  ;Indica el tiempo de recolección de un determinado recurso
    )

    (:predicates
        (tipo ?x - Entidad ?y - Entidad) ;Relaciona un objeto con el "tipo" (esto es, alguna de las constantes) al que pertenece
        (estaEn ?x - Entidad ?loc - Localizacion) ;Indica que cierto objeto está en una localización concreta
        (conectado ?a - Localizacion ?b - Localizacion) ;Indica que hay un enlace directo desde la localización a) hasta la localización b)
        (construido ?x - Edificio) ;Indica que un objeto de tipo edificio ha sido construido. Principalmente para evitar que un mismo objeto sea construido múltiples veces
        (estado ?x - Unidad ?est - Estado) ;Indica qué está haciendo una unidad. En principio, una unidad solo puede estar "ociosa" o "extrayendo". Pensado para los VCEs
        (extrayendo ?x - Unidad ?rec - Recurso) ;Indica que una unidad concreta está extrayendo un recurso
        (siendoExtraido ?rec - Recurso) ;Indica que un recurso está siendo extraído
        (entrena ?e - Edificio ?u - Unidad) ;Relaciona cada unidad con el edificio que la entrena
        ;;;; Ya no es necesario un predicado para requiere, ya que con la función de coste, si algún creable no requeire un recurso, tendrá ese valor a 0

    )
    ;;;Para navegar se ha optado por separar según las localizaciones implicadas y dependiendo de eso, el tiempo del plan que se incrementa depende de una función u otra
    ;;;En el caso de Navegar 1, se busca que la ruta no sea entre dos nodos especiales (es decir, donde navegar entre ellos tiene un coste más alto).
    ;;;Así pues se elige la función tiempoDesplazamiento1.

    ;;;Navegar 2 y 3 representan los viajes que valen 20 en vez de 10. Se usa tiempoDesplazamiento2 y se comprueba en las precondiciones que las localizaciones sean unas específicas

    ;;;Navegar 4 es el viaje que hay de 40. Se usa tiempoDesplazamiento3.

    ;;;La función tiempoDesplazamientoi devuelve el tiempo de desplazamiento de una determinada unidad dependiendo de la longitud de la ruta, es por ello que se crean 3 distintas.

    ;;;Ciertamente, si se usaran más valores, habría que transformar esta representación, pero para el problema que nos atañe, creo que es entendible y fácil de usar.
    (:action NAVEGAR_1
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition (and
            (estado ?u Ocioso) ; La unidad debe estar en estado "Ocioso"
            (estaEn ?u ?ori) ; La unidad debe estar en la ubicación de origen
            (conectado ?ori ?dest) ; Las ubicaciones de origen y destino deben estar conectadas
            (or (and (not (= ?ori LOC22))
                    (not (= ?dest LOC23))
                )
                (and (not (= ?ori LOC23))
                    (not (= ?dest LOC22))
                ))
            (or (and (not (= ?ori LOC42))
                    (not (= ?dest LOC43))
                )
                (and (not (= ?ori LOC43))
                    (not (= ?dest LOC42))
                ))
            (or (and (not (= ?ori LOC44))
                    (not (= ?dest LOC15))
                )
                (and (not (= ?ori LOC15))
                    (not (= ?dest LOC44))
                ))

        )
        :effect (and
            (estaEn ?u ?dest) ; La unidad se mueve a la ubicación de destino
            (not (estaEn ?u ?ori)) ; La unidad ya no se encuentra en la ubicación de origen
            (increase (CostePlan) 1) ;Se incrementa en 1 el coste del plan

            (forall
                (?tipo - Unidad)
                (when
                    (tipo ?u ?tipo)
                    (increase
                        (tiempoPlan)
                        (tiempoDesplazamiento1 ?tipo))
                )
            )

        )
    )

    (:action NAVEGAR_2
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition (and
            (estado ?u Ocioso)
            (estaEn ?u ?ori)
            (conectado ?ori ?dest)
            (or (and (= ?ori LOC22)
                    (= ?dest LOC23)
                )
                (and (= ?ori LOC23)
                    (= ?dest LOC22)
                ))

        )
        :effect (and
            (estaEn ?u ?dest)
            (not (estaEn ?u ?ori))
            (increase (CostePlan) 1)

            (forall
                (?tipo - Unidad)
                (when
                    (tipo ?u ?tipo)
                    (increase
                        (tiempoPlan)
                        (tiempoDesplazamiento2 ?tipo))
                )
            )

        )
    )

    (:action NAVEGAR_3
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition (and
            (estado ?u Ocioso)
            (estaEn ?u ?ori)
            (conectado ?ori ?dest)
            (or (and (= ?ori LOC42)
                    (= ?dest LOC43)
                )
                (and (= ?ori LOC43)
                    (= ?dest LOC42)
                ))

        )
        :effect (and
            (estaEn ?u ?dest)
            (not (estaEn ?u ?ori))
            (increase (CostePlan) 1)

            (forall
                (?tipo - Unidad)
                (when
                    (tipo ?u ?tipo)
                    (increase
                        (tiempoPlan)
                        (tiempoDesplazamiento2 ?tipo))
                )
            )

        )
    )
    (:action NAVEGAR_4
        :parameters (?u - Unidad ?ori - Localizacion ?dest - Localizacion)
        :precondition (and
            (estado ?u Ocioso)
            (estaEn ?u ?ori)
            (conectado ?ori ?dest)
            (or (and (= ?ori LOC44)
                    (= ?dest LOC15)
                )
                (and (= ?ori LOC15)
                    (= ?dest LOC44)
                ))

        )
        :effect (and
            (estaEn ?u ?dest)
            (not (estaEn ?u ?ori))
            (increase (CostePlan) 1)

            (forall
                (?tipo - Unidad)
                (when
                    (tipo ?u ?tipo)
                    (increase
                        (tiempoPlan)
                        (tiempoDesplazamiento3 ?tipo))
                )
            )

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
            (exists
                (?tipo - Edificio) ;(Buscamos el tipo de edificio al que pertenece el objeto que queremos construir)
                (and
                    (tipo ?e ?tipo)
                    (forall
                        (?rec - Recurso) ;Para todos los recursos...
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
                        (tipo ?e ?tipo) ;(Buscamos el tipo de edificio al que pertenece el objeto que queremos construir)
                        (decrease
                            (cantidad_almacenada ?rec)
                            (coste ?tipo ?rec)) ;Reducimos del almacen la cantidad que consumimos de cada recurso para hacer la construcción

                    )
                )
            )
            ;Aumentamos el tiempo del plan la cantidad pertinente, en función del tipo de edificio creado
            (forall
                (?tipo - Edificio)
                (when
                    (tipo ?e ?tipo) ;Buscamos el tipo de edificio al que pertenece el objeto que queremos construir
                    (increase (tiempoPlan) (tiempoCreacion ?tipo))
                )
            )
            (increase (CostePlan) 1) ;Se aumenta el coste del plan
        )
    )

    (:action RECLUTAR
        :parameters (?e - Edificio ?u - Unidad ?loc - Localizacion)
        :precondition (and
            (construido ?e) ; El edificio debe haber sido construido previamente
            (estaEn ?e ?loc) ; El edificio debe estar en la ubicación especificada
            (not (exists
                    (?estado - Estado)
                    (estado ?u ?estado))) ;Si la unidad no tiene estado, es que aún no ha sido reclutada
            (exists
                (?tipo_edificio - Edificio) ;(Buscamos el tipo del edificio que hemos seleccionado)
                (exists
                    (?tipo_unidad - Unidad) ;(Y el tipo de la unidad que queremos entrenar)
                    (and
                        (tipo ?e ?tipo_edificio)
                        (tipo ?u ?tipo_unidad)
                        (entrena ?tipo_edificio ?tipo_unidad) ;Ese tipo de edificio entrena ese tipo de unidad
                        (forall
                            (?rec - Recurso) ;Y hay recursos suficientes
                            (>= (cantidad_almacenada ?rec) (coste ?tipo_unidad ?rec))
                        )
                    )
                )
            )
        )
        :effect (and
            (estado ?u Ocioso) ;Cuando la reclutamos, la inicializamos a ociosa, a la espera de una orden
            (estaEn ?u ?loc)
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
            ;Aumentamos el tiempo del plan la cantidad pertinente, en función del tipo de unidad reclutada
            (forall
                (?tipo - Unidad)
                (when
                    (tipo ?u ?tipo)
                    (and
                        (increase (tiempoPlan) (tiempoCreacion ?tipo))
                    )
                )
            )
            (increase (CostePlan) 1) ; Se incrementa el coste del plan
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

            (increase
                (cantidad_almacenada ?rec)
                (* (factorRecurso ?rec) (cantidad_VCE ?rec ?loc))) ;Se aumenta la reserva del correspondiente recurso, 
            (increase (CostePlan) 1)                                ;proporcionalmente al número de VCEs que hay recolectando ese recurso en esa localización
            (increase (tiempoPlan) (tiempoRecurso ?rec)) ;Se aumenta el tiempo según el recurso

        )

    )

)