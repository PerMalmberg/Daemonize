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
	enum ProcessIdentity
	{
		ORIGINAL_PROCESS = 0,
		INTERMEDIATE_PROCESS,
		DAEMON_PROCESS
	};

	virtual ~Daemon()
	{
		// Close streams
	}

	// Deamonizes the process.
	// Returns true on success, false on failure.
	// processIdentity tells the caller if it is the original, intermediate or daemon processes.
	bool Daemonize( const std::string& workingDir, ProcessIdentity& identity );

	// Opens the standard output streams with redirection to the specified files.
	void OpenStreams( const std::string& stdOut, const std::string& stdErr );

	int GetDaemonPid() const
	{
		return myDaemonPid;
	}

private:
	const int FORK_ERROR = -1;
	const int FORK_NEW_PROCESS = 0;
	int myDaemonPid;
};

}