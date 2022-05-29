// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "CircleComponent.h"
#include "Game.h"
#include "Laser.h"
#include "Asteroid.h"
#include <cstdio>

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
	,revivalTime(0.0f)
{
	// Create an animated sprite component
	msc = new SpriteComponent(this, 150);
	msc->SetTexture(game->GetTexture("Assets/ShipWithThrust.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(msc->GetTexWidth() * 0.75f);
}

void Ship::UpdateActor(float deltaTime)
{
	if (revivalTime <= 0.0f)
	{
		mLaserCooldown -= deltaTime;
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				GetGame()->RemoveSprite(msc);
				RemoveComponent(msc);
				revivalTime = 2.0f;
				break;
			}
		}
	}
	else {
		if (revivalTime - deltaTime <= 0) {
			GetGame()->AddSprite(msc);
			AddComponent(msc);
			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(0.0f);
		}
		revivalTime -= deltaTime;
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());
		
		mLaserCooldown = 0.5f;
	}
}