#include	"Header.hpp"

#include	"sub.hpp"
#include	"MainScene/Player/Player.hpp"
#include	"MainScene/BackGround/BackGround.hpp"
// 
#include	"Scene/TitleScene.hpp"
#include	"Scene/MainGameScene.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	DXLib_ref::Create();
	auto* DXLib_refParts = DXLib_ref::Instance();
	if (!DXLib_refParts->StartLogic()) { return 0; }
	// 追加設定
	SetMainWindowText("Title");					// タイトル
	// 
	FPS_n2::Sceneclass::PlayerManager::Create();
	FPS_n2::Sceneclass::Cam2DControl::Create();
	FPS_n2::Sceneclass::BackGroundClassBase::Create();
	FPS_n2::Sceneclass::ButtonControl::Create();
	FPS_n2::Sceneclass::Effect2DControl::Create();
	// ローカライズ制御クラスの生成
	FPS_n2::Sceneclass::LocalizePool::Create();
	// シーン
	auto Titlescene = std::make_shared<FPS_n2::Sceneclass::TitleScene>();
	auto MainGamescene = std::make_shared<FPS_n2::Sceneclass::MainGameScene>();
	// 遷移先指定
	Titlescene->SetNextSceneList(0, MainGamescene);
	MainGamescene->SetNextSceneList(0, Titlescene);
	MainGamescene->SetNextSceneList(1, MainGamescene);

	auto* SceneParts = SceneControl::Instance();
	SceneParts->AddList(Titlescene);
	SceneParts->AddList(MainGamescene);
	// メインロジック開始
	DXLib_refParts->MainLogic();
	return 0;
}
