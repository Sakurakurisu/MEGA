#pragma once

#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include <unordered_map>
#include <memory>
#include "BaseManager.h"
#include <vector>
#include "RenderStateConfig.h"
#include "ShaderMgr.h"
#include "RenderStates.h"

using Microsoft::WRL::ComPtr;

struct RenderStateInfo
{
	std::string id;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	std::string inputLayout;
	std::string vertexShader;
	std::string pixelShader;
	std::string rasterizerStateName;  
	std::string blendStateName;       
	std::string depthStencilStateName;
	UINT stencilRef = 0;
	std::string samplerStateName;     
};


class RenderStateMgr : public BaseManager<RenderStateMgr>
{
	friend class BaseManager<RenderStateMgr>;
public:
	void LoadRenderStates(ID3D11Device* device);

	std::shared_ptr<RenderStateConfig> GetRenderState(const std::string& id);

private:
	RenderStateMgr() = default;
	~RenderStateMgr() = default;
	RenderStateMgr(const RenderStateMgr&) = delete;
	RenderStateMgr& operator=(const RenderStateMgr&) = delete;

	std::unordered_map<std::string, std::shared_ptr<RenderStateConfig>> renderStatePool;
};

static const std::vector<RenderStateInfo> renderStateInfos =
{
    {
        "2DRenderDefault",
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        "Basic_2D_VS",
        "Basic_2D_VS",
        "Basic_2D_PS",
        "",                     
        "",                     
        "DSSNoDepthTest",       
        0,
        "SSLinearWrap"          
    },
    {
        "2DRenderAlphaBlend",
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        "Basic_2D_VS",
        "Basic_2D_VS",
        "Basic_2D_PS",
        "RSNoCull",            
        "BSTransparent",       
        "DSSNoDepthTest",
        0,
        "SSLinearWrap"
    },
    {
        "2DRenderAlphaBlendPoint",
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        "Basic_2D_VS",
        "Basic_2D_VS",
        "Basic_2D_PS",
        "RSNoCull",
        "BSTransparent",
        "DSSNoDepthTest",
        0,
        "SSPointWrap"
    },
    {
        "2DRenderWireframe",
        D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
        "Basic_2D_VS",
        "Basic_2D_VS",
        "Wireframe_2D_PS",
        "RSWireframe",
        "",           
        "DSSNoDepthTest",
        0,
        "SSLinearWrap"
    },
    {
        "2DRenderAlphaBlendPointGrivatescale",
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        "Basic_2D_VS",
        "Basic_2D_VS",
        "Grayscale_2D_PS",
        "RSNoCull",
        "BSTransparent",
        "DSSNoDepthTest",
        0,
        "SSPointWrap"
    },

       {
        "PlayerShaderPoint",
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        "Basic_2D_VS",
        "Basic_2D_VS",
        "Flashable_2D_PS",
        "RSNoCull",
        "BSTransparent",
        "DSSNoDepthTest",
        0,
        "SSPointWrap"
    },
};