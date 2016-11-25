#pragma once
#include "Entities\Character.h"
class Computer :
	public Character
{
public:
	Computer(const SharedContext& sharedContext, const std::string& name);
	~Computer();

	void update(const float deltaTime);

private:
	void resolveCollisions() override;
};

