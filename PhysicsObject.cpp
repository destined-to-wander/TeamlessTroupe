#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
	: pos{}, vel{}, accel{}, mass{1.f}, m_totalForces{},
	  angularVel{}, angleDeg{}, bounciness{1.f}
{
}

void PhysicsObject::AddForce(const Vector3& force)
{
	if (mass == 0) {
		return;
	}
	m_totalForces += force;
	m_totalForces.Normalize();
	m_totalForces *= 50;
}

void PhysicsObject::AddImpulse(const Vector3& impulse)
{
	if (mass == 0) {
		return;
	}
	vel = (Vector3(0, 0, 0));
	angularVel = 0;
	m_totalForces += impulse;
	//m_totalForces *= 500;
}

void PhysicsObject::UpdatePhysics(float dt)
{
	//If mass = 0, obj immovable
	Vector3 finalAccell = accel;
	if (mass > 0) {
		finalAccell += m_totalForces * (1.f/mass);
	}

	vel += finalAccell * dt;
	pos += vel * dt;

	angleDeg += angularVel * dt;

	m_totalForces.SetZero();
}
