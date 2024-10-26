#include "fps.h"

/// <summary>
/// ���C�����[�v�O�ɍs�����e:�e�����o�ϐ��̏������Ȃ�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::Initialize(VOID)
{
	InitializeRecord();																	//FPS�l�̋L�^��������
	return;//��VOID���߂�l�̏ꍇreturn�͏����Ȃ��Ă�ok
}

/// <summary>
/// �������Ԍv���̊J�n
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::StartMeasureTime(VOID)
{
	this->m_FirstTakeTime = GetNowHiPerformanceCount();									//Windows���N�����Ă��猻�݂܂ł̎��Ԃ��}�C�N���b�œ���
	return;
}

/// <summary>
/// ����FPS��`��
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::Draw(VOID)
{
	//����ɕ`��
	DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS�F%3.1f", this->m_AverageFPS);	//%3.1f->�����_���݂�3�����ȏ�A�����_1���܂ŕ\��
	return;
}

/// <summary>
/// �������Ԍv�����I�����A����܂ł̎��ԂƖڕWFPS�Ƃőҋ@���鎞�Ԃ�����o���ҋ@���܂�
/// </summary>
/// <param name=""></param>
/// <returns></returns>
VOID FPSControl::FPSWait(VOID)
{
	LONGLONG NowTakeTime = GetNowHiPerformanceCount();									//Windows���N�����Ă��猻�݂܂ł̎��Ԃ��}�C�N���b�œ���
	float Deltatime = (NowTakeTime - this->m_FirstTakeTime) / MicroSecond;				//�v���J�n�ƏI���܂ł̍������ԁi�f���^�^�C���j���v�Z���ĕb���Ƃ��ĕۑ�
	if (Deltatime > 0.0f)																//0�Ŋ����Ă͂����Ȃ����߂�����m�F
	{
		RecordNowFPS(1.0f / Deltatime);													//����FPS�̋L�^ ���݂�FPS�l�� 1�b/1�t���[���ɂ��������b�� �ŋ��߂��܂�
	}

	float WaitSecond = (1.0f / TargetFPS - Deltatime);									//�ڕWFPS���o���̂ɂ����鎞��-�f���^�^�C��=�ҋ@����K�v������b��
	if (0.f < WaitSecond)																//�ҋ@���Ԃ�����ꍇ
	{
		WaitTimer(static_cast<int>(WaitSecond * MillSecond));							//�~���b�P�ʂ�WaitSecond�b�Ԃ�ҋ@
	}
	return;
}

/// <summary>
/// ����FPS���v�Z���鏈���̏�����
/// </summary>
/// <returns></returns>
VOID FPSControl::InitializeRecord()
{
	for (auto& RecordFPS : this->m_OldFPS)												//���X�g��S�Ĕ͈�for�ő���
	{
		RecordFPS = TargetFPS;															//�v���l�����ׂ�TargetFPS�ŏ�����
	}
	this->m_NowRecordPoint = 0;															//m_NowRecordPoint�Ŏw�肷��l��������
	this->m_AverageFPS = TargetFPS;														//���ϒl��TargetFPS�ɂȂ�͂��Ȃ̂ł�����ŏ�����
	return;
}

/// <summary>
/// ���݂�FPS�l���L�^���A����܂ł̒l���畽��FPS�l���v�Z
/// </summary>
/// <param name="nowFPS">�ۑ�����FPS�l</param>
/// <returns></returns>
VOID FPSControl::RecordNowFPS(float nowFPS)
{
	this->m_OldFPS[this->m_NowRecordPoint] = nowFPS;									//���݂�FPS��m_NowRecordPoint�̈ʒu�ɋL�^
	this->m_NowRecordPoint++;															//m_NowRecordPoint�Ŏw�肷��l���ړ�
	if (this->m_NowRecordPoint >= RecordFPSCount) {
		this->m_NowRecordPoint = 0;														//�L�^����ʒu��this->m_OldFPS�͈̔͂𒴂��Ă��܂�����ʒu�����Z�b�g
	}

	float TotalFPS = 0.0f;
	for (auto& RecordFPS : this->m_OldFPS)												//���X�g��S�Ĕ͈�for�ő���
	{
		TotalFPS += RecordFPS;															//���ϒl�����߂邽�߁A����܂ŋL�^���Ă���FPS�����Z
	}
	this->m_AverageFPS = TotalFPS / static_cast<float>(RecordFPSCount);					//�f�[�^�̑����Ŋ���A���ς����߂�
	return;
}
