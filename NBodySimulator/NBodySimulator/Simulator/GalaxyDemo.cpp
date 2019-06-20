#include "GalaxyDemo.h"

#include "glm/glm.hpp"

using namespace std;
using namespace NBody;

GalaxyDemo::GalaxyDemo()
	:m_simulator(nullptr)
{
}

GalaxyDemo::~GalaxyDemo()
{
}

void GalaxyDemo::datFileGalaxy(const std::string & path)
{
	m_simulator = shared_ptr<NBodySystem>(new NBodySystem(1, 256));
	m_simulator->loadDatFile(path);
}

void GalaxyDemo::tabFileGalaxy(const std::string & path)
{
	m_simulator = shared_ptr<NBodySystem>(new NBodySystem(1, 256));
	m_simulator->loadTabFile(path);
}

void GalaxyDemo::snapFileGalaxy(const std::string & path)
{
	m_simulator = shared_ptr<NBodySystem>(new NBodySystem(1, 256));
	m_simulator->loadSnapFile(path);
}

void GalaxyDemo::tipsyGalaxy(const std::string & path)
{
	m_simulator = shared_ptr<NBodySystem>(new NBodySystem(1, 256));
	m_simulator->loadTipsyFile(path); 
}

void GalaxyDemo::shellGalaxy(float clusterScale, float velocityScale, int numBodies)
{
	real *pos = new real[4 * numBodies];
	real *vel = new real[4 * numBodies];
	float scale = clusterScale;
	float vscale = scale * velocityScale;
	float inner = 2.5f * scale;
	float outer = 4.0f * scale;

	int count = 0;
	int pIndex = 0, vIndex = 0;

	pos[pIndex++] = pos[pIndex++] = pos[pIndex++] = 0;
	pos[pIndex++] = 100000.0f;
	vel[vIndex++] = 0;
	vel[vIndex++] = 0;
	vel[vIndex++] = 0;
	vel[vIndex++] = 0;
	++count;

	while (count < numBodies)
	{
		glm::vec3 point;
		point.x = rand() / (float)RAND_MAX * 2 - 1;
		point.y = rand() / (float)RAND_MAX * 2 - 1;
		point.z = rand() / (float)RAND_MAX * 2 - 1;

		float len = glm::length(point);
		if (len > 1) continue;

		point = glm::normalize(point);
		pos[pIndex++] = point.x * (inner + (outer - inner) * rand() / (float)RAND_MAX);
		pos[pIndex++] = point.y * (inner + (outer - inner) * rand() / (float)RAND_MAX);
		pos[pIndex++] = point.z * (inner + (outer - inner) * rand() / (float)RAND_MAX);
		pos[pIndex++] = 1.0f;
		point = glm::normalize(point);
		glm::vec3 axis(0.0f, 0.0f, 1.0f);
		axis = glm::normalize(axis);

		if(glm::dot(point, axis) > (1.0f - 1e-6))
		{
			axis.x = point.y;
			axis.y = point.x;
			axis = glm::normalize(axis);
		}

		glm::vec3 vv;
		vv.x = pos[4 * count + 0];
		vv.y = pos[4 * count + 1];
		vv.z = pos[4 * count + 2];

		vv = glm::cross(vv, axis);
		vel[vIndex++] = vv.x * vscale;
		vel[vIndex++] = vv.y * vscale;
		vel[vIndex++] = vv.z * vscale;
		vel[vIndex++] = 1.0f;

		++count;
	}
	m_simulator = shared_ptr<NBodySystem>(new NBodySystem(numBodies, 256));
	m_simulator->setParticlesPosition(pos);
	m_simulator->setParticlesVelocity(vel);

	delete[] pos;
	delete[] vel;
}
