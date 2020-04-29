#include "pch.h"
#include "Shader.hpp"

void VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath,
	D3D11_INPUT_ELEMENT_DESC* description, UINT numElements)
{
	// read vertex shader
	HRESULT hr;

	if (FAILED(hr = D3DReadFileToBlob(filepath.c_str(), &m_vertexShaderBuffer)))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
	}

	// create vertex shader
	if (FAILED(hr = device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(), NULL,
		m_vertexShader.GetAddressOf())))
	{
		std::wstring errorMsg = L"Failed to create vertex shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
	}

	// create inputlayout
	if (FAILED(hr = device->CreateInputLayout(description, numElements,
		m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(),
		m_inputLayout.GetAddressOf())))
	{
		Error::Message(hr, "Could not create input layout");
	}
}

void PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath)
{
	HRESULT hr;

	// read pixel shader
	if (FAILED(hr = D3DReadFileToBlob(filepath.c_str(), m_pixelShaderBuffer.GetAddressOf())))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
	}

	// create pixel shader
	if (FAILED(hr = device->CreatePixelShader(m_pixelShaderBuffer.Get()->GetBufferPointer(),
		m_pixelShaderBuffer.Get()->GetBufferSize(), NULL,
		m_pixelShader.GetAddressOf())))
	{
		std::wstring errorMsg = L"Failed to create pixel shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
	}
}
