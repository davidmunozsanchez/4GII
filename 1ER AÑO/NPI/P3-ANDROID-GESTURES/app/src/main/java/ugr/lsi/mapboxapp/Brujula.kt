package com.example.try4

import android.hardware.GeomagneticField
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorManager
import android.location.Location
import android.location.LocationManager
import android.view.animation.Animation
import android.view.animation.RotateAnimation
import android.widget.ImageView
import android.widget.TextView

class Brujula {
    private val accelerometerReading = FloatArray(3)
    private val magnetometerReading = FloatArray(3)
    private lateinit var compassIV: ImageView
    private lateinit var  latitud_v: TextView
    private lateinit var  longitud_v: TextView


    private val rotationMatrix = FloatArray(9)
    private val orientationAngles = FloatArray(3)
    private var orientacionActual = 0f
    private var locationActual = Location(LocationManager.GPS_PROVIDER)
    private var locationTarget = Location("Kaba")

    private var targetFijado = false

    private var accelerometerReaded = false
    private var magnetometerReaded = false
    private var inicializada = false

    fun onCreate(img: ImageView, v1: TextView, v2: TextView){
        compassIV = img
        inicializada = true

        locationActual.altitude = 738.0

        locationActual.latitude = 37.180907
        locationActual.longitude = -3.600350

        locationTarget.altitude = locationActual.altitude

        if(!targetFijado){
            locationTarget.latitude = locationActual.latitude
            locationTarget.longitude = locationActual.longitude
        }

        latitud_v = v1
        longitud_v = v2

        longitud_v.text = "Longitud"
        latitud_v.text = "Latitud"

    }

    fun setTarget(lat: Double, lon: Double){
        locationTarget.latitude = lat
        locationTarget.longitude = lon
        targetFijado = true
    }

    fun setActual(lat: Double, lon: Double){
        locationActual.latitude = lat
        locationActual.longitude = lon
        longitud_v.text = locationActual.longitude.toString()
        latitud_v.text = locationActual.latitude.toString()
    }

    fun estaInicializada(): Boolean{ return inicializada }

    fun onSensorChanged(event: SensorEvent) {
        if (event.sensor.type == Sensor.TYPE_ACCELEROMETER) {
            System.arraycopy(event.values, 0, accelerometerReading, 0, accelerometerReading.size)
            accelerometerReaded = true
        } else if (event.sensor.type == Sensor.TYPE_MAGNETIC_FIELD) {
            System.arraycopy(event.values, 0, magnetometerReading, 0, magnetometerReading.size)
            magnetometerReaded = true
        }
        if (accelerometerReaded && magnetometerReaded) {
            // Update rotation matrix, which is needed to update orientation angles.
            SensorManager.getRotationMatrix(
                rotationMatrix,
                null,
                accelerometerReading,
                magnetometerReading
            )
            // "mRotationMatrix" now has up-to-date information.

            SensorManager.getOrientation(rotationMatrix, orientationAngles)

            // "mOrientationAngles" now has up-to-date information.

            var orientacionNueva = (Math.toDegrees(orientationAngles[0].toDouble()) + 360).toFloat() % 360
            orientacionNueva = getBearing(orientacionNueva)
            val rotacion = RotateAnimation(
                orientacionActual,
                orientacionNueva,
                Animation.RELATIVE_TO_SELF, 0.5f,
                Animation.RELATIVE_TO_SELF,
                0.5f
            )
            rotacion.duration = 1
            rotacion.fillAfter = true
            compassIV.startAnimation(rotacion)
            orientacionActual = orientacionNueva
        }
    }

    fun getBearing(nuevaOrientacion: Float): Float {
        var nuevaOrientacion = nuevaOrientacion
        val geomagneticField = GeomagneticField(
            locationActual.latitude.toFloat(),
            locationActual.longitude.toFloat(),
            locationActual.altitude.toFloat(),
            System.currentTimeMillis()
        )
        nuevaOrientacion -= geomagneticField.declination
        return locationActual.bearingTo(locationTarget) - nuevaOrientacion
    }

}