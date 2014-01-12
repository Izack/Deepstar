#include "Deepstar.h"

void Deepstar::SetupPropulsion(FILEHANDLE cfg)
{
	prop_main   = CreatePropellantResource(1.3e7); //hydrogen 13 000 000

	prop_acs    = CreatePropellantResource(812.5e3); //Monoprop

	//Fusion drive thruster
	double fus_thrust = 20e6;
	double fus_isp    = 1.00e5;

	//Monopropellant thruster x4
	double tr_thrust = 80.0e4;
	double tr_isp    = 5.3e3;
	//Aerojet R-40A: Thrust: 3.87 kN Ve: 3002.778 m/s
	//Fictional thruster: 20kN Ve: 3.0km/s

	th_fus = CreateThruster(_V(0,0,-193.49),_V(0,0,1),fus_thrust,prop_main,fus_isp);
	SURFHANDLE fus_tex = oapiRegisterExhaustTexture("Perseus_IEx");
	CreateThrusterGroup(&th_fus,1,THGROUP_MAIN);
	AddExhaust(th_fus,1000,8,_V(0,0,-210),_V(0,0,-1),fus_tex); //oapiRegisterExhaustTexture("deepex")
	//AddExhaust(th_fus,60,6,_V(0,0,-195),_V(0,0,-1));
	
	th_trans[0] = CreateThruster(_V(0.00, 12.11,135.16),_V(0,0,1),tr_thrust,prop_acs,tr_isp); //FRONT TOP
	th_trans[1] = CreateThruster(_V(0.00,-12.11,135.16),_V(0,0,1),tr_thrust,prop_acs,tr_isp); //FRONT BOTTOM
	th_trans[2] = CreateThruster(_V( 12.11,0.00,135.16),_V(0,0,1),tr_thrust,prop_acs,tr_isp); //FRONT RIGHT
	th_trans[3] = CreateThruster(_V(-12.11,0.00,135.16),_V(0,0,1),tr_thrust,prop_acs,tr_isp); //FRONT LEFT

	CreateThrusterGroup (th_trans, 4, THGROUP_ATT_BACK);

	th_trans[0] = CreateThruster(_V(0.00, 6.16,-175.29),_V(0,0,-1),tr_thrust,prop_acs,tr_isp); //BACK TOP
	th_trans[1] = CreateThruster(_V(0.00,-6.16,-175.29),_V(0,0,-1),tr_thrust,prop_acs,tr_isp); //BACK BOTTOM
	th_trans[2] = CreateThruster(_V( 6.16,0.00,-175.29),_V(0,0,-1),tr_thrust,prop_acs,tr_isp); //BACK RIGHT
	th_trans[3] = CreateThruster(_V(-6.16,0.00,-175.29),_V(0,0,-1),tr_thrust,prop_acs,tr_isp); //BACK LEFT

	CreateThrusterGroup (th_trans, 4, THGROUP_ATT_FORWARD);

	                                                                                                    //Z X or Z Y POSITION, NOT DIRECTION
	th_rot[ 0] = CreateThruster(_V(  0.00, 13.51, 131.59),_V( 0,-1, 0),tr_thrust,prop_acs,tr_isp); //FRONT TOP
	th_rot[ 1] = CreateThruster(_V(  0.00,-13.51, 131.59),_V( 0, 1, 0),tr_thrust,prop_acs,tr_isp); //FRONT BOTTOM
	th_rot[ 2] = CreateThruster(_V( 13.51,  0.00, 131.59),_V(-1, 0, 0),tr_thrust,prop_acs,tr_isp); //FRONT RIGHT alternate pos: 13.49,1.01,131.59
	th_rot[ 3] = CreateThruster(_V(-13.51,  0.00, 131.59),_V( 1, 0, 0),tr_thrust,prop_acs,tr_isp); //FRONT LEFT

	th_rot[ 4] = CreateThruster(_V(  0.00,  3.14,-181.38),_V( 0,-1, 0),tr_thrust,prop_acs,tr_isp); //BACK TOP
	th_rot[ 5] = CreateThruster(_V(  0.00, -3.14,-181.38),_V( 0, 1, 0),tr_thrust,prop_acs,tr_isp); //BACK BOTTOM
	th_rot[ 6] = CreateThruster(_V(  3.14,  0.00,-181.38),_V(-1, 0, 0),tr_thrust,prop_acs,tr_isp); //BACK RIGHT
	th_rot[ 7] = CreateThruster(_V( -3.14,  0.00,-181.38),_V( 1, 0, 0),tr_thrust,prop_acs,tr_isp); //BACK LEFT

	th_rot[ 8] = CreateThruster(_V(  0.00, 13.33,  74.12),_V(-1, 0, 0),tr_thrust,prop_acs,tr_isp); //MIDDLE TOP pointing RIGHT
	th_rot[ 9] = CreateThruster(_V(  0.00, 13.33,  74.12),_V( 1, 0, 0),tr_thrust,prop_acs,tr_isp); //MIDDLE TOP pointing LEFT
	th_rot[10] = CreateThruster(_V(  0.00,-13.33,  74.12),_V(-1, 0, 0),tr_thrust,prop_acs,tr_isp); //MIDDLE BOTTOM pointing RIGHT
	th_rot[11] = CreateThruster(_V(  0.00,-13.33,  74.12),_V( 1, 0, 0),tr_thrust,prop_acs,tr_isp); //MIDDLE BOTTOM pointing LEFT

	group[0] = th_rot[1];
	group[1] = th_rot[4];//
	CreateThrusterGroup (group, 2, THGROUP_ATT_PITCHUP);

	group[0] = th_rot[0];
	group[1] = th_rot[5];//
	CreateThrusterGroup (group, 2, THGROUP_ATT_PITCHDOWN);

	group[0] = th_rot[3];
	group[1] = th_rot[6];//
	CreateThrusterGroup (group, 2, THGROUP_ATT_YAWRIGHT);

	group[0] = th_rot[2];
	group[1] = th_rot[7];//
	CreateThrusterGroup (group, 2, THGROUP_ATT_YAWLEFT);

	group[0] = th_rot[9];
	group[1] = th_rot[10];//
	CreateThrusterGroup (group, 2, THGROUP_ATT_BANKRIGHT);

	group[0] = th_rot[8];
	group[1] = th_rot[11];//
	CreateThrusterGroup (group, 2, THGROUP_ATT_BANKLEFT);
}