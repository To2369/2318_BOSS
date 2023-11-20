#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//�}�X�N�f�[�^
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
};

// �����_�[�R���e�L�X�g
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	MaskData maskData;
};
