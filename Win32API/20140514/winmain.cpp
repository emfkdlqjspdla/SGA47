#include "SampleApp.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	SampleApp app;

	app.Setup(hInst);

	return app.Run();
}