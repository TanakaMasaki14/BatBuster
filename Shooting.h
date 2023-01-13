#include "DxLib.h"
#ifndef __TAMAATE_H__
#define __TAMAATE_H__
#define EFFECT_MAX_NUM				(256)		
#define TEKI_MAX_NUM				(256)		
#define TEKI_SHOT_MAX_NUM			(1024)		
#define PC_SHOT_MAX_NUM				(256)		
#define ITEM_MAX_NUM				(512)		
#define SCENE_TITLE					(0)			
#define SCENE_GAME					(1)			
#define ITEM_TYPE_POWER_UP			(0)			
#define ITEM_TYPE_HP_UP_20			(1)			
#define ITEM_TYPE_HP_UP_50			(2)			
#define ITEM_TYPE_SUB_WEAPON		(3)			
#define ITEM_TYPE_NUM				(4)			
#define PC_SHOT_TYPE_NORMAL			(0)			
#define PC_SHOT_TYPE_SUB			(1)			
#define TEKI_SHOT_TYPE_NORMAL		(0)			
#define TEKI_SHOT_TYPE_SCALE		(1)			
#define TEKI_SHOT_TYPE_BOM			(2)			
#define TEKI_SHOT_TYPE_BREAK		(3)			
#define TEKI_TYPE_SIMPLESHOT		(0)			
#define TEKI_TYPE_ITEM				(1)			
#define TEKI_TYPE_2WAYHEIGHTSHOT	(2)			
#define TEKI_TYPE_3WAYSHOT			(3)			
#define TEKI_TYPE_SPEEDSHOT			(4)			
#define TEKI_TYPE_BOMSHOT			(5)			
#define TEKI_TYPE_BREAKSHOT			(6)			


struct ITEM
{
	int			UseFlag;
	int			Type;
	VECTOR		Position;
	float		JumpPowerY;
};

struct ITEMCOMMON
{
	int			GraphHandle[ITEM_TYPE_NUM];

	int			DataNum;
	ITEM		Data[ITEM_MAX_NUM];
};

struct EFFECT
{
	int			UseFlag;
	int			Type;
	VECTOR		Position;
	float		Size;
	float		Move;
	int			ColorR, ColorG, ColorB;
	int			State;
	int			Counter;
};

struct EFFECTCOMMON
{
	int			DataNum;
	EFFECT		Data[EFFECT_MAX_NUM];
};

struct TEKISHOT
{
	int			UseFlag;
	int			HitFlag;
	int			Type;
	VECTOR		Position;
	VECTOR		Speed;
	int			State;
	int			Counter;
	float		CounterF;
};

struct TEKI
{
	int			UseFlag;
	int			HitFlag;
	int			ModelHandle;
	int			AttachAnimIndex;
	int			Fade;
	float		Opacity;
	int			Type;
	VECTOR		Position;
	float		AnimCounter;
	float		Speed;
	float		YAngle;
	int			Hp;
	int			Damage;
	int			State;
	int			Counter;
	int			MoveState;
	int			MoveCounter;
	int			Value;
};

struct TEKICOMMON
{
	int			ModelHandle;
	int			ShotGraphHandle;
	int			Shot2GraphHandle;
	int			CharNum;
	TEKI		Char[TEKI_MAX_NUM];
	int			ShotNum;
	TEKISHOT	Shot[TEKI_SHOT_MAX_NUM];
	VECTOR		CharCreatePosition;
	int			CharCreateRand;
	int			CharCreateState;
	int			CharCreateCounter;
};

struct PCSHOT
{
	int			UseFlag;
	int			Type;
	VECTOR		Position;
	VECTOR		Speed;
};

struct PC
{
	int			ModelHandle;
	VECTOR		Position;
	int			HitFlag;
	int			JumpFlag;
	float		SpeedY;
	int			Hp;
	int			Damage;
	int			MutekiTime;
	int			BrinkCounter;
	int			HpBlockAlpha[10];
	float		Opacity;
	int			EndCounter;
	int			ShotGraphHandle;
	int			ShotNum;
	PCSHOT		Shot[PC_SHOT_MAX_NUM];
	int			ShotPower;
	int			SubWeaponPower;
};

struct TITLE
{
	float		IconAngle;
	float		IconSin, IconCos;
	int			CursorPosition;
	float		CameraZ;
	float		CameraSpeed;
};

struct GAMEDATA
{
	int			StageModelHandle;
	int			ShadowGraphHandle;
	int			LifeGraphHandle;
	int			TitleGraphHandle;
	int			TitleSquareGraphHandle;
	int			ShotSoundHandle;
	int			DamageSoundHandle;
	int			PcDamageSoundHandle;
	int			PowerUpSoundHandle;
	int         TitleSoundHandle;
	VECTOR		CameraPosition;
	int			Level;
	int			PrevLevelPoint;
	int			NextLevelPoint;
	int			EnemyBreakNum;
	int			EnemyBreakPoint;
	int			Score;
	int			ScoreRate;
	int			HiScore;
	int			Scene;
	int			EndFlag;
	int			NowInput;
	int			EdgeInput;
};

extern void		ScoreUp(int Score);
extern void		ScoreUp_TekiBreak(int Score);
extern void		EnemyBreakPointUp(int BreakPoint);
extern void		ItemInitialize(void);
extern ITEM* ItemAdd(int Type);
extern void		ItemMove(void);
extern void		ItemDraw(void);
extern void		EffectInitialize(void);
extern EFFECT* EffectAdd(int Type, VECTOR Pos);
extern void		EffectMove(void);
extern void		EffectDraw(void);
extern int		TekiGlobalInitialize(void);
extern void		TekiGlobalTerminate(void);
extern void		TekiInitialize(void);
extern TEKI* TekiAdd(int Type, VECTOR Pos, int Hp);
extern void		TekiMove(void);
extern void		TekiDraw(void);
extern TEKISHOT* TekiShotAdd(int Type, VECTOR Pos);
extern void		TekiShotMove(void);
extern void		TekiShotDraw(void);
extern void		GameInitialize(void);
extern void		GameMove(void);
extern void		GameDraw(void);
extern void		TitleInitialize(void);
extern void		TitleMove(void);
extern void		TitleDraw(void);
extern void		DrawTitleTriangle(int x, int y);
extern void		DrawTitleSquare(int x, int y);
extern void		DrawTitleCircle(int x, int y);
extern int		PcInitialize(void);
extern void		PcMove(void);
extern void		PcDraw(void);
extern void		PcShotPowerUp(int UpPoint);
extern void		PcSubWeaponPowerUp(int UpPoint);
extern void		PcHpUp(int UpPoint);
extern PCSHOT* PcShotAdd(int Type, VECTOR Pos);
extern void		PcShotMove(void);
extern void		PcShotDraw(void);
extern int		PcShotTypeNum(int Type);
extern void		DrawUI(void);
extern void		DrawShadow(VECTOR Pos, float Size, int Alpha = 255);

#endif