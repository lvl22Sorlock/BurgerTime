#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
namespace dae
{
	class Scene;
}

//-------------------------------------------------------------------------
//	Level Class
//-------------------------------------------------------------------------
class Level final : public ComponentBase
{
public:
	Level(dae::GameObject* pParent);
	~Level() = default;

	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------

	Level(const Level& other) = delete;
	Level(Level&& other) noexcept = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	void MakeLadder(const Vector2<int>& topIdcs, int bottomIdx, dae::Scene& scene);

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	virtual std::size_t GetTypeHash() override;

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
};
