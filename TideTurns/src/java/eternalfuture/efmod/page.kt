package eternalfuture.efmod

import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.OutlinedTextField
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Switch
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import org.json.JSONException
import org.json.JSONObject
import java.io.File

class page {
    var platform: String = ""
    var private: String = ""

    @OptIn(ExperimentalMaterial3Api::class)
    val ModView: (@Composable () -> Unit) = {
        Scaffold(
            content = { innerPadding ->
                LazyColumn(
                    modifier = Modifier
                        .fillMaxSize()
                        .padding(innerPadding)
                        .padding(horizontal = 10.dp),
                    verticalArrangement = Arrangement.spacedBy(10.dp)
                ) {
                    item {
                        Column(modifier = Modifier.padding(25.dp)) {
                            Text(
                                text = "时来运转"
                            )
                        }
                    }
                    item {
                        FunctionItem(
                            item = function(
                                title = "幸运伤害",
                                information = "buff除外的伤害全部随机",
                                isEnabled = get_state("LuckyDamage"),
                                key = "LuckyDamage"
                            )
                        )
                        var mode by remember { mutableIntStateOf(get_key("LuckyDamage", "mode") as Int) }
                        Column(modifier = Modifier.padding(16.dp)) {
                            OutlinedTextField(
                                value = mode.toString(),
                                onValueChange = { newValue ->
                                    val parsedValue = newValue.toIntOrNull() ?: return@OutlinedTextField
                                    mode = parsedValue
                                    edit_key("LuckyDamage", "mode", parsedValue)
                                },
                                label = { Text("模式(0/1)") },
                                modifier = Modifier.fillMaxWidth()
                            )
                        }
                    }
                    item {
                        FunctionItem(
                            item = function(
                                title = "物品轮盘",
                                information = "随机禁用一个幸运物品",
                                isEnabled = get_state("ItemRoulette"),
                                key = "ItemRoulette"
                            )
                        )
                        var index by remember { androidx.compose.runtime.mutableIntStateOf(get_key("ItemRoulette", "index") as Int) }
                        var number by remember { androidx.compose.runtime.mutableIntStateOf(get_key("ItemRoulette", "number") as Int) }
                        Column(modifier = Modifier.padding(16.dp)) {
                            OutlinedTextField(
                                value = index.toString(),
                                onValueChange = { newValue ->
                                    val parsedValue = newValue.toIntOrNull() ?: return@OutlinedTextField
                                    index = parsedValue
                                    edit_key("ItemRoulette", "index", parsedValue)
                                },
                                label = { Text("设置游戏物品数量") },
                                modifier = Modifier.fillMaxWidth()
                            )
                            OutlinedTextField(
                                value = number.toString(),
                                onValueChange = { newValue ->
                                    val parsedValue = newValue.toIntOrNull() ?: return@OutlinedTextField
                                    number = parsedValue
                                    edit_key("ItemRoulette", "number", parsedValue)
                                },
                                label = { Text("设置禁用物品数量") },
                                modifier = Modifier.fillMaxWidth()
                            )
                        }
                    }
                }
            }
        )
    }

    @OptIn(ExperimentalMaterial3Api::class)
    @Composable
    private fun FunctionItem(item: function) {
        var isEnabled by remember { mutableStateOf(item.isEnabled) }

        Card(
            modifier = Modifier
                .fillMaxWidth()
                .padding(horizontal = 8.dp, vertical = 2.5.dp)
                .padding(5.dp),
            shape = MaterialTheme.shapes.medium,
            colors = CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.surface),
            elevation = CardDefaults.cardElevation(defaultElevation = 2.dp)
        ) {
            Row(
                modifier = Modifier
                    .fillMaxWidth()
                    .padding(horizontal = 8.dp, vertical = 8.dp),
                verticalAlignment = Alignment.CenterVertically,
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Box(modifier = Modifier.weight(1f)) {
                    Row(verticalAlignment = Alignment.CenterVertically) {
                        Column(modifier = Modifier.weight(1f)) {
                            Text(
                                text = item.title,
                                fontSize = 18.sp,
                                fontWeight = FontWeight.Bold,
                                color = MaterialTheme.colorScheme.onSurface
                            )
                            Spacer(modifier = Modifier.height(4.dp))
                            Text(
                                text = item.information,
                                fontSize = 14.sp,
                                color = MaterialTheme.colorScheme.onSurfaceVariant
                            )
                        }
                    }
                }

                Column(verticalArrangement = Arrangement.Center, horizontalAlignment = Alignment.End) {
                    Switch(
                        checked = isEnabled,
                        onCheckedChange = { newValue ->
                            isEnabled = newValue
                            edit_state(item.key, newValue)
                        }
                    )
                }
            }
        }
    }

    private fun get_state(name: String): Boolean {
        return try {
            val jsonString = File(private, "private/config.json").bufferedReader().use { it.readText() }
            val jsonObject = JSONObject(jsonString)
            jsonObject.getJSONObject(name).optBoolean("enabled")
        } catch (e: JSONException) {
            e.printStackTrace()
            false
        }
    }

    private fun edit_state(name: String, new_state: Boolean) {
        try {
            val jsonString = File(private, "private/config.json").bufferedReader().use { it.readText() }
            val jsonObject = JSONObject(jsonString)
            jsonObject.getJSONObject(name).putOpt("enabled", new_state)
            File(private, "private/config.json").bufferedWriter().use { it.write(jsonObject.toString(4)) }
        } catch (e: JSONException) {
            e.printStackTrace()
        }
    }

    private fun get_key(name: String, name1: String): Any? {
        val jsonString = File(private, "private/config.json").bufferedReader().use { it.readText() }
        val jsonObject = JSONObject(jsonString)
        return jsonObject.getJSONObject(name).opt(name1)
    }

    private fun edit_key(name: String, name1: String, newValue: Any) {
        try {
            val jsonString = File(private, "private/config.json").bufferedReader().use { it.readText() }
            val jsonObject = JSONObject(jsonString)
            jsonObject.getJSONObject(name).putOpt(name1, newValue)
            File(private, "private/config.json").bufferedWriter().use { it.write(jsonObject.toString(4)) }
        } catch (e: JSONException) {
            e.printStackTrace()
        }
    }
}

@Preview
@Composable
fun Main() {
    page().ModView
}