#pragma once  
#include "Atlas.h"  

class AnimationClip  
{  
public:  
   // コンストラクタ: アトラス、フレームごとの時間、ループフラグを初期化  
   AnimationClip(std::shared_ptr<Atlas> atlas, float timePerFrame, bool loop = false);  

   ~AnimationClip() = default;  

   // フレームごとの時間を取得  
   float GetTimePerFrame() const;  

   // ループするかどうかを確認  
   bool IsLoop() const;  

   // 指定されたフレームインデックスのスプライトを取得  
   std::shared_ptr<Sprite> GetFrameSprite(int frameIndex) const;  

   // フレーム数を取得  
   int GetFrameCount() const;  

   // アトラスを取得  
   const std::shared_ptr<Atlas> GetAtlas() const;  

private:  
   std::shared_ptr<Atlas> atlas; // アトラスオブジェクト  
   float timePerFrame = 0.0f;    // フレームごとの時間  
   bool isLoop = false;         // ループフラグ  
};