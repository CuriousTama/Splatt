#pragma once
#include "Lunar_GroundContainer.h"
#include "Controles.h"
#include "Tools.hpp"
#include "SoundManager.hpp"

#include "pch.h"

class Lander
{
protected:
	SoundBuffer mSb;
	Sound mSound;

	Texture mTexture;
	Sprite mSprite;
	Vector2f mPosition;
	Vector2f mVelocity;
	float mAngle;
	int mNbLifePoints;
	bool mEngineOn;
	bool mIsAlive;
	int mScore;
	float mFuel;
	bool mIsHardMode;
	float mSpeed;

public:
	Lander(RenderWindow &_window);

#pragma region "Lander_update"

	void Update(RenderWindow& _window, GroundContainer &_myContainer);
	void MoveRight();
	void MoveLeft();
	void Inpulse();
	void Landing(RenderWindow& _window, GroundContainer& _myContainer);
	void Collide(RenderWindow& _window, GroundContainer &_myContainer);
	void ResetPlayer();
	bool HasFuel();
	//void ChangeMode();
	void PlaySound(string _sound);

#pragma endregion

	void Display(RenderWindow &_window, Shader &_myShader);

	void Explode();
	~Lander();

#pragma region "Get/Set"

	float GetPosX();
	float GetPosY();
	Vector2f GetPos();
	float GetVelocityX();
	float GetVelocityY();
	float GetHeight();
	float GetWidth();
	int GetNbLife();
	Sprite GetSprite();
	FloatRect GetBound();
	bool GetIsAlive();
	int GetScore();
	float GetFuel();
	float GetAngle();
	bool GetHard();

#pragma endregion
	
};


