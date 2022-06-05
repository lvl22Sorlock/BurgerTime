#pragma once
#include "Transform.h"
#include "SceneObject.h"
class ComponentBase;
#include <list>
#include <unordered_map>
#include <unordered_set>

#include <typeinfo>
#include "StructsEnums.h"

namespace dae
{
	class GameObject final: public SceneObject
	{
	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime) override;
		void LateUpdate(float deltaTime) override;
		void FixedUpdate(float deltaTime) override;
		void Render() const override;
		bool HasParent() const override;

		void AddComponent(ComponentBase* pComponent);

		template<typename ComponentType>
		ComponentType* GetAddComponent(ComponentBase* pComponent)
		{
			AddComponent(pComponent);
			return GetComponentPtr<ComponentType>();
		}

		ComponentBase* GetComponentPtr(const std::type_info& componentType);

		template<typename ComponentType>
		ComponentType* GetComponentPtr()
		{
			size_t typeHash{ (typeid(ComponentType)).hash_code() };
			if (m_ComponentPtrs.find(typeHash) == m_ComponentPtrs.end())
			{
				throw;
			}
			ComponentBase* pComponentBase{ m_ComponentPtrs[typeHash] };
			if (!pComponentBase) throw;
			ComponentType* pComponent{ dynamic_cast<ComponentType*>(pComponentBase) };
			if (!pComponent) throw;

			return pComponent;
		}


		void RemoveComponent(const std::type_info& componentType);
		const Transform& GetTransformConstRef();
		
		void SetParent(GameObject* pParent);
		GameObject* GetParentPtr() const;

		size_t GetChildCount() const;
		void RemoveChild(GameObject* pChild);
		GameObject* AddChild(GameObject* pChild);
		std::unordered_set<GameObject*> GetChildrenPointers() const;

		void SetPosition(float x, float y);
		void SetPosition(const Vector2<float>& newPosVector);
		Vector2<float> GetPosition() const;
		Vector2<float> GetLocalPosition() const;
		void Move(const Vector2<float>& moveVector);

	private:
		void CalculateTransform();

		Transform m_Transform;
		Transform m_LocalTransform;
		bool m_HasPositionChanged;
		GameObject* m_pParent;
		std::unordered_set<GameObject*> m_ChildrenPtrs;

		std::unordered_map<size_t, ComponentBase*> m_ComponentPtrs;
	};
}
