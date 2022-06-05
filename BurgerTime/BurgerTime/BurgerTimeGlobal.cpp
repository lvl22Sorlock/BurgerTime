#include "StructsEnums.h"
#include "BurgerTimeGlobal.h"


namespace SimonGlobalFunctions
{
	Vector2<int> GetClosestIdxFromPos(const Vector2<float>& pos)
	{
		return {
			static_cast<int>(std::roundf(pos.x / SimonGlobalConstants::CELL_WIDTH) + SimonGlobalConstants::EPSILON),
			static_cast<int>(std::roundf(pos.y / SimonGlobalConstants::CELL_WIDTH) + SimonGlobalConstants::EPSILON)
		};
	}
	Vector2<float> GetLeftBottomCornerPosIdx(const Vector2<int>& idx)
	{
		return { 
			SimonGlobalConstants::CELL_OFFSET.x + idx.x * SimonGlobalConstants::CELL_WIDTH,
			SimonGlobalConstants::CELL_OFFSET.y + idx.y * SimonGlobalConstants::CELL_WIDTH 
		};
	}
	Vector2<float> GetPosFromIdx(const Vector2<int>& idx)
	{
		return GetLeftBottomCornerPosIdx(idx);
	}

	float GetRandomFloat(float min, float max)
	{
		// https://stackoverflow.com/questions/686353/random-float-number-generation
		float randomNr{ static_cast<float>(rand()) / static_cast<float>(RAND_MAX) }; // 0.0f to 1.0f
		randomNr *= (max - min); // 0.0f to (max-min)
		randomNr += min; // min to max
		return randomNr;
	}
}