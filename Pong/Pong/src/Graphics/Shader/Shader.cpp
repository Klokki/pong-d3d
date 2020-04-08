#include "pch.h"
#include "Shader.hpp"

void VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath)
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
}
