/*******************************************************************************
 * 文件名称: item_drop_database
 * 项目名称: ForgottenItem
 * 创建时间: 25-6-28
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

#include <BaseType.hpp>
#include <TEFMod.hpp>

namespace ItemDropDatabase {

    /**
     * int type
     * IItemDropRule entry
     */
    inline TEFMod::Method<void*>* RegisterToNPC;

    /**
    * int itemId,
    * int chanceDenominator = 1,
    * int minimumDropped = 1,
    * int maximumDropped = 1
    */
    inline TEFMod::Method<void*>* Common; // ItemDropRule

    inline void (*original_Populate)(TEFMod::TerrariaInstance);
    void Populate_HookT(TEFMod::TerrariaInstance i);
    void hook_Populate(TEFMod::TerrariaInstance i);

    inline TEFMod::HookTemplate HookTemplate_Populate {
        reinterpret_cast<void*>(Populate_HookT),
        {  }
    };

    void init(TEFMod::TEFModAPI* api);
}
