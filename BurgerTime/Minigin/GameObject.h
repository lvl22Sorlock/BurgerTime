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

		//void AddComponent(const std::string& componentName, ComponentBase* pComponent);
		void AddComponent(ComponentBase* pComponent);

		template<typename ComponentType>
		ComponentType* GetAddComponent(ComponentBase* pComponent)
		{
			AddComponent(pComponent);
			return GetComponentPtr<ComponentType>();
		}

			//ComponentBase* GetComponentPtr(const std::string& componentName);
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


			//void RemoveComponent(const std::string& componentName);
		void RemoveComponent(const std::type_info& componentType);
		const Transform& GetTransformConstRef();
		
		// Fix these (e.g. now you remove the parent for something but don't remove it as a child...)
		void SetParent(GameObject* pParent);
		GameObject* GetParentPtr() const;

		size_t GetChildCount() const;
		//GameObject* GetChildAtIdx(int index) const;
			//void RemoveChild(int index);
		void RemoveChild(GameObject* pChild);
		GameObject* AddChild(GameObject* pChild);
		std::unordered_set<GameObject*> GetChildrenPointers() const;

		void SetPosition(float x, float y);
		void SetPosition(const Vector2<float>& newPosVector);
		Vector2<float> GetPosition() const;
		void Move(const Vector2<float>& moveVector);


	private:
		void CalculateTransform();

		Transform m_Transform;
		Transform m_LocalTransform;
		bool m_HasPositionChanged;
		//bool m_HasRotationChanged;
		//bool m_HasScaleChanged;
		GameObject* m_pParent;
		std::unordered_set<GameObject*> m_ChildrenPtrs;
		// contentmanager or texturemanager, and ComponentTexture instead
		//std::shared_ptr<Texture2D> m_Texture{};

		std::unordered_map<size_t, ComponentBase*> m_ComponentPtrs;
	};
}
