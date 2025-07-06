/*******************************************************************************
 * 文件名称: tefmod
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

#include "TEFMod.hpp"
#include "BaseType.hpp"
#include "Logger.hpp"

inline TEFMod::Logger* logger = nullptr;

inline TEFMod::Field<int>* (*ParseIntField)(void*) = nullptr;
inline TEFMod::Field<bool>* (*ParseBoolField)(void*) = nullptr;
inline TEFMod::Field<void*>* (*ParseOtherField)(void*) = nullptr;
inline TEFMod::Field<float>* (*ParseFloatField)(void*) = nullptr;
inline TEFMod::Field<uint16_t>* (*ParseUShortField)(void*) = nullptr;

inline TEFMod::Method<int>* (*ParseIntMethod)(void*) = nullptr;
inline TEFMod::Method<void>* (*ParseVoidMethod)(void*) = nullptr;
inline TEFMod::Method<uint16_t>* (*ParseUShortMethod)(void*) = nullptr;
inline TEFMod::Method<void*>* (*ParseOtherMethod)(void*) = nullptr;

inline TEFMod::Array<int>* (*ParseIntArray)(void*) = nullptr;
inline TEFMod::Array<bool>* (*ParseBoolArray)(void*) = nullptr;
inline TEFMod::Array<void*>* (*ParseOtherArray)(void*) = nullptr;

inline TEFMod::String* (*ParseString)(void*) = nullptr;
