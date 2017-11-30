// Spark.cpp: implementation of the ASpark class.
//
//////////////////////////////////////////////////////////////////////

#include "aSpark.h"
#include <math.h>

#ifndef GRAVITY
#define GRAVITY 9.8f
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ASpark::ASpark()
{
	//coefficients of restitution equals 0.25
	m_COR = 0.25f;
	m_mass = 1.0;
}

ASpark::ASpark(float* color): AParticle()
{
	for (int i = 0; i < 3; i++)
		m_color[i] = color[i];
 
	//coefficients of restitution equals 0.25
	m_COR = 0.25f;
}

ASpark::~ASpark()
{

}

//Set attractor position
void ASpark::setAttractor(vec3 position)
{
	m_attractorPos = position;
}

//Set repeller position
void ASpark::setRepeller(vec3 position)
{
	m_repellerPos = position;
}

void ASpark::setWind(vec3 wind)
{
	m_windForce = wind;
}

void ASpark::display()
{
	float fadeTime = 3.0;
	if (m_alive)
	{
		float alpha = 1.0;
		if (m_state[10] < fadeTime)
		{
			alpha = m_state[10] / 10.0f;
		}
		float scale = 1.0;

		glPushMatrix();
		glColor4f(m_color[0], m_color[1], m_color[2], alpha);
		glTranslatef(m_state[0], m_state[1], m_state[2]);
		glScalef(scale, scale, scale);
		glutSolidSphere(1.0, 10, 10);
		glPopMatrix();
	}

}
	


void ASpark::update(float deltaT, int extForceMode)
{
	m_deltaT = deltaT;
	if (m_state[10] <= 0.0)
	{
		m_alive = false;
		return;
	}

	if (!(extForceMode & EXT_SPARKFORCES_ACTIVE))
		extForceMode = 0;
	
	computeForces(extForceMode);
	
	updateState(deltaT, EULER);

	resolveCollisions();
	
	
}


 
void ASpark::computeForces(int extForceMode)
//	computes the forces applied to this spark
{
	// zero out all forces
	m_state[6] = 0.0;
	m_state[7] = 0.0;
	m_state[8] = 0.0;

	// gravity force
	addForce(m_mass*m_gravity);


	// wind force
	if (extForceMode & WIND_ACTIVE)
	{
		//TODO: Add your code here
		addForce(m_windForce); 

	

	}

	if (extForceMode & DRAG_ACTIVE)
	{
		//TODO: Add your code here
		// use equation
		vec3 vel = vec3(m_state[3], m_state[4], m_state[5]); 
		double velocity_squared = vel.SqrLength(); 
		vec3 drag_force = -(0.5) * (1.29) * velocity_squared * 6.4 * (4 * M_PI * 0.1 * 0.1) *  vel.Normalize(); 
		addForce(drag_force); 
	}


	// attractor force
	if (extForceMode & ATTRACTOR_ACTIVE)
	{
		//TODO: Add your code here
		vec3 dist = m_attractorPos - m_Pos;
		float distLength = dist.Length(); 
		dist.Normalize();

		// take random number and divide by distLength ^2 all mutlipled by dist
		vec3 attractor_force = (90000 / (distLength * distLength)) * dist; 
		addForce(attractor_force);
	}

	// repeller force
	if (extForceMode & REPELLER_ACTIVE)
	{
		//TODO: Add your code here
		vec3 dist = m_Pos - m_repellerPos;
		float distLength = dist.Length();
		dist.Normalize();

		// take random number and divide by distLength ^2 all mutlipled by dist
		vec3 repeller_force = (90000 / (distLength * distLength)) * dist;
		addForce(repeller_force);

	}

	// random force
	if (extForceMode & RANDOM_ACTIVE)
	{
		//TODO: Add your code here

		vec3 force; 
		force[0] = rand() / RAND_MAX; 
		force[1] = rand() / RAND_MAX; 
		force[2] = rand() / RAND_MAX;  
		force.Length(); 
		vec3 direction = force.Normalize(); 
		vec3 rand_force = force * direction; 

		addForce(rand_force); 

	}

}

void ASpark::resolveCollisions()
// resolves collisions of the spark with the ground
{
	//TODO: Add  code here that reverses the y value of the spark velocity vector when the y position value of the spark is < 0
	if (m_Pos[1] < 0) {
		m_Vel[1] = -m_Vel[1]; 
	}

}
