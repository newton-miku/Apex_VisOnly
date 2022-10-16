#ifndef _AIMASSIST_CPP
#define _AIMASSIST_CPP
#pragma once
#include "Header Files/aimassist.hpp"
#include "Header Files/time.h"
#include "Header Files/solver.hpp"
#include "glow.h"
#include <chrono>
using namespace Util;
using namespace std;

bool IsZooming(uint32_t base){
	uint32_t a = read<uint32_t>(base + OFFSET_ZOOMING);
	if ((a & 0x0000ff00) != 0)
		return true;
	return false;
}
void mouse_move(int dx, int dy) {
	/*
	INPUT input{ INPUT_MOUSE };
	input.mi = MOUSEINPUT{ dx, dy, 0, MOUSEEVENTF_MOVE, 0, 0 };
	SendInput(1, &input, sizeof(INPUT));
	/**/
	if ((GetAsyncKeyState(Config::AimbotKey_Code[Config::AimbotKey]) || GetKeyState(VK_CAPITAL))&&farGoal) {
		//cout << "AIM Yes" << endl;
		INPUT inputs[1] = {};
		ZeroMemory(inputs, sizeof(inputs));

		inputs[0].type = INPUT_MOUSE;
		inputs[0].mi.dwFlags = MOUSEEVENTF_MOVE;
		inputs[0].mi.dx = dx;
		inputs[0].mi.dy = dy;

		int nSize = ARRAYSIZE(inputs);
		UINT uSent;
		uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
	}
}
static const int TICKRATE = 1000;
void AimAssistThread() {
	AimAssist aim{};
	init_time();
	while (true)
	{
	aim.run();
	}
}
void AimAssist::run() {
	while (true)
	{
		double time = get_time();
		// Rate limit the aimbot to 60 times per sec
		if (time < next_tick) {
			//cout << "Time:"<< time<<"tick:"<<next_tick << endl;
			continue;
		}
		// Fixup timer to maintain 60 times per sec
		const double step_time = 1.0 / TICKRATE;
		next_tick = get_time() < next_tick + step_time ? next_tick + step_time : get_time() + step_time;

		const auto local = Offset::localEntity;
		//if (GetKeyState(config.aim_key) && local)
		//if((GetKeyState(Config::AimbotKey_Code[Config::AimbotKey]) || GetKeyState(VK_CAPITAL))&&local)
		if (local)
		{
			track(local);
		}
		else {
			target_entity = 0;
			target_locked = false;
		}
	}
}

void AimAssist::track(uint64_t local) {
	// If we were locked onto a target, only find a new target when sufficient time has passed
	// Else find a new target if we don't already have one
	const bool new_target = target_locked ? get_time() > target_time + config.idle_time : !(target_entity != 0);
	player target;
	// Find a new target if desired
	if (new_target) {
		target = find_target(local);
		if (target.Entity) {
			target_entity = target.Entity;
			target_locked = true;
			pidx.reset();
			pidy.reset();
		}
	}

	// Aim at the target if we have one
	if (target.Entity) {
		trackEntity = target.Entity;
		TargetInfo info{};
		info.origin = GetEntityBasePosition(target.Entity);
		info.velocity = Util::GetVelocity(target.Entity);
		info.bone_pos = Util::GetEntityBonePosition(target.Entity, config.aim_bone);
		if (validate(local, target, info, target.i) && !teleported(new_target, info)) {
			// Target is valid, aim at the target
			const float fov_scale = get_fov_scale(local, target);
			aim(info, fov_scale);
			// Update the time we've last seen this target
			target_time = get_time();
		}
		else {
			// Target is no longer valid, drop it but keep the locked flag
			target_entity = 0;
		}
	}
}
bool AimAssist::teleported(bool new_target, TargetInfo& info) {
	// If we only just acquired the target it did not teleport
	bool teleported = !new_target && Vector3::distance(target_pos, info.hit) > (config.teledist*2);
	// Keep track of the target's position
	// Use bone position to ensure smooth transitions
	//target_pos = info.hit;
	info.hit= target_pos;
	//cout << "teleported:" << teleported << endl;
	return teleported;
}

const player AimAssist::find_target(uint64_t local) {
	player target;
	Rank rank = Rank::Low;
	float priority = 99999999.0f;
	// Consider every player a target
	for (int i = 0; i < Offset::players.size(); i += 1) {
		target = Offset::players[i];
		if (Offset::localTeamID == target.teamid) {
			continue;
		}
		if (target.dead) {
			//cout << "dead" << endl;
			continue;
		}
		player candidate = Offset::players[i];
		if (candidate.Entity) {
			// If this candidate target is a valid target
			TargetInfo info;
			/*
			Vector3 a = GetEntityBasePosition(target.Entity);
			info.origin = a ;
			info.velocity = Util::GetVelocity(target.Entity);
			info.bone_pos = Util::GetEntityBonePosition(target.Entity, config.aim_bone);
			//cout << "a:" << target.Entity<<a.x << a.y << a.z << endl;
			//cout << "CAM:" << info.bone_pos.x << info.bone_pos.y << info.bone_pos.z << endl;
			/**/
			if (validate(local, candidate, info, target.i)) {
				// With a higher priority
				if (info.rank > rank || info.rank == rank && info.priority < priority) {
					// Consider this player the best target
					rank = info.rank;
					priority = info.priority;
					target = candidate;
				}
			}
		}
		trackI = i;
	}
	return target;
}

bool AimAssist::validate(uint64_t local, player target, TargetInfo& info, int i) const {
	if (!rules(local, target, info, i)) {
		//cout << "rules" << endl;
		return false;
	}
	if (!hitpoint(local, target, info)) {
		//cout << "hitpoint" << endl;
		return false;
	}
	if (!compute(local, target, info)) {
		//cout << "compute" << endl;
		return false;
	}
	if (!fov_check(local, target, info)) {
		//cout << "fov" << endl;
		return false;
	}
	info.priority = info.angle + logf(info.distance);
	return true;
}
bool AimAssist::rules(uint64_t local, player target, TargetInfo& info, int i) const {
	if (const auto player = target.Entity) {
		if (local == player) {
			//cout << "me"<<endl;
			return false;
		}
		//if (Util::IsVisible(player, i)) {
		entNewVisTime1 = read<float>(target.Entity + OFFSET_VISIBLE_TIME);
		if (entNewVisTime1 == entOldVisTime1[i]) {
			//cout << "unvis" << endl;
			return false;
		}
		entOldVisTime1[i] = entNewVisTime1;
		//cout << "vis" << endl;
	
		if (Offset::localTeamID == target.teamid) {
		//cout << "TEAM" << endl;
		return false;
		}
		if (target.dead) {
		//cout << "dead" << endl;
		return false;
		}
		info.rank = target.down ? Rank::Low : Rank::Normal;
		return true;
	}
	/*else if (const auto base_npc = dynamic_cast<const BaseNPCEntity*>(target)) {
		if (!base_npc->is_visible(state.curtime)) {
			return false;
		}
		info.rank = Rank::Low;
		return true;
	}*/
	else {
		return false;
	}
}
bool AimAssist::hitpoint(uint64_t local, player target, TargetInfo& info) const {
	if (1) {
		
		info.origin = GetEntityBasePosition(target.Entity);
		info.velocity = Util::GetVelocity(target.Entity);
		info.bone_pos = Util::GetEntityBonePosition(target.Entity, config.aim_bone);
		/**/
		//cout << "CAM:" << info.origin.x << info.origin.y << info.origin.z << endl;
		return true;
	}
	/*
	else if (const auto base_npc = dynamic_cast<const BaseNPCEntity*>(target)) {
		info.origin = base_npc->origin;
		info.velocity = {};
		info.bone_pos = base_npc->get_bone_pos(config.aim_bone);
		return true;
	}/**/
	else {
		info.origin = {};
		info.velocity = {};
		info.bone_pos = {};
		return false;
	}
}
bool AimAssist::compute(uint64_t local, player& target, TargetInfo& info) const {
	//Offset::localCam = read<Vector3>(local + OFFSET_ORIGIN);
	Offset::localCam = read<Vector3>(local + OFFSET_CAMERAPOS);
	info.distance = Vector3::distance(info.origin, Offset::localCam);
	if (info.distance / 34 > 50) farGoal = true;
	else farGoal = false;
	//cout << "CAM:" << Offset::localCam.x << Offset::localCam.y << Offset::localCam.z << endl;

	// Projectile aimbot calculations :)
	auto WeaponEntity = GetWeaponEntity(local);
	if (WeaponEntity) {
		projectile_speed = read<float>(WeaponEntity + OFFSET_BULLET_SPEED);
		 projectile_scale = read<float>(WeaponEntity + OFFSET_BULLET_GRAVITY);
		if (projectile_speed > 1.0f) {
			Vector3 a = GetEntityBonePosition(target.Entity, config.aim_bone);
			LinearPredictor predictor{ a, GetVelocity(target.Entity) };
			Solution sol;
			//cout << "A:" << a.x << "\t" << a.y << "\t" << a.z << endl;
			if (solve(Offset::localCam, WeaponEntity, predictor, sol)) {
				info.time = sol.time;
				info.aim = Vector3{ -rad2deg(sol.pitch), rad2deg(sol.yaw), 0.0f };
				return true;
			}
			else {
				//cout << "computeERR" << endl;
				return false;
			}
		}
	}

	// Hitscan weapons and others fall back to aiming at the target.
	info.time = 0.0f;
	info.aim = (info.hit - Offset::localCam).to_angles().norm_angles();
	//cout << "hit:" << info.hit.x << "\t" << info.hit.y << "\t" << info.hit.z << endl;
	//info.aim = (GetEntityBasePosition(target.Entity) - Offset::localCam).to_angles().norm_angles();
	return true;
}
bool AimAssist::fov_check(uint64_t local, player& target, TargetInfo& info) const {
	Offset::localCamAngles = read<Vector3>(local + OFFSET_CAMERAANG);
	info.pitch = info.aim.x - Offset::localCamAngles.x;
	info.yaw = info.aim.y - Offset::localCamAngles.y;
	while (info.yaw <= -180.0f) info.yaw += 360.0f;
	while (info.yaw > 180.0f) info.yaw -= 360.0f;
	info.angle = sqrt(info.pitch * info.pitch + info.yaw * info.yaw);
	const float fov = get_fov(target) * get_fov_scale(local, target);
	if(info.angle < fov)
		return true;
	//cout << "fov:" << fov << "ang" << info.angle << endl;
	return false;
}

float AimAssist::get_fov(player& target) const {
	return  (target.Entity != 0) && target_locked ? config.fov_drop : config.fov_aim;
}
float AimAssist::get_fov_scale(uint64_t local, player& target) {
	if (IsZooming(local)) {
		if (uint64_t weapon = GetWeaponEntity(local)) {
			float target_zoom_fov = read<float>(weapon + OFFSET_ZOOM_FOV);
			//cout << "zoom:"<<target_zoom_fov << endl;
			if (target_zoom_fov != 0.0f && target_zoom_fov != 1.0f) {
				return target_zoom_fov / 90.0f;
			}
		}
	}
	return 1.0f;
}

void AimAssist::aim_pid(const TargetInfo& info, float fov_scale, float& dx, float& dy) {
	const float dt = 1.0f / TICKRATE;
	const float strength = 1.0f / fov_scale;
	//cout << info.yaw << "\t" << info.pitch<<"\t" << fov_scale << endl;
	dx = -pidx.step(info.yaw * strength, dt, config.pid_config);
	dy = pidy.step(info.pitch * strength, dt, config.pid_config);
}
void AimAssist::aim_smooth(const TargetInfo& info, float fov_scale, float& dx, float& dy) {
	// Avoid aim jitter with a minimum angle
	if (info.angle < config.fov_min * fov_scale) {
		dx = 0.0f;
		dy = 0.0f;
	}
	// Magic aim smoothing formula :)
	const float aim_strength = config.aim_strength;
	const float speed = logf(aim_strength + info.angle / (fov_scale * fov_scale) * aim_strength) * aim_strength + aim_strength;
	dx = -info.yaw * speed;
	dy = info.pitch * speed;
}
void AimAssist::aim(const TargetInfo& info, float fov_scale) {
	//cout << "almost" << endl;
	float dx, dy;
	if (config.pid_enable) {
		aim_pid(info, fov_scale, dx, dy);
	}
	else {
		aim_smooth(info, fov_scale, dx, dy);
	}
	// Moving the mouse can only be done in whole steps
	// Keep track of the delta 'residue' and add it next time
	dx += addx;
	dy += addy;
	//cout << dx << "\t" << dy << endl;
	const int mdx = static_cast<int>(dx);
	const int mdy = static_cast<int>(dy);
	addx = dx - static_cast<float>(mdx);
	addy = dy - static_cast<float>(mdy);
	if (mdx != 0 || mdy != 0) {
		//cout << mdx << "\t" << mdy << endl;
		mouse_move(mdx, mdy);
	}
}

#endif