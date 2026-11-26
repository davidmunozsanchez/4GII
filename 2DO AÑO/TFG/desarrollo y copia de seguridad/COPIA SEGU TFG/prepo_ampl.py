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

        if(modo == 4):
                imgs_final = []
                ##Rehacer para mosaico
                ##para hacer mosaico debemos obtener el número de imágenes
                ##debe ser para para que no quede ninguna zona hueca
                ##si se hace la division entera de la longitud de canales entre los trozos en que queremos partir la imagen, se obtiene el step
                ##es decir, la primera media sería 0 a step-1, la siguiente de step a step*2-1, etc, etc. En el final, pueden estar incluido más canales

                elems2d = [] 
                for trio_np in imgs_nifti:
                        
                        parte = []
                        for elem in trio_np:
                                # Dividimos el arreglo en base a su profundidad
                                trozos = np.array_split(elem, 4, 2)
                                
                                for trozo in trozos:
                                        # print(trozo.shape)
                                        print("control")
                                        trozo2d = np.mean(trozo, axis=2)
                                        parte.append(trozo2d)
                        elems2d.append(parte)
                            ###Si es 4, se apilan 2 a 2 horizontalmente y despues el resultado en vertical
                             ###si es 6, 3 y 3 y así sucesivamente
                for parte in elems2d:
                    print(len(elems2d))
                    for index in range(0,len(parte), modo*3):
                     imagen1 = np.stack((parte[index], parte[index+modo], parte[index+modo*2]), axis=2)
                     imagen2 = np.stack((parte[index+1], parte[index+1+modo], parte[index+1+modo*2]), axis=2)
                     imagen3 = np.stack((parte[index+2], parte[index+2+modo], parte[index+2+modo*2]), axis=2)
                     imagen4 = np.stack((parte[index+3], parte[index+3+modo], parte[index+3+modo*2]), axis=2)
                     imgs_final.append(imagen1)
                     imgs_final.append(imagen2)
                     imgs_final.append(imagen3)
                     imgs_final.append(imagen4)
                return imgs_final, pacs, hosp

def main():                     
        rutas_a90 = busqueda_rutas('A90')

        rutas_fdg = busqueda_rutas('FDG')

        rutas_1ct = busqueda_rutas('1CT')


        trios = formacion_trios(rutas_a90, rutas_fdg, rutas_1ct)
        
        imagenes_media_1, pac_media_1, hosp_media_1  = lectura_img(trios[0], 4)
        
        imagenes_media_2, pac_media_2, hosp_media_2  = lectura_img(trios[1], 4)
        
        ##Las imágenes en grupos de 4, pertenencen al mismo paciente y hospital
        

        ruta_guardado1 = "./IMGS_MEDIA_AMPL"

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
        
        for i in range(0, len(imagenes_media_1), 4):
              
                # displayIm(im[:,:,2])
                im1 = rangeDisplay01(imagenes_media_1[i], False)
                im2 = rangeDisplay01(imagenes_media_1[i+1], False)
                im3 = rangeDisplay01(imagenes_media_1[i+2], False)
                im4 = rangeDisplay01(imagenes_media_1[i+3], False)
                im1 = im1.astype(np.float32)
                im2 = im2.astype(np.float32)
                im3 = im3.astype(np.float32)
                im4 = im4.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == pac_media_1[i//4]) & (df['Centro'] == hosp_media_1[i//4]), 'Etiqueta'].values
                if (etiqueta != 2):
                        # im = im.transpose(1, 2, 0)           
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_1[i//4]}_{hosp_media_1[i//4]}_{int(etiqueta[0]+1)}_1.jpg", cv2.cvtColor(im1*255, cv2.COLOR_RGB2BGR))
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_1[i//4]}_{hosp_media_1[i//4]}_{int(etiqueta[0]+3)}_2.jpg", cv2.cvtColor(im2*255, cv2.COLOR_RGB2BGR))
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_1[i//4]}_{hosp_media_1[i//4]}_{int(etiqueta[0]+5)}_3.jpg", cv2.cvtColor(im3*255, cv2.COLOR_RGB2BGR))
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_1[i//4]}_{hosp_media_1[i//4]}_{int(etiqueta[0]+7)}_4.jpg", cv2.cvtColor(im4*255, cv2.COLOR_RGB2BGR))
        for i in range(0, len(imagenes_media_2), 4):
              
                # displayIm(im[:,:,2])
                im1 = rangeDisplay01(imagenes_media_2[i], False)
                im2 = rangeDisplay01(imagenes_media_2[i+1], False)
                im3 = rangeDisplay01(imagenes_media_2[i+2], False)
                im4 = rangeDisplay01(imagenes_media_2[i+3], False)
                im1 = im1.astype(np.float32)
                im2 = im2.astype(np.float32)
                im3 = im3.astype(np.float32)
                im4 = im4.astype(np.float32)
                etiqueta = df.loc[(df['Pac'] == pac_media_2[i//4]) & (df['Centro'] == hosp_media_2[i//4]), 'Etiqueta'].values
                if (etiqueta != 2):
                        # im = im.transpose(1, 2, 0)           
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_2[i//4]}_{hosp_media_2[i//4]}_{int(etiqueta[0]+1)}_1.jpg", cv2.cvtColor(im1*255, cv2.COLOR_RGB2BGR))
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_2[i//4]}_{hosp_media_2[i//4]}_{int(etiqueta[0]+3)}_2.jpg", cv2.cvtColor(im2*255, cv2.COLOR_RGB2BGR))
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_2[i//4]}_{hosp_media_2[i//4]}_{int(etiqueta[0]+5)}_3.jpg", cv2.cvtColor(im3*255, cv2.COLOR_RGB2BGR))
                        cv2.imwrite(f"{ruta_guardado1}/_{pac_media_2[i//4]}_{hosp_media_2[i//4]}_{int(etiqueta[0]+7)}_4.jpg", cv2.cvtColor(im4*255, cv2.COLOR_RGB2BGR))



####añadir etiqueta al nombre, las tres etiquetas del trio tienen que ser consistentes
if __name__ == "__main__":
        main()