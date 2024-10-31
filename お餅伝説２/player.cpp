#include "player.h"
#include "DxLib.h"

#include "map.h"


/*
�L�����N�^�[�ړ��ɂ��āA���󂢂������Ȃ̂ł����A����OllPlayerData��G�L�����N�^�[�ɑ΂��Ă��g�p���鎖�ɂȂ�Ǝv���܂��׈ȉ��̌`�ɂ���Ă݂Ă��������B

�EPlayerPos�𒼐ڑ��삵�Ă��遨PlayerVelocity�������o�[�ϐ��Ƃ��č쐬
�@Playerappdate�P�񒆂ɓ����ʂ�PlayerVelocity�ɑ�����Ă݂܂��傤
�@���̌�PlayerVelocity��PlayerPos�ɑ����܂��B

�EPlayerVelocity����A�j���[�V������ݒ肷��
�@�������̈ړ��ʂ������牺�����A���������ړ��ʂ��Ȃ�������A�j���[�V�������~
*/

void OllCharacterData::PlayerInit(int displaywidth, int displayhight)
{
	PlayerPos.x = float(displaywidth / 2);
	PlayerPos.y = float(displayhight / 2);
	animNowType;
	animNowIndex = 0;
	animDirectionOffset = 0;

	//initRect(playerHit,hitSizeX,hitSize)
	LoadDivGraph("chara/walk.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX);
	drawOffsetY = (hitSizeY - animSizeY);

}

void OllCharacterData::Playerappdate()
{
	//time += 1.0f / 60.f;						//60FPS�͂P�b�Ԃ����炻���time�Ɏ��Ԃ����Ă���
	//
	////0,1,2
	////3,4,5
	////6,7,8
	////9,10,11
	//if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1�b�ȏ�ɂȂ�Ȃ��悤�ɂ���
	//	time -= 1.0f;							//�P�b�ȏ�ɂȂ��1�b�}�C�i�X������
	//	animNowIndex += 1;						//�P�b�ɂȂ�Ɣz��̗v�f�����P���₵�Ă���
	//	if (animNowIndex >= animPatternNum)		//animNowIndex���A�j���[�V�����̉��v�f���ɂȂ��
	//	{
	//		animNowIndex -= animPatternNum;		//�RanimPatternNum��animIndex�Ƀ}�C�i�X������0�`�ɂ��Ă���
	//	}
	//}

	
	

	//�ړ��O�̃v���C���[���W��ۑ�
	Vector2DX prevPos = PlayerPos;


	//A�������ꂽ�Ƃ�
	if (CheckHitKey(KEY_INPUT_A))
	{	
		animDirectionOffset = 8;
		AnimStopLeftFlag =true;
		PlayerPos.x -= PlayerVelocity;
		if (PlayerPos.x -= PlayerVelocity)
		{
			
		}
	
	}
	//D�������ꂽ�Ƃ�
	if (CheckHitKey(KEY_INPUT_D))
	{
		animDirectionOffset = 40;
		AnimStopRighttFlag = true;
		PlayerPos.x += PlayerVelocity;
		
	}
	//S�������ꂽ�Ƃ�
	if (CheckHitKey(KEY_INPUT_S))
	{

		animDirectionOffset = 0;
		AnimStopUnderFlag = true;
		PlayerPos.y += PlayerVelocity;
	
	}
	//W�������ꂽ�Ƃ�
	if (CheckHitKey(KEY_INPUT_W))
	{
		animDirectionOffset = 24;
		PlayerPos.y -= PlayerVelocity;
		
	}

	
	//if(CheckHitKey(KEY_INPUT_))


	//�ړ������O��̐����ƃ}�b�v�ɂ���ǂƂ̔���
	MapData* MapDataOllPtr = MapData::Instance();
	MapDataOllPtr->CalcVectorSlideOnWallChips(
		prevPos,
		&PlayerPos,
		Vector2DX::vget(static_cast<float>(-hitSizeX / 2), static_cast<float>(-hitSizeY / 2)),
		Vector2DX::vget(static_cast<float>(hitSizeX / 2), static_cast<float>(hitSizeY / 2))
	);
}

void OllCharacterData::PlayerDraw()
{

	DrawExtendGraph(static_cast<int>(PlayerPos.x) + drawOffsetX, static_cast<int>(PlayerPos.y) + drawOffsetY, PlayerPos.x + drawOffsetX + 110, PlayerPos.y + drawOffsetY + 110, charaimg[animDirectionOffset + animNowIndex], TRUE);

	//�ړ�����ɐ��m�ȃL�����N�^�[�`��
	Vector2DX PlayerMinSize = Vector2DX::vget(static_cast<float>(-hitSizeX / 2), static_cast<float>(-hitSizeY / 2));
	Vector2DX PlayerMaxSize = Vector2DX::vget(static_cast<float>(hitSizeX / 2), static_cast<float>(hitSizeY / 2));
	Vector2DX PlayerMinPos = PlayerPos + PlayerMinSize;
	Vector2DX PlayerMaxPos = PlayerPos + PlayerMaxSize;
	DrawExtendGraph(
		static_cast<int>(PlayerMinPos.x), static_cast<int>(PlayerMinPos.y),
		static_cast<int>(PlayerMaxPos.x), static_cast<int>(PlayerMaxPos.y),
		charaimg[animDirectionOffset + PlayerVelocity], TRUE);
	DrawBox(static_cast<int>(PlayerMinPos.x), static_cast<int>(PlayerMinPos.y),
		static_cast<int>(PlayerMaxPos.x), static_cast<int>(PlayerMaxPos.y),
		GetColor(255, 0, 0), FALSE);
}

void OllCharacterData::PlayerFinalize()
{
	if (charaimg[0] != -1)
	{
		for (int i = 0; i < animPatternNum * animTypeNum; i++)
		{
			DeleteGraph(charaimg[i]);
		}
	}
}
