#include "pch.h"
#include "Shader.hpp"

void VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath,
	D3D11_INPUT_ELEMENT_DESC* description, UINT numElements)
{
	// read vertex shader
	HRESULT hr = D3DReadFileToBlob(filepath.c_str(), &m_vertexShaderBuffer);

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
		exit(EXIT_FAILURE);
	}

	// create vertex shader
	hr = device->CreateVertexShader(m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(), NULL,
		m_vertexShader.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create vertex shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
		exit(EXIT_FAILURE);
	}

	// create inputlayout
	hr = device->CreateInputLayout(description, numElements,
		m_vertexShaderBuffer->GetBufferPointer(),
		m_vertexShaderBuffer->GetBufferSize(),
		m_inputLayout.GetAddressOf());

	if (FAILED(hr))
	{
		Error::Message(hr, "Could not create input layout");
		exit(EXIT_FAILURE);
	}
}

void PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath)
{
	// read pixel shader
	HRESULT hr = D3DReadFileToBlob(filepath.c_str(), m_pixelShaderBuffer.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
		exit(EXIT_FAILURE);
	}

	// create pixel shader
	hr = device->CreatePixelShader(m_pixelShaderBuffer.Get()->GetBufferPointer(),
		m_pixelShaderBuffer.Get()->GetBufferSize(), NULL,
		m_pixelShader.GetAddressOf());

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to create pixel shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
		exit(EXIT_FAILURE);
	}
}
