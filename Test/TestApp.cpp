// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include <chrono>
#include <thread>
#include "TestApp.h"

using namespace std::chrono_literals;
using namespace std;

TestApp::TestApp( const std::string& workingDirectory )
		: Application( workingDirectory )
{}

int TestApp::Main()
{
	OpenStandardStreams("./stdout.log", "./stderr.log");

	for( int i = 0; i < 2; ++i )
	{
		cout << "Daemon " << GetDaemonPid() << std::endl;
		this_thread::sleep_for( 1s );
	}

	return 0;
}
