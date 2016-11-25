#pragma once
#include "Entities\Computer.h"
#include "Game\Timer.h"
class Rat :
	public Computer
{
public:
	Rat(const SharedContext& sharedContext, const std::string& name = "Rat");
	~Rat();

	void update(const float deltaTime) override;
	void onEntityCollision(Entity& entity) override;

private:
	bool m_attackReady;
	Timer m_attackTimer;

	void handleAttack(const float deltaTime);
	void handleMovement();
};

