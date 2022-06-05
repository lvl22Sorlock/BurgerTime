#include "BurgerTimePCH.h"
#include "ComponentSpriteRenderer.h"
#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

std::size_t ComponentSpriteRenderer::GetTypeHash()
{
	return typeid(*this).hash_code();
}



ComponentSpriteRenderer::ComponentSpriteRenderer(
	dae::GameObject* pParent,
	const Vector2<float>& widthHeight,
	bool hasAnimations,
	const Vector2<float>& posOffset
)
	:ComponentBase(pParent)
	, m_SPRITE_WIDTH{ 16 }
	, m_SPRITE_HEIGHT{ 16 }
	, m_OffsetPixelsToFirstSprite{}
	, m_CurrentColIdx{}
	, m_NrCols{3}
	, m_CurrentRowIdx{}
	, m_NrRows{1}
	, m_TimeSinceLastSpriteSwitch{0.0f}
	, m_TimeBetweenSpriteSwitches{1.0f / 15.0f}
	, m_IsSpriteSheetSet{false}
	, m_IsActive{true}
	, m_IsMirrored{true}
	, m_HasAnimations{hasAnimations}
	, m_PosOffset{posOffset}
	, m_WidthHeight{widthHeight}
{
}

void ComponentSpriteRenderer::Update(float deltaTime)
{
	if (!m_HasAnimations) return;
	if (!CheckIsValid()) return;

	if (m_IsActive)
		m_TimeSinceLastSpriteSwitch += deltaTime;
	else
	{
		m_CurrentColIdx = 0;
		m_CurrentRowIdx = 0;
	}

	if (m_TimeSinceLastSpriteSwitch > m_TimeBetweenSpriteSwitches)
	{
		m_TimeSinceLastSpriteSwitch -= m_TimeBetweenSpriteSwitches;
		GoToNextSprite();
	}
}

void ComponentSpriteRenderer::Render() const
{
	if (!CheckIsValid()) return;

	glm::vec3 pos =
		{ m_pParentGameObject->GetPosition().x + m_PosOffset.x,	//GetTransformConstRef().
		  m_pParentGameObject->GetPosition().y + m_PosOffset.y,
		  m_pParentGameObject->GetTransformConstRef().GetPosition().z };

	SDL_Rect srcRect{ GetSpriteSheetPos().x, GetSpriteSheetPos().y, m_SPRITE_WIDTH, m_SPRITE_HEIGHT};
	dae::Renderer::GetInstance().RenderTexture(*m_pSpritesheet, pos.x, pos.y, m_WidthHeight.x, m_WidthHeight.y, srcRect, m_IsMirrored);
}

void ComponentSpriteRenderer::SetSpritesheet(const std::string& filename)
{
	m_pSpritesheet = dae::ResourceManager::GetInstance().LoadTexture(filename);
	m_IsSpriteSheetSet = true;
}

void ComponentSpriteRenderer::GoToNextSprite()
{
	++m_CurrentColIdx;
	if (m_CurrentColIdx >= m_NrCols)
	{
		m_CurrentColIdx = 0;

		++m_CurrentRowIdx;
		if (m_CurrentRowIdx >= m_NrRows)
		{
			m_CurrentRowIdx = 0;
		}
	}
}

Vector2<int> ComponentSpriteRenderer::GetSpriteSheetPos() const
{
	Vector2<int> spriteSheetPos{ m_OffsetPixelsToFirstSprite.x, m_OffsetPixelsToFirstSprite.y};
	spriteSheetPos.x += m_CurrentColIdx * m_SPRITE_WIDTH;
	spriteSheetPos.y += m_CurrentRowIdx * m_SPRITE_HEIGHT;
	return spriteSheetPos;
}

void ComponentSpriteRenderer::AddAnimation(const std::wstring& animName, const AnimInfo& animInfo)
{
	m_HasAnimations = true;
	m_Animations[animName] = animInfo;
}

bool ComponentSpriteRenderer::TrySetAnimation(const std::wstring& animName)
{
	auto animIterator{ m_Animations.find(animName) };
	if (animIterator == m_Animations.end()) return false;

	AnimInfo& animInfo{ (*animIterator).second };
	m_NrRows = animInfo.nrRows;
	m_NrCols = animInfo.nrCols;
	m_OffsetPixelsToFirstSprite = animInfo.offsetPixelsToFirstSprite;
	ResetAnimation();

	return true;
}

void ComponentSpriteRenderer::ResetAnimation()
{
	m_CurrentColIdx = 0;
	m_CurrentRowIdx = 0;
	m_TimeSinceLastSpriteSwitch = 0.0f;
}

bool ComponentSpriteRenderer::CheckIsValid() const
{
	if (!m_IsSpriteSheetSet)
	{
		std::cout << "Spritesheet not set" << std::endl;
		return false;
	}

	return true;
}

bool* ComponentSpriteRenderer::GetIsActivePtr()
{
	return &m_IsActive;
}

bool* ComponentSpriteRenderer::GetIsMirroredPtr()
{
	return &m_IsMirrored;
}

void ComponentSpriteRenderer::SetOffsetPixelsToNonAnimatedSprite(const Vector2<int>& offset)
{
	m_OffsetPixelsToFirstSprite = offset;
}