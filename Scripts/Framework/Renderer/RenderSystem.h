#pragma once  

#include "BaseManager.h"  
#include "VertexBuffer.h"  
#include "LightHelper.h"  
#include "UVRect.h"  
#include <memory>  

class Camera;  
class GameObject;  
struct Material;  
struct RenderStateConfig;  

using namespace DirectX;  

struct RenderCommand  
{  
GameObject* gameObject = nullptr;  
ID3D11ShaderResourceView* texture = nullptr;  
XMMATRIX worldMatrix = XMMatrixIdentity();  
XMFLOAT4 color{ 1.0f,1.0f,1.0f,1.0f };  
Material material{};  
Mesh mesh;  

VertexBuffer* vertexBuffer = nullptr;  
RenderStateConfig* renderStateConfig = nullptr;  
};  

struct TextCommand  
{  
std::string text;  
XMFLOAT3 position{ 0.0f, 0.0f, 0.0f };  
float fontSize = 20.0f;  
XMFLOAT4 color{ 1.0f, 1.0f, 1.0f, 1.0f };  
const WCHAR* fontFamilyName = L"MS ゴシック";  
};  

class RenderSystem : public BaseManager<RenderSystem>  
{  
friend class BaseManager<RenderSystem>;  
public:  
// レンダーコマンドを追加します  
void AddRenderCommand(const RenderCommand& command);  

// テキストレンダーコマンドを追加します  
void AddRenderTextCommand(const TextCommand& command);  

// カメラを使用してレンダリングを実行します  
void Render(Camera* camera);  

// カメラを使用してテキストをレンダリングします  
void RenderText(Camera* camera);  
private:  
RenderSystem() = default;  
RenderSystem(const RenderSystem&) = delete;  
RenderSystem& operator=(const RenderSystem&) = delete;  

std::vector<RenderCommand> renderCommands{};  
std::vector<TextCommand> textCommands{};  
};
