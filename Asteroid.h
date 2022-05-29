#pragma once

#include "Actor.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	void UpdateActor(float deltaTime) override;
	~Asteroid();

	float GetMass() { return mMass; }
	class MoveComponent* GetMoveComponent() { return mc; }
	class CircleComponent* GetCircle() { return mCircle; }
private:
	float mMass;
	class MoveComponent* mc;
	class CircleComponent* mCircle;
};