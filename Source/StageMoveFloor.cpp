#include "StageMoveFloor.h"

StageMoveFloor::StageMoveFloor()
{
    scale.x  = scale.z = 3.0f;
    scale.y = 0.5f;
    model = new Model("Data/Model/Cube/Cube.mdl");
}

StageMoveFloor::~StageMoveFloor()
{
    delete model;
}

void StageMoveFloor::Update(float elapsedTime)
{
    oldTransform = TransForm;
    oldAngle = angle;
    DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR Goal = DirectX::XMLoadFloat3(&goal);
    DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Goal, Start);
    DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
    float length;
    DirectX::XMStoreFloat(&length, Length);
    //スタートからゴールまでの間を一秒間で進む割合(0.0,1.0)を算出する
    float speed = moveSpeed * elapsedTime;
    float speedRate = speed / length;
    moveRate += speedRate;
    //ゴールに到達、またはスタートに戻った場合、移動方向を反転させる
    if (moveRate <= 0.0f || moveRate >= 1.0f)
    {
        moveSpeed = -moveSpeed;
    }
    //線形補完で位置を算出する
    DirectX::XMVECTOR Position = DirectX::XMVectorLerp(Start, Goal, moveRate);
    DirectX::XMStoreFloat3(&position, Position);

    angle.x += torque.x * elapsedTime;
    angle.y += torque.y * elapsedTime;
    angle.z += torque.z * elapsedTime;

    UpdateTransform();
   // model->UpdateTransform(TransForm);
    const DirectX::XMFLOAT4X4 transformIdentity = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
    model->UpdateTransform(transformIdentity);
}

void StageMoveFloor::Render(RenderContext&rc, ModelShader* shader)
{
    model->UpdateTransform(TransForm);
    shader->Draw(rc, model);
}

bool StageMoveFloor::RayCast(const DirectX::XMFLOAT3& start, const DirectX::XMFLOAT3& end, HitResult& Hit)
{
    //return Collision::InstersectRayVsModel(start,end,model,Hit);
#if 1
   // 前回のワールド行列と逆行列を求める
    DirectX::XMMATRIX WorldTransForm = DirectX::XMLoadFloat4x4(&oldTransform);
    DirectX::XMMATRIX inverseWorldTransform = DirectX::XMMatrixInverse(nullptr, WorldTransForm);

    //前回のローカル空間でのレイに変更する
    DirectX::XMVECTOR WorldStart = DirectX::XMLoadFloat3(&start);
    DirectX::XMVECTOR WorldEnd = DirectX::XMLoadFloat3(&end);

    DirectX::XMVECTOR LocalStart = DirectX::XMVector3TransformCoord(WorldStart, inverseWorldTransform);
    DirectX::XMVECTOR LocalEnd = DirectX::XMVector3TransformCoord(WorldEnd, inverseWorldTransform);

    DirectX::XMFLOAT3 loacalStart, loacalEnd;
    DirectX::XMStoreFloat3(&loacalStart, LocalStart);
    DirectX::XMStoreFloat3(&loacalEnd, LocalEnd);
    HitResult hit;
    if (Collision::InstersectRayVsModel(loacalStart, loacalEnd, model, hit))
    {
        //前回のローカル空間から今回のワールド空間へ変換
        //前回から今回にかけて変更された内容が乗っているオブジェクトに反映される
        WorldTransForm = DirectX::XMLoadFloat4x4(&TransForm);
        DirectX::XMVECTOR LocalPosition = DirectX::XMLoadFloat3(&hit.position);
        DirectX::XMVECTOR WorldPosition = DirectX::XMVector3TransformCoord(LocalPosition,WorldTransForm);

        DirectX::XMVECTOR LocalNormal = DirectX::XMLoadFloat3(&hit.normal);
        DirectX::XMVECTOR WorldNormal = DirectX::XMVector3TransformNormal(LocalNormal, WorldTransForm);

        DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(WorldPosition, WorldStart);
        DirectX::XMVECTOR Dist = DirectX::XMVector3Length(Vec);
        DirectX::XMStoreFloat3(&Hit.position, WorldPosition);
        DirectX::XMStoreFloat3(&Hit.normal, WorldNormal);
        DirectX::XMStoreFloat(&Hit.distance, Dist);
        Hit.materialIndex = hit.materialIndex;
        Hit.rotation.x = angle.x - oldAngle.x;
        Hit.rotation.y = angle.y - oldAngle.y;
        Hit.rotation.z = angle.z - oldAngle.z;
        return true;
    }
    return false;
#endif
}

void StageMoveFloor::UpdateTransform()
{
    DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX W = S * R * T;
    DirectX::XMStoreFloat4x4(&TransForm, W);
}
