#include"TargetPoint.h"
#include"player.h"
#include"PlayerManager.h"
#include"collision.h"
//コンストラクタ
TargetPoint::TargetPoint()
{
    model = new Model("Data/Model/Mato/Mato.mdl");
    //モデルが大きいのでスケーリング
    scale.x = scale.y = scale.z = 0.08f;
    //円柱の幅高さ設定
    radius = 0.0f;
    height = 0.0f;
}

//デストラクタ
TargetPoint::~TargetPoint()
{
    delete model;
}

void TargetPoint::Update(float elapsedTime, FierdBuff& FB)
{
    //オブジェクト行列を更新
    UpdateTransform();

    TargetPoint::MaterialVsArrow("Red", 2.0f);
    //モデル行列更新
    model->UpdateTransform(transform);
}

//描画処理
void TargetPoint::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

void TargetPoint::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
    DebugRenderer* debugRnderer = Graphics::Instance().GetDebugRenderer();
}

//マテリアルと弓矢の衝突判定
void TargetPoint::MaterialVsArrow(const char* materialname, float boneRadius)
{
   
}