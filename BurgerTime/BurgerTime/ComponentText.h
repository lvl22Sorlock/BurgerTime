#pragma once

#include "Transform.h"
#include "ComponentBase.h"
namespace dae
{
	class Font;
	class Texture2D;
};

#include <memory>


class ComponentText final : public ComponentBase
{
public:
	ComponentText(dae::GameObject* pParent, const std::string& text, const std::shared_ptr<dae::Font>& font);
	~ComponentText() = default;

	virtual void Update(float deltaTime) override;
	virtual void Render() const override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	void SetText(const std::string& text);

	virtual std::size_t GetTypeHash() override;

	ComponentText(const ComponentText& other) = delete;
	ComponentText(ComponentText&& other) noexcept = delete;
	ComponentText& operator=(const ComponentText& other) = delete;
	ComponentText& operator=(ComponentText&& other)	noexcept = delete;

private:

	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<dae::Font> m_Font;
	std::shared_ptr<dae::Texture2D> m_TextTexture;
	const dae::Transform& m_Transform;
};

