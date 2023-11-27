#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//�}�X�N�f�[�^
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
};

//���s�������
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction; //����
	DirectX::XMFLOAT4 color; //�F
};

// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	DirectX::XMFLOAT4 viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	MaskData maskData;
	//���C�g���
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLightData directionalLightData;
};
