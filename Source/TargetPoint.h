#pragma once
#include"Graphics/Shader.h"
#include"enemy.h"
class TargetPoint :public Enemy
{
public:
    TargetPoint();
    ~TargetPoint()override;
    //更新処理
    void Update(float elpsedTime, FierdBuff& FB)override;
    //描画処理
    void Render(RenderContext& rc, ModelShader* shader)override;

    void DrawDebugPrimitive()override;

    //マテリアルと弓矢の衝突判定
    void MaterialVsArrow(const char* materialname, float boneRadius);
private:
    Model* model = nullptr;
   
};