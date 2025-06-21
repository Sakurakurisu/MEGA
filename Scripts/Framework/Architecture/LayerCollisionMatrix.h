#pragma once

#include <array>
#include "E_Layer.h"

class LayerCollisionMatrix
{
public:
	LayerCollisionMatrix() = delete;
	LayerCollisionMatrix(const LayerCollisionMatrix&) = delete;
	LayerCollisionMatrix& operator=(const LayerCollisionMatrix&) = delete;

	static bool ShouldCollide(E_Layer layer1, E_Layer layer2)
	{
		return collisionRules[static_cast<size_t>(layer1)][static_cast<size_t>(layer2)];
	}

private:
	static constexpr bool collisionRules[static_cast<size_t>(E_Layer::MaxLayers)][static_cast<size_t>(E_Layer::MaxLayers)] =
	{
		//                 Default   Player    Enemy     Projectile  Ground    UI 	  HitBox GroundChecker BlackHole EnemyPerc   fan      UIField
		/* Default */      {true,    true,     true,     true,      true,     false,    false,    false,    false,    false,	false,    false,},
		/* Player */       {true,    false,    false,    true,      true,     false,    false,    false,    true,     true,		true,     false,},
		/* Enemy */        {true,    false,    false,    true,      true,     false,    true,     false,    true,     false,	false,    false,},
		/* Projectile */   {true,    true,     true,     false,     true,     false,    false,    false,    false,    false,	false,    false,},
		/* Ground */       {true,    true,     true,     true,      false,    false,    false,    true,     true,     false,	true,     false,},
		/* UI */           {false,   false,    false,    false,     false,    false,    false,    false,    true,     false,	false,    true,},
		/* HitBox */       {false,   false,    true,     false,     false,    false,    false,    false,    false,    false,	false,	  false,},
		/* GroundChecker */{false,   false,    false,    false,     true,     false,    false,    false,    false,    false,	false,	  false,},
		/* BlackHole */    {false,   true,     true,     false,     true,     true,     false,    false,    false,    false,	false,	  false,},
		/* EnemyPerceive */{false,   true,     false,    false,     false,    false,    false,    false,    false,    false,	false,	  false,},
		/* Fan */          {false,   true,     false,    false,     true,     false,    false,    false,    false,    false,	false,	  false,},
		/* UIFiled */      {false,   false,    false,    false,     false,    true,     false,    false,    false,    false,	false,	  false,},
	};
};