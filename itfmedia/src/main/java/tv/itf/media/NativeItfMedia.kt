package tv.itf.media

class NativeItfMedia {

    init {
        System.loadLibrary("lib_itfmedia")
    }

    fun source(url: String) {
        native_itf_source(url)
    }

    private external fun native_itf_source(url: String)
}