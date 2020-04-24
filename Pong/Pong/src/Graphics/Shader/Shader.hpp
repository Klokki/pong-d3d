#pragma once

class VertexShader
{
public:
    void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath,
        D3D11_INPUT_ELEMENT_DESC* description, UINT numElements);
    ID3D11VertexShader* GetShader() { return m_vertexShader.Get(); }
    ID3D10Blob* GetBuffer() { return m_vertexShaderBuffer.Get(); }
    ID3D11InputLayout* GetInputLayout() { return m_inputLayout.Get(); }
private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_vertexShaderBuffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
};

class PixelShader
{
public:
    void Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring filepath);
    ID3D11PixelShader* GetShader() { return m_pixelShader.Get(); }
    ID3D10Blob* GetBuffer() { return m_pixelShaderBuffer.Get(); }
private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
    Microsoft::WRL::ComPtr<ID3D10Blob> m_pixelShaderBuffer = nullptr;
};
