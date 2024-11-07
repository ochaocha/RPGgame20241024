#include	"Header.hpp"

#include	"sub.hpp"
#include	"MainScene/Player/Player.hpp"
#include	"MainScene/BackGround/BackGround.hpp"
// 
#include	"Scene/TitleScene.hpp"
#include	"Scene/MainGameScene.hpp"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	DXLib_ref::Create();
	// DXLIB�̒ǉ��ݒ�
	DxLib::SetMainWindowText("Title");								// �^�C�g��
	// 
	DXLIB_Sample::Sceneclass::PlayerManager::Create();				// �v���C���[�}�l�[�W���[

	DXLIB_Sample::Sceneclass::Cam2DControl::Create();				// 2D�J�����}�l�[�W���[

	DXLIB_Sample::Sceneclass::BackGroundClassBase::Create();		// �w�i�}�l�[�W���[
	DXLIB_Sample::Sceneclass::EventDataBase::Create();				// �w�i�}�l�[�W���[

	DXLIB_Sample::UI::ButtonControl::Create();						// UI�p�{�^���}�l�[�W���[
	DXLIB_Sample::UI::FadeControl::Create();						// UI�p�t�F�[�h�C���}�l�[�W���[

	DXLIB_Sample::Sceneclass::Effect2DControl::Create();			// 2D�G�t�F�N�g�}�l�[�W���[

	DXLIB_Sample::Sceneclass::StoryTextDataBase::Create();				// �e�L�X�g��ID����Ƃ��Ă���N���X

	// �V�[���𐶐�
	auto Titlescene = std::make_shared<DXLIB_Sample::Sceneclass::TitleScene>();
	auto MainGamescene = std::make_shared<DXLIB_Sample::Sceneclass::MainGameScene>();
	// �e�V�[���̑J�ڐ�w��
	Titlescene->SetNextSceneList(0, MainGamescene);			// �^�C�g�����烁�C���V�[���ւ̑J��
	MainGamescene->SetNextSceneList(0, Titlescene);			// ���C���V�[������^�C�g���ւ̑J��
	MainGamescene->SetNextSceneList(1, MainGamescene);		// ���C���V�[�����玟�̃��C���V�[���ւ̑J��
	// �ŏ��̃V�[����ݒ�
	auto* SceneParts = SceneControl::Instance();
	SceneParts->SetFirstScene(Titlescene);
	// ���C�����W�b�N�J�n
	DXLib_ref::Instance()->MainLogic();
	return 0;
}