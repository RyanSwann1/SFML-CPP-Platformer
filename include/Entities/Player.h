#pragma once
#include "Entities\Character.h"
#include "EventDetails.h"
class Player :
	public Character
{
public:
	Player(SharedContext& sharedContext, const std::string& name = "Player");
	~Player();

	void onEntityCollision(Entity& entity) override;
	
private:
	void reactToInput(const EventDetails& eventDetails);
	void resolveCollisions(std::vector<CollisionElement*>& collisions) override;
};

