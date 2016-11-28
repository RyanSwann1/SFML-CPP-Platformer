#pragma once
#include "Entities\Character.h"
#include "Game\Timer.h"
class Rat :
	public Character
{
public:
	Rat(const SharedContext& sharedContext, const std::string& name = "Rat");
	~Rat();

private:
	bool m_attackReady;
	Timer m_attackTimer;

	void handleAttack(const float deltaTime);
	void handleMovement();
	void onEntityCollision(Entity& entity) override; //Check to see if this is being properly called
	void resolveCollisions(std::vector<CollisionElement*>& collisions) override;
	void update(const float deltaTime) override;
};