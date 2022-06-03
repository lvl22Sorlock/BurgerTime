//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------
#include "MiniginPCH.h"
#include "DebugManager.h"
#include "ResourceManager.h"
#include "Renderer.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
DebugManager::DebugManager()
	:m_IsDebugRendering{false}
	,m_DebugSquareTexture{}
{
	m_DebugSquareTexture = dae::ResourceManager::GetInstance().LoadTexture("DebugGreenCollider3.png");
}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void DebugManager::SetIsDebugRendering(bool isDebugRendering)
{
	m_IsDebugRendering = isDebugRendering;
}

void DebugManager::RenderDebugSquare(const Vector2<float>& pos, const Vector2<float>& widthHeight)
{
	if (!m_IsDebugRendering) return;
	if (m_DebugSquareTexture == nullptr) return;
	dae::Renderer::GetInstance().RenderTexture(*m_DebugSquareTexture, pos.x, pos.y, widthHeight.x, widthHeight.y);
}
