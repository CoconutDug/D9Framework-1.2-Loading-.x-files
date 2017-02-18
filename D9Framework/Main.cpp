#include "Types.h"
#include "Program.h"
#include "Game.h"
#include "GameStateManager.h"
#include "Timer.h"

int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	Timer timer;
	unsigned __int64 lastTime;
	unsigned __int64 timerFrequency;

	Program::Create();

	MSG msg;
	Program::GetInstance()->GenerateProgram(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	GameStateManager::Create();
	GameStateManager::GetInstance()->AddState(std::make_unique<Game>());

	timerFrequency = timer.GetFrequency();
	lastTime = timer.GetTime();

	while (Program::GetInstance()->GetRunning())
	{
		unsigned __int64 timeNow = timer.GetTime();
		unsigned __int64 timeDiff = timeNow - lastTime;
		lastTime = timeNow;

		if (timeDiff > timerFrequency)
		{
			timeDiff = timerFrequency;
		}

		float timeDiffFloat = (float)timeDiff / (float)timerFrequency;


		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			BOOL bGetResult = GetMessage(&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Program::GetInstance()->Update(timeDiffFloat);
		Program::GetInstance()->Render();
	}

	Program::Destroy();
	GameStateManager::Destroy();
	return (int)msg.wParam;

}