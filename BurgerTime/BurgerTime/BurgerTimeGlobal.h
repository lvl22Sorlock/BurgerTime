#pragma once
#include <string>

namespace SimonGlobalConstants
{
	static const float CELL_WIDTH{ 30.0f };	// the width and height of 1 cell
	static const float EPSILON{ 0.0001f };	// for comparing floats / very small float value
	static const Vector2<int> SCREEN_SIZE{ 640,480 };
	static const Vector2<float> CELL_OFFSET{ 30.f, 0 };
	static const std::string SPRITESHEET_PATH{ "BurgerTimeSprites2.png" };
}

namespace SimonGlobalFunctions
{
	Vector2<int> GetClosestIdxFromPos(const Vector2<float>& pos);
	Vector2<float> GetLeftBottomCornerPosIdx(const Vector2<int>& idx);
	Vector2<float> GetPosFromIdx(const Vector2<int>& idx);
	float GetRandomFloat(float min, float max);
}

namespace SimonGlobalEnums
{
	enum class CollisionTag
	{
		ladder,
		ladderDownward,
		ladderUpward,
		platform,
		platformLeft,
		platformRight,
		pepperCloud,
		fallingBurgerIngredient,
		ingredientPart,
		player
	};
	enum class CharacterType
	{
		player,
		mrHotDog,
		mrPickle,
		mrEgg
	};
	enum class IngredientType
	{
		bunTop,
		patty,
		cheese,
		lettuce,
		tomato,
		bunBottom
	};
}

namespace SimonGlobalStructs
{

}