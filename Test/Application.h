// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <daemonize/Daemon.h>

class Application : public daemonize::Daemon
{
public:
	int Run();

	int RunAsDaemon();
};
