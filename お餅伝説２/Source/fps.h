#pragma once

#include "DXlib.h"
#include <array>

//�萔��`(define�ł������̂ł����A�L�@���Â��̂Ŕ񐄏��ł��B)
constexpr float TargetFPS = 60.f;					//�ݒ肷��FPS
constexpr int RecordFPSCount = 60;					//FPS�̐��l���L�^���鑍��
constexpr auto MicroSecond = 1000.0f * 1000.0f;		//1�}�C�N���b
constexpr auto MillSecond = 1000.0f;				//1�~���b

/// <summary>
/// FPS����N���X
/// FPS = ��ʂ̍X�V��1�b�ɉ���s����(FramePerSecond)
/// </summary>
class FPSControl {
private:								//�O�ɏo���Ȃ������o�ϐ�
	LONGLONG		m_FirstTakeTime = 0;				//�v���J�n���̎��Ԃ��}�C�N���b�P�ʂŕۑ�
														//����FPS���v�Z���邽�߂̕ϐ�
	std::array<float, RecordFPSCount> m_OldFPS{};		//�L�^����FPS�l(RecordFPSCount�t���[���O�܂ŋL�^���܂�)
	int				m_NowRecordPoint{};					//m_OldFPS�̂ǂ̈ʒu�Ɍ��݂�FPS���L�^���邩
	float			m_AverageFPS = 0.0f;				//����FPS�l

public:									//�O�ɏo�������o�֐��̃v���g�^�C�v�錾
	VOID Initialize(VOID);				//���C�����[�v�O�ɍs�����e:�e�����o�ϐ��̏������Ȃ�			
	VOID StartMeasureTime(VOID);		//�������Ԍv���̊J�n				
	VOID Draw(VOID);					//FPS�`��			
	VOID FPSWait(VOID);					//�������Ԍv�����I�����A����܂ł̎��ԂƖڕWFPS�Ƃőҋ@���鎞�Ԃ�����o���ҋ@���܂�
private:								//�O�ɏo���Ȃ������o�֐��̃v���g�^�C�v�錾
	VOID InitializeRecord();			//����FPS���v�Z���鏈���̏�����
	VOID RecordNowFPS(float nowFPS);	//���݂�FPS�l���L�^���A����܂ł̒l���畽��FPS�l���v�Z
};
