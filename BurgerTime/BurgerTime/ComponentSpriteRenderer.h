#pragma once


//-------------------------------------------------------------------------
//	Include Files
//-------------------------------------------------------------------------
#include "ComponentBase.h"
#include <string>
#include <memory>
#include "StructsEnums.h"
#include <unordered_map>

namespace dae
{
	class Texture2D;
}




//-------------------------------------------------------------------------
//	ComponentSpriteRenderer Class
//-------------------------------------------------------------------------

class ComponentSpriteRenderer final: public ComponentBase
{
public:
	struct AnimInfo
	{
		AnimInfo()
			:offsetPixelsToFirstSprite{ 0,0 }
			, nrRows{ 1 }
			, nrCols{ 1 }
		{}

		AnimInfo(const Vector2<int>& offsetPixels, int nrRowsParam, int nrColsParam)
			:offsetPixelsToFirstSprite{ offsetPixels }
			,nrRows{nrRowsParam}
			,nrCols{nrColsParam}
		{}

		Vector2<int> offsetPixelsToFirstSprite;
		int nrRows{};
		int nrCols{};
	};

	ComponentSpriteRenderer(dae::GameObject* pParent, const Vector2<float>& widthHeight, bool hasAnimations = false, const Vector2<float>& posOffset = {}, const Vector2<int>& spriteWidthHeight = {16,16});
	virtual ~ComponentSpriteRenderer() = default;

	virtual void Render() const override;
	virtual void Update(float deltaTime) override;
	virtual bool IsCollidingWithOther(const CollisionBox&) const override { return false; };

	void SetSpritesheet(const std::string & filename);


	//-------------------------------------------------------------------------
	//	Copy/move constructors and assignment operators
	//-------------------------------------------------------------------------
	ComponentSpriteRenderer(const ComponentSpriteRenderer& other)				 = delete;
	ComponentSpriteRenderer(ComponentSpriteRenderer&& other) noexcept			 = delete;
	ComponentSpriteRenderer& operator=(const ComponentSpriteRenderer& other)	 = delete;
	ComponentSpriteRenderer& operator=(ComponentSpriteRenderer&& other)	noexcept = delete;

	//-------------------------------------------------------------------------
	//	Member Functions
	//-------------------------------------------------------------------------

	void AddAnimation(const std::wstring& animName, const AnimInfo& animInfo);
	bool TrySetAnimation(const std::wstring& animName);
	void SetOffsetPixelsToNonAnimatedSprite(const Vector2<int>& offset);

	bool* GetIsActivePtr();
	bool* GetIsMirroredPtr();

	virtual std::size_t GetTypeHash() override;

private:
	//-------------------------------------------------------------------------
	//	Private Member Functions
	//-------------------------------------------------------------------------


	void GoToNextSprite();
	Vector2<int> GetSpriteSheetPos() const;
	void ResetAnimation();
	bool CheckIsValid() const;


	//-------------------------------------------------------------------------
	//	Data Members
	//-------------------------------------------------------------------------

	// Base
	bool m_IsSpriteSheetSet;
	bool m_HasAnimations;
	Vector2<float> m_PosOffset;
	Vector2<float> m_WidthHeight;

	std::shared_ptr<dae::Texture2D> m_pSpritesheet{};
	const int m_SPRITE_WIDTH;
	const int m_SPRITE_HEIGHT;
	Vector2<int> m_OffsetPixelsToFirstSprite;
	int m_CurrentRowIdx;
	int m_NrRows;
	int m_CurrentColIdx;
	int m_NrCols;

	std::unordered_map<std::wstring, AnimInfo> m_Animations;

	// Modifiers	
	bool m_IsActive;
	bool m_IsMirrored;

	// Update with time
	float m_TimeSinceLastSpriteSwitch;
	float m_TimeBetweenSpriteSwitches;
};

