#include "Core/Base.h"
#include "Core/Application.h"

int main(int argc, char** argv)
{
	Log::Init();
	NE_INFO("Initialized Log!");

	Application app;
	app.Run();

	return 0;
}