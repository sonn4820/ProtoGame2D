#include "Game/Game.hpp"

Game::Game()
{
	m_worldCamera.SetOrthographicView(Vec2(0, 0), Vec2(WORLD_SIZE_X, WORLD_SIZE_Y));
	m_screenCamera.SetOrthographicView(Vec2(0, 0), Vec2(1600.f, 800.f));
}
//..............................
Game::~Game()
{

}
//..............................
void Game::Startup()
{
	SwitchState(GameState::ATTRACT_MODE);
	m_gameClock = new Clock(*Clock::s_theSystemClock);
}
//..............................
void Game::Shutdown()
{

}

//----------------------------------------------------------------------------------------------------------------------------------------
// UPDATE

void Game::Update()
{
	m_secondIntoMode += Clock::s_theSystemClock->GetDeltaSeconds();

	if (m_state == GameState::PLAY_MODE)
	{
		UpdatePlayMode();
	}
	if (m_state == GameState::ATTRACT_MODE)
	{
		UpdateAttractMode();
	}

	UpdateCamera();

	//..............................
	//TEST DEV CONSOLE
// 	std::string print = "ProtoGame 2D Dev Console Test";
// 	g_theDevConsole->AddLine(Rgba8::COLOR_WHITE, print);

	//..............................
}
//..............................
void Game::UpdatePlayMode()
{
	HandleInput();
}
//..............................
void Game::UpdateAttractMode()
{
	if (g_theInput->WasKeyJustPressed(KEYCODE_SPACE))
	{
		SwitchState(GameState::PLAY_MODE);
	}
	m_timer += Clock::s_theSystemClock->GetDeltaSeconds();

	if (m_timer > 4.f)
	{
		m_isFlip = !m_isFlip;
		m_timer = 0.f;
	}

	m_sizeChange = (m_isFlip) ? Interpolate(0.f, 20.f, m_timer) : -Interpolate(-20.f, 0.f, m_timer);

}
//..............................
void Game::UpdateCamera()
{
	float shakeX = g_theRNG->RollRandomFloatInRange(-m_screenShakeAmount, m_screenShakeAmount);
	float shakeY = g_theRNG->RollRandomFloatInRange(-m_screenShakeAmount, m_screenShakeAmount);
	m_worldCamera.Translate2D(Vec2(shakeX, shakeY));
	m_screenShakeAmount -= SHAKE_REDUCTION_PER_SEC * Clock::s_theSystemClock->GetDeltaSeconds();
	m_screenShakeAmount = Clamp(m_screenShakeAmount, 0.f, MAX_SHAKE);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// RENDER 

void Game::Render() const
{
	if (m_state == GameState::PLAY_MODE)
	{
		RenderPlayMode();
	}
	if (m_state == GameState::ATTRACT_MODE)
	{
		RenderAttractMode();
	}

	RenderUI();
}
//..............................

void Game::RenderPlayMode() const
{
	g_theRenderer->ClearScreen(Rgba8(150, 180, 255, 255));
	g_theRenderer->BeginCamera(m_worldCamera);


	g_theRenderer->EndCamera(m_worldCamera);
}
//..............................
void Game::RenderAttractMode() const
{
	g_theRenderer->ClearScreen(Rgba8(255, 0, 255, 255));
	g_theRenderer->BeginCamera(m_screenCamera);

	std::vector<Vertex_PCU> drawVertexList;
	drawVertexList.reserve(100);

	AddVertsForAABB2D(drawVertexList, AABB2(400.f + m_sizeChange, 200.f + m_sizeChange, 1200.f + m_sizeChange, 600.f + m_sizeChange), Rgba8::COLOR_GREEN);

	g_theRenderer->BindTexture(nullptr);
 	g_theRenderer->DrawVertexArray((int)drawVertexList.size(), drawVertexList.data());
	g_theRenderer->EndCamera(m_screenCamera);
}
//..............................
void Game::RenderUI() const
{
	g_theRenderer->BeginCamera(m_screenCamera);
	g_theRenderer->EndCamera(m_screenCamera);
}

//----------------------------------------------------------------------------------------------------------------------------------------
// GAME RESTART

void Game::GameRestart()
{
	Shutdown();
	Startup();
	SwitchState(GameState::PLAY_MODE);
	g_debugDraw = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------
// HANDLE INPUT

void Game::HandleInput()
{
	if (g_theInput->WasKeyJustPressed('K'))
	{
		SoundPlaybackID test = g_theAudio->CreateOrGetSound("TestSound.mp3");
		g_theAudio->StartSound(test);
	}
}

void Game::SwitchState(GameState state)
{
	m_state = state;
}
