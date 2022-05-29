#include "Asteroid.h"
#include "Math.h"
#include "Random.h"
#include "Game.h"
#include "MoveComponent.h"
#include "SpriteComponent.h"
#include "CircleComponent.h"

Asteroid::Asteroid(Game* game) :
	Actor(game)
	, mMass(20.0)
{
	// Initialize random position & orientation
	Vector2 randPos = Random::GetVector(Vector2::Zero,
		Vector2(1024.0f, 768.0f));
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// Create sprite componetn, set texture
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));

	// Create a move component, set forward speed
	mc = new MoveComponent(this);
	mc->SetForwardSpeed(150.0f);
	
	// Create a circle component (for collision)
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(sc->GetTexWidth() * 0.8f);

	game->AddAsteroid(this);
}

void Asteroid::UpdateActor(float deltaTime)
{
	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCircle, *(ast->GetCircle())) && (mCircle != ast->GetCircle()))
		{
			Vector2 un = Vector2((mCircle->GetCenter().x)-(ast->GetCircle()->GetCenter().x),
				(mCircle->GetCenter().y) - (ast->GetCircle()->GetCenter().y));
			un.Normalize();
			Vector2 ut = Vector2(-un.y, un.x);

			Vector2 vel1 = mc->GetForwardSpeed() * GetForward();
			Vector2 vel2 = ast->GetMoveComponent()->GetForwardSpeed() * ast->GetForward();

			float v1n = Vector2::Dot(un, vel1);
			float v1t = Vector2::Dot(ut, vel1);
			float v2n = Vector2::Dot(un, vel2);
			float v2t = Vector2::Dot(ut, vel2);

			vel1 = ((v1n * static_cast<float>(GetMass() - ast->GetMass()) + 2.0f * ast->GetMass() * v2n) / (GetMass() + ast->GetMass())) * un + v1t * ut;
			vel2 = ((v2n * static_cast<float>(ast->GetMass() - GetMass()) + 2.0f * GetMass() * v1n) / (GetMass() + ast->GetMass())) * un + v2t * ut;

			mc->SetForwardSpeed(vel1.Length());
			ast->GetMoveComponent()->SetForwardSpeed(vel2.Length());

			SetRotation(Math::Atan2(-vel1.y, vel1.x));
			ast->SetRotation(Math::Atan2(-vel2.y, vel2.x));
		}
	}
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}