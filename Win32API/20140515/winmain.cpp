#include "TodayApp.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	TodayApp app;

	app.Setup(hInst);

	return app.Run();
}