#include "Deepstar.h"

#define ORBITER_MODULE

Deepstar::Deepstar(OBJHANDLE hVessel, int fmodel): VESSEL3(hVessel, fmodel)
{
	
}

Deepstar::~Deepstar()
{

}

void Deepstar::DefineAnimations()
{
	rot_speed = 14.242 / 360.0;
	static UINT wheel[7] = {2,3,4,5,38,39,61};
	static MGROUP_ROTATE hab
	(
		0,
		wheel,7,
		_V(0,0,0),
		_V(0,0,1),
		(float)(2.0*PI)
	);
	anim_centrifuge = CreateAnimation(0.0);
	AddAnimationComponent(anim_centrifuge,0,1,&hab);

	static UINT group_door_l[1] = {51}; //50 and 51
	static MGROUP_ROTATE an_d_l
	(
		0,
		group_door_l,1,
		_V(0.55,0,-4.81),
		_V(0,-1,0),
		(float)(PI/1.6) //PI/1.6 fits nicely inside the railing
	);

	anim_door_l = CreateAnimation(0);
	AddAnimationComponent(anim_door_l,0,1,&an_d_l);

	static UINT group_door_r[1] = {50}; //50 and 51
	static MGROUP_ROTATE an_d_r
	(
		0,
		group_door_r,1,
		_V(0.55,0,-4.81),
		_V(0,-1,0),
		(float)(PI/1.6) //PI/1.6 fits nicely inside the railing
	);

	anim_door_r = CreateAnimation(0);
	AddAnimationComponent(anim_door_r,0,1,&an_d_r);
}

void Deepstar::clbkSetClassCaps(FILEHANDLE cfg)
{
	AddMesh("Deepstar2.1\\Deepstar2.1");
	DefineAnimations();

	rot_speed=4.7/360.0;
	rot_angle=0.0;
	door_rot_angle=0.0;
	state_cent=true;
	state_door=false;

	SetSize(174.25);
	SetEmptyMass(2.0e6);
	SetPMI(_V(14828.39,14755.26,472.94));
	SetCrossSections(_V(10903.83,9335.47,2480.70));

	SetupPropulsion(cfg);
}

void Deepstar::clbkPreStep(double SimT, double SimDT, double MJD)
{
	//Centrifuge things
	if (!state_cent)
	{
		if (rot_speed > 0.0)
			rot_speed -= 0.0001*SimDT;
		if (rot_speed < 0.0)
			rot_speed = 0.0;
	}

	else if (state_cent)
	{
		if (rot_speed < (4.7 / 360))
			rot_speed += 0.0002*SimDT;
		else if (rot_speed > (4.7 / 360))
			rot_speed = (4.7 / 360);
	}

	if (state_cent)
	{
		double s = SimDT * rot_speed;
		rot_angle += s;
		if (rot_angle > 1.0)
			rot_angle -= floor(rot_angle);
		SetAnimation(anim_centrifuge,rot_angle);
	}

	//Front left door things
	if(!state_door)
	{
		if(door_rot_angle>0)
			door_rot_angle -= SimDT*0.2;
		SetAnimation(anim_door_l,door_rot_angle);
	}
	else if(state_door)
	{
		if(door_rot_angle<1)
			door_rot_angle += SimDT*0.2;
		SetAnimation(anim_door_l,door_rot_angle);
	}

	//Absolute Killrot by Enjo
	VECTOR3 avel;
	if (GetNavmodeState (NAVMODE_KILLROT))  // check if Killrot autopilot is initiated
	{
		OBJHANDLE objPlanet = GetSurfaceRef();  // Grab the closest planet
		const ATMCONST *atm = oapiGetPlanetAtmConstants (objPlanet);  // Get atmospheric parameters
		double CutoffAlt=0;
		if (atm!=NULL) 	CutoffAlt = atm->altlimit;  // if pointer is valid assign a value of max ATM alt
		if (CutoffAlt > 10e15) CutoffAlt=0; // sanity check
		if (GetAltitude() > CutoffAlt)    
		// ^ Check if you are above atmosphere to eliminate deliberate killrot holding in ATM
		{
			GetAngularVel(avel);  // get current angular velocity
			if (sqrt(avel.data[0]*avel.data[0] + avel.data[1]*avel.data[1] + avel.data[2]*avel.data[2] ) < 0.007 )
			// ^ check if current ang. vel. is small, ie. if killrot is finishing the manoeuvre
			{
			for (int i = 0; i < 3; i++) 
				avel.data[i] = 0.0;	 // set all angular velocity axes to 0

			SetAngularVel (avel);   // perform Der Uber Killrot
			}
		}
	}
}

int Deepstar::clbkConsumeBufferedKey(DWORD key, bool down, char *kstate)
{
	if(!down)
		return 0;

	if(!KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //CLEAN
	{
		switch (key)
		{
		case OAPI_KEY_D:
			if(state_door)
				state_door=false;
			else
				state_door=true;
			return 1;
		}
	}
	else if (KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //CTRL
	{
		
	}
	else if (!KEYMOD_CONTROL(kstate) && KEYMOD_SHIFT(kstate) && !KEYMOD_ALT(kstate)) //SHIFT
	{
		
	}
	else if (!KEYMOD_CONTROL(kstate) && !KEYMOD_SHIFT(kstate) && KEYMOD_ALT(kstate)) //ALT
	{
		
	}
	return 0;
}

DLLCLBK VESSEL *ovcInit (OBJHANDLE hvessel, int fmodel)
{
	return new Deepstar (hvessel, fmodel);
}

DLLCLBK void ovcExit (VESSEL *vessel)
{
	if (vessel) delete (Deepstar*)vessel;
}