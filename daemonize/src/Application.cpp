// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include "daemonize/Application.h"

using namespace daemonize;

int Application::Run()
{
	// Simply run the main method
	return Main();
}

int Application::RunAsDaemon()
{
	int result = 1;

	Daemon::ProcessIdentity identity;
	bool res = myDaemonizer.Daemonize( myWorkingDirectory, identity );

	if( res )
	{
		if( identity == Daemon::ProcessIdentity::DAEMON_PROCESS )
		{
			result = Main(); // Call the main method
		}
		else
		{
			// Original or intermediate process
			result = 0;
		}
	}

	return result;
}

Application::Application( const std::string& workingDirectory )
		: myWorkingDirectory( workingDirectory )
{

}

