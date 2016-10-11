// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#define CATCH_CONFIG_RUNNER

#include "Catch/include/catch.hpp"
#include "Application.h"

std::vector<std::string> cmdArgs;

int main( int argc, const char* const argv[], const char* envp[] )
{
	int result = 1;

	for( int i = 0; i < argc; ++i ) {
		cmdArgs.push_back( argv[i] );
	}

	if( argc > 1 )
	{
		if( strcmp( argv[1], "daemonize" ) == 0 )
		{
			Application app;
			result = app.RunAsDaemon();
			std::cout << "Daemonize\n";
		}
		else if( strcmp( argv[1], "regular" ) == 0 )
		{
			Application app;
			result = app.Run();
		}
		else if( strcmp( argv[1], "testrun" ) == 0 )
		{
			result = std::numeric_limits<int8_t>::max();
			std::cout << "testrun\n";
		}
	}
	else
	{
		result = Catch::Session().run( argc, argv );
	}


	return result;
}