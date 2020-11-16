#include "Initialize_component.h"

int brocks_pos[][BLOCK_TYPE_NUM] =
{
	{10, 220},
	{10, 135},
	{10, 60}
};

void menu()
{
	int Background = LoadGraph("Resources\\Background\\title.png", true);

	while (true)
	{
		//�L�[���擾
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		GetHitKeyStateAll(keys);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//�}�E�X���W/��Ԏ擾
		oldMouse = Mouse;
		Mouse = GetMouseInput();
		GetMousePoint(&Mouse_X, &Mouse_Y);

		//��ʃN���A
		ClearDrawScreen();

		DrawGraph(0, 0, Background, true);
		if (click) {
			return;
		}

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
	}
}

//void Character_Move(int pos[]);
void Start()
{
	//Load World
	World_inport();

	//background_images
	int BG_X[2] = { INIT_NUM,WIN_WIDTH };
	int BG = LoadGraph("Resources\\Background\\background.png");

	//object_images
	int defoliation_brock[DEFOLIATION_NUM];
	int jump_brock[JUMP_NUM];
	int nomal_block[NOMAL_NUM];
	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);

	//UI_images
	int UI = LoadGraph("Resources\\UI\\bar.png");
	int scrollArrow = LoadGraph("Resources\\UI\\scroll.png");//z

	int blocks_tmp[BLOCK_TYPE_NUM][BLOCK_TYPE_NUM];
	for (int i = INIT_NUM; i < BLOCK_TYPE_NUM; i++) {
		blocks_tmp[i][BLOCK_X] = brocks_pos[i][BLOCK_X];
		blocks_tmp[i][BLOCK_Y] = brocks_pos[i][BLOCK_Y];
	}

	int arrowWidth = 32;//z
	int arrowHeight = 64;//z

	while (true) {
		//�L�[���擾
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		GetHitKeyStateAll(keys);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//�}�E�X���W/��Ԏ擾
		oldMouse = Mouse;
		Mouse = GetMouseInput();
		GetMousePoint(&Mouse_X, &Mouse_Y);

		BG_scroll(BG_X, Mouse_X, Mouse_Y);//z

		//��ʃN���A
		ClearDrawScreen();

		//�u���b�N���m�̓����蔻��
		if (Mouse == MOUSE_INPUT_LEFT && collision_defoliation_mouse() == true) {
			collision_block_otherblock(&Mouse_X, &Mouse_Y, blocks_tmp, 0);
		}
		else if (Mouse == MOUSE_INPUT_LEFT && collision_normal_mouse() == true) {
			collision_block_otherblock(&Mouse_X, &Mouse_Y, blocks_tmp, 1);
		}
		else if (Mouse == MOUSE_INPUT_LEFT && collision_jump_mouse() == true) {
			collision_block_otherblock(&Mouse_X, &Mouse_Y, blocks_tmp, 2);
		}

		//�w�i�`��
		DrawGraph(BG_X[0], 0, BG, TRUE);//z
		DrawGraph(BG_X[1], 0, BG, TRUE);//z

		//�������[���h�f�[�^�`��
		for (auto i = 0; i < world_value; i++) {
			DrawGraph(world[i][BLOCK_X], world[i][BLOCK_Y], nomal_block[INIT_NUM], true); //�ʏ�u���b�N
		}

		//�C���^�[�t�F�[�X�`��
		DrawGraph(0, 0, UI, true);
		DrawGraph(WIN_WIDTH - arrowWidth, WIN_HEIGHT / 2 - arrowHeight / 2, scrollArrow, TRUE);//�O�i//z
		DrawTurnGraph(192, WIN_HEIGHT / 2 - arrowHeight / 2, scrollArrow, TRUE);//���//z

		//�u���b�N�`��
		DrawGraph(brocks_pos[0][BLOCK_X], brocks_pos[0][BLOCK_Y], defoliation_brock[INIT_NUM], true); //�����u���b�N
		DrawGraph(brocks_pos[1][BLOCK_X], brocks_pos[1][BLOCK_Y], nomal_block[INIT_NUM], true); //�ʏ�u���b�N
		DrawGraph(brocks_pos[2][BLOCK_X], brocks_pos[2][BLOCK_Y], jump_brock[INIT_NUM], true); //�W�����v�u���b�N

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }

		if (click) {
			return;
		}
	}
}

void stage()
{
	//���[���h�f�[�^����
	int world_cache[100][2];
	int user_brock_pos[3][2];
	for (auto i = 0; i < 100; i++) {
		for (auto j = 0; j < 2; j++) {
			//���[���h�ǂݍ��ݎ��̍��W�̕���
			world_cache[i][j] = world[i][j];

			//���[�U�[���u�����u���b�N���W�̕���
			if (i < 3) {
				user_brock_pos[i][j] = brocks_pos[i][j];
			}
		}
	}

	//�w�i�摜�ǂݍ���
	int Background = LoadGraph("Resources\\Background\\background.png", true);
	int BG_position[2] = { 0, WIN_WIDTH };

	//�L�����N�^�[�摜/�t���[��/�t���O
	int jump_count = -1;
	int old_block_pos_y = WIN_HEIGHT;
	int old_user_block_pos_y = WIN_HEIGHT;
	short frame = INIT_NUM, frame_cache = INIT_NUM;
	int character_pos_x = 100;
	int character_pos_y = 325;
	int character[CHAR_FRAME];
	int character_jump[8];
	int character_fall[8];
	LoadDivGraph("Resources\\chr\\char1.png", CHAR_FRAME, CHAR_FRAME, 1, 64, 64, character);
	LoadDivGraph("Resources\\chr\\rabbits_jump.png", 8, 8, 1, 64, 64, character_jump);
	LoadDivGraph("Resources\\chr\\rabbits_fall.png", 8, 8, 1, 64, 64, character_fall);

	//object_images
	int defoliation_brock[DEFOLIATION_NUM];
	int jump_brock[JUMP_NUM];
	int nomal_block[NOMAL_NUM];
	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);

	while (true)
	{
		//�L�[���擾
		for (auto i = 0; i < 256; i++) { oldkeys[i] = keys[i]; }
		GetHitKeyStateAll(keys);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//�}�E�X���W/��Ԏ擾
		oldMouse = Mouse;
		Mouse = GetMouseInput();
		GetMousePoint(&Mouse_X, &Mouse_Y);

		//��ʃN���A
		ClearDrawScreen();

		//�w�i�`��
		DrawGraph(BG_position[0], 0, Background, true);
		DrawGraph(BG_position[1], 0, Background, true);
		
		//�������[���h�f�[�^�`��
		for (auto i = 0; i < world_value; i++) {
			DrawGraph(world_cache[i][BLOCK_X], world_cache[i][BLOCK_Y], nomal_block[INIT_NUM], true); //�ʏ�u���b�N
		}

		//���[�U�[���u�����u���b�N�̕`��
		DrawGraph(user_brock_pos[0][BLOCK_X], user_brock_pos[0][BLOCK_Y], defoliation_brock[INIT_NUM], true); //�����u���b�N
		DrawGraph(user_brock_pos[1][BLOCK_X], user_brock_pos[1][BLOCK_Y], nomal_block[INIT_NUM], true); //�ʏ�u���b�N
		DrawGraph(user_brock_pos[2][BLOCK_X], user_brock_pos[2][BLOCK_Y], jump_brock[INIT_NUM], true); //�W�����v�u���b�N

		//�L�����N�^�[�A�j���[�V����
		DrawGraph(character_pos_x, character_pos_y, character[frame], true);
		if (frame_cache == 3) {
			frame++;
			frame_cache = 0;
		}
		if (frame == CHAR_FRAME) {
			frame = 0;
		}
		frame_cache++;

		//�L�����N�^�[�W�����v����
		for (auto i = 0; i < world_value; i++) {
			if (character_pos_x + 64 > world_cache[i][0] - 10 && jump_count != i && world_cache[i][1] < old_block_pos_y) {
				character_pos_y -= 64;
				old_block_pos_y = world_cache[i][1];
				jump_count = i;
				break;
			}
		}

		for (auto i = 0; i < 3; i++) {
			if (character_pos_x + 64 > user_brock_pos[i][0] - 10 && jump_count != i && user_brock_pos[i][1] < old_user_block_pos_y) {

			}
		}

		//�w�i���W�X�V����
		for (auto i = 0; i < 2; i++) {
			if (BG_position[0] == -WIN_WIDTH) {
				BG_position[0] = WIN_WIDTH;
			}
			if (BG_position[1] == -WIN_WIDTH) {
				BG_position[1] = WIN_WIDTH;
			}

			BG_position[0]--;
			BG_position[1]--;

			for (auto brock_x = 0; brock_x < world_value; brock_x++) {
				world_cache[brock_x][0]--;
			}
			for (auto brock_x = 0; brock_x < 3; brock_x++) {
				user_brock_pos[brock_x][0]--;
			}
		}

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1 || keys[KEY_INPUT_ESCAPE]) { return; }
	}
}

#pragma region utility_fanction
//�w�i�̃X�N���[��
void BG_scroll(int *BgX, const int &mouseX, const int &mouseY)//z
{
	bool keyLeftInput = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && oldMouse == !(GetMouseInput() & MOUSE_INPUT_LEFT);
	bool scrollLeftFlag = mouseX > 192 && mouseX < 192 + 32 && mouseY > WIN_HEIGHT / 2 - 32 && mouseY < WIN_HEIGHT / 2 + 32;
	bool scrollRightFlag = mouseX > WIN_WIDTH - 32 && mouseX < WIN_WIDTH &&mouseY > WIN_HEIGHT / 2 - 32 && mouseY < WIN_HEIGHT / 2 + 32;

	int scrollVel = 100;

	if (scrollRightFlag && keyLeftInput)
	{
		BgX[0] -= scrollVel;
		BgX[1] -= scrollVel;
	}
	if (scrollLeftFlag && keyLeftInput)
	{
		BgX[0] += scrollVel;
		BgX[1] += scrollVel;
	}
	for (int i = 0; i < 2; ++i)
	{
		if (BgX[i] < -WIN_WIDTH)
		{
			BgX[i] = WIN_WIDTH - scrollVel;
		}
		if (BgX[i] > WIN_WIDTH)
		{
			BgX[i] = -WIN_WIDTH + scrollVel;
		}
	}
}

//�����t�u���b�N�ƒʏ�u���b�N�̏Փ˔���
bool collision_defoliation_normal() {
	return Collision.box_Fanc(
		static_cast<double>(brocks_pos[0][BLOCK_X]),
		static_cast<double>(brocks_pos[0][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[0][BLOCK_Y]),
		static_cast<double>(brocks_pos[0][BLOCK_Y]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_X]),
		static_cast<double>(brocks_pos[1][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_Y]),
		static_cast<double>(brocks_pos[1][BLOCK_Y]) + BROCKS_SIZE);
}

//�ʏ�u���b�N�ƃW�����v�u���b�N�̏Փ˔���
bool collision_normal_jump() {
	return Collision.box_Fanc(
		static_cast<double>(brocks_pos[1][BLOCK_X]),
		static_cast<double>(brocks_pos[1][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_Y]),
		static_cast<double>(brocks_pos[1][BLOCK_Y]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_X]),
		static_cast<double>(brocks_pos[2][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_Y]),
		static_cast<double>(brocks_pos[2][BLOCK_Y]) + BROCKS_SIZE);
}

//�W�����v�u���b�N�Ɨ����t�u���b�N�̏Փ˔���
bool collision_jump_defoliation() {
	return Collision.box_Fanc(
		static_cast<double>(brocks_pos[0][BLOCK_X]),
		static_cast<double>(brocks_pos[0][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[0][BLOCK_Y]),
		static_cast<double>(brocks_pos[0][BLOCK_Y]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_X]),
		static_cast<double>(brocks_pos[2][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_Y]),
		static_cast<double>(brocks_pos[2][BLOCK_Y]) + BROCKS_SIZE);
}

//�u���b�N���m�̏Փˏ���
int collision_block_otherblock(int *mouse_x, int *mouse_y, const int(*pos_tmp)[BLOCK_TYPE_NUM], int num) {
	if (mouse_x == nullptr || mouse_y == nullptr) {
		return true;
	}

	if (collision_defoliation_normal() == true && (num == 0 || num == 1)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else if (collision_normal_jump() == true && (num == 1 || num == 2)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else if (collision_jump_defoliation() == true && (num == 2 || num == 0)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else {
		brocks_pos[num][BLOCK_X] = *mouse_x - BLOCK_SIZE / 2;
		brocks_pos[num][BLOCK_Y] = *mouse_y - BLOCK_SIZE / 2;
	}

	return false;
}

//�}�E�X�Ɨ����t�u���b�N�̏Փ˔���
bool collision_defoliation_mouse() {
	return Collision.box_Fanc(
		Mouse_X, Mouse_X,
		Mouse_Y, Mouse_Y,
		static_cast<double>(brocks_pos[0][BLOCK_X]),
		static_cast<double>(brocks_pos[0][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[0][BLOCK_Y]),
		static_cast<double>(brocks_pos[0][BLOCK_Y]) + BROCKS_SIZE);
}

//�}�E�X�ƒʏ�u���b�N�̏Փ˔���
bool collision_normal_mouse() {
	return Collision.box_Fanc(
		Mouse_X, Mouse_X,
		Mouse_Y, Mouse_Y,
		static_cast<double>(brocks_pos[1][BLOCK_X]),
		static_cast<double>(brocks_pos[1][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[1][BLOCK_Y]),
		static_cast<double>(brocks_pos[1][BLOCK_Y]) + BROCKS_SIZE);
}

//�}�E�X�ƃW�����v�u���b�N�̏Փ˔���
bool collision_jump_mouse() {
	return Collision.box_Fanc(
		Mouse_X, Mouse_X,
		Mouse_Y, Mouse_Y,
		static_cast<double>(brocks_pos[2][BLOCK_X]),
		static_cast<double>(brocks_pos[2][BLOCK_X]) + BROCKS_SIZE,
		static_cast<double>(brocks_pos[2][BLOCK_Y]),
		static_cast<double>(brocks_pos[2][BLOCK_Y]) + BROCKS_SIZE);
}
#pragma endregion