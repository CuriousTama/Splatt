#include "Lunar_player.h"

Lander::Lander(RenderWindow& _window)
{
	if (!mTexture.loadFromFile("../ressources/Lunar_lander/Lander.png"))
		exit(EXIT_FAILURE);

	mSprite.setTexture(mTexture);
	mSprite.setScale(Vector2f(2.0f, 2.0f));

	mPosition = Vector2f(150, 150);
	mAngle = 0;
	mEngineOn = false;
	mVelocity = Vector2f(0, 0);

	mSprite.setPosition(mPosition);
	mSprite.setTextureRect(IntRect(0, 0, mTexture.getSize().x / 3, mTexture.getSize().y));
	mSprite.setOrigin(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2);

	mSpeed = 3;
	mScore = 0;
	mIsAlive = true;
	mIsHardMode = false;
	mNbLifePoints = 3;
	mFuel = 100000;
}

#pragma region "Lander_update"

void Lander::Update(RenderWindow& _window, GroundContainer& _myContainer)
{
	if (mPosition.x - GetWidth() / 2 <= 0 || mPosition.x + GetWidth() / 2 >= _window.getSize().x)
		Explode();

	if (mPosition.y - GetHeight() / 2 <= 0 || mPosition.y + GetHeight() / 2 >= _window.getSize().y)
		Explode();

	if (mIsAlive && HasFuel())
	{

		getSound("engine1").stop();

		//ChangeMode();

		mVelocity.y += .1f * MainTime.GetTimeDeltaF();

		MoveRight();
		MoveLeft();
		Inpulse();

		mPosition += mVelocity;
		mSprite.setPosition(mPosition);
		mSprite.setRotation(mAngle);
		Collide(_window, _myContainer);
	}
	else
	{
		if (mNbLifePoints <= 0 || !HasFuel())
		{
			_myContainer.DeathScreen(_window);

			mIsAlive = false;

			if (isButtonPressed(Action::Lunar_Select))
				_myContainer.ResetLevel();
		}
		else
			ResetPlayer();
	}
}

void Lander::MoveRight()
{
	if (isButtonPressed(Action::Lunar_Droite))
	{
		getSound("engine1").play();

		if (!mIsHardMode)
			mVelocity.x += .4f * MainTime.GetTimeDeltaF();
		else
			mAngle += 90 * MainTime.GetTimeDeltaF();

		mFuel -= 1.5f;
	}
}

void Lander::MoveLeft()
{
	if (isButtonPressed(Action::Lunar_Gauche))
	{
		getSound("engine1").play();
		if (!mIsHardMode)
			mVelocity.x -= .4f * MainTime.GetTimeDeltaF();
		else
			mAngle -= 90 * MainTime.GetTimeDeltaF();

		mFuel -= 1.5f;
	}
}

void Lander::Inpulse()
{

	if (isButtonPressed(Action::Lunar_Impulse))
	{
		getSound("engine1").play();

		if (!mIsHardMode)
			mVelocity.y -= .4f * MainTime.GetTimeDeltaF();
		else
		{
			float angle_rad = mAngle * 180 / pi;

			float forceX = cos(angle_rad) * (mSpeed * MainTime.GetTimeDeltaF());
			float forceY = sin(angle_rad) * (mSpeed * MainTime.GetTimeDeltaF());

			mVelocity.x += forceX;
			mVelocity.y += forceY;
		}

		mEngineOn = true;
		mSprite.setTextureRect(IntRect(mTexture.getSize().x / 3, 0, mTexture.getSize().x / 3, mTexture.getSize().y));
		mFuel -= 1.5f;
	}

	if (mEngineOn && !isButtonPressed(Action::Lunar_Impulse))
	{
		mEngineOn = false;
		
		mSprite.setTextureRect(IntRect(0, 0, mTexture.getSize().x / 3, mTexture.getSize().y));
	}
}

void Lander::Landing(RenderWindow& _window, GroundContainer& _myContainer)
{
	mPosition = Vector2f(50, 50);

	mScore += 100;
	mVelocity.x = 0.0f;
	mVelocity.y = 0.0f;

	if (_myContainer.GetLvl() == 5)
		_myContainer.SetNbLvl(1);
	else
		_myContainer.GoToNextLvl(_window);
}

void Lander::Collide(RenderWindow& _window, GroundContainer& _myContainer)
{
	for (int i = 0; i <= mSprite.getLocalBounds().width; i++)
	{
		for (int j = 0; j <= mSprite.getLocalBounds().height; j++)
		{
			float x = mSprite.getPosition().x - mSprite.getOrigin().x + i;
			float y = mSprite.getPosition().y - mSprite.getOrigin().y + j;

			if (_myContainer.GetImage().getPixel(x, y) == Color::White || _myContainer.GetImage().getPixel(x, y) == Color::Green && mVelocity.y > .15f)
				Explode();

			if (_myContainer.GetImage().getPixel(x, y) == Color::Green && mVelocity.y < .15f && mIsAlive)
				Landing(_window, _myContainer);
		}
	}
}

void Lander::ResetPlayer()
{
	mNbLifePoints--;

	if (!mTexture.loadFromFile("../ressources/Lunar_lander/Lander.png"))
		exit(EXIT_FAILURE);

	mSprite.setTexture(mTexture);
	mSprite.setScale(Vector2f(2.0f, 2.0f));

	mPosition = Vector2f(50, 50);
	mAngle = 0;
	mEngineOn = false;
	mVelocity = Vector2f(0, 0);

	mSprite.setPosition(mPosition);
	mSprite.setTextureRect(IntRect(0, 0, mTexture.getSize().x / 3, mTexture.getSize().y));
	mSprite.setOrigin(mSprite.getLocalBounds().width / 2, mSprite.getLocalBounds().height / 2);
	mSprite.setRotation(mAngle);

	mIsAlive = true;
}

bool Lander::HasFuel()
{
	if (mFuel <= 0)
	{
		mIsAlive = false;

		return false;
	}

	return true;
}

//void Lander::ChangeMode()
//{
//	if (Keyboard::isKeyPressed(Keyboard::M))
//		mIsHardMode = !mIsHardMode;
//}

void Lander::PlaySound(string _sound)
{
	if (!mSb.loadFromFile("../ressources/Lunar_lander/" + _sound + ".wav"))
		exit(EXIT_FAILURE);

	mSound.setBuffer(mSb);
	mSound.play();
	mSound.stop();
}

#pragma endregion

void Lander::Display(RenderWindow& _window, Shader& _myShader)
{
	_window.draw(mSprite);
}

void Lander::Explode()
{
	//PlaySound("Lunar_explode");

	mVelocity.x = 0.0f;
	mVelocity.y = 0.0f;

	if (!mTexture.loadFromFile("../ressources/Lunar_lander/Lander_explode.png"))
		exit(EXIT_FAILURE);

	mSprite.setPosition(mPosition);
	mSprite.setTexture(mTexture);
	mSprite.setScale(Vector2f(2.0f, 2.0f));
	mSprite.setTextureRect(IntRect(0, 0, mTexture.getSize().x, mTexture.getSize().y));
	mSprite.setOrigin(mSprite.getGlobalBounds().width / 2, mSprite.getGlobalBounds().height / 2);

	mIsAlive = false;
}



Lander::~Lander()
{

}

float Lander::GetPosX()
{
	return mPosition.x;
}

float Lander::GetPosY()
{
	return mPosition.y;
}

Vector2f Lander::GetPos()
{
	return mPosition;
}

float Lander::GetVelocityX()
{
	return mVelocity.x;
}

float Lander::GetVelocityY()
{
	return mVelocity.y;
}

float Lander::GetHeight()
{
	return mSprite.getGlobalBounds().height;
}

float Lander::GetWidth()
{
	return mSprite.getGlobalBounds().width;
}

int Lander::GetNbLife()
{
	return mNbLifePoints;
}

Sprite Lander::GetSprite()
{
	return mSprite;
}

FloatRect Lander::GetBound()
{
	return mSprite.getGlobalBounds();
}

bool Lander::GetIsAlive()
{
	return mIsAlive;
}

int Lander::GetScore()
{
	return mScore;
}

float Lander::GetFuel()
{
	return mFuel;
}

float Lander::GetAngle()
{
	return mAngle;
}

bool Lander::GetHard()
{
	return mIsHardMode;
}
