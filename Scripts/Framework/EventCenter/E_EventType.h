#pragma once

enum class E_EventType
{
	Attack,

	BeAttacked,

	Player_BlackHoleCancel,

	Player_BlackHoleExpand,

	Player_BlackHoleCreate,

	Player_JustPoint,

	BlackHole_Destroy,

	Anim_Complete,

	Enemy_Dead,

	Ground_Check,

	CheckExplosion,

	ReachTheGoal,

	ReceiveBlast,

	GameEnd,
};