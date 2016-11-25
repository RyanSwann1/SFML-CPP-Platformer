#pragma once
enum class EntityState
{
	Idle,
	Walking,
	Attacking,
	Hurt,
	Dead,
	Count = Dead + 1
};