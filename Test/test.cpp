// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.


#include <string>
#include <cstring>
#include <unistd.h>
#include "Catch/include/catch.hpp"

extern std::vector<std::string> cmdArgs;

int Execute( const std::string& cmd, std::vector<std::string>& args, std::string& output )
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

			auto code = pclose(file);
			result =  WEXITSTATUS( code );
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

SCENARIO("Application can be run as daemon")
{
	GIVEN( "The path to the test" )
	{
		THEN( "Our execution function can run the test as a daemon" )
		{
			std::vector<std::string> args{ "daemonize" };
			std::string output;
			unlink( "./stdout.log");
			unlink( "./stderr.log");
			REQUIRE( 0 == Execute( cmdArgs.front().c_str(), args, output ) );
			// Wait for daemon to complete
			sleep(3);
			REQUIRE( GetFileSize( "./stdout.log" ) > 15 );
			REQUIRE( GetFileSize( "./stderr.log" ) == 0 );
			unlink( "./stdout.log");
			unlink( "./stderr.log");
		}
	}
}


