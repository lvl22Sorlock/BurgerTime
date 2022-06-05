#pragma once
namespace dae
{
	class GameObject;
}
#include "StructsEnums.h"
namespace std
{
	class type_info;
}
#include <typeinfo>

class ComponentBase
{
public:
	
	virtual ~ComponentBase() = default;

	virtual void Update(float) {};
	virtual void LateUpdate(float) {};
	virtual void FixedUpdate(float) {};
	virtual void Render() const {};

	virtual bool IsCollidingWithOther(const CollisionBox& /*otherCollisionBox*/) const = 0;
	dae::GameObject* GetParent() const { return m_pParentGameObject; }

	//https://stackoverflow.com/questions/32016809/using-typeid-to-get-name-of-derived-class/32016926
	virtual std::size_t GetTypeHash() = 0;

	ComponentBase(const ComponentBase& other) = delete;
	ComponentBase(ComponentBase&& other) noexcept = delete;
	ComponentBase& operator=(const ComponentBase& other) = delete;
	ComponentBase& operator=(ComponentBase&& other)	noexcept = delete;
protected:
	ComponentBase(dae::GameObject* pParent);
	dae::GameObject* m_pParentGameObject;
};

