/*******************************************************************************
 * 文件名称: vein_miner
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

#include "vein_miner.hpp"

#include "config.hpp"
#include "tile_data.hpp"

#include <algorithm>

bool vein_miner::IsTileTypeCanBeVeinMined(const int tileType) {
    static auto Ore_v = ParseBoolArray(TileData::Ore->Get())->to_vector();
    return config::WhiteList.find(tileType) != config::WhiteList.end();
}

void vein_miner::Mine(int x, int y) {
    if (!TileData::IsInitialized) {
        TileData::Initialize();
    }

    const TEFMod::TerrariaInstance hitTile = TileData::get_Item->Call(TileData::tile, 2, x, y);
    const int hitTileType = TileData::type->Get(hitTile);

    if (!IsTileTypeCanBeVeinMined(hitTileType)) return;

    // 使用栈实现广度优先搜索
    std::vector stack = {TileData::_tileOffset->Get(hitTile)};

    while (!stack.empty()) {
        const int currentPoint = stack.back();
        stack.pop_back();

        // 检查所有邻域方块
        for (const int offset : TileData::OFFSETS) {
            int nextPoint = currentPoint + offset;

            if (nextPoint < 0 || nextPoint >= TileData::WORLD_SIZE) {
                continue;
            }

            if (std::find(TileData::REMOVE_LIST.begin(), TileData::REMOVE_LIST.end(), nextPoint) != TileData::REMOVE_LIST.end()) {
                continue;
            }

            if (const int tileType = TileData::GetType->Call(nullptr, 1, nextPoint); tileType == hitTileType) {
                TileData::REMOVE_LIST.push_back(nextPoint);
                stack.push_back(nextPoint);
            }
        }
    }
}

void vein_miner::Update() {
    if (!TileData::IsInitialized) return;

    if (TileData::REMOVE_UPDATE_COUNTER >= config::RemoveInterval) {
        for (int i = 0; i < config::RemovePerInterval && !TileData::REMOVE_LIST.empty(); i++) {
            const int tileOffset = TileData::REMOVE_LIST.front();
            TileData::REMOVE_LIST.erase(TileData::REMOVE_LIST.begin());

            const int x = tileOffset % TileData::WORLD_WIDTH;
            const int y = tileOffset / TileData::WORLD_WIDTH;
            TileData::KillTile->Call(nullptr, 5, x, y, false, false, false);
        }
        TileData::REMOVE_UPDATE_COUNTER = 0;
    }

    TileData::REMOVE_UPDATE_COUNTER++;
}
