// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <sys/types.h>
#include <string>
#include <streambuf>

namespace daemonize {

class Daemon
{
public:
	virtual ~Daemon()
	{
		// Close streams
	}

	// Deamonizes the process.
	// Returns true on success, false on failure.
	// isDaemon is true if the current process is the daemon.
	bool Daemonize( const std::string& workingDir, bool& isDaemon );

	int GetDaemonPid() const { return daemonPid; }
private:
	const int FORK_ERROR = -1;
	const int FORK_NEW_PROCESS = 0;
	__pid_t daemonPid;
};

}