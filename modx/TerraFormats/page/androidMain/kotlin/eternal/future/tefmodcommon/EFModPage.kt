package eternal.future.tefmodcommon

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
import androidx.compose.foundation.border
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
        val version: String,
        val filePath: String,
        val iconBitmap: android.graphics.Bitmap?,
        val isEnabled: Boolean,
        val priority: Int,
        val type: String
    )

    @OptIn(ExperimentalMaterial3Api::class)
    @SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
    val Content: (@Composable () -> Unit) = {
        val context = LocalContext.current
        var packInfos by remember { mutableStateOf<List<PackInfo>>(emptyList()) }
        var isLoading by remember { mutableStateOf(false) }
        var errorMessage by remember { mutableStateOf<String?>(null) }

        val configFile = File(params["private-path"] as String, "config.json")

        fun readPackInfoFromZip(zipFile: File): PackInfo {
            val inputStream = zipFile.inputStream().buffered()
            val zipInputStream = ZipInputStream(inputStream)

            var jsonStr: String? = null
            var iconBitmap: android.graphics.Bitmap? = null

            try {
                var entry: ZipEntry?
                while (zipInputStream.nextEntry.also { entry = it } != null) {
                    when {
                        entry?.name.equals("config.json", ignoreCase = true) -> {
                            jsonStr = zipInputStream.readBytes().toString(Charsets.UTF_8)
                        }
                        entry?.name.equals("icon.png", ignoreCase = true) -> {
                            val bytes = zipInputStream.readBytes()
                            iconBitmap = BitmapFactory.decodeByteArray(bytes, 0, bytes.size)
                        }
                    }
                    zipInputStream.closeEntry()
                }

                if (jsonStr == null) throw IOException("未找到TerraFormats配置文件")

                val json = JSONObject(jsonStr)

                return PackInfo(
                    name = json.getString("name"),
                    author = json.getString("author"),
                    description = json.optString("introduce", "暂无描述"),
                    version = json.getString("version"),
                    filePath = zipFile.absolutePath,
                    iconBitmap = iconBitmap,
                    isEnabled = false,
                    priority = 0,
                    type = json.optString("type", "Unknown")
                )
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
                        val zipFile = File(packsDir, item.getString("path"))

                        if (!zipFile.exists()) continue

                        val isEnabled = item.getBoolean("enable")
                        val priority = item.getInt("priority")

                        try {
                            val packData = readPackInfoFromZip(zipFile)
                            infos.add(packData.copy(isEnabled = isEnabled, priority = priority))
                        } catch (e: Exception) {
                            e.printStackTrace()
                            errorMessage = "加载TerraFormats包 ${zipFile.name} 失败: ${e.message}"
                            continue
                        }
                    }
                }

                // 按优先级和启用状态排序
                packInfos = infos.sortedWith(
                    compareByDescending<PackInfo> { it.isEnabled }
                        .thenByDescending { it.priority }
                        .thenBy { it.name }
                )
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
                        put("path", File(pack.filePath).name)
                        put("enable", pack.isEnabled)
                        put("priority", pack.priority)
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

                val fileName = "tfpack_${System.currentTimeMillis()}.zip"
                val destFile = File(packsDir, fileName)

                context.contentResolver.openInputStream(uri)?.use { input ->
                    FileOutputStream(destFile).use { output ->
                        input.copyTo(output)
                    }
                }

                val newPack = readPackInfoFromZip(destFile)
                saveConfig(packInfos + newPack)
                loadPackInfos()
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "安装失败: ${e.message}"

                // 清理无效文件
                val packsDir = File(params["private-path"] as String)
                packsDir.listFiles()?.forEach { file ->
                    if (file.name.startsWith("tfpack_") && file.name.endsWith(".zip") &&
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
                val enabledCount = packInfos.count { it.isEnabled }
                if (enabled && enabledCount >= 7) {
                    errorMessage = "最多只能启用7个包"
                    return
                }

                val updatedPacks = packInfos.map {
                    if (it.filePath == pack.filePath) it.copy(isEnabled = enabled) else it
                }
                saveConfig(updatedPacks)
                loadPackInfos()
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "更新失败: ${e.message}"
            } finally {
                isLoading = false
            }
        }

        fun updatePriority(pack: PackInfo, newPriority: Int) {
            isLoading = true
            try {
                val updatedPacks = packInfos.map {
                    if (it.filePath == pack.filePath) it.copy(priority = newPriority.coerceIn(0, 6)) else it
                }
                saveConfig(updatedPacks)
                loadPackInfos()
            } catch (e: Exception) {
                e.printStackTrace()
                errorMessage = "更新优先级失败: ${e.message}"
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
                        Icon(Icons.Default.Add, "添加TerraFormats包")
                    }
                },
                containerColor = MaterialTheme.colorScheme.background
            ) { paddingValues ->
                Surface(
                    modifier = Modifier
                        .fillMaxSize(),
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
                                Column(
                                    modifier = Modifier
                                        .align(Alignment.TopCenter)
                                        .padding(16.dp)
                                ) {
                                    Text(
                                        text = errorMessage!!,
                                        color = MaterialTheme.colorScheme.error,
                                        style = MaterialTheme.typography.bodyMedium
                                    )
                                    Spacer(modifier = Modifier.height(8.dp))
                                    Text(
                                        text = "已启用 ${packInfos.count { it.isEnabled }}/7 个包",
                                        color = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.7f),
                                        style = MaterialTheme.typography.bodySmall
                                    )
                                }
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
                                        text = "暂无TerraFormats包",
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
                                Column(modifier = Modifier.fillMaxSize()) {
                                    // 状态提示
                                    Box(
                                        modifier = Modifier
                                            .fillMaxWidth()
                                            .background(MaterialTheme.colorScheme.surfaceVariant)
                                            .padding(),
                                        contentAlignment = Alignment.Center
                                    ) {
                                        Text(
                                            text = "已启用 ${packInfos.count { it.isEnabled }}/7 个包",
                                            color = if (packInfos.count { it.isEnabled } >= 7) {
                                                MaterialTheme.colorScheme.error
                                            } else {
                                                MaterialTheme.colorScheme.onSurfaceVariant
                                            },
                                            style = MaterialTheme.typography.labelLarge
                                        )
                                    }

                                    LazyColumn(
                                        modifier = Modifier
                                            .fillMaxSize()
                                            .padding(16.dp),
                                        verticalArrangement = Arrangement.spacedBy(12.dp)
                                    ) {
                                        items(packInfos, key = { it.filePath }) { pack ->
                                            ExpandablePackCard(
                                                pack = pack,
                                                enabledCount = packInfos.count { it.isEnabled },
                                                onDelete = { deletePack(it) },
                                                onToggle = { p, enabled -> togglePack(p, enabled) },
                                                onPriorityChange = { p, priority -> updatePriority(p, priority) }
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
    }

    @OptIn(ExperimentalMaterial3Api::class)
    @Composable
    private fun ExpandablePackCard(
        pack: PackInfo,
        enabledCount: Int,
        onDelete: (PackInfo) -> Unit,
        onToggle: (PackInfo, Boolean) -> Unit,
        onPriorityChange: (PackInfo, Int) -> Unit
    ) {
        var expanded by remember { mutableStateOf(false) }
        var isEnabled by remember { mutableStateOf(pack.isEnabled) }
        var currentPriority by remember { mutableStateOf(pack.priority) }

        val maxReached = enabledCount >= 7 && !isEnabled
        val priorityColor = when (currentPriority) {
            6 -> MaterialTheme.colorScheme.primary
            5 -> MaterialTheme.colorScheme.secondary
            4 -> MaterialTheme.colorScheme.tertiary
            3 -> MaterialTheme.colorScheme.error
            2 -> MaterialTheme.colorScheme.primaryContainer
            1 -> MaterialTheme.colorScheme.secondaryContainer
            else -> MaterialTheme.colorScheme.tertiaryContainer
        }

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
                containerColor = if (isEnabled) {
                    MaterialTheme.colorScheme.surfaceContainerHighest
                } else {
                    MaterialTheme.colorScheme.surfaceVariant
                },
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
                    // 优先级指示器
                    if (isEnabled) {
                        Box(
                            modifier = Modifier
                                .size(24.dp)
                                .clip(CircleShape)
                                .background(priorityColor)
                                .border(
                                    1.dp,
                                    MaterialTheme.colorScheme.onSurface.copy(alpha = 0.2f),
                                    CircleShape
                                ),
                            contentAlignment = Alignment.Center
                        ) {
                            Text(
                                text = (currentPriority + 1).toString(),
                                color = MaterialTheme.colorScheme.onPrimary,
                                style = MaterialTheme.typography.labelSmall
                            )
                        }
                        Spacer(modifier = Modifier.width(12.dp))
                    }

                    pack.iconBitmap?.asImageBitmap()?.let { bitmap ->
                        Image(
                            bitmap = bitmap,
                            contentDescription = null,
                            modifier = Modifier
                                .size(48.dp)
                                .clip(RoundedCornerShape(8.dp))
                        )
                        Spacer(modifier = Modifier.width(12.dp))
                    } ?: Box(
                        modifier = Modifier
                            .size(48.dp)
                            .clip(RoundedCornerShape(8.dp))
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
                            overflow = TextOverflow.Ellipsis,
                            color = if (maxReached) MaterialTheme.colorScheme.onSurface.copy(alpha = 0.5f)
                            else MaterialTheme.colorScheme.onSurface
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
                                if (!maxReached) {
                                    isEnabled = enabled
                                    onToggle(pack, enabled)
                                }
                            },
                            enabled = !maxReached,
                            colors = SwitchDefaults.colors(
                                checkedThumbColor = MaterialTheme.colorScheme.primary,
                                checkedTrackColor = MaterialTheme.colorScheme.primaryContainer,
                                uncheckedThumbColor = MaterialTheme.colorScheme.outline,
                                uncheckedTrackColor = MaterialTheme.colorScheme.surfaceVariant,
                                disabledCheckedThumbColor = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.38f),
                                disabledCheckedTrackColor = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.12f),
                                disabledUncheckedThumbColor = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.38f),
                                disabledUncheckedTrackColor = MaterialTheme.colorScheme.onSurface.copy(alpha = 0.12f)
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
                                text = "类型: ${pack.type}",
                                style = MaterialTheme.typography.bodyMedium,
                                color = MaterialTheme.colorScheme.secondary
                            )
                        }

                        Spacer(modifier = Modifier.height(8.dp))

                        // 优先级控制
                        if (isEnabled) {
                            Column(
                                modifier = Modifier
                                    .fillMaxWidth()
                                    .padding(vertical = 8.dp)
                            ) {
                                Text(
                                    text = "加载优先级",
                                    style = MaterialTheme.typography.labelMedium,
                                    color = MaterialTheme.colorScheme.onSurfaceVariant
                                )
                                Spacer(modifier = Modifier.height(4.dp))
                                Row(
                                    verticalAlignment = Alignment.CenterVertically,
                                    modifier = Modifier.fillMaxWidth()
                                ) {
                                    Button(
                                        onClick = {
                                            if (currentPriority > 0) {
                                                currentPriority--
                                                onPriorityChange(pack, currentPriority)
                                            }
                                        },
                                        modifier = Modifier.weight(1f),
                                        shape = RoundedCornerShape(topStart = 8.dp, bottomStart = 8.dp),
                                        colors = ButtonDefaults.buttonColors(
                                            containerColor = MaterialTheme.colorScheme.surfaceContainerHigh,
                                            contentColor = MaterialTheme.colorScheme.onSurface
                                        ),
                                        enabled = currentPriority > 0
                                    ) {
                                        Text("降低")
                                    }
                                    Box(
                                        modifier = Modifier
                                            .weight(1f)
                                            .fillMaxWidth()
                                            .height(36.dp)
                                            .background(MaterialTheme.colorScheme.surfaceContainerLow),
                                        contentAlignment = Alignment.Center
                                    ) {
                                        Text(
                                            text = "级别 ${currentPriority + 1}",
                                            style = MaterialTheme.typography.labelLarge,
                                            color = priorityColor
                                        )
                                    }
                                    Button(
                                        onClick = {
                                            if (currentPriority < 6) {
                                                currentPriority++
                                                onPriorityChange(pack, currentPriority)
                                            }
                                        },
                                        modifier = Modifier.weight(1f),
                                        shape = RoundedCornerShape(topEnd = 8.dp, bottomEnd = 8.dp),
                                        colors = ButtonDefaults.buttonColors(
                                            containerColor = MaterialTheme.colorScheme.surfaceContainerHigh,
                                            contentColor = MaterialTheme.colorScheme.onSurface
                                        ),
                                        enabled = currentPriority < 6
                                    ) {
                                        Text("提高")
                                    }
                                }
                            }
                            Spacer(modifier = Modifier.height(8.dp))
                        }

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