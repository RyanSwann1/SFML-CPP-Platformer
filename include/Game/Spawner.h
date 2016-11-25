#pragma once

struct SharedContext;
class Spawner
{
public:
	Spawner(SharedContext* sharedContext);
	~Spawner();

private:
	SharedContext* m_sharedContext;
};

