#pragma once

class VertexShader
{
public:
	void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath);
	ID3D11VertexShader* GetShader() { return m_vertexShader.Get(); }
	ID3D10Blob* GetBuffer() { return m_vertexShaderBuffer.Get(); }
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D10Blob> m_vertexShaderBuffer = nullptr;
};