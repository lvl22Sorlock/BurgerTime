#include "BurgerTimePCH.h"
#include "ComponentTexture.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

std::size_t ComponentTexture::GetTypeHash()
{
	return typeid(*this).hash_code();
}

ComponentTexture::ComponentTexture(dae::GameObject* pParent)
	: ComponentBase(pParent)
	, m_IsUsingWidthHeight{false}
	, m_WidthHeight{}
{}

ComponentTexture::ComponentTexture(dae::GameObject* pParent, const Vector2<float>& widthHeight)
	: ComponentBase(pParent)
	, m_IsUsingWidthHeight{true}
	, m_WidthHeight{widthHeight}
{}

void ComponentTexture::Render() const
{
	const auto& pos = m_pParentGameObject->GetTransformConstRef().GetPosition();
	if (!m_IsUsingWidthHeight)
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	else
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y, m_WidthHeight.x, m_WidthHeight.y);
}

void ComponentTexture::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}