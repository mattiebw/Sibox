#pragma once

#include "Shape.h"
#include "Math/Quaternion.h"

struct Body;

struct BodyContact
{
	Vector3F WorldSpacePointOnA = {};
	Vector3F WorldSpacePointOnB = {};
	Vector3F BodySpacePointOnA  = {};
	Vector3F BodySpacePointOnB  = {};
	Vector3F NormalWorldSpace   = {};

	// Distance between the two bodies at the contact point. Negative means they are penetrating, positive means they are separating.
	f32 SeparationDistance = 0;
	f32 TimeOfImpact       = 0;

	Body *BodyA = nullptr, *BodyB = nullptr;
};

// The values here are so we can use this enum with qsort.
enum class FirstContact : s8
{
	ContactA = -1,
	ContactB = 0,
	SameTime = 1
};

struct Body
{
public:
	explicit Body(const Shape &shape)
		: Position(0, 0, 0),
		  Rotation(0, 0, 0, 1),
		  LinearVelocity(0, 0, 0),
		  AngularVelocity(0, 0, 0),
		  InverseMass(1.f),
		  Elasticity(1.f),
		  Friction(0.5f),
		  m_Shape(shape)
	{
	}

	Vector3F    Position;
	QuaternionF Rotation;
	Vector3F    LinearVelocity;
	Vector3F    AngularVelocity;
	f32         InverseMass;
	f32         Elasticity;
	f32         Friction;

	// Inlined for performance.
	FORCEINLINE void Update(f32 deltaSeconds)
	{
		Position += LinearVelocity * deltaSeconds;

		// Get our new world space positions.
		Vector3F worldSpaceCM = GetCenterOfMassWorldSpace();
		Vector3F centerOfMassToPosition = Position - worldSpaceCM;

		// Find the new angular velocity.
		// This bit is a little confusing.
		Matrix3x3F orientation = Rotation.ToMat3(); // Since our inertia tensor is a Matrix, we need our rotation in matrix form.
		Matrix3x3F inertiaTensor = orientation * m_Shape.GetInertiaTensor() * orientation.Transpose(); // See GetInverseInertiaTensorWorldSpace
		// Now we compute the angular acceleration. This is just Euler's equation of rigid body motion: α=I−1(ω×(Iω))
		Vector3F angularAcceleration = inertiaTensor.Inverse() * AngularVelocity.Cross(inertiaTensor * AngularVelocity);
		AngularVelocity += angularAcceleration * deltaSeconds;

		// Add our angular velocity - this is fairly simple:
		Vector3F angleChange = AngularVelocity * deltaSeconds;
		QuaternionF deltaQuat = QuaternionF(angleChange, angleChange.Length());
		Rotation = deltaQuat * Rotation; // Adding quaternions is multiplication
		Rotation.Normalize();

		// We update our position such that our center of mass remains in the same place relative
		// to where it was previously with the new rotation applied.
		Position = worldSpaceCM + deltaQuat.RotatePoint(centerOfMassToPosition);
	}
	
	NODISCARD const Shape& GetShape() const { return m_Shape; }
	NODISCARD Shape&       GetShape() { return m_Shape; }

	NODISCARD Vector3F WorldSpaceToBodySpace(const Vector3F &worldSpace) const;
	NODISCARD Vector3F BodySpaceToWorldSpace(const Vector3F &bodySpace) const;

	NODISCARD Vector3F GetCenterOfMassWorldSpace() const;
	NODISCARD Vector3F GetCenterOfMassBodySpace() const;

	NODISCARD Matrix3x3F GetInverseInertiaTensorWorldSpace() const;
	NODISCARD Matrix3x3F GetInverseInertiaTensorBodySpace() const;

	void ApplyImpulse(const Vector3F &impulsePoint, const Vector3F &impulse);
	void ApplyLinearImpulse(const Vector3F &impulse);
	void ApplyAngularImpulse(const Vector3F &angularImpulse);

	static bool Intersects(Body &bodyA, Body &bodyB, BodyContact &contact, f32 delta);

protected:
	Shape m_Shape;
};
