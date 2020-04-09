#include "pch.h"
#include "Shader.hpp"

void VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath,
	D3D11_INPUT_ELEMENT_DESC* description, UINT numElements)
{
	HRESULT hr = D3DReadFileToBlob(filepath.c_str(), &m_vertexShaderBuffer);

	if (FAILED(hr))
	{
		std::wstring errorMsg = L"Failed to load shader: ";
		errorMsg += filepath;
		Error::Message(hr, errorMsg);
		exit(EXIT_FAILURE);
	}

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
