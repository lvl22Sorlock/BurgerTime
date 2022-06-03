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

class ComponentBase
{
public:
	
	virtual ~ComponentBase() = default;

	virtual void Update(float) {};
	virtual void LateUpdate(float) {};
	virtual void FixedUpdate(float) {};
	virtual void Render() const {};

	virtual bool IsCollidingWithOther(const CollisionBox& /*otherCollisionBox*/) const { return false; };

	ComponentBase(const ComponentBase& other) = delete;
	ComponentBase(ComponentBase&& other) noexcept = delete;
	ComponentBase& operator=(const ComponentBase& other) = delete;
	ComponentBase& operator=(ComponentBase&& other)	noexcept = delete;
protected:
	ComponentBase(dae::GameObject* pParent);
	dae::GameObject* m_pParentGameObject;
	const std::type_info& m_DerivedTypeId;
};

