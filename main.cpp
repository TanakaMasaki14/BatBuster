#include "shooting.h"
#include <math.h>

#define JIMEN_Y						(-840.24f)				
#define STAGE_W						(4500.0f)				
#define TITLE_ICON_SIZE				(24)					
#define TITLE_ICON_X_NUM			(26)					
#define TITLE_ICON_Y_NUM			(20)					
#define CAMERA_X					(2668.0f)
#define CAMERA_Y					(294.0f)
#define CAMERA_Z					(0.0f)
#define CAMERA_SPEED				(16.0f)
#define ITEM_INIT_X					PC_INIT_X			
#define ITEM_INIT_Y					1200.0f				
#define ITEM_GRAVITY_Y				(1.5f)				
#define ITEM_BOUND_Y				(0.2f)				
#define ITEM_SIZE					(128.0f)			
#define ITEM_POP_W					(STAGE_W - 250.0f)	
#define ITEM_SHOTPOWERMAX_SCORE		(5000)				
#define ITEM_HPMAX_HP20_SCORE		(2000)				
#define ITEM_HPMAX_HP50_SCORE		(5000)				
#define ITEM_SUBWEAPONMAX_SCORE		(8000)				
#define EFFECT_HIT_DISPTIME			(15)				
#define EFFECT_HIT_FADESPEED		(256 / 15)			
#define EFFECT_HIT_MOVESPEED		(20.0f)				
#define EFFECT_HIT_POLYNUM			(8)					
#define EFFECT_HIT_R				(40.0f)				
#define EFFECT_HIT_LENGTH			(40.0f)				
#define TEKI_ANIM_SPEED				(1.0f)			
#define TEKI_SPEED					(16.0f)				
#define TEKI_CREATE_WAIT			(100)				
#define TEKI_INIT_X					(-4324.0f)			
#define TEKI_INIT_Y1				(-272.0f)			
#define TEKI_INIT_Y2				(-586.0f)			
#define TEKI_INIT_Z					(0.0f)				
#define TEKI_FADE_IN_SPEED			(1.0f / 60.0f)		
#define TEKI_POP_X1					(-2368.0f)			
#define TEKI_POP_Z1					(-3326.0f)			
#define TEKI_POP_X2					(-680.0f)			
#define TEKI_POP_Z2					(3326.0f)			
#define TEKI_YANGLE_SPEED			(DX_PI_F / 60.0f)	
#define TEKI_YANGLE_HEIGHT			(120.0f)			
#define TEKI_LR_SPEED				(25.0f)			
#define TEKI_SHOT_CLIP_X1			(-6200)			
#define TEKI_SHOT_CLIP_X2			(2000)				
#define TEKI_SHOT_CLIP_Y1			(-1000)				
#define TEKI_SHOT_CLIP_Y2			(1000)			
#define TEKI_SHOT_CLIP_Z1			(-5000)				
#define TEKI_SHOT_CLIP_Z2			(5000)				
#define TEKI_SHOT_SPEED_1			(25.0f)				
#define TEKI_SHOT_SPEED_2			(40.0f)				
#define TEKI_SHOT_SPEED_3			(12.5f)				
#define TEKI_HIT_SIZE				(200.0f)			
#define TEKI_SHOT_SCALE_INIT		(0.1f)			
#define TEKI_SHOT_SCALESPEED		(1.0f)				
#define TEKI_SHOT_SCALE_HIT_SIZEX	(32.0f)				
#define TEKI_BOM_SIZE				(64.0f)				
#define TEKI_BOM_SCALESPEED			(16.0f)				
#define TEKI_BOM_DELETEWAIT			(30)			
#define TEKI_BOM_FADEWAIT			(20)				
#define TEKI_BOM_ANGLE_RANGE		(1.0f)				
#define TEKI_BREAK_SIZE				(128.0f)			
#define TEKI_BREAK_ANGLE_RANGE		(2.0f)				
#define TEKI_BREAK_FADETIME			(20)				
#define PC_INIT_X					(968.0f)			
#define PC_INIT_Y					(-736.0f)			
#define PC_INIT_Z					(0.0f)				
#define PC_SPEED					(24.0f)				
#define PC_JUMP_POWER				(30.0f)				
#define PC_GRAVITY					(1.5f)				
#define PC_LIFEALPHA_SPEED			(16)				
#define PC_HIT_SIZE					(150.0f)			
#define PC_OPACITY_SPEED			(1.0f / 120.0f)		
#define PC_END_TIME					(60)				
#define PC_HP_MAX					(100)				
#define PC_SHOTPOWER_MAX			(5)					
#define PC_SUBWEAPONPOWER_MAX		(50)			
#define PC_SHOT_SPEED				(64.0f)				
#define PC_SHOT_KABE_X				(-4200.0f)			
#define PC_SHOT_GRAVITY				(0.2f)				
#define PC_SHOT_UPPOWER				(7.0f)				

TITLE        Title;
ITEMCOMMON   ItemCommon;
EFFECTCOMMON EffectCommon;
TEKICOMMON   TekiCommon;
PC           Pc;
GAMEDATA     GameData;
VECTOR Pos, Pos2;

extern int ToolHLSToRGB(float H, float L, float S, float* dr, float* dg, float* db)
{
	float r, g, b;

	H = fmod(H, 1.0f);
	if (H >= 0.0f && H <= 1.0f / 6.0f)
	{
		H = H * 6.0f;				r = 1.0f;		g = H;			b = 0.0f;
	}
	else if (H > 1.0f / 6.0f && H <= 2.0f / 6.0f)
	{
		H = H * 6.0f - 1.0f;		r = 1.0f - H;	g = 1.0f;		b = 0.0f;
	}
	else if (H > 2.0f / 6.0f && H <= 3.0f / 6.0f)
	{
		H = H * 6.0f - 2.0f;		r = 0.0f;		g = 1.0f;		b = H;
	}
	else if (H > 3.0f / 6.0f && H <= 4.0f / 6.0f)
	{
		H = H * 6.0f - 3.0f;		r = 0.0f;		g = 1.0f - H;	b = 1.0f;
	}
	else if (H > 4.0f / 6.0f && H <= 5.0f / 6.0f)
	{
		H = H * 6.0f - 4.0f;		r = H;			g = 0.0f;		b = 1.0f;
	}
	else if (H > 5.0f / 6.0f && H <= 6.0f / 6.0f)
	{
		H = H * 6.0f - 5.0f;		r = 1.0f;		g = 0.0f;		b = 1.0f - H;
	}
	{
		r += (0.5f - r) * (1.0f - S);
		g += (0.5f - g) * (1.0f - S);
		b += (0.5f - b) * (1.0f - S);

		if (L <= 0.5f)
		{
			r *= L * 2.0f;
			g *= L * 2.0f;
			b *= L * 2.0f;
		}
		else
		{
			r += (1.0f - r) * ((L - 0.5f) * 2.0f);
			g += (1.0f - g) * ((L - 0.5f) * 2.0f);
			b += (1.0f - b) * ((L - 0.5f) * 2.0f);
		}
	}
	if (dr) *dr = r;
	if (dg) *dg = g;
	if (db) *db = b;

	return 0;
}

extern void ScoreUp(int Score)
{
	GameData.Score += Score;
	if (GameData.HiScore < GameData.Score)
		GameData.HiScore = GameData.Score;
}

extern void ScoreUp_TekiBreak(int Score)
{
	GameData.Score += Score + GameData.ScoreRate * Score / 1000;
	if (GameData.HiScore < GameData.Score)
		GameData.HiScore = GameData.Score;
	GameData.ScoreRate += 1;
}

extern void EnemyBreakPointUp(int BreakPoint)
{
	GameData.EnemyBreakPoint += BreakPoint;
}

extern void ItemInitialize(void)
{
	ITEM* Item;
	int i;
	ItemCommon.DataNum = 0;
	Item = ItemCommon.Data;
	for (i = 0; i < ITEM_MAX_NUM; i++, Item++)
	{
		Item->UseFlag = 0;
	}
}

extern ITEM* ItemAdd(int Type)
{
	ITEM* Item;
	if (ItemCommon.DataNum == ITEM_MAX_NUM) return NULL;
	for (Item = ItemCommon.Data; Item->UseFlag; Item++) {}
	Item->UseFlag = TRUE;
	Item->Type = Type;
	Item->Position.x = ITEM_INIT_X;
	Item->Position.y = ITEM_INIT_Y;
	Item->Position.z = GetRand(65536) * ITEM_POP_W * 2 / 65536.0f - ITEM_POP_W;
	Item->JumpPowerY = 0.0f;

	ItemCommon.DataNum++;
	return Item;
}

extern void ItemMove(void)
{
	int i;
	ITEM* Item;

	Item = ItemCommon.Data;
	for (i = 0; i < ITEM_MAX_NUM; i++, Item++)
	{
		if (Item->UseFlag == FALSE) continue;

		Item->JumpPowerY -= ITEM_GRAVITY_Y;

		Item->Position.y += Item->JumpPowerY;
		if (Item->Position.y < JIMEN_Y + ITEM_SIZE / 2.0f)
		{
			Item->Position.y = JIMEN_Y + ITEM_SIZE / 2.0f;
			Item->JumpPowerY *= -ITEM_BOUND_Y;
		}

		if (VSquareSize(VSub(Item->Position, Pc.Position)) < PC_HIT_SIZE * PC_HIT_SIZE)
		{
			switch (Item->Type)
			{
			case ITEM_TYPE_POWER_UP:
				PcHpUp(20);
				if (Pc.ShotPower == PC_SHOTPOWER_MAX)
				{
					ScoreUp(ITEM_SHOTPOWERMAX_SCORE);
				}
				else
				{
					PcShotPowerUp(1);
				}
				break;

			case ITEM_TYPE_HP_UP_20:
				if (Pc.Hp == PC_HP_MAX)
				{
					ScoreUp(ITEM_HPMAX_HP20_SCORE);
				}
				else
				{
					PcHpUp(20);
				}
				break;

			case ITEM_TYPE_HP_UP_50:
				if (Pc.Hp == PC_HP_MAX)
				{
					ScoreUp(ITEM_HPMAX_HP50_SCORE);
				}
				else
				{
					PcHpUp(50);
				}
				break;

			case ITEM_TYPE_SUB_WEAPON:
				if (Pc.Hp == PC_HP_MAX)
				{
					ScoreUp(ITEM_SUBWEAPONMAX_SCORE);
				}
				else
				{
					PcSubWeaponPowerUp(PC_SUBWEAPONPOWER_MAX);
				}
				break;
			}

			Item->UseFlag = FALSE;
			ItemCommon.DataNum--;
		}
	}
}

extern void ItemDraw(void)
{
	int i;
	ITEM* Item;

	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(TRUE);

	Item = ItemCommon.Data;
	for (i = 0; i < ITEM_MAX_NUM; i++, Item++)
	{
		if (Item->UseFlag == FALSE) continue;

		DrawShadow(VGet(Item->Position.x, JIMEN_Y + 3.0f, Item->Position.z), ITEM_SIZE * 0.8f, (int)(255.0f - 255.0f * (Item->Position.y - JIMEN_Y) / (float)(ITEM_INIT_Y - JIMEN_Y)));
	}

	Item = ItemCommon.Data;
	for (i = 0; i < ITEM_MAX_NUM; i++, Item++)
	{
		if (Item->UseFlag == FALSE) continue;

		DrawExtendGraph3D(Item->Position.x, Item->Position.y, Item->Position.z, ITEM_SIZE / 64.0f, ITEM_SIZE / 64.0f, ItemCommon.GraphHandle[Item->Type], TRUE);
	}

	SetUseZBufferFlag(FALSE);
	SetWriteZBufferFlag(FALSE);
}

extern void	EffectInitialize(void)
{
	int i;
	EFFECT* Data;
	EffectCommon.DataNum = 0;
	Data = EffectCommon.Data;
	for (i = 0; i < EFFECT_MAX_NUM; i++, Data++)
	{
		Data->UseFlag = 0;
	}
}

extern EFFECT* EffectAdd(int Type, VECTOR Pos)
{
	int i;
	EFFECT* Data;

	if (EffectCommon.DataNum == EFFECT_MAX_NUM) return NULL;
	for (i = 0; EffectCommon.Data[i].UseFlag; i++) {}
	Data = &EffectCommon.Data[i];

	Data->UseFlag = TRUE;
	Data->Type = Type;
	Data->Position = Pos;
	Data->Size = 0.0f;
	Data->State = 0;
	Data->Counter = 0;

	return Data;
}

extern void EffectMove(void)
{
	int i;
	EFFECT* Data;

	Data = EffectCommon.Data;
	for (i = 0; i < EFFECT_MAX_NUM; i++, Data++)
	{
		if (Data->UseFlag == 0) continue;

		switch (Data->Type)
		{
		case 0:
			Data->Move += EFFECT_HIT_MOVESPEED;
			switch (Data->State)
			{
			case 0:
				Data->Counter++;
				if (Data->Counter < EFFECT_HIT_DISPTIME)
					break;
				Data->Counter = 0;
				Data->State += 10;
				break;

			case 10:
				Data->Counter += EFFECT_HIT_FADESPEED;
				if (Data->Counter < 255)
					break;
				Data->UseFlag = 0;
				EffectCommon.DataNum--;
				break;
			}
			break;
		}
	}
}

extern void EffectDraw(void)
{
	int i, j, Alpha;
	EFFECT* Data;
	VERTEX_3D Vertex[EFFECT_HIT_POLYNUM * 3], * vert;

	SetUseZBufferFlag(TRUE);
	SetWriteZBufferFlag(FALSE);

	Data = EffectCommon.Data;
	for (i = 0; i < EFFECT_MAX_NUM; i++, Data++)
	{
		if (Data->UseFlag == 0) continue;

		switch (Data->Type)
		{
		case 0:
		{
			VECTOR BaseVector[3], Translate;
			int ColorR, ColorG, ColorB;

			switch (Data->State)
			{
			case 0:
				ColorR = (255 - Data->ColorR) * (EFFECT_HIT_DISPTIME - Data->Counter) / EFFECT_HIT_DISPTIME + Data->ColorR;
				ColorG = (255 - Data->ColorG) * (EFFECT_HIT_DISPTIME - Data->Counter) / EFFECT_HIT_DISPTIME + Data->ColorG;
				ColorB = (255 - Data->ColorB) * (EFFECT_HIT_DISPTIME - Data->Counter) / EFFECT_HIT_DISPTIME + Data->ColorB;
				Alpha = 255;
				break;

			case 10:
				ColorR = Data->ColorR;
				ColorG = Data->ColorG;
				ColorB = Data->ColorB;
				Alpha = 255 - Data->Counter;
				break;
			}

			vert = Vertex;
			for (j = 0; j < EFFECT_HIT_POLYNUM; j++, vert += 3)
			{
				BaseVector[0].z = cos(DX_TWO_PI_F * j / EFFECT_HIT_POLYNUM) * EFFECT_HIT_R;
				BaseVector[0].y = sin(DX_TWO_PI_F * j / EFFECT_HIT_POLYNUM) * EFFECT_HIT_R;
				BaseVector[0].x = 0.0f;

				BaseVector[1].z = cos(DX_TWO_PI_F * (j + 1) / EFFECT_HIT_POLYNUM) * EFFECT_HIT_R;
				BaseVector[1].y = sin(DX_TWO_PI_F * (j + 1) / EFFECT_HIT_POLYNUM) * EFFECT_HIT_R;
				BaseVector[1].x = 0.0f;

				BaseVector[2].z = cos(DX_TWO_PI_F * (j + 0.5f) / EFFECT_HIT_POLYNUM) * (EFFECT_HIT_R + EFFECT_HIT_LENGTH);
				BaseVector[2].y = sin(DX_TWO_PI_F * (j + 0.5f) / EFFECT_HIT_POLYNUM) * (EFFECT_HIT_R + EFFECT_HIT_LENGTH);
				BaseVector[2].x = 0.0f;

				Translate.z = cos(DX_TWO_PI_F * (j + 0.5f) / EFFECT_HIT_POLYNUM) * Data->Move + Data->Position.z;
				Translate.y = sin(DX_TWO_PI_F * (j + 0.5f) / EFFECT_HIT_POLYNUM) * Data->Move + Data->Position.y;
				Translate.x = Data->Position.x;

				vert[0].pos = VAdd(BaseVector[0], Translate);
				vert[1].pos = VAdd(BaseVector[1], Translate);
				vert[2].pos = VAdd(BaseVector[2], Translate);

				vert[0].r = ColorR;
				vert[0].g = ColorG;
				vert[0].b = ColorB;
				vert[0].a = Alpha;
				vert[0].u = 0.0f;
				vert[0].v = 0.0f;

				vert[1].r = ColorR;
				vert[1].g = ColorG;
				vert[1].b = ColorB;
				vert[1].a = Alpha;
				vert[1].u = 0.0f;
				vert[1].v = 0.0f;

				vert[2].r = ColorR;
				vert[2].g = ColorG;
				vert[2].b = ColorB;
				vert[2].a = Alpha;
				vert[2].u = 0.0f;
				vert[2].v = 0.0f;
			}

			SetDrawBlendMode(DX_BLENDMODE_ADD, Alpha);
			DrawPolygon3D(Vertex, EFFECT_HIT_POLYNUM, DX_NONE_GRAPH, FALSE);
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
		break;
		}
	}
	SetUseZBufferFlag(FALSE);
}

int TekiGlobalInitialize(void)
{
	int i;
	TEKI* Teki;

	SetTransColor(0, 0, 0);
	TekiCommon.ShotGraphHandle = LoadGraph("Data\\TekiTama.bmp");
	SetTransColor(255, 0, 255);
	TekiCommon.Shot2GraphHandle = LoadGraph("Data\\TekiTama2.tga");
	SetTransColor(255, 0, 255);
	TekiCommon.ModelHandle = MV1LoadModel("Data\\Teki.x");

	Teki = TekiCommon.Char;
	for (i = 0; i < TEKI_MAX_NUM; i++, Teki++)
	{
		Teki->ModelHandle = MV1DuplicateModel(TekiCommon.ModelHandle);
		Teki->AttachAnimIndex = MV1AttachAnim(Teki->ModelHandle, 0);
	}
	TekiCommon.CharCreateCounter = 0;

	return 0;
}

void TekiGlobalTerminate(void)
{
	int i;

	DeleteGraph(TekiCommon.ShotGraphHandle);

	MV1DeleteModel(TekiCommon.ModelHandle);

	for (i = 0; i < TEKI_MAX_NUM; i++)
	{
		MV1DeleteModel(TekiCommon.Char[i].ModelHandle);
	}
}
void TekiInitialize(void)
{
	int i;

	TekiCommon.CharCreateState = 100;
	TekiCommon.CharCreateCounter = 120;
	TekiCommon.ShotNum = 0;

	for (i = 0; i < TEKI_MAX_NUM; i++)
		TekiCommon.Shot[i].UseFlag = 0;

	TekiCommon.CharNum = 0;

	for (i = 0; i < TEKI_MAX_NUM; i++)
		TekiCommon.Char[i].UseFlag = 0;
}

TEKI* TekiAdd(int Type, VECTOR Pos, int Hp)
{
	int i;
	TEKI* Teki;

	if (Pos.z < -STAGE_W + 250.0f || Pos.z > STAGE_W - 250.0f) return NULL;

	if (TekiCommon.CharNum == TEKI_MAX_NUM) return NULL;
	for (i = 0; TekiCommon.Char[i].UseFlag; i++) {}
	Teki = &TekiCommon.Char[i];

	Teki->UseFlag = TRUE;
	Teki->HitFlag = TRUE;
	Teki->Type = Type;
	Teki->AnimCounter = 0.0f;
	Teki->Opacity = 0.0f;
	Teki->State = 0;
	Teki->Counter = 0;
	Teki->MoveState = 0;
	Teki->MoveCounter = 0;
	Teki->Damage = 0;
	Teki->Hp = Hp;
	Teki->Fade = TRUE;
	Teki->Position = Pos;
	Teki->YAngle = GetRand(32768) * DX_TWO_PI_F / 32768;
	TekiCommon.CharNum++;

	return Teki;
}

void TekiMove(void)
{
	int i, num;
	TEKI* Teki;
	TEKISHOT* Shot;

#if 1
	switch (TekiCommon.CharCreateState)
	{
	case 0:
		TekiCommon.CharCreateState = 100;
		TekiCommon.CharCreateCounter = GetRand(60 * 10 - ((GameData.Level % 10) * 50)) + 60 * 3 - GameData.Level;
		break;

	case 100:
		TekiCommon.CharCreateCounter--;
		if (TekiCommon.CharCreateCounter < 0 || TekiCommon.CharNum == 0 || GameData.EdgeInput & PAD_INPUT_DOWN)
		{
			if (GameData.EdgeInput & PAD_INPUT_DOWN)
			{
				GameData.ScoreRate += 50;
			}

			TekiCommon.CharCreateState = GetRand(6) * 100 + 200;
			TekiCommon.CharCreateCounter = 0;

			TekiCommon.CharCreateRand = GetRand(65536);

			TekiCommon.CharCreatePosition.x = (float)GetRand((int)(TEKI_POP_X2 - TEKI_POP_X1)) + TEKI_POP_X1;
			TekiCommon.CharCreatePosition.y = TEKI_INIT_Y2;
			TekiCommon.CharCreatePosition.z = (float)GetRand((int)(TEKI_POP_Z2 - TEKI_POP_Z1)) + TEKI_POP_Z1;
		}
		break;

	case 200:
		num = 3 + GetRand(GameData.Level / 10 + 1);
		for (i = 0; i < num; i++)
		{
			Teki = TekiAdd(TEKI_TYPE_3WAYSHOT, VGet(TekiCommon.CharCreatePosition.x, TekiCommon.CharCreatePosition.y, TekiCommon.CharCreatePosition.z - num * 500.0f / 2.0f + i * 500.0f), 50);
			if (Teki)
			{
				Teki->MoveState = TekiCommon.CharCreateRand < 32768 ? 0 : 10;
			}
		}
		TekiCommon.CharCreateState = 0;
		break;

	case 300:
		num = 2 + GetRand(GameData.Level / 10 + 2);
		for (i = 0; i < num; i++)
		{
			Teki = TekiAdd(TEKI_TYPE_BOMSHOT, VGet(TekiCommon.CharCreatePosition.x, TekiCommon.CharCreatePosition.y, TekiCommon.CharCreatePosition.z - num * 500.0f / 2.0f + i * 500.0f), 200);
			if (Teki)
			{
			}
		}
		TekiCommon.CharCreateState = 0;
		break;

	case 400:
	{
		VECTOR Center;
		static int InitPos[][2] =
		{
			0,0,
			-1,-1, 1,-1, -1,1, 1,1,
			-2,0, 2,0, 0,-2, 0,2,
			-3,-3, -1,-3, 1,-3, 3,-3, -3,-1, -3,1, 3,-1, 3,1, -3,3, -1,3, 1,3, 3,3
		};

		num = 5 + GetRand(GameData.Level / 10 + 1);
		if (num > sizeof(InitPos) / (sizeof(int) * 2))
			num = sizeof(InitPos) / (sizeof(int) * 2);
		Center = TekiCommon.CharCreatePosition;
		for (i = 0; i < num; i++)
		{
			Teki = TekiAdd(
				TEKI_TYPE_SIMPLESHOT
				, VGet(InitPos[i][0] * 400.0f + Center.x, TEKI_INIT_Y2, InitPos[i][1] * 400.0f + Center.z), 50);
			if (Teki)
			{
				Teki->Speed = TEKI_SPEED;
			}
		}

		TekiCommon.CharCreateState = 0;
	}
	break;

	case 500:
		Teki = TekiAdd(TEKI_TYPE_ITEM, TekiCommon.CharCreatePosition, 300);
		TekiCommon.CharCreateState = 0;
		break;

	case 600:
		num = 1 + GetRand(GameData.Level / 20 + 2);
		for (i = 0; i < num; i++)
		{
			Teki = TekiAdd(TEKI_TYPE_BREAKSHOT, VGet(TekiCommon.CharCreatePosition.x, TekiCommon.CharCreatePosition.y, TekiCommon.CharCreatePosition.z - num * 1000.0f / 2.0f + i * 1000.0f), 200);
		}
		TekiCommon.CharCreateState = 0;
		break;

	case 700:
		num = 1 + GetRand(GameData.Level / 10 + 1);
		for (i = 0; i < num; i++)
		{
			Teki = TekiAdd(TEKI_TYPE_SPEEDSHOT, VGet(TekiCommon.CharCreatePosition.x, TekiCommon.CharCreatePosition.y, TekiCommon.CharCreatePosition.z - num * 1000.0f / 2.0f + i * 1000.0f), 50);
		}
		TekiCommon.CharCreateState = 0;
		break;

	case 800:
		num = 1 + GetRand(GameData.Level / 15 + 2);
		for (i = 0; i < num; i++)
		{
			Teki = TekiAdd(TEKI_TYPE_2WAYHEIGHTSHOT, VGet(TekiCommon.CharCreatePosition.x, TekiCommon.CharCreatePosition.y, TekiCommon.CharCreatePosition.z - num * 700.0f / 2.0f + i * 1000.0f), 150);
		}
		TekiCommon.CharCreateState = 0;
		break;
	}
#else
	TekiCommon.CharCreateCounter++;
	if (TekiCommon.CharCreateCounter == TEKI_CREATE_WAIT)
	{
		int Rand;
		VECTOR Center;

		TekiCommon.CharCreateCounter = 0;

		Rand = GetRand(0);
		switch (Rand)
		{
		case 0:
		{
			static int InitPos[][2] =
			{
				0,0, -1,-1, 1,-1, -1,1, 1,1, -2,0, 2,0, 0,-2, 0,2
			};

			Center.x = (float)GetRand((int)(TEKI_POP_X2 - TEKI_POP_X1)) + TEKI_POP_X1;
			Center.y = TEKI_INIT_Y2;
			Center.z = (float)GetRand((int)(TEKI_POP_Z2 - TEKI_POP_Z1)) + TEKI_POP_Z1;
			for (i = 0; i < 1; i++)
			{
				Teki = TekiAdd(
					TEKI_TYPE_2WAYHEIGHTSHOT

					, VGet(InitPos[i][0] * 400.0f + Center.x, TEKI_INIT_Y2, InitPos[i][1] * 400.0f + Center.z), 100);
				if (Teki)
				{
					Teki->Speed = TEKI_SPEED;
				}
			}
		}
		break;
		}
	}
#endif
	Teki = TekiCommon.Char;
	for (i = 0; i < TEKI_MAX_NUM; i++, Teki++)
	{
		if (Teki->UseFlag == 0) continue;
		Teki->AnimCounter += TEKI_ANIM_SPEED;
		if (Teki->AnimCounter > MV1GetAttachAnimTotalTime(Teki->ModelHandle, Teki->AttachAnimIndex))
			Teki->AnimCounter -= MV1GetAttachAnimTotalTime(Teki->ModelHandle, Teki->AttachAnimIndex);
		MV1SetAttachAnimTime(Teki->ModelHandle, Teki->AttachAnimIndex, Teki->AnimCounter);
		Teki->YAngle += TEKI_YANGLE_SPEED;
		if (Teki->YAngle > DX_TWO_PI_F)
			Teki->YAngle -= DX_TWO_PI_F;

		Teki->Hp -= Teki->Damage;
		Teki->Damage = 0;
		if (Teki->Hp < 0 && Teki->State >= 0)
		{
			Teki->State = -100;
			Teki->Counter = 0;
			Teki->HitFlag = FALSE;
			Teki->Fade = FALSE;
		}

		switch (Teki->Type)
		{
		case TEKI_TYPE_SIMPLESHOT:
			switch (Teki->MoveState)
			{
			case 0:
				Teki->MoveCounter++;
				if (Teki->MoveCounter > 60)
				{
					Teki->MoveCounter = 0;
					Teki->MoveState = Pc.Position.z < Teki->Position.z ? 10 : 20;
				}
				break;

			case 10:
				Teki->Position.z -= 10.0f;

				Teki->MoveCounter++;
				if (Teki->MoveCounter > 60)
				{
					Teki->MoveCounter = 0;
					Teki->MoveState = 0;
				}
				break;

			case 20:
				Teki->Position.z += 10.0f;

				Teki->MoveCounter++;
				if (Teki->MoveCounter > 60)
				{
					Teki->MoveCounter = 0;
					Teki->MoveState = 0;
				}
				break;
			}

			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->Counter = 0;
				Teki->State = 0;
				Shot = TekiShotAdd(TEKI_SHOT_TYPE_NORMAL, MV1GetPosition(Teki->ModelHandle));
				if (Shot)
				{
					Shot->Speed = VScale(VNorm(VSub(VGet(Pc.Position.x, PC_INIT_Y, Teki->Position.z), MV1GetPosition(Teki->ModelHandle))), TEKI_SHOT_SPEED_1);
				}
				break;

			case -100:
				ScoreUp_TekiBreak(100);
				EnemyBreakPointUp(100);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;

		case TEKI_TYPE_ITEM:
			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->Counter = 0;
				Teki->State += 10;
				break;

			case 20:
				if (Teki->Counter % 10 == 0)
				{
					Shot = TekiShotAdd(TEKI_SHOT_TYPE_NORMAL, MV1GetPosition(Teki->ModelHandle));
					if (Shot)
					{
						float XLength, Sin, Cos;

						Cos = (float)cos(Teki->Counter * DX_TWO_PI_F / 80);
						Sin = (float)sin(Teki->Counter * DX_TWO_PI_F / 80);
						XLength = PC_INIT_X - Teki->Position.x;
						Shot->Speed = VScale(VGet(Cos, 0.0f, Sin), TEKI_SHOT_SPEED_1);
						Shot->Speed.y = (PC_INIT_Y - MV1GetPosition(Teki->ModelHandle).y) / (XLength / (Cos * TEKI_SHOT_SPEED_1));
					}
				}
				if (Teki->Counter != 70)
				{
					Teki->Counter++;
					break;
				}
				Teki->State = 0;
				Teki->Counter = 0;
				Teki->Value = 0;
				break;

			case -100:
				ItemAdd(0);
				ScoreUp_TekiBreak(500);
				EnemyBreakPointUp(200);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;

		case TEKI_TYPE_2WAYHEIGHTSHOT:
			if (Teki->Position.z < Pc.Position.z)
			{
				Teki->Position.z += 2.0f;
				if (Teki->Position.z > Pc.Position.z)
				{
					Teki->Position.z = Pc.Position.z;
				}
			}
			else
			{
				Teki->Position.z -= 2.0f;
				if (Teki->Position.z < Pc.Position.z)
				{
					Teki->Position.z = Pc.Position.z;
				}
			}

			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->Counter = 0;
				Teki->State = 0;
				Shot = TekiShotAdd(TEKI_SHOT_TYPE_SCALE, MV1GetPosition(Teki->ModelHandle));
				if (Shot)
				{
					Shot->Speed = VScale(VNorm(VSub(VGet(Pc.Position.x, PC_INIT_Y, Pc.Position.z - 1024.0f), MV1GetPosition(Teki->ModelHandle))), TEKI_SHOT_SPEED_1);
				}
				Shot = TekiShotAdd(TEKI_SHOT_TYPE_SCALE, MV1GetPosition(Teki->ModelHandle));
				if (Shot)
				{
					Shot->Speed = VScale(VNorm(VSub(VGet(Pc.Position.x, PC_INIT_Y, Pc.Position.z + 1024.0f), MV1GetPosition(Teki->ModelHandle))), TEKI_SHOT_SPEED_1);
				}
				break;

			case -100:
				ScoreUp_TekiBreak(200);
				EnemyBreakPointUp(150);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;

		case TEKI_TYPE_3WAYSHOT:
			Teki->MoveCounter++;
			if (Teki->MoveCounter == 240)
			{
				Teki->MoveCounter = 0;
			}
			Teki->Position.x += (float)sin(Teki->MoveCounter * DX_TWO_PI / 240.0f) * 5.0f;

			switch (Teki->MoveState)
			{
			case 0:
				Teki->Position.z += 7.5f;
				if (Teki->Position.z > STAGE_W - 250.0f)
				{
					Teki->MoveState = 10;
				}
				break;

			case 10:
				Teki->Position.z -= 7.5f;
				if (Teki->Position.z < -STAGE_W + 250.0f)
				{
					Teki->MoveState = 0;
				}
				break;
			}

			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->State += 10;
				Teki->Counter = 0;
				break;

			case 20:
				Teki->Counter++;
				if (Teki->Counter % 16 == 0)
				{
					float angle;

					angle = atan2(Pc.Position.z - Teki->Position.z, Pc.Position.x - Teki->Position.x);
					Shot = TekiShotAdd(TEKI_SHOT_TYPE_NORMAL, MV1GetPosition(Teki->ModelHandle));
					if (Shot)
					{
						Shot->Speed = VScale(VGet(cos(angle), 0.0f, sin(angle)), TEKI_SHOT_SPEED_1);
						Shot->Speed.y = (Pc.Position.y - Shot->Position.y) / ((Pc.Position.x - Shot->Position.x) / Shot->Speed.x);
					}

					Shot = TekiShotAdd(TEKI_SHOT_TYPE_NORMAL, MV1GetPosition(Teki->ModelHandle));
					if (Shot)
					{
						Shot->Speed = VScale(VGet(cos(angle - 0.5f), 0.0f, sin(angle - 0.5f)), TEKI_SHOT_SPEED_1);
						Shot->Speed.y = (Pc.Position.y - Shot->Position.y) / ((Pc.Position.x - Shot->Position.x) / Shot->Speed.x);
					}

					Shot = TekiShotAdd(TEKI_SHOT_TYPE_NORMAL, MV1GetPosition(Teki->ModelHandle));
					if (Shot)
					{
						Shot->Speed = VScale(VGet(cos(angle + 0.5f), 0.0f, sin(angle + 0.5f)), TEKI_SHOT_SPEED_1);
						Shot->Speed.y = (Pc.Position.y - Shot->Position.y) / ((Pc.Position.x - Shot->Position.x) / Shot->Speed.x);
					}
				}

				if (Teki->Counter > 16)
				{
					Teki->State = 10;
					Teki->Counter = 0;
				}
				break;

			case -100:
				ScoreUp_TekiBreak(300);
				EnemyBreakPointUp(200);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;

		case TEKI_TYPE_SPEEDSHOT:
			Teki->MoveCounter++;
			if (Teki->MoveCounter == 180)
			{
				Teki->MoveCounter = 0;
			}
			Teki->Position.z += (float)sin((float)Teki->MoveCounter * DX_TWO_PI / 180.0f) * TEKI_LR_SPEED;
			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->State += 10;
				Teki->Counter = 0;
				break;

			case 20:
				Teki->Counter++;
				if (Teki->Counter % 16 == 0)
				{
					float angle;

					angle = atan2(Pc.Position.z - Teki->Position.z, Pc.Position.x - Teki->Position.x);
					Shot = TekiShotAdd(TEKI_SHOT_TYPE_NORMAL, MV1GetPosition(Teki->ModelHandle));
					if (Shot)
					{
						Shot->Speed = VScale(VGet(cos(angle), 0.0f, sin(angle)), TEKI_SHOT_SPEED_2);
						Shot->Speed.y = (Pc.Position.y - Shot->Position.y) / ((Pc.Position.x - Shot->Position.x) / Shot->Speed.x);
					}
				}

				if (Teki->Counter > 64)
				{
					Teki->State = 10;
					Teki->Counter = 0;
				}
				break;

			case -100:
				ScoreUp_TekiBreak(400);
				EnemyBreakPointUp(225);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;

		case TEKI_TYPE_BOMSHOT:
			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->Counter = 0;
				Teki->State = 0;
				Shot = TekiShotAdd(TEKI_SHOT_TYPE_BOM, MV1GetPosition(Teki->ModelHandle));
				if (Shot)
				{
					float XLength, Sin, Cos, Angle;

					Angle = -TEKI_BOM_ANGLE_RANGE / 2.0f + GetRand(256) * TEKI_BOM_ANGLE_RANGE / 256.0f;
					Cos = (float)cos(Angle);
					Sin = (float)sin(Angle);
					XLength = PC_INIT_X - Teki->Position.x;
					Shot->Speed = VScale(VGet(Cos, 0.0f, Sin), TEKI_SHOT_SPEED_1);
					Shot->Speed.y = (PC_INIT_Y - MV1GetPosition(Teki->ModelHandle).y) / (XLength / (Cos * TEKI_SHOT_SPEED_1));
				}
				break;

			case -100:
				ScoreUp_TekiBreak(500);
				EnemyBreakPointUp(250);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;

		case TEKI_TYPE_BREAKSHOT:
			switch (Teki->MoveState)
			{
			case 0:
				Teki->MoveCounter = GetRand(120) + 60;
				Teki->MoveState += 10;
				break;

			case 10:
				Teki->MoveCounter--;
				if (Teki->MoveCounter < 0)
				{
					Teki->MoveCounter = 3000;
					Teki->MoveState += 10;
				}
				break;

			case 20:
				Teki->Position.y += Teki->MoveCounter / 100.0f;
				Teki->MoveCounter -= 100;
				if (Teki->Position.y < TEKI_INIT_Y2)
				{
					Teki->Position.y = TEKI_INIT_Y2;
					Teki->MoveCounter = 0;
					Teki->MoveState = 0;
				}
				break;
			}

			switch (Teki->State)
			{
			case 0:
				Teki->Value = GetRand(240) + 120 - GameData.Level;
				Teki->State += 10;
				break;

			case 10:
				Teki->Counter++;
				if (Teki->Counter < Teki->Value) break;
				Teki->Counter = 0;
				Teki->State += 10;
				break;

			case 20:
				if (Teki->Counter % 6 == 0)
				{
					Shot = TekiShotAdd(TEKI_SHOT_TYPE_BREAK, MV1GetPosition(Teki->ModelHandle));
					if (Shot)
					{
						float XLength, Sin, Cos, Angle;

						Angle = -TEKI_BREAK_ANGLE_RANGE / 2.0f + GetRand(256) * TEKI_BREAK_ANGLE_RANGE / 256.0f;
						Cos = (float)cos(Angle);
						Sin = (float)sin(Angle);
						XLength = PC_INIT_X - Teki->Position.x;
						Shot->Speed = VScale(VGet(Cos, 0.0f, Sin), TEKI_SHOT_SPEED_3);
						Shot->Speed.y = (PC_INIT_Y - MV1GetPosition(Teki->ModelHandle).y) / (XLength / (Cos * TEKI_SHOT_SPEED_3));
					}
				}
				if (Teki->Counter != 30)
				{
					Teki->Counter++;
					break;
				}
				Teki->State = 0;
				Teki->Counter = 0;
				Teki->Value = 0;
				break;

			case -100:
				ScoreUp_TekiBreak(600);
				EnemyBreakPointUp(300);
				PcSubWeaponPowerUp(1);
				Teki->State -= 10;
				break;

			case -110:
				if (Teki->Opacity <= 0.0f)
				{
					Teki->UseFlag = 0;
					TekiCommon.CharNum--;
				}
				break;
			}
			break;
		}

		if (Teki->UseFlag)
		{
			if (Teki->Fade)
			{
				Teki->Opacity += TEKI_FADE_IN_SPEED;
				if (Teki->Opacity > 1.0f)
				{
					Teki->Opacity = 1.0f;
				}
			}
			else
			{
				Teki->Opacity -= TEKI_FADE_IN_SPEED;
				if (Teki->Opacity < 0.0f)
				{
					Teki->Opacity = 0.0f;
				}
			}

			MV1SetOpacityRate(Teki->ModelHandle, Teki->Opacity);
			MV1SetPosition(Teki->ModelHandle, VGet(Teki->Position.x, Teki->Position.y + sin(Teki->YAngle) * TEKI_YANGLE_HEIGHT, Teki->Position.z));
		}
	}
}

void TekiDraw(void)
{
	int i;
	TEKI* Teki;

	SetUseZBufferFlag(TRUE);
	Teki = TekiCommon.Char;
	for (i = 0; i < TEKI_MAX_NUM; i++, Teki++)
	{
		if (Teki->UseFlag == 0) continue;

		DrawShadow(VGet(Teki->Position.x, JIMEN_Y + 3.0f, Teki->Position.z), 250.0f, (int)(Teki->Opacity * 255.0f));
	}
	SetUseZBufferFlag(FALSE);

	Teki = TekiCommon.Char;
	for (i = 0; i < TEKI_MAX_NUM; i++, Teki++)
	{
		if (Teki->UseFlag == 0) continue;

		switch (Teki->Type)
		{
		case TEKI_TYPE_SIMPLESHOT:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;

		case TEKI_TYPE_ITEM:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(1.5f, 1.5f, 0.2f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;

		case TEKI_TYPE_2WAYHEIGHTSHOT:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(2.0f, 0.2f, 0.2f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;

		case TEKI_TYPE_3WAYSHOT:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(0.2f, 2.0f, 0.2f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;

		case TEKI_TYPE_SPEEDSHOT:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(0.2f, 2.0f, 2.0f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;

		case TEKI_TYPE_BOMSHOT:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(0.3f, 0.3f, 2.0f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;

		case TEKI_TYPE_BREAKSHOT:
			MV1SetDifColorScale(Teki->ModelHandle, GetColorF(1.5f, 0.2f, 1.5f, 1.0f));
			MV1DrawModel(Teki->ModelHandle);
			break;
		}
	}
}

extern TEKISHOT* TekiShotAdd(int Type, VECTOR Pos)
{
	int i;
	TEKISHOT* Shot;

	if (TekiCommon.ShotNum == TEKI_SHOT_MAX_NUM) return NULL;
	for (i = 0; TekiCommon.Shot[i].UseFlag; i++) {}
	Shot = &TekiCommon.Shot[i];

	Shot->UseFlag = TRUE;
	Shot->Type = Type;
	Shot->HitFlag = TRUE;
	Shot->State = 0;
	Shot->Counter = 0;
	Shot->CounterF = 0.0f;
	Shot->Position = Pos;
	TekiCommon.ShotNum++;

	return Shot;
}

extern void TekiShotMove(void)
{
	TEKISHOT* Shot;
	EFFECT* Effect;
	int i;

	Shot = TekiCommon.Shot;
	for (i = 0; i < TEKI_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == 0) continue;

		switch (Shot->Type)
		{
		case TEKI_SHOT_TYPE_NORMAL:
			Shot->Position = VAdd(Shot->Position, Shot->Speed);
			if (Shot->Position.x < TEKI_SHOT_CLIP_X1 ||
				Shot->Position.x > TEKI_SHOT_CLIP_X2 ||
				Shot->Position.y < TEKI_SHOT_CLIP_Y1 ||
				Shot->Position.y > TEKI_SHOT_CLIP_Y2 ||
				Shot->Position.z < TEKI_SHOT_CLIP_Z1 ||
				Shot->Position.z > TEKI_SHOT_CLIP_Z2)
			{
				Shot->UseFlag = FALSE;
				TekiCommon.ShotNum--;
			}
			else
			{
				if (Shot->HitFlag && Pc.HitFlag && VSquareSize(VSub(Shot->Position, Pc.Position)) < PC_HIT_SIZE * PC_HIT_SIZE)
				{
					Effect = EffectAdd(0, VGet(Shot->Position.x, Shot->Position.y, Shot->Position.z));
					if (Effect)
					{
						Effect->Size = 3.0f;
						Effect->Move = 0.0f;
						Effect->ColorR = 255;
						Effect->ColorG = 64;
						Effect->ColorB = 64;
					}

					Pc.Damage = 10;

					Shot->UseFlag = FALSE;
					TekiCommon.ShotNum--;
				}
			}
			break;

		case TEKI_SHOT_TYPE_SCALE:
			Shot->Counter++;
			Shot->Position = VAdd(Shot->Position, Shot->Speed);
			if (Shot->Position.x < TEKI_SHOT_CLIP_X1 ||
				Shot->Position.x > TEKI_SHOT_CLIP_X2 ||
				Shot->Position.y < TEKI_SHOT_CLIP_Y1 ||
				Shot->Position.y > TEKI_SHOT_CLIP_Y2 ||
				Shot->Position.z < TEKI_SHOT_CLIP_Z1 ||
				Shot->Position.z > TEKI_SHOT_CLIP_Z2)
			{
				Shot->UseFlag = FALSE;
				TekiCommon.ShotNum--;
			}
			else
			{
				if (Shot->HitFlag && Pc.HitFlag)
				{
					if (((Shot->Position.x - TEKI_SHOT_SCALE_HIT_SIZEX > Pc.Position.x - PC_HIT_SIZE &&
						Shot->Position.x - TEKI_SHOT_SCALE_HIT_SIZEX < Pc.Position.x + PC_HIT_SIZE) ||
						(Shot->Position.x + TEKI_SHOT_SCALE_HIT_SIZEX > Pc.Position.x - PC_HIT_SIZE &&
							Shot->Position.x + TEKI_SHOT_SCALE_HIT_SIZEX < Pc.Position.x + PC_HIT_SIZE)) &&
						(Shot->Position.z > Pc.Position.z - PC_HIT_SIZE &&
							Shot->Position.z < Pc.Position.z + PC_HIT_SIZE))
					{
						float CenterY, HitSize;

						HitSize = 256 * (TEKI_SHOT_SCALE_INIT + TEKI_SHOT_SCALESPEED * Shot->Counter / 60.0f) / 2.0f;
						CenterY = Shot->Position.y + HitSize;
						if ((CenterY - HitSize > Pc.Position.y - PC_HIT_SIZE &&
							CenterY - HitSize < Pc.Position.y + PC_HIT_SIZE) ||
							(CenterY + HitSize > Pc.Position.y - PC_HIT_SIZE &&
								CenterY + HitSize < Pc.Position.y + PC_HIT_SIZE) ||
							(Pc.Position.y - PC_HIT_SIZE > CenterY - HitSize &&
								Pc.Position.y - PC_HIT_SIZE < CenterY + HitSize) ||
							(Pc.Position.y + PC_HIT_SIZE > CenterY - HitSize &&
								Pc.Position.y + PC_HIT_SIZE < CenterY + HitSize))
						{
							Effect = EffectAdd(0, VGet(Shot->Position.x, Shot->Position.y, Shot->Position.z));
							if (Effect)
							{
								Effect->Size = 3.0f;
								Effect->Move = 0.0f;
								Effect->ColorR = 255;
								Effect->ColorG = 64;
								Effect->ColorB = 64;
							}

							Pc.Damage = 10;

							Shot->UseFlag = FALSE;
							TekiCommon.ShotNum--;
						}
					}
				}
			}
			break;

		case TEKI_SHOT_TYPE_BOM:
			switch (Shot->State)
			{
			case 0:
				Shot->Position = VAdd(Shot->Position, Shot->Speed);
				if (Shot->Position.x > PC_INIT_X)
				{
					Shot->State = 1;
				}
				break;

			case 1:
				Shot->Counter++;
				if (Shot->Counter > TEKI_BOM_FADEWAIT)
				{
					Shot->HitFlag = FALSE;
				}
				if (Shot->Counter > TEKI_BOM_DELETEWAIT)
				{
					Shot->UseFlag = FALSE;
					TekiCommon.ShotNum--;
				}
				break;
			}

			if (Shot->Position.x < TEKI_SHOT_CLIP_X1 ||
				Shot->Position.x > TEKI_SHOT_CLIP_X2 ||
				Shot->Position.y < TEKI_SHOT_CLIP_Y1 ||
				Shot->Position.y > TEKI_SHOT_CLIP_Y2 ||
				Shot->Position.z < TEKI_SHOT_CLIP_Z1 ||
				Shot->Position.z > TEKI_SHOT_CLIP_Z2)
			{
				if (Shot->UseFlag)
				{
					Shot->UseFlag = FALSE;
					TekiCommon.ShotNum--;
				}
			}
			else
			{
				float HitSize;
				HitSize = (TEKI_BOM_SIZE - 12.0f) * (1.0f + TEKI_BOM_SCALESPEED * Shot->Counter / 60.0f);
				if (Shot->HitFlag && Pc.HitFlag && VSquareSize(VSub(Shot->Position, Pc.Position)) < (PC_HIT_SIZE + HitSize) * (PC_HIT_SIZE + HitSize))
				{
					switch (Shot->State)
					{
					case 0:
						Effect = EffectAdd(0, VGet(Shot->Position.x, Shot->Position.y, Shot->Position.z));
						if (Effect)
						{
							Effect->Size = 3.0f;
							Effect->Move = 0.0f;
							Effect->ColorR = 255;
							Effect->ColorG = 64;
							Effect->ColorB = 64;
						}

						Pc.Damage = 10;
						Shot->HitFlag = FALSE;

						Shot->UseFlag = FALSE;
						TekiCommon.ShotNum--;
						break;

					case 1:
						Effect = EffectAdd(0, VGet(Pc.Position.x, Pc.Position.y, Pc.Position.z));
						if (Effect)
						{
							Effect->Size = 3.0f;
							Effect->Move = 0.0f;
							Effect->ColorR = 255;
							Effect->ColorG = 64;
							Effect->ColorB = 64;
						}

						Pc.Damage = 10;
						Shot->HitFlag = FALSE;
						break;
					}
				}
			}
			break;

		case TEKI_SHOT_TYPE_BREAK:
			if (Shot->HitFlag)
			{
				int j;
				PCSHOT* PcShot;

				PcShot = Pc.Shot;
				for (j = 0; j < PC_SHOT_MAX_NUM; j++, PcShot++)
				{
					if (PcShot->UseFlag == FALSE) continue;

					if (VSquareSize(VSub(Shot->Position, PcShot->Position)) < TEKI_BREAK_SIZE * TEKI_BREAK_SIZE)
					{
						Effect = EffectAdd(0, PcShot->Position);
						if (Effect)
						{
							Effect->Size = 3.0f;
							Effect->Move = 0.0f;
							Effect->ColorR = 64;
							Effect->ColorG = 255;
							Effect->ColorB = 64;
						}

						PcShot->UseFlag = FALSE;
						Pc.ShotNum--;
						break;
					}
				}
				if (j != PC_SHOT_MAX_NUM)
				{
					ScoreUp_TekiBreak(50);
					Shot->HitFlag = FALSE;
					Shot->State = 1;
				}
			}

			Shot->Position = VAdd(Shot->Position, Shot->Speed);
			switch (Shot->State)
			{
			case 0:
				if (Shot->Position.x < TEKI_SHOT_CLIP_X1 ||
					Shot->Position.x > TEKI_SHOT_CLIP_X2 ||
					Shot->Position.y < TEKI_SHOT_CLIP_Y1 ||
					Shot->Position.y > TEKI_SHOT_CLIP_Y2 ||
					Shot->Position.z < TEKI_SHOT_CLIP_Z1 ||
					Shot->Position.z > TEKI_SHOT_CLIP_Z2)
				{
					Shot->UseFlag = FALSE;
					TekiCommon.ShotNum--;
				}
				else
				{
					if (Shot->HitFlag && Pc.HitFlag && VSquareSize(VSub(Shot->Position, Pc.Position)) < (PC_HIT_SIZE + TEKI_BREAK_SIZE - 32.0f) * (PC_HIT_SIZE + TEKI_BREAK_SIZE - 32.0f))
					{
						Effect = EffectAdd(0, VGet(Shot->Position.x, Shot->Position.y, Shot->Position.z));
						if (Effect)
						{
							Effect->Size = 3.0f;
							Effect->Move = 0.0f;
							Effect->ColorR = 255;
							Effect->ColorG = 64;
							Effect->ColorB = 64;
						}

						Pc.Damage = 10;

						Shot->UseFlag = FALSE;
						TekiCommon.ShotNum--;
					}
				}
				break;

			case 1:
				Shot->Counter++;
				if (Shot->Counter > TEKI_BREAK_FADETIME)
				{
					Shot->UseFlag = FALSE;
					TekiCommon.ShotNum--;
				}
				break;
			}
			break;
		}
	}
}

extern void TekiShotDraw(void)
{
	int i;
	TEKISHOT* Shot;

	SetUseZBufferFlag(TRUE);
	Shot = TekiCommon.Shot;
	for (i = 0; i < TEKI_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == FALSE) continue;

		switch (Shot->Type)
		{
		case TEKI_SHOT_TYPE_NORMAL:
			DrawShadow(VGet(Shot->Position.x, JIMEN_Y + 3.0f, Shot->Position.z), 64.0f);
			break;

		case TEKI_SHOT_TYPE_SCALE:
			DrawShadow(VGet(Shot->Position.x, JIMEN_Y + 3.0f, Shot->Position.z), 128.0f);
			break;

		case TEKI_SHOT_TYPE_BOM:
			if (Shot->State == 0)
			{
				DrawShadow(VGet(Shot->Position.x, JIMEN_Y + 3.0f, Shot->Position.z), 64.0f);
			}
			break;

		case TEKI_SHOT_TYPE_BREAK:
			if (Shot->State == 0)
			{
				DrawShadow(VGet(Shot->Position.x, JIMEN_Y + 3.0f, Shot->Position.z), 128.0f);
			}
			else
			{
				DrawShadow(VGet(Shot->Position.x, JIMEN_Y + 3.0f, Shot->Position.z), 128.0f - Shot->Counter * 128.0f / TEKI_BREAK_FADETIME);
			}
			break;
		}
	}
	SetWriteZBufferFlag(TRUE);
	Shot = TekiCommon.Shot;
	for (i = 0; i < TEKI_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == 0) continue;
		switch (Shot->Type)
		{
		case TEKI_SHOT_TYPE_NORMAL:
			DrawExtendGraph3D(Shot->Position.x, Shot->Position.y, Shot->Position.z, 1.0f, 1.0f, TekiCommon.ShotGraphHandle, TRUE);
			break;

		case TEKI_SHOT_TYPE_SCALE:
		{
			float ScaleY, UpY;

			ScaleY = TEKI_SHOT_SCALE_INIT + TEKI_SHOT_SCALESPEED * Shot->Counter / 60;
			UpY = 256.0f * ScaleY / 2.0f;
			DrawExtendGraph3D(Shot->Position.x, Shot->Position.y + UpY, Shot->Position.z, 0.5f, ScaleY, TekiCommon.Shot2GraphHandle, TRUE);
		}
		break;

		case TEKI_SHOT_TYPE_BOM:
			switch (Shot->State)
			{
			case 0:
				SetDrawBright(32, 32, 255);
				DrawExtendGraph3D(Shot->Position.x, Shot->Position.y, Shot->Position.z, 2.0f, 2.0f, Pc.ShotGraphHandle, TRUE);
				SetDrawBright(255, 255, 255);
				break;

			case 1:
				if (Shot->Counter > TEKI_BOM_FADEWAIT)
				{
					int Count;
					Count = Shot->Counter - TEKI_BOM_FADEWAIT;
					SetDrawBlendMode(DX_BLENDMODE_ADD, ((TEKI_BOM_DELETEWAIT - TEKI_BOM_FADEWAIT) - Count) * 255 / (TEKI_BOM_DELETEWAIT - TEKI_BOM_FADEWAIT));
				}
				else
				{
					SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
				}
				SetWriteZBufferFlag(FALSE);
				DrawSphere3D(Shot->Position, TEKI_BOM_SIZE * (1.0f + TEKI_BOM_SCALESPEED * Shot->Counter / 60.0f), 6, GetColor(64, 64, 255), GetColor(255, 255, 255), TRUE);
				SetWriteZBufferFlag(TRUE);
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
				break;
			}
			break;

		case TEKI_SHOT_TYPE_BREAK:
		{
			float BaseScale, Scale;

			SetDrawBright(255, 32, 255);
			BaseScale = TEKI_BREAK_SIZE / 32.0f;
			if (Shot->State == 0)
			{
				DrawExtendGraph3D(Shot->Position.x, Shot->Position.y, Shot->Position.z, BaseScale, BaseScale, Pc.ShotGraphHandle, TRUE);
			}
			else
			{
				Scale = BaseScale - BaseScale * Shot->Counter / TEKI_BREAK_FADETIME;
				DrawExtendGraph3D(Shot->Position.x, Shot->Position.y, Shot->Position.z, Scale, Scale, Pc.ShotGraphHandle, TRUE);
			}
			SetDrawBright(255, 255, 255);
		}
		break;
		}
	}
	SetWriteZBufferFlag(FALSE);
	SetUseZBufferFlag(FALSE);
}

extern void GameInitialize(void)
{
	GameData.CameraPosition.x = CAMERA_X;
	GameData.CameraPosition.y = CAMERA_Y;
	GameData.CameraPosition.z = CAMERA_Z;
	GameData.Level = 0;
	GameData.PrevLevelPoint = 0;
	GameData.NextLevelPoint = 20;
	GameData.EnemyBreakNum = 0;
	GameData.EnemyBreakPoint = 0;
	GameData.Score = 0;
	GameData.ScoreRate = 0;

	PcInitialize();
	TekiInitialize();
	EffectInitialize();
	ItemInitialize();
}

extern void GameMove(void)
{
	if (GameData.NextLevelPoint <= GameData.EnemyBreakPoint)
	{
		int PrevLevelPoint;

		GameData.Level++;
		PrevLevelPoint = GameData.PrevLevelPoint;
		GameData.PrevLevelPoint = GameData.NextLevelPoint;
		GameData.NextLevelPoint = 20 * GameData.Level + GameData.NextLevelPoint;
	}

	PcMove();
	PcShotMove();
	TekiMove();
	TekiShotMove();
	ItemMove();
	EffectMove();
}

extern void GameDraw(void)
{
	TekiDraw();
	PcDraw();
	TekiShotDraw();
	PcShotDraw();
	ItemDraw();
	EffectDraw();
}

extern void TitleInitialize(void)
{
	Title.CursorPosition = 0;
	Title.IconAngle = 0.0f;
	Title.CameraZ = 0.0f;
	Title.CameraSpeed = CAMERA_SPEED;
}

extern void TitleMove(void)
{
	Title.CameraZ += Title.CameraSpeed;
	if (Title.CameraZ < -STAGE_W ||
		Title.CameraZ >  STAGE_W)
		Title.CameraSpeed = -Title.CameraSpeed;
	SetCameraPositionAndTarget_UpVecY(VGet(CAMERA_X, CAMERA_Y, Title.CameraZ), VGet(0.0f, 0.0f, 0.0f));
	if (Title.IconAngle > DX_TWO_PI_F)
	{
		Title.IconAngle -= DX_TWO_PI_F;
	}
	Title.IconSin = (float)sin(Title.IconAngle);
	Title.IconCos = (float)cos(Title.IconAngle);
	if (GameData.EdgeInput & PAD_INPUT_UP)
	{
		Title.CursorPosition--;
		if (Title.CursorPosition < 0)
			Title.CursorPosition = 0;
	}

	if (GameData.EdgeInput & PAD_INPUT_10)
	{
		switch (Title.CursorPosition)
		{
		case 0:
			GameData.Scene = SCENE_GAME;
			GameInitialize();
			break;
		}
	}
}

extern void TitleDraw(void)
{
	int i;
	int x, y;

	DrawGraph(0, 0, GameData.TitleGraphHandle, TRUE);
	DrawString(260, 360, "START", GetColor(255, 0, 0));
	DrawTitleCircle(220, 360 + Title.CursorPosition * 32 + TITLE_ICON_SIZE / 2);
}

extern void DrawTitleTriangle(int x, int y)
{
	static int InitFlag;
	static float BasePos[3][2];

	DrawTriangle(
		(int)(BasePos[0][0] * Title.IconCos - BasePos[0][1] * Title.IconSin) + x,
		(int)(BasePos[0][0] * Title.IconSin + BasePos[0][1] * Title.IconCos) + y,
		(int)(BasePos[1][0] * Title.IconCos - BasePos[1][1] * Title.IconSin) + x,
		(int)(BasePos[1][0] * Title.IconSin + BasePos[1][1] * Title.IconCos) + y,
		(int)(BasePos[2][0] * Title.IconCos - BasePos[2][1] * Title.IconSin) + x,
		(int)(BasePos[2][0] * Title.IconSin + BasePos[2][1] * Title.IconCos) + y, GetColor(0, 0, 0), TRUE);
}

extern void DrawTitleSquare(int x, int y)
{
	DrawRotaGraph(x, y, 1.0, Title.IconAngle, GameData.TitleSquareGraphHandle, FALSE);
}

extern void DrawTitleCircle(int x, int y)
{
	DrawCircle((int)(Title.IconCos * 3.0f) + x, (int)(Title.IconSin * 3.0f) + y, (int)(TITLE_ICON_SIZE / 2.0f - 3.0f), GetColor(255, 0, 255), TRUE);
}

int PcInitialize(void)
{
	int i;

	Pc.Position.x = PC_INIT_X;
	Pc.Position.y = PC_INIT_Y;
	Pc.Position.z = PC_INIT_Z;
	Pc.ShotNum = 0;
	Pc.Opacity = 1.0f;
	Pc.EndCounter = 0;
	Pc.ShotPower = 0;
	Pc.SubWeaponPower = 0;
	Pc.HitFlag = 1;
	Pc.Hp = PC_HP_MAX;
	for (i = 0; i < 10; i++)
		Pc.HpBlockAlpha[i] = 0;

	return 0;
}

void PcMove(void)
{
	int i;
	if (Pc.Damage != 0)
	{
		if (Pc.MutekiTime == 0)
		{
			PlaySoundMem(GameData.PcDamageSoundHandle, DX_PLAYTYPE_BACK);

			Pc.Hp -= Pc.Damage;
			if (Pc.Hp < 0)
			{
				Pc.Hp = 0;
				Pc.HitFlag = 0;
			}
			Pc.MutekiTime = 120;
			Pc.BrinkCounter = 1;

			Pc.ShotPower--;
			if (Pc.ShotPower < 0)
				Pc.ShotPower = 0;
		}
		Pc.Damage = 0;
	}

	if (Pc.MutekiTime != 0)
	{
		Pc.MutekiTime--;
	}
	else
	{
		Pc.BrinkCounter = 0;
	}

	for (i = 0; i < 10; i++)
	{
		if (i * 10 < Pc.Hp)
		{
			Pc.HpBlockAlpha[i] += PC_LIFEALPHA_SPEED;
			if (Pc.HpBlockAlpha[i] > 255)
			{
				Pc.HpBlockAlpha[i] = 255;
			}
		}
		else
		{
			Pc.HpBlockAlpha[i] -= PC_LIFEALPHA_SPEED;
			if (Pc.HpBlockAlpha[i] < 0)
			{
				Pc.HpBlockAlpha[i] = 0;
			}
		}
	}

	if (Pc.Hp == 0.0f)
	{
		Pc.Opacity -= PC_OPACITY_SPEED;
		if (Pc.Opacity < 0.0f)
		{
			Pc.Opacity = 0.0f;
			Pc.EndCounter++;
			if (Pc.EndCounter > PC_END_TIME)
			{
				GameData.Scene = SCENE_TITLE;
				TitleInitialize();
			}
		}
	}
	else
	{
		if (GameData.NowInput & PAD_INPUT_LEFT)
		{
			Pc.Position.z -= PC_SPEED;
			if (Pc.Position.z < -STAGE_W)
			{
				Pc.Position.z = -STAGE_W;
			}
		}
		else
			if (GameData.NowInput & PAD_INPUT_RIGHT)
			{
				Pc.Position.z += PC_SPEED;
				if (Pc.Position.z > STAGE_W)
				{
					Pc.Position.z = STAGE_W;
				}
			}

		if (GameData.EdgeInput & PAD_INPUT_10)
		{
			PCSHOT* Shot;

			switch (Pc.ShotPower)
			{
			case 0:
			case 1:
				if (PcShotTypeNum(0) < 4 + Pc.ShotPower * 4)
				{
					PlaySoundMem(GameData.ShotSoundHandle, DX_PLAYTYPE_BACK);
					Shot = PcShotAdd(PC_SHOT_TYPE_NORMAL, Pc.Position);
					if (Shot)
					{
						Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, 0.0f)), PC_SHOT_SPEED);
						Shot->Speed.y += PC_SHOT_UPPOWER;
					}
				}
				break;

			case 2:
			case 3:
				if ((PcShotTypeNum(0) + 1) / 2 < (Pc.ShotPower == 2 ? 4 : 8))
				{
					PlaySoundMem(GameData.ShotSoundHandle, DX_PLAYTYPE_BACK);
					Shot = PcShotAdd(PC_SHOT_TYPE_NORMAL, VGet(Pc.Position.x, Pc.Position.y, Pc.Position.z - 40.0f));
					if (Shot)
					{
						Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, 0.0f)), PC_SHOT_SPEED);
						Shot->Speed.y += PC_SHOT_UPPOWER;
					}

					Shot = PcShotAdd(PC_SHOT_TYPE_NORMAL, VGet(Pc.Position.x, Pc.Position.y, Pc.Position.z + 40.0f));
					if (Shot)
					{
						Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, 0.0f)), PC_SHOT_SPEED);
						Shot->Speed.y += PC_SHOT_UPPOWER;
					}
				}
				break;

			case 4:
			case 5:
				if ((PcShotTypeNum(0) + 1) / 2 < (Pc.ShotPower == 4 ? 4 : 8))
				{
					PlaySoundMem(GameData.ShotSoundHandle, DX_PLAYTYPE_BACK);
					if ((PcShotTypeNum(1) + 1) / 2 < (Pc.ShotPower == 4 ? 4 : 8))
					{
						Shot = PcShotAdd(PC_SHOT_TYPE_NORMAL, VGet(Pc.Position.x, Pc.Position.y, Pc.Position.z - 40.0f));
						if (Shot)
						{
							Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, 0.0f)), PC_SHOT_SPEED);
							Shot->Speed.y += PC_SHOT_UPPOWER;
						}

						Shot = PcShotAdd(PC_SHOT_TYPE_NORMAL, VGet(Pc.Position.x, Pc.Position.y, Pc.Position.z + 40.0f));
						if (Shot)
						{
							Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, 0.0f)), PC_SHOT_SPEED);
							Shot->Speed.y += PC_SHOT_UPPOWER;
						}

						Shot = PcShotAdd(PC_SHOT_TYPE_SUB, Pc.Position);
						if (Shot)
						{
							Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, -1000.0f)), PC_SHOT_SPEED);
							Shot->Speed.y += PC_SHOT_UPPOWER;
						}

						Shot = PcShotAdd(PC_SHOT_TYPE_SUB, Pc.Position);
						if (Shot)
						{
							Shot->Speed = VScale(VNorm(VGet(TEKI_INIT_X - Pc.Position.x, TEKI_INIT_Y2 - Pc.Position.y, 1000.0f)), PC_SHOT_SPEED);
							Shot->Speed.y += PC_SHOT_UPPOWER;
						}
					}
				}
				break;
			}
		}

		if (GameData.EdgeInput & PAD_INPUT_B)
		{
			if (Pc.JumpFlag == 0)
			{
				Pc.JumpFlag = 1;
				Pc.SpeedY = PC_JUMP_POWER;
			}
		}
		if (Pc.JumpFlag)
		{
			Pc.Position.y += Pc.SpeedY;
			Pc.SpeedY -= PC_GRAVITY;
			if (Pc.Position.y < PC_INIT_Y)
			{
				Pc.Position.y = PC_INIT_Y;
				Pc.JumpFlag = 0;
			}
		}
	}

	MV1SetOpacityRate(Pc.ModelHandle, Pc.Opacity);
	MV1SetPosition(Pc.ModelHandle, Pc.Position);
	SetCameraPositionAndTarget_UpVecY(VGet(CAMERA_X, CAMERA_Y + Pc.Position.y - PC_INIT_Y, Pc.Position.z), VGet(0.0f, Pc.Position.y - PC_INIT_Y, Pc.Position.z));
}

void PcDraw(void)
{
	if (Pc.MutekiTime)
	{
		Pc.BrinkCounter = Pc.BrinkCounter == 0 ? 1 : 0;
	}
	if (Pc.BrinkCounter == 0)
	{
		MV1SetRotationXYZ(Pc.ModelHandle, VGet(0.0f, DX_PI_F, 0.0f));
		SetUseZBufferFlag(TRUE);
		DrawShadow(VGet(Pc.Position.x, JIMEN_Y + 3.0f, Pc.Position.z), 250.0f, (int)(Pc.Opacity * 255));
		SetUseZBufferFlag(FALSE);
		MV1DrawModel(Pc.ModelHandle);
	}
}

extern void PcShotPowerUp(int UpPoint)
{
	PlaySoundMem(GameData.PowerUpSoundHandle, DX_PLAYTYPE_BACK);

	Pc.ShotPower += UpPoint;
	if (Pc.ShotPower > PC_SHOTPOWER_MAX)
	{
		Pc.ShotPower = PC_SHOTPOWER_MAX;
	}
}

extern void PcSubWeaponPowerUp(int UpPoint)
{
	Pc.SubWeaponPower += UpPoint;
	if (Pc.SubWeaponPower > PC_SUBWEAPONPOWER_MAX)
	{
		Pc.SubWeaponPower = PC_SUBWEAPONPOWER_MAX;
	}
}

extern void PcHpUp(int UpPoint)
{
	Pc.Hp += UpPoint;
	if (Pc.Hp > PC_HP_MAX)
	{
		Pc.Hp = PC_HP_MAX;
	}
}

PCSHOT* PcShotAdd(int Type, VECTOR Pos)
{
	int i;
	PCSHOT* Shot;

	if (Pc.ShotNum == PC_SHOT_MAX_NUM) return NULL;
	for (i = 0; Pc.Shot[i].UseFlag; i++) {}
	Shot = &Pc.Shot[i];

	Shot->UseFlag = TRUE;
	Shot->Type = Type;
	Shot->Position = Pos;

	Pc.ShotNum++;

	return Shot;
}

void PcShotMove(void)
{
	int i, j;
	PCSHOT* Shot;
	TEKI* Teki;
	EFFECT* Effect;

	Shot = Pc.Shot;
	for (i = 0; i < PC_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == FALSE) continue;

		switch (Shot->Type)
		{
		case PC_SHOT_TYPE_NORMAL:
		case PC_SHOT_TYPE_SUB:
			Shot->Position = VAdd(Shot->Position, Shot->Speed);
			Shot->Speed.y -= PC_SHOT_GRAVITY;
			if (Shot->Position.x < PC_SHOT_KABE_X || Shot->Position.y < JIMEN_Y)
			{
				Shot->UseFlag = FALSE;
				Pc.ShotNum--;
			}
			else
			{
				Teki = TekiCommon.Char;
				for (j = 0; j < TEKI_MAX_NUM; j++, Teki++)
				{
					if (Teki->UseFlag == 0 || Teki->HitFlag == 0) continue;

					if (VSquareSize(VSub(Shot->Position, Teki->Position)) > TEKI_HIT_SIZE * TEKI_HIT_SIZE)
						continue;

					PlaySoundMem(GameData.DamageSoundHandle, DX_PLAYTYPE_BACK);
					Effect = EffectAdd(0, VGet(Shot->Position.x, Shot->Position.y, Shot->Position.z));
					if (Effect)
					{
						Effect->Size = 3.0f;
						Effect->Move = 0.0f;
						Effect->ColorR = 64;
						Effect->ColorG = 255;
						Effect->ColorB = 64;
					}

					Teki->Damage += Shot->Type == PC_SHOT_TYPE_NORMAL ? 100 : 80;

					Shot->UseFlag = 0;
					Pc.ShotNum--;
				}
			}
			break;
		}
	}
}

void PcShotDraw(void)
{
	int i;
	PCSHOT* Shot;

	SetUseZBufferFlag(TRUE);
	Shot = Pc.Shot;
	for (i = 0; i < PC_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == FALSE) continue;
		DrawShadow(VGet(Shot->Position.x, JIMEN_Y + 3.0f, Shot->Position.z), 64.0f);
	}
	SetWriteZBufferFlag(TRUE);
	Shot = Pc.Shot;
	for (i = 0; i < PC_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == FALSE) continue;
		switch (Shot->Type)
		{
		case PC_SHOT_TYPE_NORMAL:
			SetDrawBright(0, 255, 0);
			DrawExtendGraph3D(Shot->Position.x, Shot->Position.y, Shot->Position.z, 2.0f, 2.0f, Pc.ShotGraphHandle, TRUE);
			break;

		case PC_SHOT_TYPE_SUB:
			SetDrawBright(0, 200, 128);
			DrawExtendGraph3D(Shot->Position.x, Shot->Position.y, Shot->Position.z, 2.0f, 2.0f, Pc.ShotGraphHandle, TRUE);
			break;
		}
	}
	SetDrawBright(255, 255, 255);
	SetWriteZBufferFlag(FALSE);
	SetUseZBufferFlag(FALSE);
}

extern int PcShotTypeNum(int Type)
{
	int i, num;
	PCSHOT* Shot;

	SetUseZBufferFlag(TRUE);
	Shot = Pc.Shot;
	num = 0;
	for (i = 0; i < PC_SHOT_MAX_NUM; i++, Shot++)
	{
		if (Shot->UseFlag == FALSE) continue;

		if (Shot->Type == Type)
			num++;
	}

	return num;
}

extern void DrawUI(void)
{
	int i, w;

	w = GetDrawFormatStringWidth("LV %d", GameData.Level);
	DrawString(8, 480 - 32, "HP", GetColor(255, 64, 64));
	for (i = 0; i < 10; i++)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, Pc.HpBlockAlpha[i]);
		DrawGraph(40 + i * 16, 480 - 24, GameData.LifeGraphHandle, TRUE);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

extern void DrawShadow(VECTOR Pos, float Size, int Alpha)
{
	VERTEX_3D Vertex[6];

	Vertex[0].pos = VGet(Pos.x - Size, Pos.y, Pos.z + Size);
	Vertex[1].pos = VGet(Pos.x + Size, Pos.y, Pos.z + Size);
	Vertex[2].pos = VGet(Pos.x - Size, Pos.y, Pos.z - Size);
	Vertex[3].pos = VGet(Pos.x + Size, Pos.y, Pos.z - Size);

	Vertex[0].u = 0.0f; Vertex[0].v = 0.0f;
	Vertex[1].u = 1.0f; Vertex[1].v = 0.0f;
	Vertex[2].u = 0.0f; Vertex[2].v = 1.0f;
	Vertex[3].u = 1.0f; Vertex[3].v = 1.0f;

	Vertex[0].a = 255; Vertex[0].r = 255; Vertex[0].g = 255; Vertex[0].b = 255;
	Vertex[1].a = 255; Vertex[1].r = 255; Vertex[1].g = 255; Vertex[1].b = 255;
	Vertex[2].a = 255; Vertex[2].r = 255; Vertex[2].g = 255; Vertex[2].b = 255;
	Vertex[3].a = 255; Vertex[3].r = 255; Vertex[3].g = 255; Vertex[3].b = 255;

	Vertex[4] = Vertex[2];
	Vertex[5] = Vertex[1];

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, Alpha);
	DrawPolygon3D(Vertex, 2, GameData.ShadowGraphHandle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	LONGLONG Time, RefT, NowT, FpsUpdateTime;
	int RunFrame, i, FpsFont;
	float Fps;

	SetMainWindowText("BatBuster");
	SetGraphMode(640, 480, 16);

	if (DxLib_Init() < 0)
		return -1;

#ifdef SMALL_SCREEN
	SetWindowSizeExtendRate(0.5f);
#endif

	TekiGlobalInitialize();

	GameData.ShotSoundHandle = LoadSoundMem("Data\\SEShot.mp3");
	GameData.PcDamageSoundHandle = LoadSoundMem("Data\\SEPcDamage.mp3");

	SetTransColor(0, 0, 0);
	Pc.ShotGraphHandle = LoadGraph("Data\\PcTama.bmp");

	SetTransColor(0, 0, 0);
	GameData.LifeGraphHandle = LoadGraph("Data\\Life.tga");

	SetTransColor(0, 0, 0);
	GameData.TitleGraphHandle = LoadGraph("Data\\Title.tga");

	SetTransColor(255, 0, 255);
	GameData.ShadowGraphHandle = LoadGraph("Data\\Shadow.tga");

	SetTransColor(255, 0, 255);
	Pc.ModelHandle = MV1LoadModel("Data\\Sikaku.mqo");
	GameData.StageModelHandle = MV1LoadModel("Data\\Stage.mqo");

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseLighting(TRUE);
	SetGlobalAmbientLight(GetColorF(0.2f, 0.2f, 0.2f, 0.0f));

	SetLightEnable(TRUE);
	ChangeLightTypeDir(VGet(-0.19062f, -0.45937f, -0.40312f));
	SetLightDifColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));
	SetLightSpcColor(GetColorF(1.0f, 1.0f, 1.0f, 0.0f));
	SetLightAmbColor(GetColorF(0.33f, 0.33f, 0.33f, 0.0f));
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_EDGE);
	SetFontSize(24);

	FpsFont = CreateFontToHandle(NULL, 9, 0, DX_FONTTYPE_NORMAL);

	GameData.NextLevelPoint = 1000;
	GameData.PrevLevelPoint = 0;

	GameData.Scene = SCENE_TITLE;
	PlaySoundMem(GameData.TitleSoundHandle, DX_PLAYTYPE_BACK);
	TitleInitialize();

	RunFrame = 1;
	Pos.x = CAMERA_X;
	Pos.y = CAMERA_Y;
	Pos.z = CAMERA_Z;
	Pos2.x = PC_INIT_X;
	Pos2.y = PC_INIT_Y;
	Pos2.z = PC_INIT_Z;
	RefT = GetNowHiPerformanceCount();
	Fps = 60.0f;
	FpsUpdateTime = 0;
	Time = 0;
	while (ProcessMessage() == 0 && GameData.EndFlag == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ClearDrawScreen();

		for (i = 0; i < RunFrame; i++)
		{
			{
				int OldInput;

				OldInput = GameData.NowInput;
				GameData.NowInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
				GameData.EdgeInput = GameData.NowInput & ~OldInput;
			}

			if (CheckHitKey(KEY_INPUT_LSHIFT))
			{
				if (CheckHitKey(KEY_INPUT_LEFT)) Pos2.z -= 8.0f;
				if (CheckHitKey(KEY_INPUT_RIGHT)) Pos2.z += 8.0f;
				if (CheckHitKey(KEY_INPUT_UP)) Pos2.x += 8.0f;
				if (CheckHitKey(KEY_INPUT_DOWN)) Pos2.x -= 8.0f;
				if (CheckHitKey(KEY_INPUT_N)) Pos2.y += 8.0f;
				if (CheckHitKey(KEY_INPUT_M)) Pos2.y -= 8.0f;
			}
			else
			{
				if (CheckHitKey(KEY_INPUT_LEFT)) Pos.z -= 8.0f;
				if (CheckHitKey(KEY_INPUT_RIGHT)) Pos.z += 8.0f;
				if (CheckHitKey(KEY_INPUT_UP)) Pos.x += 8.0f;
				if (CheckHitKey(KEY_INPUT_DOWN)) Pos.x -= 8.0f;
				if (CheckHitKey(KEY_INPUT_N)) Pos.y += 8.0f;
				if (CheckHitKey(KEY_INPUT_M)) Pos.y -= 8.0f;
			}

			switch (GameData.Scene)
			{
			case SCENE_TITLE:
				TitleMove();
				break;

			case SCENE_GAME:
				GameMove();
				break;
			}
		}
		{
			float r, g, b;

			ToolHLSToRGB(GameData.Level / 100.0f, 0.5f, 0.5f, &r, &g, &b);
			MV1SetMaterialDifColor(GameData.StageModelHandle, 0, GetColorF(r, g, b, 1.0f));
			MV1DrawModel(GameData.StageModelHandle);
		}

		switch (GameData.Scene)
		{
		case SCENE_TITLE:
			TitleDraw();
			DrawString(260, 210, "BatBuster", GetColor(150, 0, 255));
			break;

		case SCENE_GAME:
			GameDraw();
			break;
		}
		DrawUI();
		ScreenFlip();

		NowT = GetNowHiPerformanceCount();
		Time = NowT - RefT;
		RunFrame = (int)(Time / (1000000 / 60));
		if (RunFrame == 0) RunFrame = 1;
		RefT = NowT;

		if (RefT - FpsUpdateTime > 500000)
		{
			FpsUpdateTime = RefT;
			Fps = 1000000.0f / Time;
		}
	}
	DxLib_End();

	return 0;
}