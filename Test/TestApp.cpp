// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include "TestApp.h"

using namespace std::chrono_literals;
using namespace std;


TestApp::TestApp( const std::string& workingDirectory, const std::string& daemonPidFile )
		: Application( workingDirectory ), myDaemonPidFile( daemonPidFile )
{
}

TestApp::TestApp( const std::string& workingDirectory )
		: TestApp( workingDirectory, "" )
{}

int TestApp::Main()
{
	OpenStandardStreams( "./stdout.log", "./stderr.log" );

	if( myDaemonPidFile.length() > 0 )
	{
		ofstream f( "./daemon.pid", ios::binary );
		int pid = GetDaemonPid();
		f.write( reinterpret_cast<char*>( &pid ), sizeof( pid ) );
		f.close();

		// Run until sigterm is received
		while( !PollSignal( SIGTERM ) )
		{
			this_thread::sleep_for( 100ms );
		}

		cout << "Received SIGTERM" << std::endl;

		std::string dummy{ "Dummy" };
		ofstream out( "./sigterm.signal", ios::trunc );
		out.write( dummy.c_str(), dummy.length() );
		out.close();
	}
	else
	{
		for( int i = 0; i < 2; ++i )
		{
			cout << "Daemon " << GetDaemonPid() << std::endl;
			this_thread::sleep_for( 1s );
		}
	}

	return 0;
}

