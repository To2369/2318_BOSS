#pragma once
#include<memory>
#include<string>
#include<map>
#include"Graphics/ModelResource.h"

class Resoucemanager
{
private:
    Resoucemanager() {};
    ~Resoucemanager() {};
public:
    static Resoucemanager& Instance()
    {
        static Resoucemanager instance;
        return instance;
    }
    ModelResource* LoadModelResouce(const char* filename);
private:
    using ModelMap = std::map<std::string, std::unique_ptr<ModelResource>>;
    ModelMap models;
};