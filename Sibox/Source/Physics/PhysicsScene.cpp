#include "siboxpch.h"
#include "Physics/PhysicsScene.h"

#include "Core/Application.h"

void PhysicsScene::Update()
{
	f64 delta = Application::Get()->GetDeltaTime();

	for (Body *body : m_Bodies)
	{
		// Values we'll be using for the physics simulation.
		// We use inverse masses to represent infinite masses as an inverse mass of 0 - so, we could get a division by 0
		// here! Obviously, this'll cause a NaN, and then we'll get NaN propogation - however, we return out of ApplyLinearImpulse
		// if our inverse mass is 0, so that won't happen.
		const f32 mass = 1.0f / body->InverseMass;

		// Apply gravity
		Vector3F gravity = Vector3F{0.0f, -9.81f, 0.0f} * mass * delta;
		body->ApplyLinearImpulse(gravity);

		// Apply linear velocity
		body->Position += body->LinearVelocity * static_cast<float>(delta);
	}
}

void PhysicsScene::AddBody(Body *body)
{
	m_Bodies.push_back(body);
}

void PhysicsScene::RemoveBody(Body *body)
{
	for (s32 i = static_cast<s32>(m_Bodies.size()) - 1; i >= 0; i--)
	{
		if (m_Bodies[i] == body)
		{
			m_Bodies.erase(m_Bodies.begin() + i);
			break;
		}
	}
}
