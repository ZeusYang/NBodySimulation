#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/glm.hpp>

typedef float real;

struct DarkParticle
{
	real mass;
	real pos[3];
	real vel[3];
	real eps;
	int phi;
};

struct StarParticle
{
	real mass;
	real pos[3];
	real vel[3];
	real metals;
	real tform;
	real eps;
	int phi;
};

struct Header
{
	double time;
	int nbodies;
	int ndimension;
	int nsph;
	int ndark;
	int nstar;
};

void readTipsyFile(
	const std::string &fileName,
	std::vector<real> &positions,
	std::vector<real> &velocities,
	std::vector<int> &bodiesIds,
	int &nTotal,
	int &nFirst,
	int &nSecond,
	int &nThird)
{
	char fullFileName[256];
	sprintf(fullFileName, "%s", fileName.c_str());
	
	std::ifstream input(fullFileName, std::ios::in | std::ios::binary);
	if (!input.is_open())
	{
		std::cout << "Couldn't not open the tipsy file: " << fileName << std::endl;
		return;
	}

	Header header;
	input.read((char*)&header, sizeof(header));

	int idummy;
	glm::fvec4 pos;
	glm::fvec4 vel;

	nTotal = header.nbodies;
	nFirst = header.ndark;
	nSecond = header.nstar;
	nThird = header.nsph;
	positions.reserve(4 * nTotal);
	velocities.reserve(4 * nTotal);
	DarkParticle dark;
	StarParticle star;
	for (int index = 0; index < nTotal; ++index)
	{
		if (index < nFirst)
		{
			// dark particle.
			input.read((char*)&dark, sizeof(dark));
			vel.w = dark.eps;
			pos.w = dark.mass;
			pos.x = dark.pos[0];
			pos.y = dark.pos[1];
			pos.z = dark.pos[2];
			vel.x = dark.vel[0];
			vel.y = dark.vel[1];
			vel.z = dark.vel[2];
			idummy = dark.phi;
		}
		else
		{
			// star particle.
			input.read((char*)&star, sizeof(star));
			vel.w = star.eps;
			pos.w = star.mass;
			pos.x = star.pos[0];
			pos.y = star.pos[1];
			pos.z = star.pos[2];
			vel.x = star.vel[0];
			vel.y = star.vel[1];
			vel.z = star.vel[2];
			idummy = star.phi;
		}
		if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z))
			std::cout << "Nan Error\n";
		positions.push_back(pos.x);
		positions.push_back(pos.y);
		positions.push_back(pos.z);
		positions.push_back(pos.w);
		velocities.push_back(vel.x);
		velocities.push_back(vel.y);
		velocities.push_back(vel.z);
		velocities.push_back(vel.w);
		bodiesIds.push_back(idummy);
	}

	// round up to a multiple of 256 bodies.
	int newTotal = nTotal;
	if (nTotal % 256)
		newTotal = (nTotal / 256 + 1) * 256;
	for (int index = nTotal; index < newTotal; ++index)
	{
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		bodiesIds.push_back(index);
		++nFirst;
	}
	nTotal = newTotal;

	input.close();
}

void readTabFile(
	const std::string &fileName,
	std::vector<real> &positions,
	std::vector<real> &velocities,
	int &nTotal)
{
	std::ifstream input;
	input.open(fileName, std::ifstream::in);
	if (input.fail())
	{
		std::cout << "Couldn't not open the tipsy file: " << fileName << std::endl;
		return;
	}
	std::string line;
	glm::vec4 pos, vel;
	nTotal = 0;
	bool first = true;
	bool second = true;
	while (!input.eof())
	{
		getline(input, line);
		std::istringstream iss(line.c_str());
		// mass x y z vx vy vz.
		iss >> pos.w >> pos.x >> pos.y >> pos.z >> vel.x >> vel.y >> vel.z;
		positions.push_back(pos.x);
		positions.push_back(pos.y);
		positions.push_back(pos.z);
		positions.push_back(pos.w);
		velocities.push_back(vel.x);
		velocities.push_back(vel.y);
		velocities.push_back(vel.z);
		velocities.push_back(vel.w);
		++nTotal;
	}
	// round up to a multiple of 256 bodies.
	int newTotal = nTotal;
	if (nTotal % 256)
		newTotal = (nTotal / 256 + 1) * 256;
	for (int index = nTotal; index < newTotal; ++index)
	{
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
	}
	nTotal = newTotal;

	input.close();
}

void readDatFile(
	const std::string &fileName,
	std::vector<real> &positions,
	std::vector<real> &velocities,
	int &nTotal)
{
	std::ifstream input;
	input.open(fileName, std::ifstream::in);
	if (input.fail())
	{
		std::cout << "Couldn't not open the tipsy file: " << fileName << std::endl;
		return;
	}
	std::string line;
	glm::vec4 pos, vel;
	nTotal = 0;
	pos.w = 1.0f;
	while (!input.eof())
	{
		getline(input, line);
		if (line.empty())
			continue;
		std::istringstream iss(line.c_str());
		// z y x vz vy vz.
		iss >> pos.z >> pos.y >> pos.x >> vel.z >> vel.y >> vel.x;
		positions.push_back(pos.x);
		positions.push_back(pos.y);
		positions.push_back(pos.z);
		positions.push_back(pos.w);
		velocities.push_back(vel.x);
		velocities.push_back(vel.y);
		velocities.push_back(vel.z);
		velocities.push_back(vel.w);
		++nTotal;
	}
	// round up to a multiple of 256 bodies.
	int newTotal = nTotal;
	if (nTotal % 256)
		newTotal = (nTotal / 256 + 1) * 256;
	for (int index = nTotal; index < newTotal; ++index)
	{
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
	}
	nTotal = newTotal;

	input.close();
}

void readSnapFile(
	const std::string &fileName,
	std::vector<real> &positions,
	std::vector<real> &velocities,
	int &nTotal)
{
	std::ifstream input;
	input.open(fileName, std::ifstream::in);
	if (input.fail())
	{
		std::cout << "Couldn't not open the tipsy file: " << fileName << std::endl;
		return;
	}
	std::string line;
	glm::vec4 pos, vel;
	nTotal = 0;

	/**********************************
		nbodies 
		ndim
		time
		masses
		x y z
		vx vy vz
		eps (and NOT the potential)
	**********************************/
	// nbodies.
	getline(input, line);
	std::istringstream iss(line.c_str());
	iss >> nTotal;
	positions.resize(4 * nTotal);
	velocities.resize(4 * nTotal);

	// ndim.
	int dim = 0;
	getline(input, line);
	iss.str(line.c_str());
	iss >> dim;

	// time.
	float time;
	getline(input, line);
	iss.str(line.c_str());
	iss >> time;

	//masses.
	for (int x = 0; x < nTotal; ++x)
	{
		if (input.eof())
		{
			std::cout << "It's not a normal snap file:" << fileName << std::endl;
			return;
		}
		getline(input, line);
		std::istringstream iss(line.c_str());
		iss >> pos.w;
		positions[4 * x + 3] = pos.w;
	}

	// pos.
	for (int x = 0; x < nTotal; ++x)
	{
		if (input.eof())
		{
			std::cout << "It's not a normal snap file:" << fileName << std::endl;
			return;
		}
		getline(input, line);
		std::istringstream iss(line.c_str());
		iss >> pos.x >> pos.y >> pos.z;
		positions[4 * x + 0] = pos.x;
		positions[4 * x + 1] = pos.y;
		positions[4 * x + 2] = pos.z;
	}

	// vel.
	for (int x = 0; x < nTotal; ++x)
	{
		if (input.eof())
		{
			std::cout << "It's not a normal snap file:" << fileName << std::endl;
			return;
		}
		getline(input, line);
		std::istringstream iss(line.c_str());
		iss >> vel.x >> vel.y >> vel.z;
		velocities[4 * x + 0] = vel.x;
		velocities[4 * x + 1] = vel.y;
		velocities[4 * x + 2] = vel.z;
	}

	// eps.
	for (int x = 0; x < nTotal; ++x)
	{
		if (input.eof())
		{
			std::cout << "It's not a normal snap file:" << fileName << std::endl;
			return;
		}
		getline(input, line);
		std::istringstream iss(line.c_str());
		iss >> vel.w;
		velocities[4 * x + 3] = vel.w;
	}

	// round up to a multiple of 256 bodies.
	int newTotal = nTotal;
	if (nTotal % 256)
		newTotal = (nTotal / 256 + 1) * 256;
	for (int index = nTotal; index < newTotal; ++index)
	{
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		positions.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
		velocities.push_back(0.0f);
	}
	nTotal = newTotal;

	input.close();
}