#include "zeon.hh"

using namespace z;

int z::g_argc;
char** z::g_argv;
z::Zeon* z::g_zeon;

int main(int argc, char** argv) {
	g_argc = argc;
	g_argv = argv;

	CefMainArgs args(argc, argv);
	int result = CefExecuteProcess(args, nullptr, nullptr); // Create CEF Processes
	if (result >= 0) {
		return result;
	}

	g_zeon = new Zeon();
	g_zeon->cef_args = args;
	{
		int r = g_zeon->Init();
		if (r != 0)
			return r;
	}
	g_zeon->Run();
	g_zeon->Cleanup();
	delete g_zeon;
}
