#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

#include "Texture.h"

HRESULT CreateSampler(
	ID3D11Device* pd3dDevice,
	ID3D11DeviceContext* pImmediateContext,
	ID3D11SamplerState** pSampler
)
{
	HRESULT hr = S_OK;

	// Create the sample state
	D3D11_SAMPLER_DESC samplDesc = {};
	samplDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;


	samplDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplDesc.MinLOD = 0;
	samplDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = pd3dDevice->CreateSamplerState(&samplDesc, pSampler);
	if (FAILED(hr))
		return hr;

	// Sampler
	pImmediateContext->PSSetSamplers(0, 1, pSampler);

	return hr;
}

HRESULT Create2DTexture(
	ID3D11Device* pd3dDevice,
	ID3D11Texture2D**		   mRenderTargetTexture,
	ID3D11RenderTargetView**   mRenderTargetView,
	ID3D11ShaderResourceView** mShaderResourceView,
	const UINT width, const UINT height
)
{
	HRESULT hr = S_OK;

	// 2D Texture
	D3D11_TEXTURE2D_DESC mTextureDesc;

	bool renderTarget = true, shaderResource = true;

	// Initialize the texture description.
	ZeroMemory(&mTextureDesc, sizeof(mTextureDesc));
	mTextureDesc.Width = width;
	mTextureDesc.Height = height;
	mTextureDesc.MipLevels = 1;
	mTextureDesc.ArraySize = 1;
	mTextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	mTextureDesc.SampleDesc.Count = 1;
	mTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	mTextureDesc.CPUAccessFlags = 0;
	mTextureDesc.MiscFlags = 0;

	// Checking what kind of texture we want to create
	if (mRenderTargetView == nullptr && mShaderResourceView == nullptr)
	{
		renderTarget = false;
		shaderResource = false;

		mTextureDesc.BindFlags = 0;
	}
	else if (mRenderTargetTexture == nullptr)
	{
		mRenderTargetView = false;
		mTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	}
	else if (mShaderResourceView == nullptr)
	{
		shaderResource = false;
		mTextureDesc.BindFlags = D3D10_BIND_RENDER_TARGET;
	}
	else
	{
		mTextureDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	}

	// Create the texture
	hr = pd3dDevice->CreateTexture2D(&mTextureDesc, NULL, mRenderTargetTexture);

	// Render Target
	if (renderTarget)
	{
		D3D11_RENDER_TARGET_VIEW_DESC mRenderTargetViewDesc;

		// Creating render target
		mRenderTargetViewDesc.Format = mTextureDesc.Format;
		mRenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		mRenderTargetViewDesc.Texture2D.MipSlice = 0;

		hr = pd3dDevice->CreateRenderTargetView(*mRenderTargetTexture, &mRenderTargetViewDesc, mRenderTargetView);
	}

	// Shader Resource View
	if (shaderResource)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC mShaderResourceViewDesc;

		// Creating shader resource view
		mShaderResourceViewDesc.Format = mTextureDesc.Format;
		mShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		mShaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		mShaderResourceViewDesc.Texture2D.MipLevels = 1;

		hr = pd3dDevice->CreateShaderResourceView(*mRenderTargetTexture, &mShaderResourceViewDesc, mShaderResourceView);
	}

	return hr;
}

HRESULT CreateDepthStencilView(
	ID3D11Device* device,
	ID3D11DepthStencilView** depthStencilView,
	ID3D11DepthStencilState** depthStencilState,
	ID3D11Texture2D** texture2D,
	const UINT width,
	const UINT height
)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	// Stencil test parameters
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the Depth/Stencil View
	hr = device->CreateTexture2D(&depthStencilDesc, NULL, texture2D);
	hr = device->CreateDepthStencilView(*texture2D, NULL, depthStencilView);
	hr = device->CreateDepthStencilState(&dsDesc, depthStencilState);

	return hr;
}