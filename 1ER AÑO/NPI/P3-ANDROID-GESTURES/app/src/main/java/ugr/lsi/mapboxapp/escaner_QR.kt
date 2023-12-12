package com.example.try4

import android.app.Activity
import android.app.SearchManager
import android.content.Context
import android.content.DialogInterface
import android.content.Intent
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.ImageButton
import android.widget.ImageView
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import com.google.zxing.integration.android.IntentIntegrator

class escaner_QR : AppCompatActivity(){
    private lateinit var qrButton: ImageButton
    private var inicializado = false

    fun onCreate(qr:ImageButton, activity: Activity) {
        qrButton = qr
        qrButton.setOnClickListener {
            val intentIntegrator = IntentIntegrator(activity)
            intentIntegrator.setDesiredBarcodeFormats((IntentIntegrator.QR_CODE))
            intentIntegrator.initiateScan()
        }
        inicializado = true
    }

    fun estaInicializado(): Boolean {return inicializado}

    fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?, context: Context){
        val result = IntentIntegrator.parseActivityResult(resultCode, data)
        if (result != null) {
            if (result.contents=="Cartuja" || result.contents=="Aynadamar" || result.contents=="Fuentenueva") {
                AlertDialog.Builder(context)
                    .setMessage("Quieres ver el menu de ${result.contents}?")
                    .setPositiveButton(
                        "Si",
                        DialogInterface.OnClickListener { dialogInterface, i ->
                            val intent = Intent(Intent.ACTION_WEB_SEARCH)
                            intent.putExtra(SearchManager.QUERY, "https://scu.ugr.es/pages/menu/comedor#__doku_menu_semanal_comedores_de_fuentenueva_aynadamar_y_cartuja_-carlos_v")
                            context.startActivity(intent)
                        })
                    .setNegativeButton(
                        "No",
                        DialogInterface.OnClickListener { dialogInterface, i -> })
                    .create()
                    .show()
            }else if (result.contents=="PTS"){
                AlertDialog.Builder(context)
                    .setMessage("Quieres ver el menu de ${result.contents}?")
                    .setPositiveButton(
                        "Si",
                        DialogInterface.OnClickListener { dialogInterface, i ->
                            val intent = Intent(Intent.ACTION_WEB_SEARCH)
                            intent.putExtra(SearchManager.QUERY, "https://scu.ugr.es/pages/menu/comedor#__doku_menu_semanal_comedor_del_parque_tecnologico_de_ciencias_de_la_salud_pts")
                            context.startActivity(intent)
                        })
                    .setNegativeButton(
                        "No",
                        DialogInterface.OnClickListener { dialogInterface, i -> })
                    .create()
                    .show()
            }else{
                AlertDialog.Builder(context)
                    .setMessage("QR no reconocido")
                    .setPositiveButton(
                        "OK",
                        DialogInterface.OnClickListener { dialogInterface, i -> })
                    .create()
                    .show()
            }
        }
    }

}