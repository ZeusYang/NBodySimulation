#pragma once

#include "NBodySystem.h"

class GalaxyDemo
{
private:
	NBody::NBodySystem::ptr m_simulator;

public:
	GalaxyDemo();
	~GalaxyDemo();

	void datFileGalaxy(const std::string &path);
	void tabFileGalaxy(const std::string &path);
	void snapFileGalaxy(const std::string &path);
	void tipsyGalaxy(const std::string &path);
	void shellGalaxy(float clusterScale, float velocityScale, int numBodies);

	NBody::NBodySystem::ptr getSimulator() const
	{
		return m_simulator;
	}
};

