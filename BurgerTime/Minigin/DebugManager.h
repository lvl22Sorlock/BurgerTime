#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "Singleton.h"
#include "StructsEnums.h"
namespace dae
{
	class Texture2D;
}


//-------------------------------------------------------------------------
//	DebugManager Class
//-------------------------------------------------------------------------
class DebugManager final: public dae::Singleton<DebugManager>
{
public:
	friend class dae::Singleton<DebugManager>;
	~DebugManager() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	DebugManager(const DebugManager& other) = delete;
	DebugManager(DebugManager&& other) noexcept = delete;
	DebugManager& operator=(const DebugManager& other) = delete;
	DebugManager& operator=(DebugManager&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	void SetIsDebugRendering(bool isDebugRendering);
	void RenderDebugSquare(const Vector2<float>& pos, const Vector2<float>& widthHeight);

private:
	DebugManager();
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
	std::shared_ptr<dae::Texture2D> m_DebugSquareTexture;
	bool m_IsDebugRendering;



};
