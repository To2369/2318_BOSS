#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//マスクデータ
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
};

// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	MaskData maskData;
};
