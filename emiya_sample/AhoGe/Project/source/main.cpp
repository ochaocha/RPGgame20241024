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
	DxLib::SetMainWindowText("Title");						// �^�C�g��
	// 
	FPS_n2::Sceneclass::PlayerManager::Create();			//�v���C���[�}�l�[�W���[

	FPS_n2::Sceneclass::Cam2DControl::Create();				//2D�J�����}�l�[�W���[

	FPS_n2::Sceneclass::BackGroundClassBase::Create();		//�w�i�}�l�[�W���[
	FPS_n2::Sceneclass::EventDataBase::Create();			//�w�i�}�l�[�W���[

	FPS_n2::Sceneclass::ButtonControl::Create();			//UI�p�{�^���}�l�[�W���[

	FPS_n2::Sceneclass::Effect2DControl::Create();			//2D�G�t�F�N�g�}�l�[�W���[

	FPS_n2::Sceneclass::LocalizePool::Create();				//�e�L�X�g��ID����Ƃ��Ă���N���X

	// �V�[���𐶐�
	auto Titlescene = std::make_shared<FPS_n2::Sceneclass::TitleScene>();
	auto MainGamescene = std::make_shared<FPS_n2::Sceneclass::MainGameScene>();
	// �e�V�[���̑J�ڐ�w��
	Titlescene->SetNextSceneList(0, MainGamescene);			//�^�C�g�����烁�C���V�[���ւ̑J��
	MainGamescene->SetNextSceneList(0, Titlescene);			//���C���V�[������^�C�g���ւ̑J��
	MainGamescene->SetNextSceneList(1, MainGamescene);		//���C���V�[�����玟�̃��C���V�[���ւ̑J��
	// �ŏ��̃V�[����ݒ�
	auto* SceneParts = SceneControl::Instance();
	SceneParts->SetFirstScene(Titlescene);
	// ���C�����W�b�N�J�n
	DXLib_ref::Instance()->MainLogic();
	return 0;
}