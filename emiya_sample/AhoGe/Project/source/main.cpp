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
	DxLib::SetMainWindowText("Title");								// タイトル
	// 
	DXLIB_Sample::Sceneclass::PlayerManager::Create();				// プレイヤーマネージャー

	DXLIB_Sample::Sceneclass::Cam2DControl::Create();				// 2Dカメラマネージャー

	DXLIB_Sample::Sceneclass::BackGroundClassBase::Create();		// 背景マネージャー
	DXLIB_Sample::Sceneclass::EventDataBase::Create();				// 背景マネージャー

	DXLIB_Sample::UI::ButtonControl::Create();						// UI用ボタンマネージャー
	DXLIB_Sample::UI::FadeControl::Create();						// UI用フェードインマネージャー

	DXLIB_Sample::Sceneclass::Effect2DControl::Create();			// 2Dエフェクトマネージャー

	DXLIB_Sample::Sceneclass::StoryTextDataBase::Create();				// テキストをIDからとってくるクラス

	// シーンを生成
	auto Titlescene = std::make_shared<DXLIB_Sample::Sceneclass::TitleScene>();
	auto MainGamescene = std::make_shared<DXLIB_Sample::Sceneclass::MainGameScene>();
	// 各シーンの遷移先指定
	Titlescene->SetNextSceneList(0, MainGamescene);			// タイトルからメインシーンへの遷移
	MainGamescene->SetNextSceneList(0, Titlescene);			// メインシーンからタイトルへの遷移
	MainGamescene->SetNextSceneList(1, MainGamescene);		// メインシーンから次のメインシーンへの遷移
	// 最初のシーンを設定
	auto* SceneParts = SceneControl::Instance();
	SceneParts->SetFirstScene(Titlescene);
	// メインロジック開始
	DXLib_ref::Instance()->MainLogic();
	return 0;
}