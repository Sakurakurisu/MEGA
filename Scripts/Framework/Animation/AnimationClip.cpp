#include "AnimationClip.h"
#include <stdexcept>

AnimationClip::AnimationClip(std::shared_ptr<Atlas> atlas, float timePerFrame, bool loop) : atlas(atlas), timePerFrame(timePerFrame), isLoop(loop)
{
	if (!atlas)
		throw std::invalid_argument("Atlas cannot be null.");
}

float AnimationClip::GetTimePerFrame() const
{
	return timePerFrame;
}

bool AnimationClip::IsLoop() const
{
	return isLoop;
}

std::shared_ptr<Sprite> AnimationClip::GetFrameSprite(int frameIndex) const
{
	return atlas->GetSprite(frameIndex);
}

int AnimationClip::GetFrameCount() const
{
	return atlas->GetSpriteCount();
}

const std::shared_ptr<Atlas> AnimationClip::GetAtlas() const
{
	return atlas;
}