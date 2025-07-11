/*******************************************************************************
 * 文件名称: EFMod
 * 项目名称: ForgottenItemLoader
 * 创建时间: 2024/12/28
 * 作者: EternalFuture゙
 * Github: https://github.com/2079541547 
 * 版权声明: Copyright © 2024 EternalFuture. All rights reserved.
 * 许可证: MIT License
 *
 *         Permission is hereby granted, free of charge, to any person obtaining a copy
 *         of this software and associated documentation files (the "Software"), to deal
 *         in the Software without restriction, including without limitation the rights
 *         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *         copies of the Software, and to permit persons to whom the Software is
 *         furnished to do so, subject to the following conditions:
 *
 *         The above copyright notice and this permission notice shall be included in all
 *         copies or substantial portions of the Software.
 *
 *         THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *         IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *         FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *         LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *         OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *         SOFTWARE.
 *
 * 描述信息: EFMod 接口头文件
 *******************************************************************************/

#pragma once

#include <string>
#include <cstdint>
#include <filesystem>
#include <vector>
#include <functional>
#include <mutex>

struct ModApiDescriptor {
    std::string File;
    std::string Namespace;
    std::string Class;
    std::string Name;
    std::string Type;
    int Arg;
    
    inline size_t getID() const {
            std::string combined = File + Namespace + Class + Name + Type;
            return std::hash<std::string>{}(combined);
    }
};

struct HookTemplate {
    void* Trampoline;
    std::vector<void*> FunArray;

    void Set(std::vector<void*> f) { FunArray = std::move(f); }
};

struct ModFuncDescriptor {
    std::string File;
    std::string Namespace;
    std::string Class;
    std::string Name;
    std::string Type;
    int Arg;
    HookTemplate* Template;
    std::vector<void *> FunPtr;

    inline size_t getID() const {
            std::string combined = File + Namespace + Class + Name + Type + std::to_string(Arg);
            return std::hash<std::string>{}(combined);
    }
};

struct api {
    size_t id;
    void * apiPtr;
};

struct ModMetadata {
    std::string name;
    std::string author;
    std::string version;
};

class EFModAPI final {
private:
    std::vector<api> API;
    std::mutex APIMutex;
    
    std::vector<ModFuncDescriptor> FuncDescriptor;
    std::mutex FuncDescriptorMutex;

    std::vector<ModApiDescriptor> ApiDescriptor;
    std::mutex ApiDescriptorMutex;
public:
    inline auto getApiDescriptor() {
            return ApiDescriptor;
    }
    
    inline auto getFuncDescriptor() {
            return FuncDescriptor;
    }
    
    inline void* getAPI(const ModApiDescriptor& api) {
            std::lock_guard<std::mutex> lock(APIMutex);
            for (auto a: API) {
                    if (a.id == api.getID()) {
                            return a.apiPtr;
                    }
            }
            return nullptr;
    }

    inline void registerModApiDescriptor(const ModApiDescriptor& api) {
            std::lock_guard<std::mutex> lock(ApiDescriptorMutex);

            if (ApiDescriptor.empty()) {
                    ApiDescriptor.push_back(api);
                    return;
            }

            bool exists = false;
            for (const auto& existingApi : ApiDescriptor) {
                    if (existingApi.getID() == api.getID()) {
                            exists = true;
                            break;
                    }
            }

            if (!exists) {
                    ApiDescriptor.push_back(api);
            }
    }

    inline void registerModFuncDescriptor(const ModFuncDescriptor& Extend) {
        std::lock_guard<std::mutex> lock(FuncDescriptorMutex);
        bool found = false;

        for (auto& funcDesc : FuncDescriptor) {
            if (funcDesc.getID() == Extend.getID()) {
                funcDesc.FunPtr.insert(funcDesc.FunPtr.end(), Extend.FunPtr.begin(), Extend.FunPtr.end());
                found = true;
                break;
            }
        }

        if (!found) {
            FuncDescriptor.push_back(Extend);
        }
    }

    inline void registerAPI(size_t api_id, void *ptr) {
            API.push_back({api_id, ptr});
    }
    
    inline static EFModAPI& getEFModAPI() {
            static EFModAPI instance;
            return instance;
    }
};

class EFMod {
public:
    virtual ~EFMod() {}
    
    int standard = 20250316; //请不要乱修改，这是Mod标准，可能会导致某些错误因素

    virtual int initialize(EFModAPI *mod, std::filesystem::path path) = 0;
    virtual void Register(EFModAPI *mod, std::filesystem::path path) = 0;
    virtual ModMetadata getInfo() = 0;
};


extern "C" EFMod* CreateMod();