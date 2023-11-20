#include"ProjectileStraight.h"

ProjectileStraight::ProjectileStraight(ProjectileManager* manager)
   :Projectile(manager)
{
  //  model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
    model = new Model("Data/Model/Sword/Sword.mdl");
    //表示サイズ
   // scale.x = scale.y = scale.z = 0.5f;
    scale.x = scale.y = scale.z = 3.0f;
}

ProjectileStraight::~ProjectileStraight()
{
    delete model;
}

void ProjectileStraight::Update(float elapsedTime)
{

    //寿命処理
    lifeTime -= elapsedTime;
    if (lifeTime <= 0)
    {
        //自分を削除
        Destroy();

    }

    //移動
    float speed = this->speed * elapsedTime;
    //xの移動処理(位置＋＝向き＊速さ)
    position.x += direction.x * speed;
    //yの移動処理
    position.y += direction.y * speed;
    //zの移動処理
    position.z += direction.z * speed;

   /* DirectX::XMVECTOR dvec = DirectX::XMLoadFloat3(&direction);
    DirectX::XMVECTOR pvec = DirectX::XMLoadFloat3(&position);
    DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(pvec, DirectX::XMVectorScale(dvec, speed)));*/
    //オブジェクトの更新処理
    UpdateTransform();
    //モデル行列更新
    model->UpdateTransform(transform);

}

void ProjectileStraight::Render(RenderContext& rc, ModelShader* shader)
{
    shader->Draw(rc, model);
}

//発射
void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction,
                                const DirectX::XMFLOAT3& position)
{
    this->direction = direction;
    this->position = position;
}
