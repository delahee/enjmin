
#include <imgui.h>
#include <array>
#include <vector>

#include "C.hpp"
#include "Game.hpp"

#include "HotReloadShader.hpp"
#include "VirtualCallTest.hpp"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line)
{
	return new uint8_t[size];
}

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
	return _aligned_malloc(size,alignment);
}

static int cols = 1280 / C::GRID_SIZE;
static int lastLine = 720 / C::GRID_SIZE - 1;

Game::Game(sf::RenderWindow * win) {
	this->win = win;
	bg = sf::RectangleShape(Vector2f(win->getSize().x, win->getSize().y));

	bool isOk = tex.loadFromFile("res/bg_stars.png");
	if (!isOk) {
		printf("ERR : LOAD FAILED\n");
	}
	bg.setTexture(&tex);
	bg.setSize(sf::Vector2f(1280, 720));

	bgShader = new HotReloadShader("res/bg.vert", "res/bg.frag");
	
	for (int i = 0; i < 1280 / C::GRID_SIZE; ++i) 
		walls.push_back( Vector2i(i, lastLine) );

	walls.push_back(Vector2i(0, lastLine-1));
	walls.push_back(Vector2i(0, lastLine-2));
	walls.push_back(Vector2i(0, lastLine-3));

	walls.push_back(Vector2i(cols-1, lastLine - 1));
	walls.push_back(Vector2i(cols-1, lastLine - 2));
	walls.push_back(Vector2i(cols-1, lastLine - 3));

	walls.push_back(Vector2i(cols >>2, lastLine - 2));
	walls.push_back(Vector2i(cols >>2, lastLine - 3));
	walls.push_back(Vector2i(cols >>2, lastLine - 4));
	walls.push_back(Vector2i((cols >> 2) + 1, lastLine - 4));
	cacheWalls();
}

void Game::cacheWalls()
{
	wallSprites.clear();
	for (Vector2i & w : walls) {
		sf::RectangleShape rect(Vector2f(16,16));
		rect.setPosition(w.x * C::GRID_SIZE, w.y * C::GRID_SIZE);
		rect.setFillColor(sf::Color(0x07ff07ff));
		wallSprites.push_back(rect);
	}
}

void Game::processInput(sf::Event ev) {
	if (ev.type == sf::Event::Closed) {
		win->close();
		closing = true;
		return;
	}
	if (ev.type == sf::Event::KeyReleased) {
		
	
	}
}


static double g_time = 0.0;
static double g_tickTimer = 0.0;


void Game::pollInput(double dt) {

	float lateralSpeed = 8.0;
	float maxSpeed = 40.0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {

	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {

	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
		if (!wasPressed) {
			onSpacePressed();
			wasPressed = true;
		}
	}
	else {
		wasPressed = false;
	}

}

static bool s_RectVAFlood = false;
static bool s_RectVATick = true;
static sf::VertexArray va;
static RenderStates vaRs;
static float		vaAlpha = 1.0;
static int			vaSize = 13;

static bool s_RectShapeFlood = false;
static std::vector<sf::RectangleShape> rects;

int blendModeIndex(sf::BlendMode bm) {
	if (bm == sf::BlendAlpha) return 0;
	if (bm == sf::BlendAdd) return 1;
	if (bm == sf::BlendNone) return 2;
	if (bm == sf::BlendMultiply) return 3;
	return 4;
};

void Game::update(double dt) {
	pollInput(dt);

	g_time += dt;
	if (bgShader) bgShader->update(dt);

	beforeParts.update(dt);

	afterParts.update(dt);

	bool isTick = false;
	g_tickTimer -= dt;
	if (g_tickTimer <= 0.0) {
		onFileTick();
		isTick = true;
		g_tickTimer = 0.1;
	}


	if (ImGui::CollapsingHeader("Flood Rect - Shapes Method")) {
		int start;
		ImGui::PushID(&start);
		ImGui::Indent();
		if (!s_RectShapeFlood && ImGui::Button("Start")) {
			s_RectShapeFlood = true;
		}

		if (s_RectShapeFlood) {
			std::string format = "%0.1f";
			double size = rects.size();
			if (size > 1024) { size /= 1000.0; format = "%0.1f k"; }
			if (size > 1024) { size /= 1000.0; format = "%0.1f m"; }
			ImGui::LabelText("Nb Shapes", format.c_str(), size);
			if (isTick && dt < (1.0 / 60)) {
				for (int i = 0; i < 500; ++i) {
					sf::RectangleShape r(Vector2f(13, 13));
					r.rotate(Dice::angleDeg());
					r.setPosition(Dice::randF() * 1280, Dice::randF() * 600);
					r.setFillColor(Lib::makeFromHSV(Dice::angleDeg(), 0.9, 0.9));
					rects.push_back(r);
				}
			}
			if (ImGui::Button("Reset")) {
				rects.clear();
				s_RectShapeFlood = false;
			}
		}
		ImGui::Unindent();
		ImGui::PopID();
	}

	

	if (ImGui::CollapsingHeader("Flood Rect - Vertex Array Method")) {
		int start;
		ImGui::PushID(&start);
		ImGui::Indent();
		if (!s_RectVAFlood && ImGui::Button("Start")) {
			s_RectVAFlood = true;

			static bool once = false;
			if (!once) {
				vaRs.blendMode = sf::BlendAlpha;
				once = true;
			}
			va.setPrimitiveType(sf::PrimitiveType::Quads);
		}

		static int nbSurfaceStride = 250000;
		ImGui::DragInt("nbSurfaceStride", &nbSurfaceStride, 10000, 0, 400000);
		if (s_RectVAFlood) {
			ImGui::Checkbox( "Tick", &s_RectVATick);
			std::string format = "%0.1f";
			double size = va.getVertexCount()/4.0;
			if (size > 1024) { size /= 1000.0; format = "%0.1f k"; }
			if (size > 1024) { size /= 1000.0; format = "%0.1f m"; }
			
			ImGui::LabelText("Nb VA Rects", format.c_str(), size);
			if (!s_RectVATick)
				isTick = false;

			if (isTick && dt < (1.0 / 60)) {

				for (int i = 0; i < nbSurfaceStride; ++i)
				{
					sf::RectangleShape r(Vector2f(vaSize, vaSize));
					r.rotate(Dice::angleDeg());
					r.setPosition(Dice::randF() * 1280, Dice::randF() * 600);

					sf::Color c = Lib::makeFromHSV(Dice::angleDeg(), 0.9f, 1.0f);
					c.a = clamp<float>(vaAlpha * 255.0f,0,255);
					auto & trs = r.getTransform();
					
					sf::Vertex v0(trs * r.getPoint(0), c);
					sf::Vertex v1(trs * r.getPoint(1), c);
					sf::Vertex v2(trs * r.getPoint(2), c);
					sf::Vertex v3(trs * r.getPoint(3), c);
					
					va.append(v0);

					va.append(v1);
					va.append(v2);
					va.append(v3);
				}
			}

			if (ImGui::Button("Remove batch")) {
				if (va.getVertexCount())
					va.resize(va.getVertexCount() - nbSurfaceStride * 4);
				s_RectVATick = false;
			}
			if (ImGui::Button("Reset")) {
				va.clear();
				s_RectVAFlood = false;
			}
		}

		ImGui::SliderInt("size", &vaSize, 0, 256);
		ImGui::SliderFloat("alpha", &vaAlpha, 0, 2.2);
		std::array<string,5> blends = { "Alpha","Add","Opaque","Multiply","???"};
		if (ImGui::BeginCombo("blend", blends[blendModeIndex(vaRs.blendMode)].c_str())) {

			bool selected = false;
			if (ImGui::Selectable("Alpha", selected = (vaRs.blendMode == sf::BlendAlpha)))
				vaRs.blendMode = sf::BlendAlpha;
			if (selected) ImGui::SetItemDefaultFocus();
			if (ImGui::Selectable("Add", selected = (vaRs.blendMode == sf::BlendAdd)))
				vaRs.blendMode = sf::BlendAdd;
			if (selected) ImGui::SetItemDefaultFocus();
			if (ImGui::Selectable("Opaque", selected = (vaRs.blendMode == sf::BlendNone)))
				vaRs.blendMode = sf::BlendNone;
			if (selected) ImGui::SetItemDefaultFocus();
			if (ImGui::Selectable("Multiply", selected = (vaRs.blendMode == sf::BlendMultiply)))
				vaRs.blendMode = sf::BlendMultiply;
			if (selected) ImGui::SetItemDefaultFocus();
			ImGui::EndCombo();
		}
		ImGui::Unindent();
		ImGui::PopID();
	}

	if (ImGui::CollapsingHeader("CPU Test")) {
		static double timeToCpuN0	= 100.0;
		static double timeToCpuNN	= 100.0;
		static double timeToCpuAAA	= 100.0;
		static double refTime		= 100.0;

		if (ImGui::Button("test direct call")) {
			VirtualCallTest::flushCache();
			auto t0 = Lib::getTimeStamp();
			AA* a = new AA();
			for (int i = 0; i < 1000000 + Dice::roll(0, 5); i++)
				a->doSomething();
			auto t1 = Lib::getTimeStamp();

			if (refTime > t1 - t0)
				refTime = t1 - t0;
		}

		if (ImGui::Button("test virtual call n0")) {
			VirtualCallTest::flushCache();
			auto t0 = Lib::getTimeStamp();
			A* a = new A();

			for (int i = 0; i < 1000000 + Dice::roll(0, 5); i++)
				a->doSomething();
			auto t1 = Lib::getTimeStamp();

			if(timeToCpuN0> t1 - t0)
				timeToCpuN0 = t1 - t0;
		}

		if (ImGui::Button("test virtual call NN")) {
			VirtualCallTest::flushCache();
			auto t0 = Lib::getTimeStamp();
			H* a = new H();

			for (int i = 0; i < 1000000 + Dice::roll(0, 5); i++)
				a->doSomething();
			auto t1 = Lib::getTimeStamp();

			if(timeToCpuNN> t1 - t0)
				timeToCpuNN = t1 - t0;
		}

		if (ImGui::Button("test inline call NN")) {
			VirtualCallTest::flushCache();
			auto t0 = Lib::getTimeStamp();
			AAA* a = new AAA();
			for (int i = 0; i < 1000000 + Dice::roll(0, 5); i++)
				a->doSomething();
			auto t1 = Lib::getTimeStamp();
			if(timeToCpuAAA> t1-t0)
				timeToCpuAAA = t1 - t0;
		}

		
		if (refTime > 0.0 && refTime < 99) {
			ImGui::LabelText("refTime ", "%0.9f s", refTime);
		}
		if (timeToCpuN0 > 0.0 && timeToCpuN0 < 99) {
			//ImGui::LabelText("Timing N0", "%0.9f s", timeToCpuN0);
			ImGui::LabelText("N0 To Ref time", "%0.12f s", (timeToCpuN0 - refTime));
		}
		if (timeToCpuNN > 0.0 && timeToCpuNN < 99) {
			//ImGui::LabelText("Timing NN", "%0.9f s", timeToCpuNN);
			ImGui::LabelText("NN To Ref time", "%0.12f s", (timeToCpuNN - refTime));
		}
		if (timeToCpuAAA > 0.0 && timeToCpuAAA < 99) {
			//ImGui::LabelText("Timing N AAA", "%0.9f s", timeToCpuAAA);
			ImGui::LabelText("AAA To Ref time", "%0.12f s", (timeToCpuAAA - refTime));
		}
	}
}

 void Game::draw(sf::RenderWindow & win) {
	if (closing) return;

	sf::RenderStates states = sf::RenderStates::Default;
	sf::Shader * sh = &bgShader->sh;
	states.blendMode = sf::BlendAdd;
	states.shader = sh;
	states.texture = &tex;
	sh->setUniform("texture", tex);
	//sh->setUniform("time", g_time);
	win.draw(bg, states);

	beforeParts.draw(win);

	for (sf::RectangleShape & r : wallSprites)
		win.draw(r);

	if(s_RectShapeFlood)
		for (sf::RectangleShape& r : rects) 
			win.draw(r);
	
	if (s_RectVAFlood) {
		win.draw(va,vaRs);
	}

	afterParts.draw(win);
}


void Game::onSpacePressed() {
	
}

void Game::onFileTick() {
	
}

bool Game::isWall(int cx, int cy)
{
	for (Vector2i & w : walls) {
		if (w.x == cx && w.y == cy)
			return true;
	}
	return false;
}

void Game::im()
{

}

