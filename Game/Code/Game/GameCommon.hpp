#pragma once
#include "Engine/Core/Vertex_PCU.hpp"
#include "Engine/Core/VertexUtils.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/AudioSystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/Camera.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/RandomNumberGenerator.hpp"
#include "Game/App.hpp"
#include "Engine/Core/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Clock.hpp"
#include <math.h>

extern App* g_theApp;
extern Renderer* g_theRenderer;
extern InputSystem* g_theInput;
extern AudioSystem* g_theAudio;
extern RandomNumberGenerator* g_theRNG;
extern bool g_debugDraw;

constexpr float WORLD_SIZE_X = 200.f;
constexpr float WORLD_SIZE_Y = 100.f;
constexpr float MAX_SHAKE = 10.f;
constexpr float SHAKE_REDUCTION_PER_SEC = 7.f;

void DrawDebugRing(Vec2 const& center, float radius, float thickness, Rgba8 const& color);
void DrawDebugLine(Vec2 const& startPos, Vec2 const& endPos, float thickness, Rgba8 const& color);
void DrawDebugGlow(Vec2 const& center, float radius, Rgba8 const& colorInside, Rgba8 const& colorOutside);