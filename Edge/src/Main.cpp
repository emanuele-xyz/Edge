#include <Edge\PCH.h>                                                                                                     
#include <Edge\Logger.h>                                                                                                     

namespace Edge
{
	static void Entry()
	{
		Logger logger{};
		logger.Debug("Debug from Edge!");
		logger.Info("Info from Edge!");
		logger.Warn("Warn from Edge!");
		logger.Error("Error from Edge!");
		logger.Critical("Critical from Edge!");
		logger.Debug("Formatted Debug from Edge! {}", 0);
		logger.Info("Formatted Info from Edge! {}", 1.1);
		logger.Warn("Formatted Warn from Edge! {}", false);
		logger.Error("Formatted Error from Edge! {}", "wow");
		logger.Critical("Formatted Critical from Edge! {}", true);
	}
}

int main()
{
	Edge::Entry();

	return 0;
}
