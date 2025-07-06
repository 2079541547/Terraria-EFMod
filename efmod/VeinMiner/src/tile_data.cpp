/*******************************************************************************
 * 文件名称: tile_data
 * 项目名称: EFMod
 * 创建时间: 25-5-24
 * 作者: EternalFuture゙
 * Gitlab: https://github.com/2079541547/
 * 协议: Apache License 2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include "tile_data.hpp"
#include "config.hpp"

void TileData::init(TEFMod::TEFModAPI *api) {
    static bool inited = false;
    if (!inited) {

        const char* fields[] = { "_width", "_height" };
        for (const auto field: fields) {
            api->registerApiDescriptor({
                "Terraria",
                "TileData",
                field,
                "Field"
            });
        }

        api->registerApiDescriptor({ "Terraria", "Tile", "type", "Field" });
        api->registerApiDescriptor({ "Terraria", "Tile", "_tileOffset", "Field" });

        api->registerApiDescriptor({ "Terraria", "TileData", "GetType", "Method", 1 });
        api->registerApiDescriptor({ "Terraria", "TileData", "get_Item", "Method", 2 });

        api->registerApiDescriptor({ "Terraria", "Main", "tile", "Field" });

        api->registerApiDescriptor({ "Terraria.ID", "TileID.Sets", "Ore", "Field" });

        api->registerApiDescriptor({ "Terraria", "WorldGen", "KillTile", "Method", 5 });

        api->registerApiDescriptor({ "Terraria", "Main", "get_LocalPlayer", "Method", 0 });
        api->registerApiDescriptor({ "Terraria", "Player", "HeldItem", "Field" });
        api->registerApiDescriptor({ "Terraria", "Item", "pick", "Field" });
        api->registerApiDescriptor({ "Terraria", "HitTile", "HitTileObjectData", "Field" });

        inited = true;
    } else {
        type = ParseUShortField(api->GetAPI<void*>({ "Terraria", "Tile", "type", "Field" }));
        _tileOffset = ParseIntField(api->GetAPI<void*>({ "Terraria", "Tile", "_tileOffset", "Field" }));
        KillTile = ParseVoidMethod(api->GetAPI<void*>({ "Terraria", "WorldGen", "KillTile", "Method", 5 }));
        Ore = ParseOtherField(api->GetAPI<void*>({ "Terraria.ID", "TileID.Sets", "Ore", "Field" }));
        GetType = ParseUShortMethod(api->GetAPI<void*>({ "Terraria", "TileData", "GetType", "Method", 1 }));
        tile = ParseOtherField(api->GetAPI<void*>({ "Terraria", "Main", "tile", "Field" }));
        _width = ParseIntField(api->GetAPI<void*>({ "Terraria", "TileData", "_width", "Field" }));
        _height = ParseIntField(api->GetAPI<void*>({ "Terraria", "TileData", "_height", "Field" }));
        get_Item = ParseOtherMethod(api->GetAPI<void*>({ "Terraria", "TileData", "get_Item", "Method", 2 }));
        get_LocalPlayer = ParseOtherMethod(api->GetAPI<void*>({ "Terraria", "Main", "get_LocalPlayer", "Method", 0 }));
        HeldItem = ParseOtherField(api->GetAPI<void*>({ "Terraria", "Player", "HeldItem", "Field" }));
        pick = ParseIntField(api->GetAPI<void*>({ "Terraria", "Item", "pick", "Field" }));
        HitTileObjectData = ParseOtherField(api->GetAPI<void*>({ "Terraria", "HitTile", "HitTileObjectData", "Field" }));
    }
}

void TileData::Initialize() {
    WORLD_WIDTH = _width->Get(tile);
    WORLD_HEIGHT = _height->Get(tile);
    WORLD_SIZE = WORLD_WIDTH * WORLD_HEIGHT;

    // 根据配置设置邻域类型
    if (config::Neighborhood == "Moore") {
        // 摩尔邻域(8方向)
        OFFSETS = {
            WORLD_WIDTH,            // 上
            WORLD_WIDTH + 1,        // 右上
            1,                      // 右
            -WORLD_WIDTH + 1,       // 右下
            -WORLD_WIDTH,           // 下
            -WORLD_WIDTH - 1,       // 左下
            -1,                     // 左
            WORLD_WIDTH - 1         // 左上
        };
    } else {
        // 冯诺依曼邻域(4方向)
        OFFSETS = {
            WORLD_WIDTH,            // 上
            -WORLD_WIDTH,           // 下
            1,                      // 右
            -1                      // 左
        };
    }

    REMOVE_LIST.clear();
    REMOVE_UPDATE_COUNTER = 0;
    IsInitialized = true;
}