package eternal.future.texturepack

interface Platform {
    val name: String
}

expect fun getPlatform(): Platform