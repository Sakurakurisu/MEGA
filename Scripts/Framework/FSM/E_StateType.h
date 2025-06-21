#pragma once

enum class E_StateType
{
	Default,
	Player_Idle,
	Player_Run,
	Player_LaiSlashCharge,
	Player_Aim,
	Player_Shooting,
	Player_GravityAffected,
	Player_Jump,
	Player_LaiSlash,
	Player_Attack,
	Player_WhirlSlash,
	Player_Fall,

	BlackHole_Move,
	BlackHole_Expand,
	BlackHole_Gravity,

	Enemy_Idle,
	Enemy_Patrol,
	Enemy_Chase,
	Enemy_Attack,
	Enemy_Charge,
	Enemy_Hit,
	Enemy_BePulled,
	Enemy_Die,
};