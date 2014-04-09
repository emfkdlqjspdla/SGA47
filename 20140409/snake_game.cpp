#include "game.h"

// 주석 추가...
// 주석 또 추가...
int main(void)
{
	game snakegame;

	snakegame.Setup();

	clock_t st = clock();
	clock_t dt = 0;

	// 2. 전체 게임 시작...
	while (true)
	{
		// 3. 입력처리
		if (!snakegame.Input())
			break;

		// 4. 업데이트. 데이터 갱신
		snakegame.Update(dt);

		// 5. 그리기.
		snakegame.Draw();

		dt = clock() - st;
		st = clock();

	}


	_getch();
	return 0;
}

