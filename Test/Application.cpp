// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include <unistd.h>
#include "Application.h"


int Application::Run()
{
	return 0;
}

bool Application::RunAsDaemon()
{
	bool isDaemon = false;

	bool res = deamonizer.Daemonize( "/", isDaemon );

	if( res && isDaemon )
	{
		// We're now running as a daemon
		std::cout << "Application running as daemon\n";

		for( int i = 0; i < 10; ++i ) {
			sleep(1);
			std::cout << "Application running as daemon" << i << "\n";
			fflush( stdout );
		}

	}

	return res;
}
