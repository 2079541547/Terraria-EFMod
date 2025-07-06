/*******************************************************************************
 * 文件名称: config
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

#include <string>
#include <unordered_set>
#include <vector>

namespace config {
    const std::unordered_set WhiteList = {
        6, 7, 8, 9, 22, 37, 56, 58, 107, 108, 111,
        166, 167, 168, 169, 204, 211, 221, 222, 223, 408
    };

    constexpr bool AllowOres = true;      // 是否允许所有矿石
    constexpr int RemoveInterval = 5;     // 移除间隔(游戏刻)
    constexpr int RemovePerInterval = 10; // 每次间隔移除的数量
    const std::string Neighborhood = "Moore"; // 使用摩尔邻域(8方向)或冯诺依曼邻域(4方向)
}
