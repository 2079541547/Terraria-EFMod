package eternal.future.texturepack

import android.annotation.SuppressLint
import android.graphics.BitmapFactory
import android.net.Uri
import androidx.activity.compose.rememberLauncherForActivityResult
import androidx.activity.result.contract.ActivityResultContracts
import androidx.compose.animation.animateContentSize
import androidx.compose.animation.core.Spring
import androidx.compose.animation.core.spring
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.automirrored.filled.ArrowForward
import androidx.compose.material.icons.filled.Add
import androidx.compose.material.icons.filled.Delete
import androidx.compose.material.icons.filled.ExpandMore
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.material3.Switch
import androidx.compose.material3.SwitchDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.draw.rotate
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.unit.dp
import org.json.JSONArray
import org.json.JSONObject
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.util.zip.ZipEntry
import java.util.zip.ZipInputStream

class EFModPage {

    var params: Map<String, *> = emptyMap<String, Any>()

    data class PackInfo(
        val name: String,
        val author: String,
        val description: String,
        val version: Version,
        val filePath: String,
        val iconBitmap: android.graphics.Bitmap?,
        val isEnabled: Boolean,
        val type: Int
    ) {
        data class Version(
            val major: Int,
            val minor: Int
        ) {
            override fun toString(): String = "$major.$minor"
        }
    }

    @OptIn(ExperimentalMaterial3Api::class)
    @SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
    val Content: (@Composable () -> Unit) = {
        val context = LocalContext.current
        var packInfos by remember { mutableStateOf<List<PackInfo>>(emptyList()) }
        var isLoading by remember { mutableStateOf(false) }
        var errorMessage by remember { mutableStateOf<String?>(null) }

        val configFile = File(params["private-path"] as String, "config.json")

        fun readPackInfoFromZip(zipFile: File, type: Int): PackInfo {
            val inputStream = zipFile.inputStream().buffered()
            val zipInputStream = ZipInputStream(inputStream)

            var jsonStr: String? = null
            var iconBitmap: android.graphics.Bitmap? = null

            try {
                var entry: ZipEntry?
                while (zipInputStream.nextEntry.also { entry = it } != null) {
                    when {
                        type == 0 && entry?.name.equals("pack.json", ignoreCase = true) -> {
                            jsonStr = zipInputStream.readBytes().toString(Charsets.UTF_8)
                        }
                        type == 1 && entry?.name.equals("Settings.json", ignoreCase = true) -> {
                            jsonStr = zipInputStream.readBytes().toString(Charsets.UTF_8)
                        }
                        (type == 0 && entry?.name.equals("icon.png", ignoreCase = true)) ||
                                (type == 1 && entry?.name.equals("Icon.png", ignoreCase = true)) -> {
                            val bytes = zipInputStream.readBytes()
                            iconBitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.size)
                        }
                    }
                    zipInputStream.closeEntry()
                }

                if (jsonStr == null) throw IOException("未找到材质包信息文件")

                val json = JSONObject(jsonStr)

                return when (type) {
                    0 -> PackInfo(
                        name = json.getString("Name"),
                        author = json.getString("Author"),
                        description = json.getString("Description"),
                        version = PackInfo.Version(
                            major = json.getJSONObject("Version").optInt("major"),
                            minor = json.getJSONObject("Version").optInt("minor")
                        ),
                        filePath = zipFile.absolutePath,
                        iconBitmap = iconBitmap,
                        isEnabled = false,
                        type = type
                    )
                    1 -> {
                        val description = if (json.has("descriptionEnglish")) {
                            json.getString("descriptionEnglish")
                        } else if (json.has("descriptionRussian")) {
                            json.getString("descriptionRussian")
                        } else {
                            "暂无描述"
                        }

                        val authors = if (json.has("authors")) {
                            try {
                                val authorsArray = json.getJSONArray("authors")
                                if (authorsArray.length() > 0) {
                                    // 遍历所有作者，提取名字并用逗号连接
                                    val authorNames = mutableListOf<String>()
                                    for (i in 0 until authorsArray.length()) {
                                        val author = authorsArray.getJSONObject(i)
                                        authorNames.add(author.optString("name", "未知作者"))
                                    }
                                    authorNames.joinToString(", ") // 例如："IReapTerror, 用户2, 用户3"
                                } else {
                                    "未知作者"
                                }
                            } catch (e: Exception) {
                                "未知作者"
                            }
                        } else {
                            "未知作者"
                        }

                        PackInfo(
                            name = json.getString("title").trim('"'),
                            author = authors,  // 现在包含所有作者的名字
                            description = description,
                            version = PackInfo.Version(
                                major = json.optInt("version"),
                                minor = 0
                            ),
                            filePath = zipFile.absolutePath,
                            iconBitmap = iconBitmap,
                            isEnabled = false,
                            type = type
                        )
                    }
                    else -> throw IOException("未知的材质包类型")
                }
            } finally {
                zipInputStream.close()
                inputStream.close()
            }
        }

        fun detectPackType(zipFile: File): Int {
            val inputStream = zipFile.inputStream().buffered()
            val zipInputStream = ZipInputStream(inputStream)

            try {
                var entry: ZipEntry?
                while (zipInputStream.nextEntry.also { entry = it } != null) {
                    when {
                        entry?.name.equals("pack.json", ignoreCase = true) -> return 0
                        entry?.name.equals("Settings.json", ignoreCase = true) -> return 1
                    }
                    zipInputStream.closeEntry()
                }
                return -1
            } finally {
                zipInputStream.close()
                inputStream.close()
            }
        }

        fun loadPackInfos() {
            isLoading = true
            errorMessage = null

            try {
                val infos = mutableListOf<PackInfo>()
                val packsDir = File(params["private-path"] as String)

                if (!packsDir.exists()) {
                    packsDir.mkdirs()
                    packInfos = emptyList()
                    return
                }

                if (configFile.exists()) {
                    val jsonArray = JSONArray(configFile.readText())

                    for (i in 0 until jsonArray.length()) {
                        val item = jsonArray.getJSONObject(i)
                        val zipFile = File(packsDir, item.getString("file"))

                        if (!zipFile.exists()) continue

                        val isEnabled = item.getBoolean("enable")
                        val type = item.getInt("type")

                        try {
                            val packData = readPackInfoFromZip(zipFile, type)
                            infos.add(packData.copy(isEnabled = isEnabled))
                        } catch (e: Exception) {
                            e.printStackTrace()
                            errorMessage = "加载材质包 ${zipFile.name} 失败: ${e.message}"
                            continue
                        }
                    }
                }

                packInfos = infos.sortedByDescending { it.isEnabled }
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "加载配置失败: ${e.message}"
            } finally {
                isLoading = false
            }
        }

        fun saveConfig(packs: List<PackInfo>) {
            try {
                val jsonArray = JSONArray()
                packs.forEach { pack ->
                    jsonArray.put(JSONObject().apply {
                        put("file", File(pack.filePath).name)
                        put("enable", pack.isEnabled)
                        put("type", pack.type)
                    })
                }
                configFile.writeText(jsonArray.toString())
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "保存配置失败: ${e.message}"
            }
        }

        fun installPack(uri: Uri) {
            isLoading = true
            errorMessage = null

            try {
                val packsDir = File(params["private-path"] as String)
                if (!packsDir.exists()) packsDir.mkdirs()

                val fileName = "pack_${System.currentTimeMillis()}.zip"
                val destFile = File(packsDir, fileName)

                context.contentResolver.openInputStream(uri)?.use { input ->
                    FileOutputStream(destFile).use { output ->
                        input.copyTo(output)
                    }
                }

                val type = detectPackType(destFile)
                if (type == -1) {
                    destFile.delete()
                    throw IOException("无效的材质包格式")
                }

                val newPack = readPackInfoFromZip(destFile, type)
                saveConfig(packInfos + newPack)
                loadPackInfos()
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "安装失败: ${e.message}"

                // 清理无效文件
                val packsDir = File(params["private-path"] as String)
                packsDir.listFiles()?.forEach { file ->
                    if (file.name.startsWith("pack_") && file.name.endsWith(".zip") &&
                        file.length() == 0L) {
                        file.delete()
                    }
                }
            } finally {
                isLoading = false
            }
        }

        fun deletePack(pack: PackInfo) {
            isLoading = true
            try {
                File(pack.filePath).delete()
                val remainingPacks = packInfos.filter { it.filePath != pack.filePath }
                saveConfig(remainingPacks)
                packInfos = remainingPacks
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "删除失败: ${e.message}"
            } finally {
                isLoading = false
            }
        }

        fun togglePack(pack: PackInfo, enabled: Boolean) {
            isLoading = true
            try {
                val updatedPacks = packInfos.map {
                    if (it.filePath == pack.filePath) it.copy(isEnabled = enabled) else it
                }
                saveConfig(updatedPacks)
                packInfos = updatedPacks
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "更新失败: ${e.message}"
            } finally {
                isLoading = false
            }
        }

        val selectFileLauncher = rememberLauncherForActivityResult(
            ActivityResultContracts.GetContent()
        ) { uri ->
            if (uri != null) installPack(uri)
        }

        // 初始加载
        if (packInfos.isEmpty() && !isLoading) {
            loadPackInfos()
        }

        MaterialTheme {
            Scaffold(
                floatingActionButton = {
                    FloatingActionButton(
                        onClick = { selectFileLauncher.launch("*/*") },
                        containerColor = MaterialTheme.colorScheme.primary,
                        contentColor = MaterialTheme.colorScheme.onPrimary
                    ) {
                        Icon(Icons.Default.Add, "添加材质包")
                    }
                },
                containerColor = MaterialTheme.colorScheme.background
            ) { paddingValues ->
                Surface(
                    modifier = Modifier
                        .fillMaxSize()
                        .padding(paddingValues),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Box(modifier = Modifier.fillMaxSize()) {
                        when {
                            isLoading -> {
                                CircularProgressIndicator(
                                    modifier = Modifier.align(Alignment.Center),
                                    color = MaterialTheme.colorScheme.primary
                                )
                            }
                            errorMessage != null -> {
                                Text(
                                    text = errorMessage!!,
                                    color = MaterialTheme.colorScheme.error,
                                    modifier = Modifier
                                        .align(Alignment.TopCenter)
                                        .padding(16.dp)
                                )
                            }
                            packInfos.isEmpty() -> {
                                Column(
                                    modifier = Modifier
                                        .fillMaxSize()
                                        .padding(16.dp),
                                    verticalArrangement = Arrangement.Center,
                                    horizontalAlignment = Alignment.CenterHorizontally
                                ) {
                                    Text(
                                        text = "暂无材质包",
                                        style = MaterialTheme.typography.titleLarge,
                                        color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.6f)
                                    )
                                    Spacer(modifier = Modifier.height(8.dp))
                                    Text(
                                        text = "点击右下角按钮添加",
                                        style = MaterialTheme.typography.bodyMedium,
                                        color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.4f)
                                    )
                                }
                            }
                            else -> {
                                LazyColumn(
                                    modifier = Modifier
                                        .fillMaxSize()
                                        .padding(horizontal = 16.dp),
                                    verticalArrangement = Arrangement.spacedBy(12.dp)
                                ) {
                                    items(packInfos, key = { it.filePath }) { pack ->
                                        ExpandablePackCard(
                                            pack = pack,
                                            onDelete = { deletePack(it) },
                                            onToggle = { p, enabled -> togglePack(p, enabled) }
                                        )
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    @OptIn(ExperimentalMaterial3Api::class)
    @Composable
    private fun ExpandablePackCard(
        pack: PackInfo,
        onDelete: (PackInfo) -> Unit,
        onToggle: (PackInfo, Boolean) -> Unit
    ) {
        var expanded by remember { mutableStateOf(false) }
        var isEnabled by remember { mutableStateOf(pack.isEnabled) }

        Card(
            modifier = Modifier
                .fillMaxWidth()
                .animateContentSize(
                    animationSpec = spring(
                        dampingRatio = Spring.DampingRatioMediumBouncy,
                        stiffness = Spring.StiffnessLow
                    )
                ),
            shape = RoundedCornerShape(12.dp),
            colors = CardDefaults.cardColors(
                containerColor = MaterialTheme.colorScheme.surfaceVariant,
                contentColor = MaterialTheme.colorScheme.onSurfaceVariant
            ),
            elevation = CardDefaults.cardElevation(defaultElevation = 2.dp)
        ) {
            Column(
                modifier = Modifier
                    .fillMaxWidth()
                    .clickable { expanded = !expanded }
                    .padding(16.dp)
            ) {
                Row(
                    verticalAlignment = Alignment.CenterVertically,
                    modifier = Modifier.fillMaxWidth()
                ) {
                    pack.iconBitmap?.asImageBitmap()?.let { bitmap ->
                        Image(
                            bitmap = bitmap,
                            contentDescription = null,
                            modifier = Modifier
                                .size(48.dp)
                        )
                        Spacer(modifier = Modifier.width(12.dp))
                    } ?: Box(
                        modifier = Modifier
                            .size(48.dp)
                            .clip(CircleShape)
                            .background(MaterialTheme.colorScheme.primaryContainer.copy(alpha = 0.2f)),
                        contentAlignment = Alignment.Center
                    ) {
                        Icon(
                            imageVector = Icons.AutoMirrored.Filled.ArrowForward,
                            contentDescription = null,
                            tint = MaterialTheme.colorScheme.primary,
                            modifier = Modifier.size(24.dp)
                        )
                    }

                    Column(
                        modifier = Modifier.weight(1f)
                    ) {
                        Text(
                            text = pack.name,
                            style = MaterialTheme.typography.titleMedium,
                            fontWeight = FontWeight.SemiBold,
                            maxLines = 1,
                            overflow = TextOverflow.Ellipsis
                        )
                        Text(
                            text = "作者: ${pack.author}",
                            style = MaterialTheme.typography.bodySmall,
                            color = MaterialTheme.colorScheme.onSurfaceVariant.copy(alpha = 0.7f)
                        )
                    }

                    // 开关和展开按钮
                    Row(
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        Switch(
                            checked = isEnabled,
                            onCheckedChange = { enabled ->
                                isEnabled = enabled
                                onToggle(pack, enabled)
                            },
                            colors = SwitchDefaults.colors(
                                checkedThumbColor = MaterialTheme.colorScheme.primary,
                                checkedTrackColor = MaterialTheme.colorScheme.primaryContainer,
                                uncheckedThumbColor = MaterialTheme.colorScheme.outline,
                                uncheckedTrackColor = MaterialTheme.colorScheme.surfaceVariant
                            )
                        )
                        Spacer(modifier = Modifier.width(8.dp))
                        IconButton(
                            onClick = { expanded = !expanded },
                            modifier = Modifier.size(36.dp)
                        ) {
                            Icon(
                                imageVector = Icons.Default.ExpandMore,
                                contentDescription = if (expanded) "收起" else "展开",
                                modifier = Modifier
                                    .size(24.dp)
                                    .rotate(if (expanded) 180f else 0f),
                                tint = MaterialTheme.colorScheme.onSurfaceVariant
                            )
                        }
                    }
                }

                if (expanded) {
                    Column(
                        modifier = Modifier
                            .fillMaxWidth()
                            .padding(top = 12.dp)
                    ) {
                        Row(
                            modifier = Modifier.fillMaxWidth(),
                            horizontalArrangement = Arrangement.SpaceBetween
                        ) {
                            Text(
                                text = "版本: v${pack.version}",
                                style = MaterialTheme.typography.bodyMedium,
                                color = MaterialTheme.colorScheme.primary
                            )
                            Text(
                                text = "类型: ${if (pack.type == 0) "标准" else "TL Pro"}",
                                style = MaterialTheme.typography.bodyMedium,
                                color = MaterialTheme.colorScheme.secondary
                            )
                        }

                        Spacer(modifier = Modifier.height(8.dp))

                        // 描述
                        Text(
                            text = pack.description,
                            style = MaterialTheme.typography.bodyMedium,
                            maxLines = if (expanded) 10 else 3,
                            overflow = TextOverflow.Ellipsis
                        )

                        Spacer(modifier = Modifier.height(12.dp))

                        // 操作按钮
                        Row(
                            modifier = Modifier.fillMaxWidth(),
                            horizontalArrangement = Arrangement.End
                        ) {
                            Button(
                                onClick = { onDelete(pack) },
                                colors = ButtonDefaults.buttonColors(
                                    containerColor = MaterialTheme.colorScheme.errorContainer,
                                    contentColor = MaterialTheme.colorScheme.onErrorContainer
                                ),
                                shape = RoundedCornerShape(8.dp)
                            ) {
                                Icon(
                                    imageVector = Icons.Default.Delete,
                                    contentDescription = "删除",
                                    modifier = Modifier.size(18.dp)
                                )
                                Spacer(modifier = Modifier.width(8.dp))
                                Text("删除")
                            }
                        }
                    }
                }
            }
        }
    }
}