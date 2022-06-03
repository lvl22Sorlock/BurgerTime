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
		return { idx.x * SimonGlobalConstants::CELL_WIDTH, idx.y * SimonGlobalConstants::CELL_WIDTH };
	}
}