#include"IkeGame.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#pragma region DirectXの初期化
	Framework* gscene = new IkeGame();
	
	gscene->Run();

	delete gscene;
	return 0;
}