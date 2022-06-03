#include "MiniginPCH.h"
#include "ComponentBase.h"

#include <typeinfo>

#include <iostream>

ComponentBase::ComponentBase(dae::GameObject* pParent)
	:m_pParentGameObject{pParent}
	, m_DerivedTypeId{typeid(*this)}
{
	std::cout << "Test, should be different values: " << m_DerivedTypeId.name() << std::endl;
}
