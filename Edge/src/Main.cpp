#include <Edge\PCH.h>
#include <Edge\Registry.h>
#include <Edge\Win32.h>
#include <Edge\Crashes.h>
#include <Edge\Logger.h>
#include <Edge\Window.h>
#include <Edge\Gfx.h>

namespace Edge
{
	static void Entry()
	{
		Window window{ "Edge", 1280, 720 }; // TODO: hardcoded window dimensions and title
		Gfx::Device gfx_device{ window.Handle() };

		while (!window.ShouldClose())
		{
			window.Update();

			// render
			{
				Gfx::CommandList cmd_list{ gfx_device.GetCommandList(Gfx::CommandListType::Main) };
			}

			// present
			gfx_device.Present(false);

		}
	}
}

int main()
{
	Edge::Logger logger{};

	// add logger to registry
	{
		bool success{ Edge::Registry::Add(&logger) };
		Edge_AssertMsg(success, "Failed to register logger to registry");
	}

	try
	{
		Edge::Entry();
	}
	catch (const Edge::Crash& crash)
	{
		logger.Critical("Crash: {}", crash.what());
	}

	return 0;
}
