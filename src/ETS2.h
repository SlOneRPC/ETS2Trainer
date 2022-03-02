class Truck
{
public:
	char pad_0000[104]; //0x0000
	float ChassiDmg; //0x0068
	char pad_006C[132]; //0x006C
	float WheelDmg; //0x00F0
	char pad_00F4[36]; //0x00F4
	float EngineDmg; //0x0118
	float TransmissionDmg; //0x011C
	float CabinDmg; //0x0120
	float Fuel; //0x0124
}; 