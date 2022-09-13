class Truck
{
public:
	char pad_0000[112]; //0x0000
	float ChassiDmg; //0x0070
	char pad_0074[132]; //0x0074
	float WheelDmg; //0x00F8
	char pad_00FC[36]; //0x00FC
	float EngineDmg; //0x0120
	float TransmissionDmg; //0x0124
	float CabinDmg; //0x0128
	float Fuel; //0x012C
	char pad_0130[344]; //0x0130
};