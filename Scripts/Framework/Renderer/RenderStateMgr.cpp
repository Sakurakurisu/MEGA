#include "RenderStateMgr.h"
#include "RenderStates.h"
#include "RenderStateConfig.h"
#include <stdexcept>


void RenderStateMgr::LoadRenderStates(ID3D11Device* device)
{
    if (!RenderStates::IsInit())
        throw std::exception("RenderStates need to be initialized first!");

    for (const auto& info : renderStateInfos)
    {
        RenderStateConfig config;
        config.topology = info.topology;
        config.inputLayout = ShaderMgr::instance.GetInputLayout(info.inputLayout);
        config.vertexShader = ShaderMgr::instance.GetVertexShader(info.vertexShader);
        config.pixelShader = ShaderMgr::instance.GetPixelShader(info.pixelShader);

        if (info.rasterizerStateName == "RSNoCull")
            config.rasterizerState = RenderStates::RSNoCull;
        else if (info.rasterizerStateName == "RSWireframe")
            config.rasterizerState = RenderStates::RSWireframe;
        else if (info.rasterizerStateName == "RSCullClockWise")
            config.rasterizerState = RenderStates::RSCullClockWise;
        else
            config.rasterizerState = nullptr;

        if (info.blendStateName == "BSTransparent")
            config.blendState = RenderStates::BSTransparent;
        else if (info.blendStateName == "BSAlphaToCoverage")
            config.blendState = RenderStates::BSAlphaToCoverage;
        else if (info.blendStateName == "BSAdditive")
            config.blendState = RenderStates::BSAdditive;
        else if (info.blendStateName == "BSNoColorWrite")
            config.blendState = RenderStates::BSNoColorWrite;
        else
            config.blendState = nullptr;

        if (info.depthStencilStateName == "DSSNoDepthTest")
            config.depthStencilState = RenderStates::DSSNoDepthTest;
        else if (info.depthStencilStateName == "DSSWriteStencil")
            config.depthStencilState = RenderStates::DSSWriteStencil;
        else if (info.depthStencilStateName == "DSSDrawWithStencil")
            config.depthStencilState = RenderStates::DSSDrawWithStencil;
        else if (info.depthStencilStateName == "DSSNoDoubleBlend")
            config.depthStencilState = RenderStates::DSSNoDoubleBlend;
        else if (info.depthStencilStateName == "DSSNoDepthWrite")
            config.depthStencilState = RenderStates::DSSNoDepthWrite;
        else if (info.depthStencilStateName == "DSSNoDepthTestWithStencil")
            config.depthStencilState = RenderStates::DSSNoDepthTestWithStencil;
        else if (info.depthStencilStateName == "DSSNoDepthWriteWithStencil")
            config.depthStencilState = RenderStates::DSSNoDepthWriteWithStencil;
        else
            config.depthStencilState = nullptr;

        if (info.samplerStateName == "SSLinearWrap")
            config.samplerState = RenderStates::SSLinearWrap;
        else if (info.samplerStateName == "SSAnisotropicWrap")
            config.samplerState = RenderStates::SSAnisotropicWrap;
        else if (info.samplerStateName == "SSPointWrap")
            config.samplerState = RenderStates::SSPointWrap;
        else
            config.samplerState = nullptr;

        config.stencilRef = info.stencilRef;

        renderStatePool[info.id] = std::make_shared<RenderStateConfig>(config);
    }
}

std::shared_ptr<RenderStateConfig> RenderStateMgr::GetRenderState(const std::string& id)
{
	auto it = renderStatePool.find(id);
	if (it != renderStatePool.end())
		return it->second;

	throw std::runtime_error(id + ":Render State not found");
	return nullptr;
}
