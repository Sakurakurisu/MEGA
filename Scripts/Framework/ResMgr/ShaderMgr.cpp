#include "ShaderMgr.h"
#include "d3dUtil.h"
#include "DXTrace.h"
#include <stdexcept>

void ShaderMgr::LoadAllShaders(ID3D11Device* device)
{
	ComPtr<ID3D11InputLayout> inputLayout;
	for (const auto& info : vertexShaderInfos)
	{
		auto vs = LoadVertexShader(device, info, inputLayout);
	}

	for (const auto& info : pixelShaderInfos)
	{
		auto ps = LoadPixelShader(device, info);
	}
}

ComPtr<ID3D11VertexShader> ShaderMgr::LoadVertexShader(
	ID3D11Device* device, const VertexShaderInfo& info,
	ComPtr<ID3D11InputLayout>& inputLayout)
{
	ComPtr<ID3DBlob> blob;
	HR(CreateShaderFromFile(info.shaderFile.c_str(), info.shaderSource.c_str(),
							info.entryPoint.c_str(), info.shaderModel.c_str(), blob.GetAddressOf()));

	ComPtr<ID3D11VertexShader> vs;
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vs.GetAddressOf()));

	HR(device->CreateInputLayout(info.layoutDesc, info.numElements,
								 blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf()));

	vertexShaderPool[info.id] = vs;
	inputLayoutPool[info.id] = inputLayout;

	return vs;
}

ComPtr<ID3D11PixelShader> ShaderMgr::LoadPixelShader(ID3D11Device* device, const PixelShaderInfo& info)
{
	ComPtr<ID3DBlob> blob;
	HR(CreateShaderFromFile(info.shaderFile.c_str(), info.shaderSource.c_str(),
							info.entryPoint.c_str(), info.shaderModel.c_str(), blob.ReleaseAndGetAddressOf()));

	ComPtr<ID3D11PixelShader> ps;
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, ps.GetAddressOf()));

	pixelShaderPool[info.id] = ps;
	return ps;
}

ComPtr<ID3D11VertexShader> ShaderMgr::GetVertexShader(const std::string& id)
{
	auto it = vertexShaderPool.find(id);

	if (it != vertexShaderPool.end())
		return it->second;

	throw std::runtime_error(id + ":Vertex Shader not found");
	return nullptr;
}

ComPtr<ID3D11PixelShader> ShaderMgr::GetPixelShader(const std::string& id)
{
	auto it = pixelShaderPool.find(id);

	if (it != pixelShaderPool.end())
		return it->second;

	throw std::runtime_error(id + ":Pixel Shader not found");
	return nullptr;
}

ComPtr<ID3D11InputLayout> ShaderMgr::GetInputLayout(const std::string& id)
{
	auto it = inputLayoutPool.find(id);

	if (it != inputLayoutPool.end())
		return it->second;

	throw std::runtime_error(id + ":Input Layout not found");
	return nullptr;
}

void ShaderMgr::SetAllDebugNames()
{
	for (const auto& pair : vertexShaderPool)
	{
		const std::string& id = pair.first;
		D3D11SetDebugObjectName(pair.second.Get(), id.c_str());
	}

	for (const auto& pair : pixelShaderPool)
	{
		const std::string& id = pair.first;
		D3D11SetDebugObjectName(pair.second.Get(), id.c_str());
	}

	for (const auto& pair : inputLayoutPool)
	{
		std::string name = pair.first + " InputLayout";
		D3D11SetDebugObjectName(pair.second.Get(), name.c_str());
	}
}