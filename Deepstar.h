#include "Orbitersdk.h"

class Deepstar: public VESSEL3
{
public:
	Deepstar(OBJHANDLE hVessel, int fmodel);
	~Deepstar();

	void DefineAnimations();
	void SetupPropulsion(FILEHANDLE cfg);
	void clbkSetClassCaps(FILEHANDLE cfg);
	void clbkPreStep(double SimT, double SimDT, double mjd);
	int clbkConsumeBufferedKey(DWORD key, bool down, char *kstate);
private:
	int anim_centrifuge, anim_door_l, anim_door_r;
	double rot_speed;
	double rot_angle;
	bool state_cent;
	bool state_door;
	double door_rot_angle;

	PROPELLANT_HANDLE prop_main, prop_acs;
	THRUSTER_HANDLE th_fus, th_rot[14], th_trans[4], group[2];
};