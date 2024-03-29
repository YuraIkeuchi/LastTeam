#include "SceneFactory.h"
#include "BattleScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "GameoverScene.h"
#include "TutorialScene.h"
#include "MapScene.h"
#include "ClearScene.h"
BaseScene* SceneFactory::CreateScene(const std::string& sceneName) {
	//次のシーンの作成
	BaseScene* newScene = nullptr;
	if (sceneName=="TITLE") {
		newScene=new TitleScene();
	}
	else if (sceneName == "TUTORIAL") {
		newScene = new TutorialScene();
	}
	else if (sceneName == "MAP") {
		newScene = new MapScene();
	}
	else if(sceneName == "BATTLE"){
		newScene = new BattleScene();
	}
	else if (sceneName == "GAMEOVER") {
		newScene = new GameoverScene();
	}
	else if (sceneName == "CLEAR") {
		newScene = new ClearScene();
	}
	return newScene;
}
