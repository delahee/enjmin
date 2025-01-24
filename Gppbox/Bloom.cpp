#include "Bloom.hpp"
using namespace std;

//not a professionnal bloom ( go for pyramid ) it is fast though because only two passes with small kernels
void Bloom::m_gaussian_kernel(float* dest, int size, float radius)
{
	float* k;
	float rs, s2, sum;
	float sigma = 1.6f;
	float tetha = 2.25f;
	int r, hsize = size / 2;

	s2 = 1.0f / expf(sigma * sigma * tetha);
	rs = sigma / radius;

	k = dest;
	sum = 0.0f;

	/* compute gaussian kernel */
	for (r = -hsize; r <= hsize; r++) {
		float x = r * rs;
		float v = (1.0f / expf(x * x)) - s2;
		v = std::max(v, 0.0f);
		*k = v;
		sum += v;
		k++;
	}

	/* normalize */
	if (sum > 0.0f) {
		float isum = 1.0f / sum;
		for (r = 0; r < size; r++)
			dest[r] *= isum;
	}
}

void Bloom::getKernelOffsets(float dx, vector<float>& _kernel, vector<sf::Glsl::Vec2>& _offsets, float offsetScale, bool isHoriz) {
	int kernel_size = (int)(dx / 0.65f + 0.5f) * 2 + 1;

	_kernel.clear();
	_offsets.clear();

	for (int i = 0; i < kernel_size; i++) {
		_kernel.push_back(0.0f);
		_offsets.push_back(sf::Glsl::Vec2());
	}

	m_gaussian_kernel(_kernel.data(), kernel_size, dx);

	for (int i = 0; i < kernel_size; i++) {
		float ofs = (offsetScale * (i - kernel_size * 0.5f));
		_offsets[i].x = isHoriz ? ofs : 0.0f;
		_offsets[i].y = isHoriz ? 0.0f : ofs;
	}
}

void Bloom::blur(float dx, sf::Texture* source, sf::Shader* _blurShader, sf::RenderTexture* destX, sf::RenderTexture* destFinal) {

	vector<float> kernelX;
	vector<float> kernelY;
	vector<sf::Glsl::Vec2> offsets;
	source->setSmooth(true);
	destX->setSmooth(true);
	destFinal->setSmooth(true);
	{
		_blurShader->setUniform("texture", *source);

		sf::Sprite sprX(*source);

		getKernelOffsets(dx, kernelX, offsets);

		int nbSamples = kernelX.size();
		_blurShader->setUniform("samples", nbSamples);
		for (int i = 0; i < nbSamples; i++)
			offsets[i].x *= 1.0f / source->getSize().x;
		_blurShader->setUniformArray("kernel", kernelX.data(), nbSamples);
		_blurShader->setUniformArray("offsets", offsets.data(), nbSamples);

		auto k = 1.0f;
		_blurShader->setUniform("srcMul", sf::Glsl::Vec4(k, k, k, 1.0f));

		destX->draw(sprX, _blurShader);
		destX->display();
	}

	{
		getKernelOffsets(dx, kernelY, offsets, 1.0, false);
		int nbSamples = kernelY.size();
		_blurShader->setUniform("samples", nbSamples);
		for (int i = 0; i < nbSamples; i++)
			offsets[i].y *= 1.0f / source->getSize().y;
		_blurShader->setUniformArray("kernel", kernelY.data(), nbSamples);
		_blurShader->setUniformArray("offsets", offsets.data(), nbSamples);

		sf::Sprite sprXY(destX->getTexture());
		_blurShader->setUniform("texture", destX->getTexture());
		_blurShader->setUniform("srcMul", sf::Glsl::Vec4(1, 1, 1, 1));

		destFinal->draw(sprXY, _blurShader);
		destFinal->display();
	}
}

void Bloom::render(
	sf::RenderWindow & window,
	sf::Texture & winTex,
	sf::RenderTexture * destX,
	sf::RenderTexture * destFinal,
	sf::Shader * blurShader,
	sf::Shader * bloomShader,
	float blurWidth,
	const sf::Glsl::Vec4 & bloomMul
)
{
	winTex.update(window);
	destX->clear(sf::Color(0, 0, 0, 255));
	destFinal->clear(sf::Color(0, 0, 0, 255));
	Bloom::blur(blurWidth, &winTex, blurShader, destX, destFinal);
	sf::Sprite sp(destFinal->getTexture());
	sf::RenderStates rs;

	rs.blendMode = sf::BlendAdd;

	bloomShader->setUniform("texture", destFinal->getTexture());
	bloomShader->setUniform("bloomPass", 0.6f);
	bloomShader->setUniform("bloomMul", bloomMul);

	rs.shader = bloomShader;
	sf::Color c = sp.getColor();
	c.a = (int)(c.a * 0.8);
	sp.setColor(c);

	window.draw(sp, rs);
	//window.draw(sp);
}
