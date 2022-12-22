#include "colors.inc"

camera {location <0.0, 20.0, -100.0>
        direction <0.0, 0.1, 1.0>
        up <0.0, 1.0, 0.0>
        angle 80
    }

light_source {<40,120,10> colour White}

background {color SkyBlue}

plane { <0.0, 1.0, 0.0>, 0
        pigment {checker colour Yellow colour Green scale 5}
        finish {ambient 0.1}
}

sphere {<60, 15, 20>, 15
        pigment {colour rgb <0.2, 0.4, 0.7>}
        finish {ambient 0.1 diffuse 0.6 phong 0.8 phong_size 10}

}

sphere {<20, 15, 10>, 15
        pigment {colour rgb <0.2, 0.9, 0.2>}
        finish {//
                ambient 0.1 diffuse 0.6 
                phong 1 phong_size 10
                reflection 0.5                
    }

}

sphere {<-20, 15, 10>, 15
        pigment {colour rgbt <1, 0.0, 0.2, 0.7>}
        finish {//ambient 0.1 diffuse 0.6 
                phong 1 phong_size 10
    }

}

sphere {<-60, 15, 10>, 15
        pigment {colour rgbt <1, 0.0, 0.2, 0.6>}
        finish {//ambient 0.1 diffuse 0.6 
                phong 1 phong_size 10
    }
        //Iluminacion caustica
    interior {caustics 2
                ior 1.5 //relfexión interior}

}

//Texturas
sphere {<0, 0,0>, 1 //se normaliza
        pigment {//colour rgb <0.2, 0.4, 0.7>
                image_map { png "textura.png" map_type 1 //para la esfera es 1, para los planos 0 por ejemplo}
            }
        finish {ambient 0.1 diffuse 0.6 phong 0.8 phong_size 10}
        //y se escala
        scale <10,10,10>
        rotate <0.0, -45, 0.0>
        translate <0, 40, -10>   
}




