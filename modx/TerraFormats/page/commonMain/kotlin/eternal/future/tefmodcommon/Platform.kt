package eternal.future.tefmodcommon

interface Platform {
    val name: String
}

expect fun getPlatform(): Platform