package ugr.lsi.mapboxapp


import android.Manifest
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.drawable.BitmapDrawable
import android.graphics.drawable.Drawable
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import android.widget.Toast
import androidx.annotation.DrawableRes
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.content.res.AppCompatResources
import androidx.core.app.ActivityCompat
import com.example.try4.Brujula
import com.example.try4.escaner_QR
import com.mapbox.android.core.location.LocationEngine
import com.mapbox.android.core.location.LocationEngineCallback
import com.mapbox.android.core.location.LocationEngineProvider
import com.mapbox.android.core.location.LocationEngineResult
import com.mapbox.android.gestures.MoveGestureDetector
import com.mapbox.geojson.Point
import com.mapbox.maps.CameraOptions
import com.mapbox.maps.MapView
import com.mapbox.maps.Style
import com.mapbox.maps.extension.style.expressions.dsl.generated.interpolate
import com.mapbox.maps.plugin.LocationPuck2D
import com.mapbox.maps.plugin.annotation.annotations
import com.mapbox.maps.plugin.annotation.generated.OnPointAnnotationClickListener
import com.mapbox.maps.plugin.annotation.generated.PointAnnotationManager
import com.mapbox.maps.plugin.annotation.generated.PointAnnotationOptions
import com.mapbox.maps.plugin.annotation.generated.createPointAnnotationManager
import com.mapbox.maps.plugin.gestures.OnMoveListener
import com.mapbox.maps.plugin.gestures.gestures
import com.mapbox.maps.plugin.locationcomponent.*
import java.lang.ref.WeakReference

class MainActivity() : AppCompatActivity(), SensorEventListener {
    private var resume = false;
    private var first = true

    private lateinit var locationComponentPlugin: LocationComponentPlugin
    private var mapView: MapView? = null
    private lateinit var sensorManager: SensorManager
    private var ejex: Float = 0.0F
    private var ejey: Float = 0.0F
    private var ejez: Float = 0.0F
    private var proximidad: Float = 0.0F
    private var prim : PointAnnotationManager?=null
    private var sec : PointAnnotationManager?=null
    private var ter : PointAnnotationManager?=null
    private var cuar: PointAnnotationManager?=null

    private lateinit var locationEngine : LocationEngine
    private var punto = Point.fromLngLat(0.45,54.89)

    private lateinit var locationPermissionHelper: LocationPermissionHelper

    private val locationTarget = android.location.Location("Kaba")

    private lateinit var brujula: Brujula

    private lateinit var escaner: escaner_QR


    private val onIndicatorBearingChangedListener = OnIndicatorBearingChangedListener {
        mapView?.getMapboxMap()?.setCamera(CameraOptions.Builder().bearing(it).build())


    }
    private val onIndicatorPositionChangedListener = OnIndicatorPositionChangedListener {
        OnIndicatorPositionChangedListener {
            mapView?.getMapboxMap()?.setCamera(CameraOptions.Builder().center(it).build())
            mapView?.gestures?.focalPoint = mapView?.getMapboxMap()?.pixelForCoordinate(it)

        }
    }
    private val onMoveListener = object : OnMoveListener {

        override fun onMoveBegin(detector: MoveGestureDetector) {
            onCameraTrackingDismissed()


        }

        override fun onMove(detector: MoveGestureDetector): Boolean {
            return false
        }

        override fun onMoveEnd(detector: MoveGestureDetector) {}
    }

    private fun onCameraTrackingDismissed() {
        Toast.makeText(this, "onCameraTrackingDismissed", Toast.LENGTH_SHORT).show()
        mapView?.location
            ?.removeOnIndicatorPositionChangedListener(onIndicatorPositionChangedListener)
        mapView?.location
            ?.removeOnIndicatorBearingChangedListener(onIndicatorBearingChangedListener)
        mapView?.gestures?.removeOnMoveListener(onMoveListener)
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        Log.d("2", "Hola")
        super.onCreate(savedInstanceState)


        this.sensorManager = getSystemService(SENSOR_SERVICE) as SensorManager

        //deviceSensors = sensorManager.getSensorList(Sensor.TYPE_ALL)

        setContentView(R.layout.map_layout)
        mapView = findViewById(R.id.mapView)


        this.locationEngine = LocationEngineProvider.getBestLocationEngine(this)


        locationPermissionHelper = LocationPermissionHelper(WeakReference(this))
        locationPermissionHelper.checkPermissions {

            onMapReady()
        }

        //setContentView(R.layout.activity_main)
        setSupportActionBar(findViewById(R.id.toolbar))

        brujula = Brujula()
        escaner = escaner_QR()



    }

    private fun onMapReady() {

        mapView?.getMapboxMap()?.setCamera(
            CameraOptions.Builder()
                .zoom(14.0)
                .build()
        )
        mapView?.getMapboxMap()?.loadStyleUri(
            Style.MAPBOX_STREETS
        ) {
            initLocationComponent()
            setupGesturesListener()
        }


    }

    private fun setupGesturesListener() {
        mapView?.gestures?.addOnMoveListener(onMoveListener)
    }

    private fun initLocationComponent() {
        val locationComponentPlugin = mapView?.location
        if (locationComponentPlugin != null) {
            locationComponentPlugin.updateSettings {
                this.enabled = true
                this.locationPuck = LocationPuck2D(
                    bearingImage = AppCompatResources.getDrawable(
                        this@MainActivity,
                        com.mapbox.maps.R.drawable.mapbox_user_puck_icon,
                    ),
                    shadowImage = AppCompatResources.getDrawable(
                        this@MainActivity,
                        com.mapbox.maps.R.drawable.mapbox_user_puck_icon,
                    ),
                    scaleExpression = interpolate {
                        linear()
                        zoom()
                        stop {
                            literal(0.0)
                            literal(0.6)
                        }
                        stop {
                            literal(20.0)
                            literal(1.0)
                        }
                    }.toJson()
                )
            }
        }
        if (locationComponentPlugin != null) {
            locationComponentPlugin.addOnIndicatorPositionChangedListener(onIndicatorPositionChangedListener)
        }
        if (locationComponentPlugin != null) {
            locationComponentPlugin.addOnIndicatorBearingChangedListener(onIndicatorBearingChangedListener)
        }
    }


    private fun addAnnotationToMap(lon: Double, lat: Double, texto: String, prompt: String ):PointAnnotationManager? {
    // Create an instance of the Annotation API and get the PointAnnotationManager.
        var retorno : PointAnnotationManager? = null
        val target_bitmap = bitmapFromDrawableRes(this@MainActivity, R.drawable.red_marker)

        bitmapFromDrawableRes(
            this@MainActivity,
            R.drawable.red_marker
        )?.let {
            val annotationApi = mapView?.annotations
            val pointAnnotationManager = annotationApi?.createPointAnnotationManager(mapView!!)
            pointAnnotationManager?.addClickListener(OnPointAnnotationClickListener {
                //Cambiar objetivo del puntero
                locationTarget.latitude = lat
                locationTarget.longitude = lon
                brujula.setTarget(lat, lon)
                //Seleccionar ese comedor

                annotationApi.cleanup()
                addAnnotationToMap(lon, lat, texto, prompt)

                Toast.makeText(this@MainActivity, prompt, Toast.LENGTH_SHORT).show()
                true
            })    // Set options for the resulting symbol layer.

            val pointAnnotationOptions: PointAnnotationOptions = PointAnnotationOptions()
    // Define a geographic coordinate.
                .withPoint(Point.fromLngLat(lon, lat))
                // Specify the bitmap you assigned to the point annotation
    // The bitmap will be added to map style automatically.
                .withTextField(texto)

            pointAnnotationOptions.withIconImage(it)
            if(lon == locationTarget.longitude && lat == locationTarget.latitude) target_bitmap?.let { it1 ->
                pointAnnotationOptions.withIconImage(
                    it1
                )
            }


            // Add the resulting pointAnnotation to the map.
            pointAnnotationManager?.create(pointAnnotationOptions)
            retorno = pointAnnotationManager
        }

        return retorno

    }

    private fun bitmapFromDrawableRes(context: Context, @DrawableRes resourceId: Int) =
        convertDrawableToBitmap(AppCompatResources.getDrawable(context, resourceId))

    private fun convertDrawableToBitmap(sourceDrawable: Drawable?): Bitmap? {
        if (sourceDrawable == null) {
            return null
        }
        return if (sourceDrawable is BitmapDrawable) {
            sourceDrawable.bitmap
        } else {
    // copying drawable object to not manipulate on the same reference
            val constantState = sourceDrawable.constantState ?: return null
            val drawable = constantState.newDrawable().mutate()
            val bitmap: Bitmap = Bitmap.createBitmap(
                drawable.intrinsicWidth, drawable.intrinsicHeight,
                Bitmap.Config.ARGB_8888
            )
            val canvas = Canvas(bitmap)
            drawable.setBounds(0, 0, canvas.width, canvas.height)
            drawable.draw(canvas)
            bitmap
        }
    }


    override fun onStart() {
        super.onStart()
        mapView?.onStart()



    }

    override fun onStop() {
        super.onStop()
        mapView?.onStop()
    }

    override fun onLowMemory() {
        super.onLowMemory()
        mapView?.onLowMemory()
    }

    override fun onDestroy() {
        super.onDestroy()
        mapView?.location
            ?.removeOnIndicatorBearingChangedListener(onIndicatorBearingChangedListener)
        mapView?.location
            ?.removeOnIndicatorPositionChangedListener(onIndicatorPositionChangedListener)
        mapView?.gestures?.removeOnMoveListener(onMoveListener)
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        locationPermissionHelper.onRequestPermissionsResult(requestCode, permissions, grantResults)


    }
    override fun onSensorChanged(p0: SensorEvent) {
        var proximidad_b : Boolean = false
        var giro : Boolean = false
        when (p0?.sensor?.type) {
            Sensor.TYPE_GYROSCOPE -> {
                ejex = p0.values[0]
                ejey = p0.values[1]
                ejez = p0.values[2]
                giro = true
            }
            Sensor.TYPE_PROXIMITY -> {
                proximidad = p0.values[0]
                proximidad_b = true
            }
        }
        if(giro)
        {
            loadStile()
            giro = false
        }

        if (proximidad_b){
            if(proximidad == 0.0F){
                var hola : String = proximidad.toString()
                Log.d("1",hola)
                if (ActivityCompat.checkSelfPermission(
                        this,
                        Manifest.permission.ACCESS_FINE_LOCATION
                    ) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(
                        this,
                        Manifest.permission.ACCESS_COARSE_LOCATION
                    ) != PackageManager.PERMISSION_GRANTED
                ) {
                    // TODO: Consider calling
                    //    ActivityCompat#requestPermissions
                    // here to request the missing permissions, and then overriding
                    //   public void onRequestPermissionsResult(int requestCode, String[] permissions,
                    //                                          int[] grantResults)
                    // to handle the case where the user grants the permission. See the documentation
                    // for ActivityCompat#requestPermissions for more details.
                    return
                }
                locationEngine.getLastLocation(object : LocationEngineCallback<LocationEngineResult> {
                    override fun onSuccess(result: LocationEngineResult?) {
                        val location = (result?.locations?.lastOrNull() ?: result?.lastLocation)?.let { location ->
                            Point.fromLngLat(location.longitude, location.latitude)

                        }

                        punto = location

                        //AAAA


                        return
                    }

                    override fun onFailure(exception: Exception) {
                        return
                    }
                })
                //Centramos camara en nuestra localizacion al hacer uso del sensor de proximidad

                mapView?.getMapboxMap()?.setCamera(
                    CameraOptions.Builder()
                        .zoom(16.0)
                        .center(punto)
                        .build()
                )
            }

            proximidad_b = false
        }

        if(brujula.estaInicializada()) {
            brujula.onSensorChanged(p0)
            locationEngine.getLastLocation(object : LocationEngineCallback<LocationEngineResult> {
                override fun onSuccess(result: LocationEngineResult?) {
                    val location = (result?.locations?.lastOrNull() ?: result?.lastLocation)?.let { location ->
                        Point.fromLngLat(location.longitude, location.latitude)

                    }

                    if (location != null) {
                        brujula.setActual(location.latitude(), location.longitude())
                    }

                    return
                }

                override fun onFailure(exception: Exception) {
                    return
                }
            })

        }

    }

    fun loadStile(){
        mapView?.getMapboxMap()?.loadStyleUri(Style.MAPBOX_STREETS,
            object : Style.OnStyleLoaded{
                override fun onStyleLoaded(style: Style) {

                    if ((ejez < 0.1F && ejex > 0.5F && ejey < 0.1F) && !resume) {
                        //prim?.deleteAll()
                        //sec?.deleteAll()
                        //ter?.deleteAll()
                        //cuar?.deleteAll()

                        prim = addAnnotationToMap(
                            -3.597951,
                            37.192964,
                            "COMEDOR CARTUJA",
                            "Horario L-V DE 13:30 A 15:30"
                        );
                        sec = addAnnotationToMap(
                            -3.623841,
                            37.197229,
                            "COMEDOR AYNADAMAR",
                            "Horario L-V DE 13:30 A 15:30"
                        );
                        ter = addAnnotationToMap(
                            -3.603857,
                            37.148141,
                            "COMEDOR PTS",
                            "Horario L-V DE 13:30 A 15:30"
                        );
                        cuar = addAnnotationToMap(
                            -3.605681,
                            37.182809,
                            "COMEDOR FUENTENUEVA",
                            "Horario L-S DE 13:30 A 15:30"
                        );


                        resume = true

                    }
                    if (resume && ejez > 0.5F && ejex < 0.1F && ejey < 0.1F){
                        prim?.deleteAll()
                        sec?.deleteAll()
                        ter?.deleteAll()
                        cuar?.deleteAll()
                        resume = false


                    }




                }
            })
    }

    override fun onAccuracyChanged(p0: Sensor?, p1: Int) {
        return
    }

    override fun onResume() {
        super.onResume()
        sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE)?.also { accelerometer ->
            sensorManager.registerListener(
                this,
                accelerometer,
                SensorManager.SENSOR_DELAY_NORMAL)
        }

        sensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY)?.also { accelerometer ->
            sensorManager.registerListener(
                this,
                accelerometer,
                SensorManager.SENSOR_DELAY_NORMAL)
        }

        sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER)?.also { accelerometer ->
            sensorManager.registerListener(
                this,
                accelerometer,
                SensorManager.SENSOR_DELAY_NORMAL)
        }
        sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD)?.also { magneticField ->
            sensorManager.registerListener(
                this,
                magneticField,
                SensorManager.SENSOR_DELAY_NORMAL)
        }

    }

    override fun onPause() {
        super.onPause()
        sensorManager.unregisterListener(this)
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu, menu)
        return super.onCreateOptionsMenu(menu)
    }


    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        when(item.itemId) {
            R.id.compass_item -> {
                setContentView(R.layout.compass_layout)
                setSupportActionBar(findViewById(R.id.toolbar))
                //if(!brujula.estaInicializada()) {
                    brujula.onCreate(findViewById(R.id.compass_image), findViewById(R.id.latitud_view), findViewById(R.id.longitud_view))
                //}
            }
            R.id.qr_item -> {
                setContentView(R.layout.qr_layout)
                setSupportActionBar(findViewById(R.id.toolbar))
                if(!escaner.estaInicializado()) escaner.onCreate(findViewById(R.id.qr_button), this)
            }
            R.id.map_item -> {
                startActivity(Intent(this, MainActivity::class.java))
            }

        }
        Toast.makeText(this, "has pulsado el boton " + item.title, Toast.LENGTH_SHORT).show()
        return super.onOptionsItemSelected(item)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        escaner.onActivityResult(requestCode, resultCode, data, this)
    }

}

