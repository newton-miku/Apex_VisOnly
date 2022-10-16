#ifndef _AIMASSIST_HPP
#define _AIMASSIST_HPP
#pragma once
#include "pid.hpp"
#include <cstdint>
#include "../offset.h"
#include <windows.h>
//#include "../function.h"

struct AimAssistConfig{
	// Configuration
	bool enable = true;
	// Hold down this key to activate aimbot.
	int aim_key = VK_RBUTTON;
	// Strength of the aim assist, input values around 1.5 to 2.5.
	float aim_strength = 2.0f;
	// Which bone index to aim at.
	uint32_t aim_bone = 7;
	// When a target becomes invalid, wait this many seconds before trying to find a new target.
	float idle_time = 0.3f;
	// Stop aiming when this close to a target to prevent mouse jitter.
	float fov_min = 0.25f;
	// Angle required to lock onto a target.
	float fov_aim = 10.0f;
	// When locked onto a target, angle required to drop the target.
	float fov_drop = 25.0f;
	// If a target moves more than this distance in a single tick then it is considered teleported and will be dropped.
	float teledist = 72.0f;
	// Enable the PID controller for aim smoothing.
	bool pid_enable = false;
	// The PID controller configuration.
	PidConfig pid_config = { 2.0, 10.0, 0.0 };
};

// Target priority by rank.
enum class Rank {
	// Downed players are still targetted but at a lower priority.
	Low,
	// Regular players are always targetted before downed players.
	Normal,
};

// This structures represents intermediate data calculated when validating a target.
typedef struct TargetInfo{
	// Target origin in the world.
	Vector3 origin;
	// Target velocity for linear prediction.
	Vector3 velocity;
	// Target bone position in the world we're trying to hit.
	Vector3 bone_pos;

	// Position in the world we're trying to aim at.
	Vector3 hit;
	// Aim angle required to hit the position we're trying to shoot.
	Vector3 aim;
	// Distance to the target.
	float distance;
	// Projectile travel time required to hit the target.
	float time;

	// Delta angle from local player's current viewangles to the target.
	float angle;
	// Delta pitch and yaw in individual components.
	float pitch, yaw;

	// Priority calculated for this target.
	float priority;

	// Rank given for this target.
	Rank rank;
};

class AimAssist{
public:
	AimAssist() = default;
	void run();

	// Updates the tracking state and aims at the aqcuired targets.
	void track(uint64_t local);
	bool teleported(bool new_target,TargetInfo& info);

	// Finds a target to aim at, returns nullptr if no valid target was found.
	const player find_target(uint64_t local);

	// Checks if this target is valid to aim at.
	bool validate(uint64_t local, player target, TargetInfo& info, int i) const;
	bool rules(uint64_t local, player target, TargetInfo& info, int i) const;
	bool hitpoint(uint64_t local, player target, TargetInfo& info) const;
	bool compute(uint64_t local, player& target, TargetInfo& info) const;
	bool fov_check(uint64_t local, player& target, TargetInfo& info) const;

	// Gets the aim fov.
	float get_fov(player& target) const;
	// Gets the scalar for the aim fov, used when zoomed in with a scoped weapon.
	static float get_fov_scale(uint64_t local, player& target);

	// Moves the mouse towards the target.
	void aim_pid(const TargetInfo& info, float fov_scale, float& dx, float& dy);
	void aim_smooth(const TargetInfo& info, float fov_scale, float& dx, float& dy);
	void aim(const TargetInfo& info, float fov_scale);

private:
	double next_tick = 0.0f;

	// Aiming state
	float addx = 0.0f;
	float addy = 0.0f;
	PidController pidx;
	PidController pidy;

	// Keep track of the current target
	bool target_locked = false;
	uint32_t target_entity;
	double target_time = 0.0;
	Vector3 target_pos;

public:
	AimAssistConfig config;
};
#endif