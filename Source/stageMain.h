#pragma once
#include "Graphics/Model.h"
#include "Stage.h"

class StageMain:public Stage
{
public:
    StageMain();
    ~StageMain();
    //更新処理
    void Update(float elapsedTime);
    //描画処理
    void Render(RenderContext& rc, ModelShader* shader);
    //レイキャスト
    bool RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)override;
   
private:
    Model* model = nullptr;

};