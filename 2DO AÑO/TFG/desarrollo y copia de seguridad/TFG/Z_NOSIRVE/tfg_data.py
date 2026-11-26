import os
import nibabel as nib
import matplotlib.pyplot as plt
import numpy as np

def obtener_rutas_carpeta(carpeta):
    rutas_archivos = []
    for nombre_archivo in os.listdir(carpeta):
        ruta_archivo = os.path.join(carpeta, nombre_archivo)
        if os.path.isfile(ruta_archivo):
            rutas_archivos.append(ruta_archivo)
    return rutas_archivos

def read_and_show_nifti(nifti_path):
    imagen_nifti = nib.load(nifti_path)
    # Obtener los datos de la imagen como un arreglo numpy
    datos = imagen_nifti.get_fdata()

    # # Mostrar la imagen
    # plt.imshow(datos[:, :, 0], cmap='gray')  # Ajusta la selección de corte según tus necesidades
    # plt.title('Imagen NIfTI')
    # plt.colorbar()
    # plt.show()

    return datos

def main():
    PATH1 = "./NIFTI NO PREPROC"
    PATH2 = "./NIFTI PREPROC"
    PATH3 = "./IMAGENES_SEGMENTAR"

    rutas_no_preproc = obtener_rutas_carpeta(PATH1)
    rutas_preproc = obtener_rutas_carpeta(PATH2)
    

    # Obtener la lista de archivos en la carpeta
    archivos = os.listdir(PATH3)

    # Iterar sobre los archivos y eliminar los que no empiecen con 'm'
    for archivo in archivos:
        if not archivo.startswith('m'):
            os.remove(os.path.join(PATH3, archivo))
            print(f"Eliminando {archivo}")

    rutas_nuevas = obtener_rutas_carpeta(PATH3)
    #print(rutas_preproc)
    prueba_pre = read_and_show_nifti(rutas_preproc[0])
    ##Comprobacion que las nuevas tienen la misma forma que las preproc
    for path in rutas_nuevas:
        leido = read_and_show_nifti(path)
        print(leido.shape)
        print(prueba_pre.shape)
        if (leido.shape != prueba_pre.shape):
            print("No igual")
    # prueba = read_and_show_nifti(rutas_no_preproc[458])
    # print(rutas_no_preproc[458])
    # print(prueba)

    # indices  = np.where(prueba != 0)
    # print(indices[0][0])
    # print(prueba[indices[0][0], indices[1][0], indices[2][0]])
if __name__ == "__main__":
        main()