#pragma once

#include <SFML/Graphics.hpp>

namespace Bloom {

	void m_gaussian_kernel(float* dest, int size, float radius);
	void getKernelOffsets(float dx, std::vector<float>& _kernel, std::vector<sf::Glsl::Vec2>& _offsets, float offsetScale = 1.0f, bool isHoriz = true);
	void blur(float dx, sf::Texture* source, sf::Shader* _blurShader, sf::RenderTexture* destX, sf::RenderTexture* destFinal);
	void render(
		sf::RenderWindow& window,
		sf::Texture& winTex,
		sf::RenderTexture* destX,
		sf::RenderTexture* destFinal,
		sf::Shader* blurShader,
		sf::Shader* bloomShader,
		float blurWidth,
		const sf::Glsl::Vec4 & bloomMul);
}