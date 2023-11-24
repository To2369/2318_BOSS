#include"CameraController.h"
#include"camera.h"
#include"Input/Input.h"
#include <imgui.h>
#include"PlayerManager.h"
#include"Graphics/Graphics.h"
//�X�V����
#include"Input/Mouse.h"
//�f�o�b�O�p�ϐ�
DirectX::XMFLOAT3 old = {};


DirectX::XMFLOAT3 rightShoulderOffset{};
void CameraController::Update(float elapsedtime)
{
#if 1
    //todo:camera����ݒ�
    bool changecountflag = Changecount > 0 ? true : false;
    //�J�����̉�]���x
    float speed = rollSpeed * elapsedtime;
#if 1
    GamePad& gamePad = Input::Instance().GetGamePad();
    float ax = gamePad.GetAxisRX();
    float ay = gamePad.GetAxisRY();
    //�X�e�B�b�N�̓��͒l�ɍ��킹��X��Y����]
    {
        angle.x += ay * speed;
        angle.y += ax * speed;
    }
#else
    Mouse mouse = Input::Instance().GetMouse();
    //�}�E�X�ŃJ�����𑀍�


    if (mouse.GetOldPositionX() != mouse.GetPositionX())cameraangle.x = mouse.GetPositionX() - mouse.GetOldPositionX();
    if (mouse.GetOldPositionY() != mouse.GetPositionY()) cameraangle.y = mouse.GetPositionY() - mouse.GetOldPositionY();
    if (mouse.GetOldPositionY() == mouse.GetPositionY()) cameraangle.y = 0;
    if (mouse.GetOldPositionX() == mouse.GetPositionX()) cameraangle.x = 0;
    float ax = DirectX::XMConvertToRadians(cameraangle.x);
    float ay = DirectX::XMConvertToRadians(cameraangle.y);

    {
        angle.x += ay * 0.3f;
        angle.y += ax * 0.3f;
    }
#endif
    player* pl = PlayerManager::Instance().GetPlayer(0);
    Model::Node* node = pl->GetCharacterModel()->FindNode(pl->GetNodename(PlayerNodeName::rightarm));
    rightShoulderOffset = { node->worldTransform._41, node->worldTransform._42,node->worldTransform._43 };
    bool changeflag = pl->GetPerspectiveChangeFlag().TPS ? true : false;//�؂�ւ��
    changeflag ? SetMinAngle(-7.0f) : SetMinAngle(-80);
    changeflag ? SetMaxAngle(50.0f) : SetMaxAngle(80);
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    //�J������]�l����]�s��ɕϊ�
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    //��]�s�񂩂�O�������x�N�g�������o��
    DirectX::XMVECTOR Right = Transform.r[0];
    DirectX::XMFLOAT3 right;
    DirectX::XMStoreFloat3(&right, Right);

    //��]�s�񂩂�O�������x�N�g�������o��
    DirectX::XMVECTOR Upper = Transform.r[1];
    DirectX::XMFLOAT3 upper;
    DirectX::XMStoreFloat3(&upper, Upper);

    //��]�s�񂩂�O�������x�N�g�������o��
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    //�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�




    if (changeflag)//Tps
    {
        BlendRate(old_changeflag, changeflag, flag, elapsedtime);
        float r = 10.f;
        DirectX::XMStoreFloat(&range, DirectX::XMVectorLerp(DirectX::XMLoadFloat(&range), DirectX::XMLoadFloat(&r), 0.05f));

        eye.x = target.x - front.x * range;
        float yy = target.y - front.y * range;
        eye.y = yy;
        eye.z = target.z - front.z * range;

        origin_Pos = eye;

        if (Rate < 1.0f && changecountflag)
        {
            DirectX::XMStoreFloat3(&old_Pos, DirectX::XMVectorLerp(
                DirectX::XMLoadFloat3(&old_Pos), DirectX::XMLoadFloat3(&origin_Pos), Rate));
        }
        else
        {
            old_Pos = eye;
        }

        if (flag)
        {
            constexpr float BackCameraY = DirectX::XMConvertToRadians(22.5f);
            Rate < 1.0f ? DirectX::XMStoreFloat(&angle.x, DirectX::XMVectorLerp(DirectX::XMLoadFloat(&angle.x), DirectX::XMLoadFloat(&BackCameraY), Rate))
                : flag = false;
        }
        DirectX::XMVECTOR targetVec = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&old_Pos), DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&front)));
        DirectX::XMFLOAT3 targetfloat3;
        DirectX::XMStoreFloat3(&targetfloat3, targetVec);
        Camera::instance().SetLookAt(old_Pos, targetfloat3, DirectX::XMFLOAT3{ 0,1,0 });
    }
    else//Fps
    {
        Changecount = 1;
        BlendRate(old_changeflag, changeflag, flag, elapsedtime);

        float r = 0.8f;
        DirectX::XMStoreFloat(&range, DirectX::XMVectorLerp(DirectX::XMLoadFloat(&range), DirectX::XMLoadFloat(&r), 0.5f));
#if 0
        eye.x = target.x - front.x * range + right.x * range + upper.x * range;
        eye.y = target.y - front.y * range + right.y * range + upper.y * range;
        /*float yy = p.y - front.y ;
        yy < 0.1f ? eye.y = 0.1f : eye.y = yy;*/
        eye.z = target.z - front.z * range + right.z * range + upper.z * range;
#else

        eye.x = target.x - front.x * range + right.x * range + upper.x * range;
        eye.y = target.y - front.y * range + right.y * range + upper.y * range;
        /*float yy = p.y - front.y ;
        yy < 0.1f ? eye.y = 0.1f : eye.y = yy;*/
        eye.z = target.z - front.z * range + right.z * range + upper.z * range;
        origin_Pos = eye;

        if (Rate < 1.0f)
        {
            DirectX::XMStoreFloat3(&old_Pos, DirectX::XMVectorLerp(
                DirectX::XMLoadFloat3(&old_Pos), DirectX::XMLoadFloat3(&origin_Pos), Rate));
        }
        else
        {
            old_Pos = eye;
        }

        DirectX::XMVECTOR targetVec = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&old_Pos), DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&front)));
        DirectX::XMFLOAT3 targetfloat3;

        DirectX::XMStoreFloat3(&targetfloat3, targetVec);
        old = targetfloat3;

        Camera::instance().SetLookAt(old_Pos, targetfloat3, DirectX::XMFLOAT3{ 0,1,0 });



        if (angle.y < -DirectX::XM_PI)
        {
            angle.y += DirectX::XM_2PI;
        }
        if (angle.y > DirectX::XM_PI)
        {
            angle.y -= DirectX::XM_2PI;
        }
#endif
    }



    //�J�����̎��_�ƒ����_��ݒ�
    //Camera::instance().SetLookAt(eye, camerapos, DirectX::XMFLOAT3{ 0,1,0 });


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
#endif

}
/// <summary>
/// 
/// </summary>
/// <param name="old_flag">�؂�ւ��O�̃`�F���W�t���O</param>
/// <param name="changeflag">�؂�ւ������̃`�F���W�t���O</param>
/// <returns></returns>
float CameraController::BlendRate(bool& old_flag, bool changeflag, bool& flag, float elapsedTime)
{
    if (old_flag != changeflag)
    {
        blendtimer = 0;
        old_flag = changeflag;
    }
    if (blendtimer < blendsecond)
    {
        blendtimer += elapsedTime;
        if (blendtimer >= blendsecond)
        {
            flag = false;
        }
        Rate = blendtimer / blendsecond;
        Rate *= Rate;
    }
    return Rate;
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
            ImGui::InputFloat("CameraLength", &range);

        }
    }

    if (ImGui::CollapsingHeader("Camera�֘A", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputFloat("cameraPosY", &angle.x);
        ImGui::InputFloat("cameraPosX", &old.x);
        ImGui::InputFloat("cameraPosZ", &old.z);
        ImGui::InputFloat3("RightArm", &rightShoulderOffset.x);
        DebugRenderer* debugRender = Graphics::Instance().GetDebugRenderer();
        player* pl = PlayerManager::Instance().GetPlayer(0);

        debugRender->DrawSphere(old, 0.2f, { 1,0,0,1 });

    }
    ImGui::End();
}