// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include <unistd.h>
#include "TestApp.h"

TestApp::TestApp( const std::string& workingDirectory )
		: Application( workingDirectory )
{}

int TestApp::Main()
{
	OpenStandardStreams("./stdout.log", "./stderr.log");

	for( int i = 0; i < 2; ++i )
	{
		std::cout << "Daemon " << GetDaemonPid() << std::endl;
		sleep(1);
	}

	return 0;
}
