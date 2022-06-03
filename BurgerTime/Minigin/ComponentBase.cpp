#include "MiniginPCH.h"
#include "ComponentBase.h"

#include <typeinfo>

#include <iostream>

ComponentBase::ComponentBase(dae::GameObject* pParent)
	:m_pParentGameObject{pParent}
{}
