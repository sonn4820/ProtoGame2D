#pragma once
#include "Game/GameCommon.hpp"


enum class GameState 
{
	ATTRACT_MODE,
	PLAY_MODE,
};
class Game
{
public:
	Game();
	~Game();

	void Startup();
	void Update();
	void Render() const;
	void Shutdown();

	Camera m_worldCamera;
	Camera m_screenCamera;
	GameState m_state = GameState::ATTRACT_MODE;
	Clock* m_gameClock = nullptr;

	// STATE
	void SwitchState(GameState state);

	// GAME RESTART
	void GameRestart();

private:
	// VARIABLES
	float m_screenShakeAmount = 0.0f;
	float m_secondIntoMode = 0.f;

	float m_sizeChange = 0.f;
	float m_timer = 0.f;
	bool m_isFlip = false;

	// UPDATE
	void UpdatePlayMode();
	void UpdateAttractMode();
	void UpdateCamera();
	void HandleInput();

	// RENDER
	void RenderPlayMode() const;
	void RenderAttractMode() const;
	void RenderUI() const;
};