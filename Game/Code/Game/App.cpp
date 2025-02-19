#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"


App* g_theApp = nullptr;
RandomNumberGenerator* g_theRNG = nullptr;
Renderer* g_theRenderer = nullptr;
AudioSystem* g_theAudio = nullptr;
Window* g_theWindow = nullptr;
bool g_debugDraw;

App::App()
{

}

App::~App()
{

}

void App::Startup()
{
	InitializeGameConfig("Data/GameConfig.xml");

	Clock::s_theSystemClock = new Clock();
	Clock::s_theSystemClock->TickSystemClock();
	m_gameClock = new Clock(*Clock::s_theSystemClock);

	EventSystemConfig eventConfig;
	g_theEventSystem = new EventSystem(eventConfig);

	InputConfig inputConfig;
	g_theInput = new InputSystem(inputConfig);

	WindowConfig windowConfig;
	windowConfig.m_inputSystem = g_theInput;
	windowConfig.m_windowTitle = "ProtoGame 2D";
	windowConfig.m_clientAspect = g_gameConfigBlackboard.GetValue("aspectRation", 2.f);;
	g_theWindow = new Window(windowConfig);

	RendererConfig renderConfig;
	renderConfig.m_window = g_theWindow;
	g_theRenderer = new Renderer(renderConfig);

	AudioConfig audioConfig;
	g_theAudio = new AudioSystem(audioConfig);

	DevConsoleConfig consoleConfig;
	consoleConfig.m_renderer = g_theRenderer;
	consoleConfig.m_fontFilePath = "Data/Fonts/SquirrelFixedFont";
	consoleConfig.m_camera = new Camera();
	g_theDevConsole = new DevConsole(consoleConfig);

	m_game = new Game();

	g_theEventSystem->Startup();
	g_theInput->Startup();
	g_theWindow->Startup();
	g_theRenderer->Startup();
	g_theAudio->Startup();
	g_theDevConsole->Startup();
	m_game->Startup();

	g_theRNG = new RandomNumberGenerator();

	SubscribeEventCallbackFunction("quit", App::Event_Quit);

	ConsoleTutorial();
}

bool App::Event_Quit(EventArgs& args)
{
	UNUSED(args);
	g_theApp->HandleQuitRequested();
	return false;
}

void App::Shutdown()
{
	m_game->Shutdown();
	g_theDevConsole->Shutdown();
	g_theAudio->Shutdown();
	g_theRenderer->Shutdown();
	g_theWindow->Startup();
	g_theInput->Shutdown();
	g_theEventSystem->Shutdown();

	delete g_theRNG;
	delete m_game;
	m_game = nullptr;
	delete g_theDevConsole;
	g_theDevConsole = nullptr;
	delete g_theAudio;
	g_theAudio = nullptr;
	delete g_theRenderer;
	g_theRenderer = nullptr;
	delete g_theWindow;
	g_theWindow = nullptr;
	delete g_theInput;
	g_theInput = nullptr;
	delete g_theEventSystem;
	g_theEventSystem = nullptr;

}

void App::Run()
{
	while (!m_isQuitting)
	{
		RunFrame();
	}
}

void App::RunFrame()
{
	BeginFrame();
	Update();
	Render();
	EndFrame();
}

bool App::HandleQuitRequested()
{
	m_isQuitting = true;
	return 0;
}

void App::BeginFrame()
{
	Clock::s_theSystemClock->TickSystemClock();
	g_theEventSystem->BeginFrame();
	g_theInput->BeginFrame();
	g_theWindow->BeginFrame();
	g_theRenderer->BeginFrame();
	g_theAudio->BeginFrame();
	g_theDevConsole->BeginFrame();
}

void App::Update()
{
	if (g_theInput->WasKeyJustPressed(KEYCODE_TIDLE))
	{
		g_theDevConsole->ToggleOpen();
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_F1))
	{
		g_debugDraw = !g_debugDraw;
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_ESCAPE) || g_theInput->GetController(0).WasButtonJustPressed(XBOX_BUTTON_BACK))
	{
		if (m_game->m_state == GameState::ATTRACT_MODE)
		{
			m_isQuitting = true;
		}
		m_game->SwitchState(GameState::ATTRACT_MODE);
	}
	if (g_theInput->WasKeyJustPressed(KEYCODE_F8))
	{
		m_game->Shutdown();
		delete m_game;
		m_game = new Game();
		m_game->Startup();
	}
	if (g_theInput->IsKeyDown('T'))
	{
		m_gameClock->SetTimeScale(0.1f);
	}
	if (g_theInput->WasKeyJustReleased('T'))
	{
		m_gameClock->SetTimeScale(1.f);
	}

	if (g_theInput->WasKeyJustPressed('P'))
	{
		m_gameClock->TogglePause();
	}

	if (g_theInput->WasKeyJustPressed('O'))
	{
		m_gameClock->StepSingleFrame();
	}

	m_game->Update();
}

void App::Render() const
{
	m_game->Render();
	AABB2 screenBound(0.f, 0.f, 1600.f, 800.f);
	g_theDevConsole->Render(screenBound, g_theRenderer);
}

void App::EndFrame()
{
	g_theEventSystem->EndFrame();
	g_theInput->EndFrame();
	g_theWindow->EndFrame();
	g_theRenderer->EndFrame();
	g_theAudio->EndFrame();
	g_theDevConsole->EndFrame();
}

void App::ConsoleTutorial()
{
	g_theDevConsole->AddLine(Rgba8::COLOR_TRANSPARENT, "\n");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "			HOW TO PLAY			");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "-----------------");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "Space to Start Game");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "-----------------");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "						DEBUG			");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "-----------------");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "P to pause");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "O to run 1 frame");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "Hold T to enter slow mo mode");
	g_theDevConsole->AddLine(DevConsole::INFO_MINOR, "F1 to debug draw");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "-----------------");
	g_theDevConsole->AddLine(Rgba8::COLOR_TRANSPARENT, "\n");
	g_theDevConsole->AddLine(DevConsole::INFO_MAJOR, "Type help to see the list of events");
}

void App::InitializeGameConfig(const char* filePath)
{
	XmlDocument file;
	XmlError result = file.LoadFile(filePath);
	GUARANTEE_OR_DIE(result == tinyxml2::XML_SUCCESS, "FILE IS NOT LOADED");

	XmlElement* element = file.FirstChildElement();

	g_gameConfigBlackboard.PopulateFromXmlElementAttributes(*element);
}