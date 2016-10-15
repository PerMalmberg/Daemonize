// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.


#include <string>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <fstream>
#include <signal.h>
#include <iostream>
#include "Catch/include/catch.hpp"

using namespace std;
using namespace std::chrono_literals;

extern std::vector<std::string> cmdArgs;

int Execute( const string& cmd, vector<std::string>& args, string& output )
{
	int result = 0;

	std::string c( cmd );
	for( auto& arg : args )
	{
		c += " \"" + arg + "\""; // Add quotation marks around arguments to handle spaces in arguments.
	}


	char buffer[128];
	output = "";
	{
		FILE* file = popen( c.c_str(), "r" );
		if( file )
		{
			while( !feof( file ) )
			{
				if( fgets( buffer, 128, file ) != NULL )
				{
					output += buffer;
				}
			}

			auto code = pclose( file );
			result = WEXITSTATUS( code );
		}
		else
		{
			result = errno;
		}
	}
	return result;
}

long GetFileSize( const std::string& file )
{
	long res = 0;

	FILE* f = fopen( file.c_str(), "r" );
	if( f )
	{
		fseek( f, 0, SEEK_END );
		res = ftell( f );
		fclose( f );
	}

	return res;
}

void ClearLogs()
{
	unlink( "./stdout.log" );
	unlink( "./stderr.log" );
}

SCENARIO( "Execution of command works" )
{
	GIVEN( "A function that can execute command" )
	{
		THEN( "/bin/ls can be run" )
		{
			std::vector<std::string> args{ "/bin" };
			std::string output;
			REQUIRE( 0 == Execute( "/bin/ls", args, output ) );
			REQUIRE( output.length() > 0 );
		}
	}
}

SCENARIO( "Test application can be executed by itself" )
{
	GIVEN( "The path to the test" )
	{
		THEN( "Our execution function can run the test with an argument" )
		{
			std::vector<std::string> args{ "testrun" };
			std::string output;
			REQUIRE( std::numeric_limits<int8_t>::max() == Execute( cmdArgs.front().c_str(), args, output ) );
			REQUIRE( output.length() > 0 );
			REQUIRE( output == "testrun\n" );
		}
	}
}

SCENARIO( "Application can be run as daemon" )
{
	GIVEN( "The path to the test" )
	{
		THEN( "Our execution function can run the test as a daemon" )
		{
			std::vector<std::string> args{ "daemonize" };
			std::string output;
			ClearLogs();
			REQUIRE( 0 == Execute( cmdArgs.front().c_str(), args, output ) );
			// Wait for daemon to complete
			this_thread::sleep_for( 3s );
			REQUIRE( GetFileSize( "./stdout.log" ) > 15 );
			REQUIRE( GetFileSize( "./stderr.log" ) == 0 );
			ClearLogs();
		}
	}
}

SCENARIO( "Signals caught when run as a daemon" )
{
	GIVEN( "A running daemon" )
	{
		std::vector<std::string> args{ "signal" };
		std::string output;
		ClearLogs();
		unlink( "./daemon.pid" );
		unlink( "./sigterm.signal" );
		REQUIRE( 0 == Execute( cmdArgs.front().c_str(), args, output ) );
		// Wait for daemon to start
		this_thread::sleep_for( 1s );
		ifstream pidfFile( "./daemon.pid", ios::binary );
		REQUIRE( pidfFile.is_open() );
		int pid;
		pidfFile.read( reinterpret_cast<char*>( &pid ), sizeof( pid ) );
		REQUIRE( pidfFile.gcount() == sizeof( pid ) );
		pidfFile.close();
		kill( pid, SIGTERM );
		// Wait for termination
		this_thread::sleep_for( 1s );
		REQUIRE( GetFileSize( "./sigterm.signal" ) > 0 );
		ClearLogs();

	}
}
