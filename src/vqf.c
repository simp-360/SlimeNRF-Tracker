#include "globals.h"
#include "util.h"

#include "../vqf-c/src/vqf.h"

void vqf_init(float *q, float *g_off, unsigned int rate)
{
	// TODO: should rate be time instead?
	// TODO: load vqf state from retained
	// TODO: store vqf state to retained on sensor write
	float time = 1.0f / rate;
	initVqf(time, 0, 0);
//	setState(state);
	// TODO: how to load a quat? (quat stored with state)
	vqf_set_gyro_bias(g_off); // TODO: gyro bias was stored with state
}

void vqf_update_accel(float *a, float time)
{
	// TODO: time unused?
	updateAcc(a);
}

void vqf_update(float *g, float *a, float *m, float time)
{
	// TODO: time unused?
	updateGyr(g);
	updateAcc(a);
	updateMag(m);
}

void vqf_get_gyro_bias(float *g_off)
{
	getBiasEstimate(g_off);
}

void vqf_set_gyro_bias(float *g_off)
{
	setBiasEstimate(g_off, -1);
}

void vqf_update_gyro_sanity(float *g, float *m)
{
	// TODO: does vqf tell us a "recovery state"
	return;
}

int vqf_get_gyro_sanity(void)
{
	// TODO: does vqf tell us a "recovery state"
	return 0;
}

void vqf_get_lin_a(float *lin_a)
{
	float q[4] = {0};
	vqf_get_quat(q);
    
	float vec_gravity[3] = {0};
	vec_gravity[0] = 2.0f * (q[1] * q[3] - q[0] * q[2]);
	vec_gravity[1] = 2.0f * (q[2] * q[3] + q[0] * q[1]);
	vec_gravity[2] = 2.0f * (q[0] * q[0] - 0.5f + q[3] * q[3]);

	float *a = getState().lastAccLp;
	for (int i = 0; i < 3; i++)
	{
		lin_a[i] = a[i] -vec_gravity[i];
	}
}

void vqf_get_quat(float *q)
{
	getQuat9D(q);
}