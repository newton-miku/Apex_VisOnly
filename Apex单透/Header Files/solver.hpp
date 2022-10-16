#pragma once
#include "../vector.h"

float projectile_scale;
float projectile_speed;

class ProjectileWeapon {
public:
	virtual float get_projectile_speed() const = 0;
	virtual float get_projectile_gravity() const = 0;
};
Vector3 get_projectile_fire_setup(const Vector3& origin, const Vector3& target);
class TargetPredictor {
public:
	virtual Vector3 predict_position(float time) const = 0;
};

// Simple linear extrapolation.
class LinearPredictor : public TargetPredictor {
public:
	inline LinearPredictor(Vector3 origin, Vector3 velocity) : origin(origin), velocity(velocity) {}
	Vector3 predict_position(float time) const override;

public:
	Vector3 origin;
	Vector3 velocity;
};

struct Solution {
	// Aim the weapon at these angles to hit the target.
	// NOTE! These angles are in radians and not normalized to the game's conventions!
	float pitch, yaw;
	// Projectile travel time.
	float time;
};

// Given a projectile weapon and a predictable target, solve where to aim the weapon to thit the target.
// NOTE! Solution is returned in radians! Don't forget to convert to degrees if needed!
bool solve(const Vector3 origin, uint32_t weapon, const TargetPredictor& target, Solution& sol);