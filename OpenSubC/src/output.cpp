#include <output.h>
#include "geometry.h"
#include "calculations.h"
#include <AxialMomentumEquation.h>

void opensubc::output(double t)
{
	using namespace std;
	using namespace geometry;
	using namespace calculation;
	for (auto& channel : channels)
	{
		cout << "t=" << t << ",channelId=" << channel.id << ",OriginalId=" << channel.originalId << endl;
		cout << setiosflags(ios_base::left) << setw(15) << "h" << setw(15) << "rho" << setw(15) << "P" << setw(15) << "m" << setw(15) << "T" << setw(15) << "Tw"<<endl;
		for (unsigned int i = channel.id*(numOfBlocks+(long long)1); i < ((channel.id+(long long)1) * (numOfBlocks + (long long)1)); ++i)
		{
			cout << setiosflags(ios_base::left) << setw(15) << h.coeffRef(i) << setw(15) << rho.coeffRef(i) << setw(15) << P.coeffRef(i) << setw(15) << m.coeffRef(i) << setw(15) << T.coeffRef(i) << setw(15) << Tw.coeffRef(i)<<endl;
		}

		cout << endl << endl;
	}

}
