#include "zeon.hh"

int z::g_argc;
char** z::g_argv;
z::Zeon* z::g_zeon;

int main(int argc, char** argv) {
	z::g_argc = argc;
	z::g_argv = argv;

	CefMainArgs args(argc, argv);
	int result = CefExecuteProcess(args, nullptr, nullptr);
	if (result >= 0) {
		return result;
	}

	z::g_zeon = new z::Zeon();
	z::g_zeon->cef_args = args;
	{
		int r = z::g_zeon->Init();
		if (r != 0)
			return r;
	}
	z::g_zeon->Run();
	z::g_zeon->Cleanup();
}
