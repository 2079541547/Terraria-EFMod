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
 
#pragma once

#include "tefmod.hpp"

namespace TileData {

    static int WORLD_WIDTH;    // 世界宽度(方块数)
    static int WORLD_HEIGHT;   // 世界高度(方块数)
    static int WORLD_SIZE;     // 世界总方块数

    // 邻域偏移量(摩尔8方向或冯诺依曼4方向)
    static std::vector<int> OFFSETS;

    static std::vector<int> REMOVE_LIST; // 待移除方块列表
    static int REMOVE_UPDATE_COUNTER;    // 移除计时器

    static bool IsInitialized = false; // 是否已初始化

    inline TEFMod::Field<void*>* tile;  // TEFMod::TerrariaInstance
    inline TEFMod::Field<int>* _width;
    inline TEFMod::Field<int>* _height;
    inline TEFMod::Method<uint16_t>* GetType; // int tileIndex
    inline TEFMod::Method<void*>* get_Item; // int x, int y

    inline TEFMod::Field<uint16_t>* type;
    inline TEFMod::Field<int>* _tileOffset;

    inline TEFMod::Field<void*>* Ore; // bool[]

    inline TEFMod::Method<void>* KillTile; // int i, int j, bool fail = false, bool effectOnly = false, bool noItem = false

    inline TEFMod::Method<void*>* get_LocalPlayer;
    inline TEFMod::Field<void*>* HeldItem;
    inline TEFMod::Field<int>* pick;

    inline TEFMod::Field<void*>* HitTileObjectData;

    void init(TEFMod::TEFModAPI* api);

    void Initialize();
}