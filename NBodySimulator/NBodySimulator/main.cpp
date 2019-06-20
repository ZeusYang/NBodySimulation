#include <iostream>
#include <string>

#include "Renderer/RenderDevice.h"
#include "Renderer/Camera/TPSCamera.h"
#include "Renderer/Drawable/Geometry.h"
#include "Renderer/Drawable/InstanceDrawable.h"
#include "Renderer/Drawable/StaticModelDrawable.h"
#include "Renderer/Drawable/ParticleGroupDrawable.h"
#include "Renderer/Voxelization.h"

#include "Simulator/GalaxyDemo.h"

using namespace std;
using namespace NBody;
using namespace Renderer;

int main(int argc, char *argv[])
{
	// initialization.
	const int width = 1280, height = 780;
	RenderDevice::ptr window = RenderDevice::getSingleton();
	window->initialize("NBody Simulation", width, height, true);
	RenderSystem::ptr renderSys = window->getRenderSystem();

	// resource loading.
	MeshMgr::ptr meshMgr = renderSys->getMeshMgr();
	ShaderMgr::ptr shaderMgr = renderSys->getShaderMgr();
	TextureMgr::ptr textureMgr = renderSys->getTextureMgr();
	// shaders.
	unsigned int simpleColorShader = shaderMgr->loadShader("simple_color",
		"./glsl/simple_color.vert", "./glsl/simple_color.frag");
	unsigned int simpleTextShader = shaderMgr->loadShader("simple_texture",
		"./glsl/simple_texture.vert", "./glsl/simple_texture.frag");
	unsigned int blinnPhongShader = shaderMgr->loadShader("blinn_phong",
		"./glsl/blinn_phong.vert", "./glsl/blinn_phong.frag");

	Camera3D::ptr camera = renderSys->createTPSCamera(
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 0));
	camera->setPerspectiveProject(45.0f, static_cast<float>(width) / height, 0.1f, 500.0f);
	TPSCamera *tpsCamera = reinterpret_cast<TPSCamera*>(camera.get());
	tpsCamera->setPitch(15.0f);
	tpsCamera->setDistance(150.0f);
	tpsCamera->setDistanceLimt(0.0f, 500.0f);
	tpsCamera->setWheelSensitivity(2.0f);

	ParticleGroupDrawable *particleDrawable = new ParticleGroupDrawable(true, 4);
	particleDrawable->setParticleRadius(1.0f);
	GalaxyDemo demo;
	demo.tipsyGalaxy("./res/galaxy_20K.bin");
	NBodySystem::ptr bodySys = demo.getSimulator();
	bodySys->setSofteningFactor(0.1f);
	particleDrawable->setParticleVBO(bodySys->getCurrentBuffer(),
		bodySys->getNumBodyies());
	renderSys->addDrawable(particleDrawable);
	renderSys->setGlowBlur(true);
	renderSys->setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

	bool run = true;
	bool reset = false;
	bool bloomEffect = true;
	float softening = bodySys->getSofteningFactor();
	float deltaTime = 0.016f;
	int item = 0;
	int itemCurrent = 0;
	const char*items[] = {"galaxy20k", "dubinski", "stars", "shell"};
	// renderer loop.
	while (window->run())
	{
		if(run)
			bodySys->simulate(deltaTime);

		window->beginFrame();

		renderSys->render();

		// Demo setting window.
		{
			ImGui::Begin("Astrophysical simulation");
			ImGui::SliderFloat("Softerning factor", &softening, 0.01f, 1.0f);
			ImGui::SliderFloat("Velocity damping", &bodySys->getDamping(), 0.0f, 2.0f);
			ImGui::SliderFloat("Deltatime/Frame", &deltaTime, 0.0f, 1.0f);
			ImGui::Combo("Selected Demo", &item, items, 4);
			if (ImGui::Button("Particle glow effect"))
			{
				bloomEffect = !bloomEffect;
				renderSys->setGlowBlur(bloomEffect);
			}
			ImGui::ColorEdit3("Particle clor", (float*)&particleDrawable->getBaseColor());
			if (ImGui::Button("Reset"))
				reset = true;
			if (ImGui::Button("Run"))
				run = !run;
			ImGui::End();
		}

		if (item != itemCurrent || reset)
		{
			itemCurrent = item;
			switch (itemCurrent)
			{
			case 0:
				demo.tipsyGalaxy("./res/galaxy_20K.bin");
				break;
			case 1:
				demo.tabFileGalaxy("./res/dubinski.tab");
				break;
			case 2:
				demo.datFileGalaxy("./res/stars.dat");
				break;
			case 3:
				//demo.shellGalaxy(5.44f, 0.0f, 8192);
				demo.shellGalaxy(15.44f, 0.0f, 102400);
				break;
			}
			bodySys = demo.getSimulator();
			particleDrawable->setParticleVBO(bodySys->getCurrentBuffer(),
				bodySys->getNumBodyies());
			softening = bodySys->getSofteningFactor();
			reset = false;
		}

		if (fabs(softening - bodySys->getSofteningFactor()) > 0.0001f)
			bodySys->setSofteningFactor(softening);

		window->endFrame();
	}

	bodySys->finalize();
	window->shutdown();

	return 0;
}