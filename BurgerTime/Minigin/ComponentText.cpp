#include "MiniginPCH.h"
#include "ComponentText.h"

#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"

ComponentText::ComponentText(dae::GameObject* pParent, const std::string& text, const std::shared_ptr<dae::Font>& font)
	: ComponentBase(pParent)
	,m_NeedsUpdate(true), m_Text(text), m_Font(font), m_TextTexture(nullptr), m_Transform{pParent->GetTransformConstRef()}
{

}

void ComponentText::Update(float)
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<dae::Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}
void ComponentText::Render() const
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = m_Transform.GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

void ComponentText::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
//void ComponentText::SetPosition(float x, float y)
//{
//	m_Transform.SetPosition(x, y, 0.0f);
//}