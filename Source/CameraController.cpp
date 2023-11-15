#include"CameraController.h"
#include"camera.h"
#include"Input/Input.h"
#include <imgui.h>
#include"PlayerManager.h"
//更新処理
void CameraController::Update(float elapsedtime)
{
  /*  POINT cursor;
    ::GetCursorPos(&cursor);*/
   // DirectX::XMMatrixRotationRollPitchYaw
    GamePad& gamePad = Input::Instance().GetGamePad();
    //todo:camera操作設定
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    
   /* float ax = cursor.x;
    float ay = -cursor.y;*/
    //カメラの回転速度
    float speed = rollSpeed*elapsedtime;
    //スティックの入力値に合わせてX軸Y軸回転
    {
        angle.x += ay * speed;
        angle.y += ax * speed;
    }
    //カメラ回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    
    //回転行列から前方方向ベクトルを取り出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //注視点から後ろベクトル方向に一定距離離れたカメラ視点を求める
    
    DirectX::XMFLOAT3 eye;
    player* pl = PlayerManager::Instance().GetPlayer(0);
    bool chageflag = pl->GetPerspectiveChangeFlag().TPS ? true : false;//切り替わり
    if (chageflag)
    {
        eye.x = target.x - front.x;
        eye.y = target.y - front.y;
        eye.z = target.z - front.z;
    }
    else
    {
        eye.x = target.x - front.x;
        eye.y = target.y - front.y;
        eye.z = target.z - front.z;
    }
    //実験
    //eye.x = camerapos.x-front.x * range;
    //eye.y = camerapos.y-front.y * range;
    //eye.z = camerapos.z-front.z * range;
    
    //カメラの視点と注視点を設定
    //Camera::instance().SetLookAt(eye, camerapos, DirectX::XMFLOAT3{ 0,1,0 });
    Camera::instance().SetLookAt(eye, target, DirectX::XMFLOAT3{ 0,1,0 });

    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }

    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }
}
void CameraController::DrawDebugGUI()
{
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("CameraController", nullptr, ImGuiWindowFlags_None))
    {
        //トランスフォーム
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //位置
            ImGui::InputFloat3("target", &target.x);
            //回転
            DirectX::XMFLOAT3 a;
           

            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //スケール
           
        }
    }

    if (ImGui::CollapsingHeader("Camera関連", ImGuiTreeNodeFlags_DefaultOpen))
    {
       
        ImGui::SliderFloat3("Camerapos", &camerapos.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("cameraAngle", &cameraAngle.x, -10.0f, 10.0f);
    }
    ImGui::End();
}