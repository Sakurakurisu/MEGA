#pragma once

#include "Component.h"
#include <functional>
#include <d3d11.h>
#include <memory>

class SpriteRenderer;
struct RenderStateConfig;

class ShaderSwitcher : public Component
{
public:
	ShaderSwitcher() = default;

	void Start() override;

	void SetTargetRenderStateConfig(std::shared_ptr<RenderStateConfig> config);

	void OnDestroy() override;

private:
	SpriteRenderer* spriteRenderer = nullptr;

	std::shared_ptr<RenderStateConfig> currentRenderStateConfig = nullptr;
	std::shared_ptr<RenderStateConfig> targetRenderStateConfig = nullptr;

	void SwitchToShader();

	void SwitchBackShader();
};
