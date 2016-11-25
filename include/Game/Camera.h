#pragma once

#include "SharedContext.h"
class Entity;
class Camera
{
public:
	Camera(const SharedContext& sharedContext);
	~Camera();

	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	void update(const Entity& players);
	void reset();

private:
	SharedContext m_sharedContext;
};

