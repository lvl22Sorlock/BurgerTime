#pragma once

namespace SimonGlobalConstants
{
	static const float CELL_WIDTH{ 35.0f };	// the width and height of 1 cell
	static const float EPSILON{ 0.0001f };	// for comparing floats / very small float value
}

namespace SimonGlobalFunctions
{
	Vector2<int> GetClosestIdxFromPos(const Vector2<float>& pos);
	Vector2<float> GetLeftBottomCornerPosIdx(const Vector2<int>& idx);
}

namespace SimonGlobalEnums
{
	enum class CollisionTag
	{
		ladder,
		ladderDownward,
		ladderUpward
	};
}

namespace SimonGlobalStructs
{

}