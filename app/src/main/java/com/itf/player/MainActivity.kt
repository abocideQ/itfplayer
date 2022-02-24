package com.itf.player

import android.Manifest
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import tv.itf.media.NativeItfMedia

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermissions(mPermissions, 100)
        CAO.copyAssetsDirToSDCard(this, "video", this.obbDir.absolutePath)
//        NativeItfMedia().init(this.obbDir.absolutePath + "/" + "video/one_piece.mp4")

    }

    private val mPermissions = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE,
        Manifest.permission.READ_EXTERNAL_STORAGE
    )


}