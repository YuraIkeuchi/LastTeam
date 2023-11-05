#include "SceneFactory.h"
#include "BattleScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "GameoverScene.h"
#include "LoadScene.h"
#include "TutorialScene.h"
#include "MapScene.h"
BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//éüÇÃÉVÅ[ÉìÇÃçÏê¨
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	else if (sceneName == "TUTORIAL") {
		newScene = new TutorialScene();
	}
	else if (sceneName == "MAPSCENE") {
		newScene = new TutorialScene();
	}
	else if(sceneName == "BATTLE"){
		newScene = new BattleScene();
	}
	else if (sceneName == "GAMEOVER") {
		newScene = new GameoverScene();
	}
	return newScene;
}
