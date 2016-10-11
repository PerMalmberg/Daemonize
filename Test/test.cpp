// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.


#include <string>
#include <cstring>
#include "Catch/include/catch.hpp"

extern std::vector<std::string> cmdArgs;

class PFileCloser : public std::default_delete<FILE>
{
public:
	int operator()(FILE* file)  {
		exitCode = pclose( file ) >> 24; // Result of child process is in upper 8 bits.
	}

	int Get() const { return exitCode; }
private:
	int exitCode;
};

int Execute( const std::string& cmd, std::vector<std::string>& args, std::string& output )
{
	// http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix#478960
	int result = 0;

	std::string c( cmd );
	for( auto& arg : args )
	{
		c += " \"" + arg + "\""; // Add quotation marks around arguments to handle spaces in arguments.
	}


	char buffer[128];
	output = "";
	// provide a deleter to catch the exit code.
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

			result = pclose(file) / 256;
		}
		else
		{
			result = std::numeric_limits<int>::max();
		}
	}
	return result;
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

SCENARIO("Application can be run as non-daemon")
{
	GIVEN( "The path to the test" )
	{
		THEN( "Our execution function can run the test as a daemon" )
		{
			std::vector<std::string> args{ "daemonize" };
			std::string output;
			REQUIRE( 1 == Execute( cmdArgs.front().c_str(), args, output ) );
			REQUIRE( output.length() == 10 );
			REQUIRE( output == "Daemonize\n" );
		}
	}
}
