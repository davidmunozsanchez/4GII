import os
import nibabel as nib
import matplotlib.pyplot as plt
import numpy as np
import re
import nibabel as nib
import matplotlib.pyplot as plt
import cv2
import pandas as pd

####La función busqueda_rutas tiene como objetivo buscar y retornar 
####las rutas de archivos en un directorio específico cuyos nombres contienen una cadena dada
def busqueda_rutas(cadena):
        # Define el directorio donde se encuentran las imágenes
        directorio_imagenes = './NIFTI PREPROC'
        # Obtiene la lista completa de rutas de las imágenes en el directorio especificado
        rutas_imagenes = [os.path.join(directorio_imagenes, imagen) for imagen in os.listdir(directorio_imagenes)]
        # Crea un patrón de expresión regular que busca la cadena especificada en los nombres de las imágenes
        patron = re.compile(fr'.*({cadena})')
        # Filtra las rutas de las imágenes, seleccionando solo aquellas que coinciden con el patrón
        seleccionados = [s for s in rutas_imagenes if re.search(patron, s)]

        #Se devuelven las rutas seleccionadas
        return seleccionados


def formacion_trios(lista1, lista2, lista3):
        # Definir un patrón de búsqueda para encontrar archivos con el hospital 'H9O'
        patron = re.compile(r'.*(H9O)')
        h9O_list = [s for s in lista1 + lista2 + lista3 if re.search(patron, s)]
        
        # Definir un patrón de búsqueda para encontrar archivos con 'HVN'
        patron = re.compile(r'.*(HVN)')
        hvn_list = [s for s in lista1 + lista2 + lista3 if re.search(patron, s)]

        #Se inicializan listas para almacenar pacientes y tríos de 'H9O'
        pacientes_9O = []
        trios_9O = []
        indices_bad_h9O = []

        # Extraer el número de paciente de cada ruta de archivo en 'h9O_list'
        for ruta in h9O_list:
                ruta_sp = ruta.split('_')
                paciente = ruta_sp[1]
                #Esto concatena los dos números que vienen después de la P en la ruta
                paciente_sp = paciente[1] + paciente[2]
                pacientes_9O.append(paciente_sp)

        # Agrupar rutas en tríos por número de paciente y almacenar los índices de los grupos incompletos
        for index, pac9O in enumerate(np.unique(np.array(pacientes_9O))):
                patron = re.compile(fr'.*({pac9O})')
                seleccion = [s for s in h9O_list if re.search(patron, s)]

                ##Al hacer sort, el orden, por el nombre de archivo, es 1CT, A90, FDG, sea cual sea el orden en que metamos las listas como parámetros de la función
                seleccion.sort()
                # print(seleccion)
                trios_9O.append(seleccion)
                #En caso de que un paciente tenga menos de 3 imágenes, se almacena su índice de trios_9o
                if(len(seleccion) < 3):
                       indices_bad_h9O.append(index)
       
       
        ####Ahora se procede igual pero con otro hospital distinto
        # Inicializar listas para almacenar pacientes y tríos de 'HVN'
        pacientes_hvn = []
        trios_hvn = []
        indices_bad_hvn = []

        # Extraer el número de paciente de cada ruta de archivo en 'hvn_list'
        for ruta in hvn_list:
                ruta_sp = ruta.split('_')
                paciente = ruta_sp[1]

                paciente_sp = paciente[1] + paciente[2]
                pacientes_hvn.append(paciente_sp)

        # Agrupar rutas en tríos por número de paciente y almacenar los índices de los grupos incompletos
        for index, pac_hvn in enumerate(np.unique(np.array(pacientes_hvn))):
                patron = re.compile(fr'.*({pac_hvn})')
                seleccion = [s for s in hvn_list if re.search(patron, s)]
                seleccion.sort()
                # print(seleccion)
                trios_hvn.append(seleccion)
                if(len(seleccion) < 3):
                       indices_bad_hvn.append(index)
        
        # print(f"TOTALES BUENOS H9O: {len(trios_9O) - len(indices_bad_h9O)}")
        # print(f"TOTALES BUENOS HVN: {len(trios_hvn) - len(indices_bad_hvn)}")
        

        #La razón de ordenar indices_bad_h9O en orden descendente antes de eliminar los elementos es evitar problemas de desajuste de índices. Cuando se eliminan elementos de una lista, 
        #los índices de los elementos restantes se actualizan. Si elimináramos elementos en orden ascendente, los índices de los elementos que aún 
        #no se han eliminado cambiarían, lo que podría llevar a errores o eliminaciones incorrectas.
        # Eliminar los tríos incompletos de 'H9O'
        for indice in sorted(indices_bad_h9O, reverse=True):
                del trios_9O[indice]

        # Eliminar los tríos incompletos de 'HVN'
        for indice in sorted(indices_bad_hvn, reverse=True):
                del trios_hvn[indice]
        
        # Retornar las listas de tríos completos para 'H9O' y 'HVN'
        return [trios_9O, trios_hvn]

#La función rangeDisplay01 normaliza una imagen para que sus valores de píxeles estén en el rango [0, 1].
#Dependiendo del valor del parámetro flag_GLOBAL, la normalización puede ser global (aplicada a toda la imagen) o por bandas (aplicada a cada banda individualmente). 
def rangeDisplay01(im, flag_GLOBAL= True):
  # Convierte la imagen a tipo float para realizar operaciones de normalización
  im = im.astype(float)
  if flag_GLOBAL:
    # Normalización global: escala la imagen completa al rango [0, 1]
    im = (im - im.min())/(im.max() - im.min())
  else:
    # Normalización por bandas: escala cada banda de la imagen al rango [0, 1] individualmente
    for band in range(im.shape[2]):
      im[:, :, band] = (im[:, :, band] - im[:, :, band].min())/(im[:, :, band].max() - im[:, :, band].min())
      # Nota: para plt.imshow con datos RGB, el rango válido es [0..1] para floats y [0..255] para enteros.
  return im

def displayIm(im, title='Result', factor=2):
    # Primero normalizar el rango
    max = np.max(im)  # Obtener el valor máximo de la imagen
    min = np.min(im)  # Obtener el valor mínimo de la imagen
    
    # Normalizar la imagen si los valores están fuera del rango [0, 255]
    if min < 0 or max > 255:
        im = rangeDisplay01(im, flag_GLOBAL=True)
    
    # Verificar si la imagen tiene tres canales (por ejemplo, RGB)
    if len(im.shape) == 3:
        plt.imshow(im, cmap='jet')  # Mostrar la imagen en color usando el mapa de colores 'jet'
    else:
        plt.imshow(im, cmap='gray')  # Mostrar la imagen en escala de grises
    
    # Ajustar el tamaño de la figura multiplicando por el factor especificado
    figure_size = plt.gcf().get_size_inches()  # Obtener el tamaño actual de la figura en pulgadas
    plt.gcf().set_size_inches(factor * figure_size)  # Ajustar el tamaño de la figura
    
    plt.title(title)  # Establecer el título de la imagen
    plt.xticks([]), plt.yticks([])  # Eliminar las marcas de los ejes x e y
    plt.show()  # Mostrar la imagen


def lectura_img(trios, modo):
        # Inicializar listas para almacenar imágenes, pacientes y hospitales
        imgs_nifti = []
        pacs = []
        hosp = []

        # Recorrer cada trío de rutas de imágenes
        for trio in trios:
                        trio_np = [] # Lista para almacenar las imágenes del trío
                        paciente = trio[0].split("_")[1] # Extraer el identificador del paciente de la primera ruta
                        hospital = trio[0].split("_")[2].split(".")[0] # Extraer el identificador del hospital de la primera ruta
                        
                        # Recorrer cada ruta en el trío
                        for ruta in trio:
                                # Cargar la imagen NIfTI
                                imagen_nifti = nib.load(ruta)
                                
                                imagen_nifti = imagen_nifti.get_fdata() # Obtener los datos de la imagen como un arreglo de NumPy
                                imagen_nifti = np.nan_to_num(imagen_nifti, nan=0.0) # Sustituir los NaN por 0
                                trio_np.append(imagen_nifti) # Añadir la imagen procesada a la lista del trío
                        imgs_nifti.append(trio_np) # Añadir el trío procesado a la lista de imágenes
                        pacs.append(paciente) # Añadir el identificador del paciente a la lista de pacientes
                        hosp.append(hospital) # Añadir el identificador del hospital a la lista de hospitales

        # Modo 'mean': calcular la media a lo largo del eje 2 (profundidad) para cada trío de imágenes     
        if(modo == 'mean'):
                imgs_final = [] # Lista para almacenar las imágenes finales procesadas
                for trio_np in imgs_nifti:
                        elems2d = [] # Lista para almacenar las imágenes 2D resultantes
                        for elem in trio_np:
                                # Calcular la media a lo largo del eje 2 (profundidad)
                                elem2d = np.mean(elem, axis=2)
                                #print(elem2d.dtype)  # Imprimir el tipo de dato de la imagen 2D (para depuración)
                                #print(elem2d) # Imprimir la imagen 2D (para depuración)
                                elems2d.append(elem2d) # Añadir la imagen 2D a la lista de elementos
                        # print(trio_np[0].shape)
                        # print(elems2d[0].shape)

                        # Apilar las tres imágenes 2D a lo largo del eje 2 para formar una imagen con 3 canales
                        imagen = np.stack((elems2d[0], elems2d[1], elems2d[2]), axis=2)
                        #print(imagen.shape)
                        # print(imagen.shape)
                        
                        # Añadir la imagen final a la lista de imágenes procesadas
                        imgs_final.append(imagen)

                # Devolver las imágenes finales, pacientes y hospitales
                return imgs_final, pacs, hosp
        
        # Modo 'mosaic': dividir cada imagen en mosaicos y promediar cada sección
        if(modo == 'mosaic'):
                imgs_final = [] # Lista para almacenar las imágenes finales procesadas

                
                for trio_np in imgs_nifti:
                        elems2d = [] # Lista para almacenar las imágenes 2D resultantes
                        for elem in trio_np:
                                # Dividir el arreglo en trozos en base a su profundidad, concretamente en 4 trozos
                                trozos = np.array_split(elem, 4, 2)
                                trozos2d = [] # Lista para almacenar los trozos promediados
                                for trozo in trozos:
                                        # print(trozo.shape)
                                        #print("control")
                                        # Calcular la media a lo largo del eje 2 (profundidad) para cada trozo
                                        trozo2d = np.mean(trozo, axis=2)
                                        trozos2d.append(trozo2d) # Añadir el trozo 2D a la lista de trozos
                                        #print(trozo2d.shape)

                                apilado_horizontal1 = np.concatenate((trozos2d[:4//2])) #selecciona los primeros dos trozos (de 0 a 1) de trozos2d
                                
                                apilado_horizontal2 = np.concatenate((trozos2d[4//2:])) # selecciona los últimos dos trozos (de 2 a 3) de trozos2d
                                
                                apilado_vertical = np.concatenate((apilado_horizontal1, apilado_horizontal2), axis=1) #combina estas dos imágenes a lo largo del eje 1 (eje de las columnas), lo que resulta en una sola imagen
                                
                                elems2d.append(apilado_vertical) # Añadir la imagen apilada verticalmente a la lista de elementos
                                ###Si hay 4 trozos, se apilan 2 a 2 horizontalmente y despues el resultado en vertical
                                ###si es 6, 3 y 3 y así sucesivamente
                        # Apilar las tres imágenes 2D a lo largo del eje 2 para formar una imagen con 3 cananles
                        imagen = np.stack((elems2d[0], elems2d[1], elems2d[2]), axis=2)
                        # print(imagen.shape)
                        imgs_final.append(imagen) # Añadir la imagen final a la lista de imágenes procesadas

                return imgs_final, pacs, hosp  # Devolver las imágenes finales, pacientes y hospitales
                

def main():
        # Buscar rutas de archivos que contengan 'A90', 'FDG', y '1CT' respectivamente                    
        rutas_a90 = busqueda_rutas('A90')

        rutas_fdg = busqueda_rutas('FDG')

        rutas_1ct = busqueda_rutas('1CT')

        # Formar tríos combinando rutas de los tres tipos de imágenes, se tienen trios para el primer hosptial y para el segundo por separado
        #independientemente del orden en que se pase como parámetro, las imágenes resultantes serán:
        #canal 0: 1CT
        #canal 1: A90
        #canal 2: FDG
        trios = formacion_trios(rutas_a90, rutas_fdg, rutas_1ct)
        
        print(trios[0])
        # Leer y procesar imágenes del primer grupo de tríos en modo 'mean'
        imagenes_media_1, pac_media_1, hosp_media_1  = lectura_img(trios[0], 'mean')
        
        # Leer y procesar imágenes del primer grupo de tríos en modo 'mosaic'
        imagenes_mosaic_1, pac_mosaic_1, hosp_mosaic_1   = lectura_img(trios[0], 'mosaic')

        # Leer y procesar imágenes del segundo grupo de tríos en modo 'mean'
        imagenes_media_2, pac_media_2, hosp_media_2  = lectura_img(trios[1], 'mean')

        # Leer y procesar imágenes del segundo grupo de tríos en modo 'mosaic'
        imagenes_mosaic_2, pac_mosaic_2, hosp_mosaic_2   = lectura_img(trios[1], 'mosaic')

        # Rutas de directorios para guardar imágenes procesadas
        ruta_guardado1 = "./IMGS_MEDIA"
        ruta_guardado2 = "./IMGS_MOSAIC"

        # Leer etiquetas desde un archivo y almacenarlas en un DataFrame
        df = pd.read_csv('etiquetas.txt', delimiter='\t')
        df = df.dropna(how="all")  # Eliminar filas completamente vacías
        
        # Asegurarse de que el directorio exista
        #esta ruta de guardado sería para la media
        if not os.path.exists(ruta_guardado1):
                os.makedirs(ruta_guardado1) # Si no existe, crear el directorio
        else:
                # Si el directorio ya existe, borrar su contenido
                for archivo in os.listdir(ruta_guardado1):
                        archivo_ruta = os.path.join(ruta_guardado1, archivo)
                        os.remove(archivo_ruta)
        
        # Procesar y guardar imágenes del primer grupo de tríos para el modo mean en el directorio de guardado 1
        for im, p, h in zip(imagenes_media_1, pac_media_1, hosp_media_1):
                # Normalizar y convertir la imagen al formato adecuado
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                # Obtener la etiqueta correspondiente al paciente y hospital actual
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                # Si la etiqueta no es 2 (es decir, no es nula), guardar la imagen con la etiqueta en el nombre
                if (etiqueta != 2):
                        # Guarda la imagen en el directorio especificado, con un nombre compuesto por:
                        # - El nombre del paciente (p)
                        # - El nombre del hospital (h)
                        # - La etiqueta asociada a la imagen (etiqueta[0])
                        # - La extensión del archivo (.jpg)
                        # Además, se convierte la imagen al formato BGR antes de guardarla y se pasa a rango [0,255] al multiplicar por 255 porque ya están en rango [0,1] 
                        cv2.imwrite(f"{ruta_guardado1}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))
        
        ####En esta se procede igual paro para el segundo grupo de tríos para mean
        for im, p, h in zip(imagenes_media_2, pac_media_2, hosp_media_2):
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                            
                        cv2.imwrite(f"{ruta_guardado1}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))

        # Asegúrate de que el directorio exista
        if not os.path.exists(ruta_guardado2):
                os.makedirs(ruta_guardado2) # Si no existe, crear el directorio
        else:
                # Si el directorio ya existe, borrar su contenido
                for archivo in os.listdir(ruta_guardado2):
                        archivo_ruta = os.path.join(ruta_guardado2, archivo)
                        os.remove(archivo_ruta)

        ##Se procede igual que antes pero en otro directorio y para los grupos de tríos del modo mosaic
        for im, p, h in zip(imagenes_mosaic_1, pac_mosaic_1, hosp_mosaic_1):
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                               
                        cv2.imwrite(f"{ruta_guardado2}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))
        for im, p, h in zip(imagenes_mosaic_2, pac_mosaic_2, hosp_mosaic_2):
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                              
                        cv2.imwrite(f"{ruta_guardado2}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))
                        
        # Contar el número de imágenes guardadas en el directorio de guardado 1 (para el modo mean)
        directorio_imagenes = './IMGS_MEDIA'
        rutas_imagenes = [os.path.join(directorio_imagenes, imagen) for imagen in os.listdir(directorio_imagenes)]
        print(len(rutas_imagenes))

if __name__ == "__main__":
        main()