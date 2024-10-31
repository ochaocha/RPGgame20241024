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
	// �ǉ��ݒ�
	SetMainWindowText("Title");					// �^�C�g��
	// 
	FPS_n2::Sceneclass::PlayerManager::Create();
	FPS_n2::Sceneclass::Cam2DControl::Create();
	FPS_n2::Sceneclass::BackGroundClassBase::Create();
	FPS_n2::Sceneclass::ButtonControl::Create();
	FPS_n2::Sceneclass::Effect2DControl::Create();
	// ���[�J���C�Y����N���X�̐���
	FPS_n2::Sceneclass::LocalizePool::Create();
	// �V�[��
	auto Titlescene = std::make_shared<FPS_n2::Sceneclass::TitleScene>();
	auto MainGamescene = std::make_shared<FPS_n2::Sceneclass::MainGameScene>();
	// �J�ڐ�w��
	Titlescene->SetNextSceneList(0, MainGamescene);
	MainGamescene->SetNextSceneList(0, Titlescene);
	MainGamescene->SetNextSceneList(1, MainGamescene);

	auto* SceneParts = SceneControl::Instance();
	SceneParts->AddList(Titlescene);
	SceneParts->AddList(MainGamescene);
	// ���C�����W�b�N�J�n
	DXLib_refParts->MainLogic();
	return 0;
}
