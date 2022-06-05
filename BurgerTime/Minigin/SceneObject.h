#pragma once
namespace dae
{
	class SceneObject
	{
	public:
		virtual void Update(float deltaTime) = 0;
		virtual void LateUpdate(float) {};
		virtual void FixedUpdate(float) {};
		virtual void Render() const = 0;
		virtual bool HasParent() const = 0;

		SceneObject() : m_IsActive{ true } {}
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;


		void SetActive(bool isActive) {
			m_IsActive = isActive;
		}

		bool IsActive() const {
			return m_IsActive;
		}

	protected:
		bool m_IsActive;
	};
}
