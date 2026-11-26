import os
import nibabel as nib
import matplotlib.pyplot as plt
import numpy as np
import re
import nibabel as nib
import matplotlib.pyplot as plt
import cv2
import pandas as pd

####deteccion rutas archivos según regex
def busqueda_rutas(cadena):
        directorio_imagenes = './NIFTI PREPROC'
        rutas_imagenes = [os.path.join(directorio_imagenes, imagen) for imagen in os.listdir(directorio_imagenes)]
        patron = re.compile(fr'.*({cadena})')
        seleccionados = [s for s in rutas_imagenes if re.search(patron, s)]

        return seleccionados

def formacion_trios(lista1, lista2, lista3):

        patron = re.compile(r'.*(H9O)')
        h9O_list = [s for s in lista1 + lista2 + lista3 if re.search(patron, s)]
        
        patron = re.compile(r'.*(HVN)')
        hvn_list = [s for s in lista1 + lista2 + lista3 if re.search(patron, s)]

        #Obtenemos un listado con los números de paciente presentes en cada hospital
        pacientes_9O = []
        trios_9O = []
        indices_bad_h9O = []
        for ruta in h9O_list:
                ruta_sp = ruta.split('_')
                paciente = ruta_sp[1]

                paciente_sp = paciente[1] + paciente[2]
                pacientes_9O.append(paciente_sp)
        for index, pac9O in enumerate(np.unique(np.array(pacientes_9O))):
                patron = re.compile(fr'.*({pac9O})')
                seleccion = [s for s in h9O_list if re.search(patron, s)]
                seleccion.sort()
                # print(seleccion)
                trios_9O.append(seleccion)
                if(len(seleccion) < 3):
                       indices_bad_h9O.append(index)
       
        pacientes_hvn = []
        trios_hvn = []
        indices_bad_hvn = []
        for ruta in hvn_list:
                ruta_sp = ruta.split('_')
                paciente = ruta_sp[1]

                paciente_sp = paciente[1] + paciente[2]
                pacientes_hvn.append(paciente_sp)
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
        
        for indice in sorted(indices_bad_h9O, reverse=True):
                del trios_9O[indice]
        for indice in sorted(indices_bad_hvn, reverse=True):
                del trios_hvn[indice]
        
        return [trios_9O, trios_hvn]

def rangeDisplay01(im, flag_GLOBAL= True):
  im = im.astype(float)
  if flag_GLOBAL:
    im = (im - im.min())/(im.max() - im.min())
  else:
    # bands normalization
    for band in range(im.shape[2]):
      im[:, :, band] = (im[:, :, band] - im[:, :, band].min())/(im[:, :, band].max() - im[:, :, band].min())
      # Note: remember that, for plt.imshow with RGB data, the valid range is [0..1] for floats and [0..255] for integers.
  return im

def displayIm(im, title='Result',factor= 2):
  # First normalize range
  max=np.max(im)
  min=np.min(im)
  if min<0 or max>255:
    im=rangeDisplay01(im,flag_GLOBAL=True)
  if len(im.shape) == 3:
    # im has three-channels
    plt.imshow(im, cmap='jet')
  else:
    # im has a single-channel
    plt.imshow(im, cmap='gray')
  figure_size = plt.gcf().get_size_inches()
  plt.gcf().set_size_inches(factor * figure_size)
  plt.title(title)
  plt.xticks([]), plt.yticks([])
  plt.show()

def lectura_img(trios, modo):
        imgs_nifti = []
        pacs = []
        hosp = []
        for trio in trios:
                        trio_np = []
                        paciente = trio[0].split("_")[1]
                        hospital = trio[0].split("_")[2].split(".")[0]
                        
                        for ruta in trio:
                                # Cargar la imagen NIfTI
                                imagen_nifti = nib.load(ruta)
                                
                                imagen_nifti = imagen_nifti.get_fdata()
                                ##se sutitutyen los nan por 0
                                imagen_nifti = np.nan_to_num(imagen_nifti, nan=0.0)
                                trio_np.append(imagen_nifti)
                        imgs_nifti.append(trio_np)
                        pacs.append(paciente)
                        hosp.append(hospital)

                
        if(modo == 'mean'):
                imgs_final = []
                for trio_np in imgs_nifti:
                        elems2d = []
                        for elem in trio_np:
                                # Calcular la media a lo largo del eje 2 (profundidad)
                                elem2d = np.mean(elem, axis=2)
                                print(elem2d.dtype)
                                print(elem2d)
                                elems2d.append(elem2d)
                        # print(trio_np[0].shape)
                        # print(elems2d[0].shape)
                        imagen = np.stack((elems2d[0], elems2d[1], elems2d[2]), axis=2)
                        print(imagen.shape)
                        # print(imagen.shape)
                        
                        imgs_final.append(imagen)
                return imgs_final, pacs, hosp
        if(modo == 'mosaic'):
                imgs_final = []
                ##Rehacer para mosaico
                ##para hacer mosaico debemos obtener el número de imágenes
                ##debe ser para para que no quede ninguna zona hueca
                ##si se hace la division entera de la longitud de canales entre los trozos en que queremos partir la imagen, se obtiene el step
                ##es decir, la primera media sería 0 a step-1, la siguiente de step a step*2-1, etc, etc. En el final, pueden estar incluido más canales

                
                for trio_np in imgs_nifti:
                        elems2d = []
                        for elem in trio_np:
                                # Dividimos el arreglo en base a su profundidad
                                trozos = np.array_split(elem, 4, 2)
                                trozos2d = []
                                for trozo in trozos:
                                        # print(trozo.shape)
                                        print("control")
                                        trozo2d = np.mean(trozo, axis=2)
                                        trozos2d.append(trozo2d)
                                        print(trozo2d.shape)

                                apilado_horizontal1 = np.concatenate((trozos2d[:4//2]))
                                
                                apilado_horizontal2 = np.concatenate((trozos2d[4//2:]))
                                
                                apilado_vertical = np.concatenate((apilado_horizontal1, apilado_horizontal2), axis=1)
                                
                                elems2d.append(apilado_vertical)
                                ###Si es 4, se apilan 2 a 2 horizontalmente y despues el resultado en vertical
                                ###si es 6, 3 y 3 y así sucesivamente
                        imagen = np.stack((elems2d[0], elems2d[1], elems2d[2]), axis=2)
                        # print(imagen.shape)
                        imgs_final.append(imagen)
                return imgs_final, pacs, hosp

def main():                     
        rutas_a90 = busqueda_rutas('A90')

        rutas_fdg = busqueda_rutas('FDG')

        rutas_1ct = busqueda_rutas('1CT')


        trios = formacion_trios(rutas_a90, rutas_fdg, rutas_1ct)
        
        imagenes_media_1, pac_media_1, hosp_media_1  = lectura_img(trios[0], 'mean')
        
        
        imagenes_mosaic_1, pac_mosaic_1, hosp_mosaic_1   = lectura_img(trios[0], 'mosaic')
        imagenes_media_2, pac_media_2, hosp_media_2  = lectura_img(trios[1], 'mean')
        
        imagenes_mosaic_2, pac_mosaic_2, hosp_mosaic_2   = lectura_img(trios[1], 'mosaic')

        ruta_guardado1 = "./IMGS_MEDIA"
        ruta_guardado2 = "./IMGS_MOSAIC"

        df = pd.read_csv('etiquetas.txt', delimiter='\t')
        df = df.dropna(how="all")
        
# Asegúrate de que el directorio exista
        if not os.path.exists(ruta_guardado1):
                os.makedirs(ruta_guardado1)
        else:
        # Si el directorio ya existe, borra su contenido
                for archivo in os.listdir(ruta_guardado1):
                        archivo_ruta = os.path.join(ruta_guardado1, archivo)
                        os.remove(archivo_ruta)
        
        for im, p, h in zip(imagenes_media_1, pac_media_1, hosp_media_1):
                # displayIm(im[:,:,2])
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                        # im = im.transpose(1, 2, 0)           
                        cv2.imwrite(f"{ruta_guardado1}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))
        for im, p, h in zip(imagenes_media_2, pac_media_2, hosp_media_2):
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                        # im = im.transpose(1, 2, 0)           
                        cv2.imwrite(f"{ruta_guardado1}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))

# Asegúrate de que el directorio exista
        if not os.path.exists(ruta_guardado2):
                os.makedirs(ruta_guardado2)
        else:
        # Si el directorio ya existe, borra su contenido
                for archivo in os.listdir(ruta_guardado2):
                        archivo_ruta = os.path.join(ruta_guardado2, archivo)
                        os.remove(archivo_ruta)
        for im, p, h in zip(imagenes_mosaic_1, pac_mosaic_1, hosp_mosaic_1):
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                        # im = im.transpose(1, 2, 0)           
                        cv2.imwrite(f"{ruta_guardado2}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))
        for im, p, h in zip(imagenes_mosaic_2, pac_mosaic_2, hosp_mosaic_2):
                im = rangeDisplay01(im, False)
                im = im.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == p) & (df['Centro'] == h), 'Etiqueta'].values
                if (etiqueta != 2):
                        # im = im.transpose(1, 2, 0)           
                        cv2.imwrite(f"{ruta_guardado2}/_{p}_{h}_{int(etiqueta[0])}_.jpg", cv2.cvtColor(im*255, cv2.COLOR_RGB2BGR))
        
        directorio_imagenes = './IMGS_MEDIA'
        rutas_imagenes = [os.path.join(directorio_imagenes, imagen) for imagen in os.listdir(directorio_imagenes)]
        print(len(rutas_imagenes))

####añadir etiqueta al nombre, las tres etiquetas del trio tienen que ser consistentes
if __name__ == "__main__":
        main()