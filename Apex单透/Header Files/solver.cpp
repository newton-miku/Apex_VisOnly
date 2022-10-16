#pragma once
#include "solver.hpp"
#include <stdint.h>
#include <cmath>
#include <iostream>
#include "../offset.h"
using namespace std;


Vector3 get_projectile_fire_setup(const Vector3& origin, const Vector3& target) {
	return target - origin;
}

Vector3 LinearPredictor::predict_position(float time) const {
	//cout << "targetO:" << origin.x << "\t" << origin.y << "\t" << origin.z << endl;
	//cout << "targetV:" << velocity.x << "\t" << velocity.y << "\t" << velocity.z << endl;
	return origin + velocity * time;
}

// https://en.wikipedia.org/wiki/Projectile_motion#Angle_%7F%27%22%60UNIQ--postMath-0000003A-QINU%60%22%27%7F_required_to_hit_coordinate_(x,y)
static bool optimal(float x, float y, float v0, float g, float& pitch) {
	double root = pow(v0,4) - g * (g * x * x + 2.0f * y * v0 * v0);
	if (root < 0.0f) {
		//cout << "root:" << root << endl;
		return false;
	}
	pitch = atan((v0 * v0 - sqrt(root)) / (g * x));
	return true;
}

static bool solve2d(const Vector3& origin, uint32_t weapon, const Vector3& target, Solution& sol) {
	const auto v = get_projectile_fire_setup(origin, target);
	const float dx = sqrt(pow(v.x,2) + pow(v.y,2));
	const float dy = v.z;
	const float v0 = projectile_speed;
	const float g = (750.0f/ projectile_scale);
	if (!optimal(dx, dy, v0, g, sol.pitch)) {
		//cout << "v:" << dx << "\t" << dy << "\t" << v.z <<endl << "optimal:" << v0 << "\t" << g << endl;
		return false;
	}
	sol.time = dx / (cos(sol.pitch) * v0);
	sol.yaw = atan2(v.y, v.x);
	return true;
}

bool solve(const Vector3 origin, uint32_t weapon, const TargetPredictor& target, Solution& sol) {
	static const float MAX_TIME = 1.0f;
	static const float TIME_STEP = 1.0 / 256.0f;
	for (float target_time = 0.0f; target_time <= MAX_TIME; target_time += TIME_STEP) {
		const auto target_pos = target.predict_position(target_time);
		if (!solve2d(origin, weapon, target_pos,sol)) {
			cout << "solve2dErr" << endl;
			return false;
		}
		if (sol.time < target_time) {
			return true;
		}
	}
	return false;
}
