#pragma once


#include "common.h"


static Vec2 util_multiply_vec2(Vec2 v1, float val) {

	return (Vec2) { v1.x * val, v1.y * val };
}



static Vec2 util_divide_vec2(Vec2 v1, float val) {

	return (Vec2) { v1.x / val, v1.y / val };
}



static Vec2 util_add_vec2(Vec2 v1, Vec2 v2) {

	return (Vec2) { v1.x + v2.x, v1.y + v2.y };
}



static Vec2 util_sub_vec2(Vec2 v1, Vec2 v2) {

	return (Vec2) { v1.x - v2.x, v1.y - v2.y };
}



static float util_dot_vec2(Vec2 v1, Vec2 v2) {

	return v1.x * v2.x + v1.y * v2.y;
}



static float util_mag_vec2(Vec2 v1) {

	return sqrt(v1.x * v1.x + v1.y * v1.y);
}



static Vec2 util_normalize_vec2(Vec2 v1) {

	float mag = util_mag_vec2(v1);
    if (mag == 0)
        return (Vec2) { 0, 0 };

	return (Vec2) { v1.x / mag, v1.y / mag };
}



static Vec2 util_vec2_normal(Vec2 v1) {

    return util_normalize_vec2(SBRE_VEC2(-v1.y, v1.x));
}





