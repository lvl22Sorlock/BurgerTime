#pragma once
#include <cmath>

//struct Rectf
//{
//	Rectf(float x, float y, float widthParam, float heightParam)
//		:xPos{x}
//		,yPos{y}
//		,width{widthParam}
//		,height{heightParam}
//	{}
//
//	float xPos{};
//	float yPos{};
//	float width{};
//	float height{};
//};

template <typename T>
struct Vector2
{
	Vector2()
		:x{}
		,y{}
	{}
	Vector2(T xParam, T yParam)
		:x{xParam}
		,y{yParam}
	{}
	Vector2(const Vector2<T>& other)
		:x{other.x}
		,y{other.y}
	{}
	Vector2(Vector2<T>&& other) noexcept
		:x{ other.x }
		,y{ other.y }
	{}

	T x{};
	T y{};

	inline void operator=(const Vector2<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	inline void operator=(Vector2<T>&& rhs) noexcept
	{
		x = rhs.x;
		y = rhs.y;
	}
	inline bool operator==(const Vector2<T>& rhs) const
	{
		const T  EPSILON{ 0.001f };
		return 
			(std::abs(x - rhs.x) < EPSILON
			&&
			std::abs(y - rhs.y) < EPSILON);
	}
	inline bool operator!=(const Vector2<T>& rhs) const
	{
		return !(*this == rhs);
	}
	inline Vector2<T>& operator+= (const Vector2<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return (*this);
	}
	inline Vector2<T> operator+ (const Vector2<T>& rhs) const
	{
		Vector2<T> temp{ x, y };
		return temp += rhs;
	}
	inline Vector2<T> operator-= (const Vector2<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return (*this);
	}
	inline Vector2<T> operator- (const Vector2<T>& rhs) const
	{
		Vector2<T> temp{ x, y };
		return temp -= rhs;
	}
	inline Vector2<T> operator*(float rhs) const
	{
		return Vector2<T>(x * rhs, y * rhs);
	}

	float GetCalculateLength() const
	{
		return std::sqrtf(x*x + y*y);
	}

	void Normalize()
	{
		const float LENGTH{ GetCalculateLength() };
		x /= LENGTH;
		y /= LENGTH;
	}

	Vector2<T> GetNormalized() const
	{
		const float LENGTH{ GetCalculateLength() };
		return Vector2<T>(x / LENGTH, y / LENGTH);
	}
};

struct CollisionBox
{
	CollisionBox(const Vector2<float>& leftBottomParam, float width, float height)
		:leftBottom{leftBottomParam}
		,rightTop{leftBottomParam.x + width, leftBottomParam.y + height}
	{}

	Vector2<float> leftBottom{};
	Vector2<float> rightTop{};

	Vector2<float> GetWidthHeight() const
	{
		return Vector2<float>(rightTop.x - leftBottom.x, rightTop.y - leftBottom.y);
	}

	//CollisionBox(const Vector2<float>& leftTopParam, float width, float height)
	//	:leftTop(leftTopParam)
	//	,rightBottom{ leftTopParam.x + width, leftTopParam.y - height }
	//{}

	//CollisionBox(const Vector2<float>& leftTopParam, const Vector2<float>& rightBottomParam)
	//	:leftTop(leftTopParam)
	//	,rightBottom{rightBottomParam}
	//{}

	//Vector2<float> leftTop{};
	//Vector2<float> rightBottom{};


	bool IsColliding(const CollisionBox& other) const
	{
		//if (rightTop.x >= other.leftBottom.x &&     // r1 right edge past r2 left
		//	leftBottom.x <= other.rightTop.x &&       // r1 left edge past r2 right
		//	rightTop.y >= other.leftBottom.y &&       // r1 top edge past r2 bottom
		//	leftBottom.y <= other.rightTop.y) {       // r1 bottom edge past r2 top
		//	return true;
		//}
		//return false;

		if (leftBottom.x > other.rightTop.x
			||
			other.leftBottom.x > rightTop.x
			||
			leftBottom.y > other.rightTop.y
			||
			other.leftBottom.y > rightTop.y)
			return false;


		return true;
	}
};