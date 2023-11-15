#pragma once
#include<DirectXMath.h>
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>

class EffectManager
{
private:
    EffectManager() {};
    ~EffectManager() {};
public:
    static EffectManager& Instance()
    {
        static EffectManager M_Instance;
        return M_Instance;
    }
    //������
    void Initialize();
    //�I����
    void Finalize();
    //�X�V����
    void Update(float elapsedTime);
    //�`�揈��
    void Render(const DirectX::XMFLOAT4X4&view,const DirectX::XMFLOAT4X4&projection );
    //Effeckseer�}�l�[�W���[�̎擾
    Effekseer::ManagerRef GetEffekseerManager() { return EffekseerManager; };

private:
    //Effeckseer�}�l�[�W���[�̎擾
    Effekseer::ManagerRef EffekseerManager;
    EffekseerRenderer::RendererRef EffekseerRenderer;
};
