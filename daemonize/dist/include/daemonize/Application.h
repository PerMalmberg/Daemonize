// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <daemonize/Daemon.h>
#include <vector>

namespace daemonize {

class Application
{
public:
	Application( const std::string& workingDirectory );

	int Run();

	int RunAsDaemon();

protected:
	// Method called once the application is initialized, possible as a daemon.
	virtual int Main() = 0;

	// Opens the standard streams, with output to the specified files.
	void OpenStandardStreams( const std::string& stdOut, const std::string& stdErr )
	{
		myDaemonizer.OpenStreams( stdOut, stdErr );
	}

	int GetDaemonPid() const { return myDaemonizer.GetDaemonPid(); }

private:
	daemonize::Daemon myDaemonizer;
	const std::string myWorkingDirectory;
};

}