#pragma once
//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
namespace dae
{
	class Scene;
}
namespace SimonGlobalEnums
{
	enum class IngredientType;
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

	void ParseFile(const std::string& filePath, dae::Scene& scene);
	static std::ofstream GetLevelOutputFileStream(const std::string& levelName);
	static void MakeLadderFile(const Vector2<int>& topIdcs, int bottomIdx, std::ofstream& ofs);
	static void MakePlatformFile(const Vector2<int>& leftIdcx, int rightIdx, std::ofstream& ofs);
	void MakeLadder(const Vector2<int>& topIdcs, int bottomIdx, dae::Scene& scene);
	void MakePlatform(const Vector2<int>& leftIdcx, int rightIdx, dae::Scene& scene);
	void MakeWaitingPlate(const Vector2<int>& leftIdcs, dae::Scene& scene);
	void MakeIngredient(const Vector2<int>& leftIdcs, SimonGlobalEnums::IngredientType type, dae::Scene& scene);
	virtual std::size_t GetTypeHash() override;

private:
	enum class BlockId
	{
		ladder,
		platform
	};
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------

	void ParseFileBlock(std::ifstream& ifs, dae::Scene& scene);
	//static std::vector<unsigned char> GetIntToBytes(int32_t integer);
	static std::vector<char>& GetIntToBytes(int32_t integer, std::vector<char>& byteArrayVector);
	static int32_t GetIntFrom4Chars(char* pFirstchar);

	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------
};
