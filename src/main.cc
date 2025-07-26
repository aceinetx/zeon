#include "zeon.hh"

int z::g_argc;
char** z::g_argv;

int main(int argc, char** argv) {
	z::g_argc = argc;
	z::g_argv = argv;

	CefMainArgs args(argc, argv);
	int result = CefExecuteProcess(args, nullptr, nullptr);
	if (result >= 0) {
		return result;
	}

	z::Zeon zeon;
	zeon.cef_args = args;
	{
		int r = zeon.Init();
		if (r != 0)
			return r;
	}
	zeon.Run();
	zeon.Cleanup();
}
