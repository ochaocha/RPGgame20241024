#include	"Header.hpp"

#include	"sub.hpp"
#include	"MainScene/Player/Player.hpp"
#include	"MainScene/BackGround/BackGround.hpp"
// 
#include	"Scene/TitleScene.hpp"
#include	"Scene/MainGameScene.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	DXLib_ref::Create();
	// DXLIBの追加設定
	DxLib::SetMainWindowText("Title");						// タイトル
	// 
	FPS_n2::Sceneclass::PlayerManager::Create();			//プレイヤーマネージャー

	FPS_n2::Sceneclass::Cam2DControl::Create();				//2Dカメラマネージャー

	FPS_n2::Sceneclass::BackGroundClassBase::Create();		//背景マネージャー
	FPS_n2::Sceneclass::EventDataBase::Create();			//背景マネージャー

	FPS_n2::Sceneclass::ButtonControl::Create();			//UI用ボタンマネージャー

	FPS_n2::Sceneclass::Effect2DControl::Create();			//2Dエフェクトマネージャー

	FPS_n2::Sceneclass::LocalizePool::Create();				//テキストをIDからとってくるクラス

	// シーンを生成
	auto Titlescene = std::make_shared<FPS_n2::Sceneclass::TitleScene>();
	auto MainGamescene = std::make_shared<FPS_n2::Sceneclass::MainGameScene>();
	// 各シーンの遷移先指定
	Titlescene->SetNextSceneList(0, MainGamescene);			//タイトルからメインシーンへの遷移
	MainGamescene->SetNextSceneList(0, Titlescene);			//メインシーンからタイトルへの遷移
	MainGamescene->SetNextSceneList(1, MainGamescene);		//メインシーンから次のメインシーンへの遷移
	// 最初のシーンを設定
	auto* SceneParts = SceneControl::Instance();
	SceneParts->SetFirstScene(Titlescene);
	// メインロジック開始
	DXLib_ref::Instance()->MainLogic();
	return 0;
}