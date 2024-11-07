#include "DxLib.h"
#include "player.h"
#include "map.h"

const CharacterManager* CharacterManager::s_Singleton = nullptr;
void OllCharacterData::PlayerInit(int displaywidth, int displayhight)
{
	PlayerPos.x = float(displaywidth / 2);
	PlayerPos.y = float(displayhight / 2);
	animNowType;
	animNowIndex = 0;
	animDirectionOffset = 0;

	//initRect(playerHit,hitSizeX,hitSize)
	LoadDivGraph("Data/chara/walk.png", animPatternNum * animTypeNum, animPatternNum, animTypeNum, animSizeX, animSizeY, charaimg);
	drawOffsetX = (hitSizeX - animSizeX);
	drawOffsetY = (hitSizeY - animSizeY);

}

void OllCharacterData::CalcInput(const Input& input)
{
	MyInput = input;

}

void OllCharacterData::Update()
{
	//�ړ��O�̃v���C���[���W��ۑ�
	Vector2DX prevPos = PlayerPos;
	
	PlayerVelocity.x = 0;
	PlayerVelocity.y = 0;

	//A�������ꂽ�Ƃ�
	if (MyInput.IsMoveLeft())
	{	
		PlayerVelocity.x = -1;
	}
	//D�������ꂽ�Ƃ�
	if (MyInput.IsMoveRight())
	{
		PlayerVelocity.x = 1;
	}
	//S�������ꂽ�Ƃ�
	if (MyInput.IsMoveDown())
	{
		PlayerVelocity.y = 1;
	}
	//W�������ꂽ�Ƃ�
	if (MyInput.IsMoveUp())
	{
		PlayerVelocity.y = -1;
	}

	if (PlayerVelocity.magnitude() > 0.f) {
		//�������Ă�
		time += 1.0f / 60.f;						//60FPS�͂P�b�Ԃ����炻���time�Ɏ��Ԃ����Ă���
		//0,1,2
		//3,4,5
		//6,7,8
		//9,10,11
		if (time > 1.0f) {							//0,1,2,3-3=0,1,2,,	1�b�ȏ�ɂȂ�Ȃ��悤�ɂ���
			time -= 1.0f;							//�P�b�ȏ�ɂȂ��1�b�}�C�i�X������
			animNowIndex += 1;						//�P�b�ɂȂ�Ɣz��̗v�f�����P���₵�Ă���
			if (animNowIndex >= animPatternNum)		//animNowIndex���A�j���[�V�����̉��v�f���ɂȂ��
			{
				animNowIndex -= animPatternNum;		//�RanimPatternNum��animIndex�Ƀ}�C�i�X������0�`�ɂ��Ă���
			}
		}

#if FALSE
		//�A�j���[�V�����w��(��1)�@�c���Ή�
		if (PlayerVelocity.x > 0.f && PlayerVelocity.y == 0.f) {
			//�E
			animDirectionOffset = 40;
		}
		if (PlayerVelocity.x == 0.f && PlayerVelocity.y < 0.f) {
			//��
			animDirectionOffset = 24;
		}
		if (PlayerVelocity.x < 0.f && PlayerVelocity.y == 0.f) {
			//��
			animDirectionOffset = 8;
		}
		if (PlayerVelocity.x == 0.f && PlayerVelocity.y > 0.f) {
			//��
			animDirectionOffset = 0;
		}
#else
		//�A�j���[�V�����w��(��2) �p�x�����߂Ă�������
		float radian = std::atan2f(PlayerVelocity.x, PlayerVelocity.y);//-DX_PI_F~DX_PI_F�͈̔͂œ��͊p�x�𓾂�
		float degree = radian * 180.f / DX_PI_F;//�p�x��Degree�ɕϊ�

		//��
		if (-150.f > degree || degree > 150.f) {
			animDirectionOffset = 8 * 3;
		}
		//�E��
		if (90.f <= degree && degree <= 150.f) {
			animDirectionOffset = 8 * 4;
		}
		//�E��
		if (30.f <= degree && degree <= 90.f) {
			animDirectionOffset = 8 * 5;
		}
		//��
		if (-30.f <= degree && degree <= 30.f) {
			animDirectionOffset = 8 * 0;
		}
		//����
		if (-90.f <= degree && degree <= -30.f) {
			animDirectionOffset = 8 * 1;
		}
		//����
		if (-150.f <= degree && degree <= -90.f) {
			animDirectionOffset = 8 * 2;
		}
#endif
	}
	else {
		//�~�܂��Ă���
	}



	PlayerPos += PlayerVelocity;


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

	DrawExtendGraph(static_cast<int>(PlayerPos.x) + drawOffsetX, static_cast<int>(PlayerPos.y) + drawOffsetY, static_cast<int>(PlayerPos.x) + drawOffsetX + 110, static_cast<int>(PlayerPos.y)+ drawOffsetY + 110, charaimg[animDirectionOffset + animNowIndex], TRUE);

	//�ړ�����ɐ��m�ȃL�����N�^�[�`��
	Vector2DX PlayerMinSize = Vector2DX::vget(static_cast<float>(-hitSizeX / 2), static_cast<float>(-hitSizeY / 2));
	Vector2DX PlayerMaxSize = Vector2DX::vget(static_cast<float>(hitSizeX / 2), static_cast<float>(hitSizeY / 2));
	Vector2DX PlayerMinPos = PlayerPos + PlayerMinSize;
	Vector2DX PlayerMaxPos = PlayerPos + PlayerMaxSize;
	DrawExtendGraph(
		static_cast<int>(PlayerMinPos.x), static_cast<int>(PlayerMinPos.y),
		static_cast<int>(PlayerMaxPos.x), static_cast<int>(PlayerMaxPos.y),
		charaimg[animDirectionOffset + animNowIndex], TRUE);
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
