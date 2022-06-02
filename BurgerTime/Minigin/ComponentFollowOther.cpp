//-------------------------------------------------------------------------
//	Includes
//-------------------------------------------------------------------------

#include "MiniginPCH.h"
#include "ComponentFollowOther.h"
#include "GameObject.h"

//-------------------------------------------------------------------------
//	Static datamembers
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//	Constructor(s) & Destructor
//-------------------------------------------------------------------------
ComponentFollowOther::ComponentFollowOther(dae::GameObject* pParent, dae::GameObject* pTarget)
	:ComponentBase(pParent)
	, m_pTarget{pTarget}
{

}

//-------------------------------------------------------------------------
//	Member functions
//-------------------------------------------------------------------------

void ComponentFollowOther::Update(float)
{
	if (!m_pTarget) return;

	m_pParentGameObject->SetPosition(m_pTarget->GetPosition());
}

