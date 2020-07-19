class Truck
{
public:
	char pad_0000[24]; //0x0000
	class TruckParts* partsPTR; //0x0018
	char pad_0020[256]; //0x0020
	float wheelDmg; //0x0120
	char pad_0124[28]; //0x0124
	float Fuel; //0x0140
}; //Size: 0x0144

class TruckParts
{
public:
	char pad_0000[8]; //0x0000
	class truckPart* chassisPTR; //0x0008
	class truckPart* cabinPTR; //0x0010
	char pad_0018[8]; //0x0018
	class truckPart* enginePTR; //0x0020
	class truckPart* transPTR; //0x0028
}; //Size: 0x0088

class truckPart
{
public:
	char pad_0000[16]; //0x0000
	float damage; //0x0010
}; //Size: 0x0014