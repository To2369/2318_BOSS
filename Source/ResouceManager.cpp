#include"Graphics/Graphics.h"
#include"ResouceManager.h"
ModelResource* Resoucemanager::LoadModelResouce(const char* filename)
{


    auto it = models.find(filename);
    if (it != models.end())
    {
        return  it->second.get();
    }
    models[filename] = std::make_unique<ModelResource>();
    models[filename]->Load(Graphics::Instance().GetDevice(), filename);
    return models[filename].get();
}