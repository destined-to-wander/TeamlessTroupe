#include "CollisionDetection.h"

bool OverlapCircle2Circle(const Vector3& pos1, float r1, const Vector3& pos2, float r2)
{
	Vector3 LengthV = (pos1- pos2);
	return (LengthV.LengthSquared() <= (r1 + r2) * (r1 + r2));
}

bool OverlapCircle2Circle(PhysicsObject& circle1, float r1, PhysicsObject& circle2, float r2, CollisionData& cd)
{
	Vector3 disp = circle1.pos - circle2.pos;
	if (disp.LengthSquared() <= (r1 + r2) * (r1 + r2)) {
		cd.pObj1 = &circle1;
		cd.pObj2 = &circle2;
		cd.penetration = (r1 + r2) - disp.Length();
		cd.collisionNormal = disp.Normalize();
		return true;
	}
	return false;
}

bool OverlapAABB2AABB(const Vector3& min1, const Vector3& max1,
					  const Vector3& min2, const Vector3& max2)
{
	return !(max1.x < min2.x || min1.x > max2.x || max1.z < min2.z || min1.z > max2.z);
}

bool OverlapAABB2AABB(PhysicsObject& box1, float w1, float h1,
					  PhysicsObject& box2, float w2, float h2, CollisionData& cd)
{
	Vector3 extent1(w1 * 0.5f, 0.f, h1 * 0.5f);
	Vector3 extent2(w2 * 0.5f, 0.f, h2 * 0.5f);

	Vector3 min1 = box1.pos - extent1;
	Vector3 max1 = box1.pos + extent1;
	Vector3 min2 = box2.pos - extent2;
	Vector3 max2 = box2.pos + extent2;

	// Check overlap in X and Z
	if (max1.x < min2.x || min1.x > max2.x || max1.z < min2.z || min1.z > max2.z)
		return false;

	// Calculate overlap distances
	float overlapX = std::min(max1.x, max2.x) - std::max(min1.x, min2.x);
	float overlapZ = std::min(max1.z, max2.z) - std::max(min1.z, min2.z);

	// Determine collision normal based on smallest penetration
	Vector3 CN;
	float pendist;
	if (overlapX < overlapZ) {
		pendist = overlapX;
		CN = (box2.pos.x - box1.pos.x > 0.f) ? Vector3(-1.f, 0.f, 0.f) : Vector3(1.f, 0.f, 0.f);
	}
	else {
		pendist = overlapZ;
		CN = (box2.pos.z - box1.pos.z > 0.f) ? Vector3(0.f, 0.f, -1.f) : Vector3(0.f, 0.f, 1.f);
	}

	cd.pObj1 = &box1;
	cd.pObj2 = &box2;
	cd.penetration = pendist;
	cd.collisionNormal = CN;

	return true;
}

bool OverlapCircle2Line(const Vector3& circlePos, float radius,
						const Vector3& lineStart,
						const Vector3& lineEnd)
{
	Vector3 lineTangent = lineEnd - lineStart;
	if (lineTangent.IsZero()) return false;  //this is not a line, but a point. reject collision test

	float lineLength = lineTangent.Length();
	lineTangent.Normalize();

	Vector3 LineNormal(-lineTangent.y, lineTangent.x, 0.f);

	float shortestdist = abs((circlePos - lineStart).Dot(LineNormal));

	if (shortestdist > radius)
	{
		return false;
	}
	else {
		float projectedDist = (circlePos - lineStart).Dot(lineTangent);
		if (projectedDist > lineLength) {
			projectedDist = lineLength;
		}
		if (projectedDist < 0.f) {
			projectedDist = 0.f;
		}
		//This takes the pos of lineStart and adds the distance from linestart to the closest point.
		//Since lineTangent = 1 in direction of the line, * projDist would turn it into the distane along the line the closest point is.
		Vector3 closestPoint = lineStart + lineTangent * projectedDist;
		return (circlePos - closestPoint).Length() < radius;
	}
}


bool OverlapCircle2OBB(PhysicsObject& circle, float radius, PhysicsObject& box, float w, float h, CollisionData& cd)
{
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	Vector3 disp = circle.pos - box.pos;
	disp.y = 0.f;

	float rad = Math::DegreeToRadian(box.angleDeg);

	float c = cos(rad);
	float s = sin(rad);

	// World -> Local
	Vector3 localDisp;
	localDisp.x = disp.x * c + disp.z * s;
	localDisp.z = -disp.x * s + disp.z * c;

	// Clamp to box
	Vector3 closestLocal;
	closestLocal.x = std::max(-halfW, std::min(localDisp.x, halfW));
	closestLocal.z = std::max(-halfH, std::min(localDisp.z, halfH));
	closestLocal.y = 0.f;

	// Local -> World
	Vector3 closestWorld;
	closestWorld.x = closestLocal.x * c - closestLocal.z * s;
	closestWorld.z = closestLocal.x * s + closestLocal.z * c;
	closestWorld += box.pos;
	closestWorld.y = 0.f;

	// Distance test
	Vector3 diff = circle.pos - closestWorld;
	diff.y = 0.f;

	float distSq = diff.LengthSquared();

	if (distSq <= radius * radius)
	{
		float dist = sqrt(distSq);

		cd.pObj1 = &circle;
		cd.pObj2 = &box;
		cd.penetration = radius - dist;

		cd.collisionNormal =
			(dist > 0.0001f) ? diff.Normalize() : Vector3(1, 0, 0);

		return true;
	}

	return false;
}

void ResolveCollision(CollisionData& cd)
{
	PhysicsObject& obj1 = *cd.pObj1;
	PhysicsObject& obj2 = *cd.pObj2;

	float invMass1 = (obj1.mass == 0.f) ? 0.f : 1.f / obj1.mass;
	float invMass2 = (obj2.mass == 0.f) ? 0.f : 1.f / obj2.mass;
	float totalInvMass = invMass1 + invMass2;

	if (totalInvMass == 0.f) return;

	obj1.pos += cd.collisionNormal * (cd.penetration * (invMass1 / totalInvMass));
	obj2.pos -= cd.collisionNormal * (cd.penetration * (invMass2 / totalInvMass));

	Vector3 vel1 = obj1.vel;
	Vector3 vel2 = obj2.vel;

	Vector3 r1 = cd.contactPoint - obj1.pos;
	Vector3 r2 = cd.contactPoint - obj2.pos;

	if (obj1.angularVel != 0.f)
	{
		float w = Math::DegreeToRadian(obj1.angularVel);
		Vector3 angVel1(-w * r1.y, w * r1.x, 0.f);
		vel1 += angVel1 * 0.05f;
	}

	if (obj2.angularVel != 0.f)
	{
		float w = Math::DegreeToRadian(obj2.angularVel);
		Vector3 angVel2(-w * r2.y, w * r2.x, 0.f);
		vel2 += angVel2 * 0.05f;
	}

	Vector3 relVel = vel1 - vel2;
	float velAlongNormal = relVel.Dot(cd.collisionNormal);
	if (velAlongNormal > 0.f) return;
	float bounciness = std::min(obj1.bounciness, obj2.bounciness);

	obj1.vel += (-(1.f + bounciness) * velAlongNormal) / totalInvMass * cd.collisionNormal * invMass1;
	obj2.vel -= (-(1.f + bounciness) * velAlongNormal) / totalInvMass * cd.collisionNormal * invMass2;
}

void ResolveCircle2StaticCircle(PhysicsObject& ball1, float radius1, PhysicsObject& ball2, float radius2)
{
	//1) resolve penetration
	Vector3 offset = ball1.pos - ball2.pos;
	float pd = (radius1 + radius2) - offset.Length();
	offset.Normalize();
	ball1.pos += offset * pd;
	//2) resolve velocity
	//3) implement bounciness
	ball1.vel = ball1.vel - (2 * ball1.bounciness * (ball1.vel.Dot(offset) * offset));
}


void ResolveCircle2StaticLine(PhysicsObject& ball, float radius, const Vector3& lineStart, const Vector3& lineEnd)
{
	Vector3 lineTangent = lineEnd - lineStart;
	float lineLength = lineTangent.Length();
	lineTangent.Normalize();

	float penetrationDist{};
	Vector3 normal{};
	Vector3 lineVec = ball.pos - lineStart;
	float projectedDist = lineVec.Dot(lineTangent);

	//ball is to the "right" of lineEnd
	if (projectedDist > lineLength)
	{
		//TODO:
		//1) calculate the collision normal
		Vector3 CollisionNormal = (ball.pos - lineEnd).Normalize();
		//2) calculate the penetration distance
		float ShortestDist = abs((ball.pos - lineEnd).Dot(CollisionNormal));

		normal = CollisionNormal;
		penetrationDist = radius - ShortestDist;
	}
	//ball is to the "left" of lineStart
	else if (projectedDist < 0)
	{
		//TODO:
		//1) calculate the collision normal
		Vector3 CollisionNormal = (ball.pos - lineStart).Normalize();
		//2) calculate the penetration distance
		float ShortestDist = abs((ball.pos - lineStart).Dot(CollisionNormal));

		normal = CollisionNormal;
		penetrationDist = radius - ShortestDist;
	}
	//ball is within line segment
	else
	{
		//TODO:
		//1) calculate the collision normal
		Vector3 CollisionNormal = Vector3(-lineTangent.y, lineTangent.x, 0).Normalize();
		//2) calculate the penetration distance
		float ShortestDist = abs((ball.pos - lineStart).Dot(CollisionNormal));

		normal = CollisionNormal;
		penetrationDist = radius - ShortestDist;
	}

	//resolve penetration and velocity
	ball.pos += normal * penetrationDist;
	ball.vel -= ball.vel.Dot(normal) * normal;

}

