#pragma once

#include "BaseManager.h"
#include <unordered_map>
#include <string>
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <vector>

class Atlas;
class Sprite;
class AnimationClip;

struct SpriteResInfo
{
std::string id;
std::wstring path;

SpriteResInfo(const std::string& id, const std::wstring& path)
	: id(id), path(path) {}
};

struct AtlasResInfo
{
std::string id;
std::wstring path;
unsigned columns;
unsigned rows;

AtlasResInfo(const std::string& id, const std::wstring& path, unsigned columns, unsigned rows = 1)
	: id(id), path(path), columns(columns), rows(rows) {}
};

struct AnimationResInfo
{
std::string id;
std::wstring path;
float timePerFrame;
bool loop;
unsigned columns;
unsigned rows;

AnimationResInfo(const std::string& id, const std::wstring& path, float framesPerUpdate, bool loop, unsigned columns, unsigned rows = 1)
	: id(id), path(path), timePerFrame(framesPerUpdate), loop(loop), columns(columns), rows(rows) {}
};

class ResourceMgr : public BaseManager<ResourceMgr>
{
friend class BaseManager<ResourceMgr>;
public:

// 指定されたIDに基づいてスプライトを取得します。
std::shared_ptr<Sprite> GetSprite(const std::string& id) const;

// 指定されたIDに基づいてアトラスを取得します。
std::shared_ptr<Atlas> GetAtlas(const std::string& id) const;

// 指定されたIDに基づいてアニメーションクリップを取得します。
std::shared_ptr<AnimationClip> GetAnimationClip(const std::string& id) const;

// リソースをロードします。
void Load();

private:
ResourceMgr() = default;
ResourceMgr(const ResourceMgr&) = delete;
ResourceMgr& operator=(const ResourceMgr&) = delete;

bool LoadSprite(const SpriteResInfo& info);

bool LoadAtlas(const AtlasResInfo& info);

bool LoadAnimationClip(const AnimationResInfo& info);

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateTexture(const std::wstring& filename);

std::unordered_map<std::string, std::shared_ptr<Sprite>> spritePool;
std::unordered_map<std::string, std::shared_ptr<Atlas>> atlasPool;
std::unordered_map<std::string, std::shared_ptr<AnimationClip>> animPool;
};

static const std::vector<SpriteResInfo> spriteInfos =
{
//{"test_grid", L"assets/sprite/test_grid.png"},
};

static const std::vector<AtlasResInfo> atlasInfos =
{
//{"ScoreNumber", L"assets/atlas/Number_Score.png", 12},
};

static const std::vector<AnimationResInfo> animInfos =
{
//{"player_idle", L"assets/animation/player/player_idle.png", 0.067f, true, 4},
};
