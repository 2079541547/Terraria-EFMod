/*******************************************************************************
 * 文件名称: efmod
 * 项目名称: ForgottenItem
 * 创建时间: 25-5-18
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

#include "logger.hpp"
#include "utility.hpp"

#include "efmod_core.hpp"

#include "item_id.hpp"
#include "recipe.hpp"
#include "item.hpp"
#include "item_drop_database.hpp"
#include "prefix_legacy.hpp"

class ForgottenItem final : public EFMod {
public:

    void Send(const std::string &path, MultiChannel *multiChannel) override {
        const auto api = multiChannel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        ItemID::init(api);
        Recipe::init(api);
        Item::init(api);
        PrefixLegacy::init(api);
        ItemDropDatabase::init(api);
    }

    void Receive(const std::string &path, MultiChannel *multiChannel) override {
        const auto api = multiChannel->receive<TEFMod::TEFModAPI*>("TEFMod::TEFModAPI");
        ItemID::init(api);
        Recipe::init(api);
        Item::init(api);
        PrefixLegacy::init(api);
        ItemDropDatabase::init(api);
    }

    int Load(const std::string &path, MultiChannel *multiChannel) override {
        logger = multiChannel->receive<TEFMod::Logger*(*)(const std::string& Tag, const std::string& filePath, const std::size_t)>("TEFMod::CreateLogger")("ForgottenItem-EternalFuture゙", "", 0);
        logger->init();

        ParseIntField = multiChannel->receive<decltype(ParseIntField)>("TEFMod::Field<Int>::ParseFromPointer");
        ParseBoolField = multiChannel->receive<decltype(ParseBoolField)>("TEFMod::Field<Bool>::ParseFromPointer");
        ParseOtherField = multiChannel->receive<decltype(ParseOtherField)>("TEFMod::Field<Other>::ParseFromPointer");
        ParseFloatField = multiChannel->receive<decltype(ParseFloatField)>("TEFMod::Field<Float>::ParseFromPointer");

        ParseIntMethod = multiChannel->receive<decltype(ParseIntMethod)>("TEFMod::Method<Int>::ParseFromPointer");
        ParseVoidMethod = multiChannel->receive<decltype(ParseVoidMethod)>("TEFMod::Method<Void>::ParseFromPointer");
        ParseOtherMethod = multiChannel->receive<decltype(ParseOtherMethod)>("TEFMod::Method<Other>::ParseFromPointer");

        ParseIntArray = multiChannel->receive<decltype(ParseIntArray)>("TEFMod::Array<Int>::ParseFromPointer");
        ParseBoolArray = multiChannel->receive<decltype(ParseBoolArray)>("TEFMod::Array<Bool>::ParseFromPointer");
        ParseOtherArray = multiChannel->receive<decltype(ParseOtherArray)>("TEFMod::Array<Other>::ParseFromPointer");

        return 0;
    }

    Metadata GetMetadata() override {
        return {
        "ForgottenItem",
        "EternalFuture゙",
        "1.2.0",
            20250517,
            ModuleType::Game,
            {true}
        };
    }

    int Initialize(const std::string &path, MultiChannel *multiChannel) override {return 0;}
    int UnLoad(const std::string &path, MultiChannel *multiChannel) override {return 0;}
};

EFMod* CreateMod() {
    static ForgottenItem mod;
    return &mod;
}