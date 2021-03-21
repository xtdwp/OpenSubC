#include "fuelRod.h"
#include <geometry.h>

//给定坐标位置与半径进行初始化
opensubc::fuelRod::fuelRod(unsigned _id, double _x, double _y, double _r) :id(_id), x(_x), y(_y), r(_r)
{
	originalId = opensubc::geometry::rodIdConverter[id];
}

opensubc::fuelRod::~fuelRod()
{

}

std::string opensubc::fuelRod::toString()const
{
	//打印开头
	std::string outputString;
	for (int i = 0; i < 10; ++i)
		outputString += "-";
	outputString += "fuelRod";
	for (int i = 0; i < 10; ++i)
		outputString += "-";
	outputString += "\n";

	//打印id及几何信息
	outputString += "id: " + std::to_string(id) + ", originalId: " + std::to_string(originalId) + "\n";
	outputString += "x: " + std::to_string(x) + ", y: " + std::to_string(y) + "\n";
	outputString += "r: " + std::to_string(r) + "\n";

	//打印子通道id
	outputString += "channelIds: ";
	for (auto& channelId : channelIds)
		outputString += std::to_string(channelId) + ", ";
	outputString.erase(outputString.size() - 2);
	outputString += "\n";

	//打印gap的id
	outputString += "gapIds: ";
	for (auto& gapId : gapIds)
		outputString += std::to_string(gapId) + ", ";
	outputString.erase(outputString.size() - 2);
	outputString += "\n";

	//打印结尾
	for (int i = 0; i < 27; ++i)
		outputString += "-";
	outputString += "\n";

	return outputString;
}
