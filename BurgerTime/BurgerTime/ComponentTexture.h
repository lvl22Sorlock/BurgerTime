#pragma once

#include "ComponentBase.h"
#include <string>
namespace dae
{
	class Texture2D;
}
#include "StructsEnums.h"


class ComponentTexture final : public ComponentBase
{
public:
	ComponentTexture(dae::GameObject* pParent);
	ComponentTexture(dae::GameObject* pParent, const Vector2<float>& widthHeight);
	virtual ~ComponentTexture() = default;

	virtual void Render() const override;

	void SetTexture(const std::string& filename);

	ComponentTexture(const ComponentTexture& other) = delete;
	ComponentTexture(ComponentTexture&& other) noexcept = delete;
	ComponentTexture& operator=(const ComponentTexture& other) = delete;
	ComponentTexture& operator=(ComponentTexture&& other)	noexcept = delete;

private:
	virtual std::size_t GetTypeHash() override;

	std::shared_ptr<dae::Texture2D> m_Texture{};
	bool m_IsUsingWidthHeight;
	Vector2<float> m_WidthHeight;
};
