// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include "Application.h"


int Application::Run()
{
	return 0;
}

int Application::RunAsDaemon()
{
	return Daemonize() ? 0 : 1;
}
