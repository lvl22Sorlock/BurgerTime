#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->Update(deltaTime);
	}
}

void Scene::LateUpdate(float deltaTime)
{
	for (auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->LateUpdate(deltaTime);
	}
}

void Scene::FixedUpdate(float deltaTime)
{
	for (auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->FixedUpdate(deltaTime);
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if (!object->HasParent())
			object->Render();
	}
}

