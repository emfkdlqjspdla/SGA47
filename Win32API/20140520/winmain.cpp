#include "CreditApp.h"

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, INT)
{
	CreditApp app;

	app.Setup(hInst);

	return app.Run();
}