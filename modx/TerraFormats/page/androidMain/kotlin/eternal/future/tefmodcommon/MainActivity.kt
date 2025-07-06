package eternal.future.tefmodcommon

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.material.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.tooling.preview.Preview
import java.io.File

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContent {
            MaterialTheme {
                val a = EFModPage()
                a.params = mapOf<String, Any>(
                    "private-path" to File(this.getExternalFilesDir(null), "").absolutePath
                )
                a.Content()
            }
        }
        //AndroidImageZipProcessor(this).processZip()
        File(this.getExternalFilesDir(null), "").mkdirs()
    }
}

@Preview
@Composable
fun AppAndroidPreview() {
    App()
}

