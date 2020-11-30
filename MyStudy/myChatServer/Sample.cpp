#include "myNetwork.h"

int main()
{
	myNetwork myNet;
	if (myNet.InitNetwork() == false)
	{
		return 0;
	}
	myNet.Run();
	myNet.DeleteNetwork();

	return 0;
}