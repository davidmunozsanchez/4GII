function showError(divInput, divError, texto){
    divInput.style.border = '1px solid red';
    divError.innerHTML = `<img class="icon-error" src="./IMAGES/error-icon.svg"><p class="error"> ${texto} </p>`;
  }

  function notShowError(divInput, divError){
    divInput.style.border = '1px solid green';
    divError.innerHTML = `<img class="icon-error" src="./IMAGES/success-icon.svg">`;

  }
  
  function comprobarClave() {
    clave1 = document.getElementById('contra');
    clave2 = document.getElementById('contra2');
    error = document.getElementById('contra2Error');
    if (clave1.value == clave2.value && clave2.value.length <= 8 && clave2.value.length > 0) {
      notShowError(clave2, error);
      return true;
    }else if(clave2.value.length > 8){
      showError(clave2, error, 'La contraseña tiene más de 8 caracteres.');
      return false;
    } else if(clave1.value != clave2.value && clave2.value.length > 0) {
      showError(clave2, error, 'Las contraseñas no coinciden.');
      return false;
    }else if(clave2.value.length == 0){
      showError(clave2, error, 'Este campo es obligatorio.');
    }
  }

  function validateEmail() {

    // Get our input reference.
    var emailField = document.getElementById('correo');
    var error = document.getElementById('correoError')

    // Define our regular expression.
    var validEmail = /^\w+([.-_+]?\w+)*@\w+([.-]?\w+)*(\.\w{2,10})+$/;

    // Using test we can check if the text match the pattern
    if (validEmail.test(emailField.value)) {
      notShowError(emailField, error);  
      return true;
    } else if(!validEmail.test(emailField.value) && emailField.value.length > 0) {
      showError(emailField, error, 'El email introducido no tiene un formato válido.');
      return false;
    }
    if(emailField.value.length == 0){
      showError(emailField, error, 'Este campo es obligatorio.');
      return false;
    }
  }
  function validateNombre() {
    var nombreField = document.getElementById('nombres');
    var error = document.getElementById('nameError');
    var validNombre = /^[a-zA-Z]+$/; 

    if(validNombre.test(nombreField.value)){
      notShowError(nombreField, error);
      return true;
    } else if(!validNombre.test(nombreField.value) && nombreField.value.length > 0){
      showError(nombreField, error, 'El nombre solo puede contener caracteres latinos.');
      return false;
    }

    if (nombreField.value.length == 0){
      showError(nombreField, error, 'Este campo es obligatorio.');
      return false;
    }
  } 

  function validateClave() {
    var contrasenia = document.getElementById('contra').value;
    var error = document.getElementById('contraError');
    var tamanio_contrasenia = contrasenia.length;
    console.log(tamanio_contrasenia);
    if(tamanio_contrasenia <= 8 && tamanio_contrasenia > 0){
      notShowError(document.getElementById('contra'), error);
      return true;
    }
    else if (tamanio_contrasenia > 8){
      showError(document.getElementById('contra'), error, 'La clave no puede tener más de 8 caracteres.');
      return false;
    }
    else if(tamanio_contrasenia == 0){
      showError(document.getElementById('contra'), error, 'Este campo es obligatorio.');
      return false;
    }
  }


  function funEnvio(){
    
    nombre = validateNombre();
    email = validateEmail();
    contra = validateClave();
    contra2 = comprobarClave();

    if (nombre && email && contra && contra2){
      alert("Datos enviados correctamente.");
    }
    /*Aqui comprobaria que ningun campo es vacío ni erroneo Si son true los otros validate vemos si esta vacio y si todo esta bien enviamos y mostramos alert*/
  }