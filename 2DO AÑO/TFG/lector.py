import nibabel as nib
import matplotlib.pyplot as plt
import os

# Obtener el directorio actual
directorio_actual = os.getcwd()

# Listar los nombres de los archivos en el directorio actual
extension_deseada = '.nii'

nombres_de_archivos = [nombre for nombre in os.listdir(directorio_actual) if nombre.endswith(extension_deseada)]# Ruta al archivo NIfTI
ruta_nifti = nombres_de_archivos[0]

# Cargar la imagen NIfTI
imagen_nifti = nib.load(ruta_nifti)

# Obtener los datos de la imagen como un arreglo numpy
datos = imagen_nifti.get_fdata()

# Mostrar la imagen
plt.imshow(datos[:, :, 0], cmap='gray')  # Ajusta la selección de corte según tus necesidades
plt.title('Imagen NIfTI')
plt.colorbar()
plt.show()
