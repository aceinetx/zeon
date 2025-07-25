#include "zeon.hh"

int main() {
	z::Zeon zeon;
	{
		int r = zeon.Init();
		if (r != 0)
			return r;
	}
	zeon.Run();
	zeon.Cleanup();
}
