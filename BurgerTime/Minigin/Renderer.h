#pragma once
#include "Singleton.h"

#pragma region Week02Assignment

#pragma endregion

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const SDL_Rect& src) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, const SDL_Rect& src, bool isFlipped) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }

	private:

		#pragma region Week02Assignment
		void CalculateExercise1Duration() const;
		void CalculateExercise2Duration() const;
		void CalculateExercise2BDuration() const;
		#pragma endregion
	};
}

