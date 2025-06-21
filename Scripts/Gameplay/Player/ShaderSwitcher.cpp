#include "ShaderSwitcher.h"
#include "Debug.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "EventCenter.h"
#include "RenderStateConfig.h"

void ShaderSwitcher::Start()
{
	spriteRenderer = gameObject->GetComponent<SpriteRenderer>();
	if (!spriteRenderer)
	{
		Debug::Log("SpriteRenderer is not found.", Debug::LogLevel::ERR);
		return;
	}
	currentRenderStateConfig = spriteRenderer->GetRenderStateConfig();

	EventCenter::instance.AddEventListener(E_EventType::Player_BlackHoleCreate, this, &ShaderSwitcher::SwitchToShader);
	EventCenter::instance.AddEventListener(E_EventType::Player_BlackHoleExpand, this, &ShaderSwitcher::SwitchBackShader);
	EventCenter::instance.AddEventListener(E_EventType::BlackHole_Destroy, this, &ShaderSwitcher::SwitchBackShader);
}

void ShaderSwitcher::SetTargetRenderStateConfig(std::shared_ptr<RenderStateConfig> config)
{
	targetRenderStateConfig = config;
}

void ShaderSwitcher::OnDestroy()
{
	EventCenter::instance.RemoveEventListener(E_EventType::Player_BlackHoleCreate, this, &ShaderSwitcher::SwitchToShader);
	EventCenter::instance.RemoveEventListener(E_EventType::Player_BlackHoleExpand, this, &ShaderSwitcher::SwitchBackShader);
	EventCenter::instance.RemoveEventListener(E_EventType::BlackHole_Destroy, this, &ShaderSwitcher::SwitchBackShader);
}

void ShaderSwitcher::SwitchToShader()
{
	spriteRenderer->SetRenderStateConfig(targetRenderStateConfig);
}

void ShaderSwitcher::SwitchBackShader()
{
	spriteRenderer->SetRenderStateConfig(currentRenderStateConfig);
}