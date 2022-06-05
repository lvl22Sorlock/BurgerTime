#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
using namespace dae;
#include "ComponentBase.h"
#include <typeinfo>

GameObject::GameObject()
	: m_ChildrenPtrs{}
	, m_ComponentPtrs{}
	, m_pParent{nullptr}
	, m_Transform{}
	, m_LocalTransform{}
	, m_HasPositionChanged{false}
{

}

dae::GameObject::~GameObject()
{
	for (std::pair<const size_t, ComponentBase*>& pairStringPtrComponent : m_ComponentPtrs)
	{
		delete pairStringPtrComponent.second;
		pairStringPtrComponent.second = nullptr;
	}
}

void dae::GameObject::Update(float deltaTime)
{
	// Update All Components
	for (std::pair<const size_t, ComponentBase*>& pairStringPtrComponent : m_ComponentPtrs)
	{
		pairStringPtrComponent.second->Update(deltaTime);
	}

	// Update All Children
	for (GameObject* pChild : m_ChildrenPtrs)
	{
		if (pChild->IsActive())
			pChild->Update(deltaTime);
	}
}

void dae::GameObject::LateUpdate(float deltaTime)
{
	// LateUpdate All Components
	for (std::pair<const size_t, ComponentBase*>& pairStringPtrComponent : m_ComponentPtrs)
	{
		pairStringPtrComponent.second->LateUpdate(deltaTime);
	}

	// LateUpdate All Children
	for (GameObject* pChild : m_ChildrenPtrs)
	{
		if (pChild->IsActive())
			pChild->LateUpdate(deltaTime);
	}
}

void dae::GameObject::FixedUpdate(float deltaTime) 
{
	// FixedUpdate All Components
	for (std::pair<const size_t, ComponentBase*>& pairStringPtrComponent : m_ComponentPtrs)
	{
		pairStringPtrComponent.second->FixedUpdate(deltaTime);
	}

	// FixedUpdate All Children
	for (GameObject* pChild : m_ChildrenPtrs)
	{
		if (pChild->IsActive())
			pChild->FixedUpdate(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	// Render All Components
	for (const std::pair<const size_t, ComponentBase*>& pairStringPtrComponent : m_ComponentPtrs)
	{
		pairStringPtrComponent.second->Render();
	}

	// Render All Children
	for (GameObject* pChild : m_ChildrenPtrs)
	{
		if (pChild->IsActive())
			pChild->Render();
	}
}

// sets local position
void dae::GameObject::SetPosition(float x, float y)
{
	m_LocalTransform.SetPosition(x, y, 0.0f);
	m_HasPositionChanged = true;
	CalculateTransform();
}

void GameObject::SetPosition(const Vector2<float>& newPosVector)
{
	SetPosition(newPosVector.x, newPosVector.y);
}

// returns world position
Vector2<float> GameObject::GetPosition() const
{
	Vector2<float> position{GetLocalPosition()};
	if (m_pParent != nullptr)
		position += m_pParent->GetPosition();
	return position;
}

// returns local position
Vector2<float> GameObject::GetLocalPosition() const
{
	return { m_LocalTransform.GetPosition().x, m_LocalTransform.GetPosition().y };
}

void GameObject::Move(const Vector2<float>& moveVector)
{
	SetPosition(GetPosition() + moveVector);
}

void dae::GameObject::SetParent(GameObject* pParent)
{
	// remove from old parent
	if (m_pParent)
		m_pParent->RemoveChild(this);

	// set new parent
	m_pParent = pParent;

	// set relative transform etc.
}

GameObject* dae::GameObject::GetParentPtr() const
{
	return m_pParent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_ChildrenPtrs.size();
}

void dae::GameObject::RemoveChild(GameObject* pChild)
{
	auto childIterator{ m_ChildrenPtrs.find(pChild) };

	if (childIterator == m_ChildrenPtrs.end()) return;

	m_ChildrenPtrs.erase(pChild);

	pChild->SetParent(nullptr);
}

GameObject* dae::GameObject::AddChild(GameObject* pChild)
{
	if (m_pParent == pChild)
		SetParent(nullptr);
	m_ChildrenPtrs.insert(pChild);
	pChild->SetParent(this);
	return pChild;
}

void GameObject::AddComponent(ComponentBase* pComponent)
{
	size_t typeHash{typeid(*pComponent).hash_code()};
	if (m_ComponentPtrs.find(typeHash) != m_ComponentPtrs.end())
	{
		delete m_ComponentPtrs[typeHash];
		m_ComponentPtrs[typeHash] = nullptr;
	}

	m_ComponentPtrs[typeHash] = pComponent;
}

ComponentBase* GameObject::GetComponentPtr(const std::type_info& componentType)
{
	size_t typeHash{ componentType.hash_code() };

	if (m_ComponentPtrs.find(typeHash) == m_ComponentPtrs.end())
	{
		throw;
	}

	return m_ComponentPtrs[typeHash];
}

void GameObject::RemoveComponent(const std::type_info& componentType)
{
	m_ComponentPtrs.erase(m_ComponentPtrs.find(componentType.hash_code()));
}

const Transform& GameObject::GetTransformConstRef()
{
	return m_Transform;
}

bool GameObject::HasParent() const
{
	if (m_pParent == nullptr)
		return false;
	return true;
}

std::unordered_set<GameObject*> GameObject::GetChildrenPointers() const
{
	return m_ChildrenPtrs;
}

void GameObject::CalculateTransform()
{
	if (!m_pParent)
	{
		m_Transform = m_LocalTransform;
		return;
	}

	const Transform& parentTransform{ m_pParent->GetTransformConstRef() };
	
	//Position
	if (m_HasPositionChanged)
	{
		m_Transform.SetPosition(m_LocalTransform.GetPosition() + parentTransform.GetPosition());
		m_HasPositionChanged = false;
	}
}