#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

//マスクデータ
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
};

//平行光源情報
struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction; //向き
	DirectX::XMFLOAT4 color; //色
};

// レンダーコンテキスト
struct RenderContext
{
	ID3D11DeviceContext* deviceContext;

	DirectX::XMFLOAT4 viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	DirectX::XMFLOAT4		lightDirection;
	MaskData maskData;
	//ライト情報
	DirectX::XMFLOAT4 ambientLightColor;
	DirectionalLightData directionalLightData;
};
