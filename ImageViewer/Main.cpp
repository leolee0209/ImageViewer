#include "WindowControl.h"
#include "EventHandle.h"
#include "FileControl.h"
#include "Logger.h"

using namespace std;
namespace ev = EventHandle;

int main(int argc, char* argv[])
{
	NOTE("program starts.");
	if (argc < 2) {
		return 1;
	}
	LoggerInit();
	WindowControl windowC;
	FileControl fileC( &windowC);
	if (!windowC.InitWindow(&fileC))
	{
		ERROR("Failed to initialize!");
		return 1;
	}
	fileC.InitFile(argv[1]);

	windowC.UpdateImage();

	bool quit = false;

	while (!quit)
	{
		quit = ev::HandleEvents(&windowC, &fileC);
	}

	windowC.close();
	fileC.close();

	return 0;
}