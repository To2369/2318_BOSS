#include"CameraController.h"
#include"camera.h"
#include"Input/Input.h"
#include <imgui.h>
#include"PlayerManager.h"
//�X�V����
void CameraController::Update(float elapsedtime)
{
  /*  POINT cursor;
    ::GetCursorPos(&cursor);*/
   // DirectX::XMMatrixRotationRollPitchYaw
    GamePad& gamePad = Input::Instance().GetGamePad();
    //todo:camera����ݒ�
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    
   /* float ax = cursor.x;
    float ay = -cursor.y;*/
    //�J�����̉�]���x
    float speed = rollSpeed*elapsedtime;
    //�X�e�B�b�N�̓��͒l�ɍ��킹��X��Y����]
    {
        angle.x += ay * speed;
        angle.y += ax * speed;
    }
    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
    
    //��]�s�񂩂�O�������x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
    
    DirectX::XMFLOAT3 eye;
    player* pl = PlayerManager::Instance().GetPlayer(0);
    bool chageflag = pl->GetPerspectiveChangeFlag().TPS ? true : false;//�؂�ւ��
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
    //����
    //eye.x = camerapos.x-front.x * range;
    //eye.y = camerapos.y-front.y * range;
    //eye.z = camerapos.z-front.z * range;
    
    //�J�����̎��_�ƒ����_��ݒ�
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
        //�g�����X�t�H�[��
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            //�ʒu
            ImGui::InputFloat3("target", &target.x);
            //��]
            DirectX::XMFLOAT3 a;
           

            a.x = DirectX::XMConvertToDegrees(angle.x);
            a.y = DirectX::XMConvertToDegrees(angle.y);
            a.z = DirectX::XMConvertToDegrees(angle.z);
            ImGui::InputFloat3("Angle", &a.x);
            angle.x = DirectX::XMConvertToRadians(a.x);
            angle.y = DirectX::XMConvertToRadians(a.y);
            angle.z = DirectX::XMConvertToRadians(a.z);
            //�X�P�[��
           
        }
    }

    if (ImGui::CollapsingHeader("Camera�֘A", ImGuiTreeNodeFlags_DefaultOpen))
    {
       
        ImGui::SliderFloat3("Camerapos", &camerapos.x, -10.0f, 10.0f);
        ImGui::SliderFloat3("cameraAngle", &cameraAngle.x, -10.0f, 10.0f);
    }
    ImGui::End();
}