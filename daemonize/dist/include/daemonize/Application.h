// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <daemonize/Daemon.h>
#include <vector>
#include <signal.h>
#include <atomic>
#include <array>
#include "ISignalInfo.h"

namespace daemonize {

class Application : public ISignalInfo
{
public:
	Application( const std::string& workingDirectory );

	int Run();

	int RunAsDaemon();

	bool PollSignal( int signal ) override ;

protected:
	// Method called once the application is initialized, possible as a daemon.
	virtual int Main() = 0;

	// Opens the standard streams, with output to the specified files.
	void OpenStandardStreams( const std::string& stdOut, const std::string& stdErr )
	{
		myDaemonizer.OpenStreams( stdOut, stdErr );
	}

	int GetDaemonPid() const
	{
		return myDaemonizer.GetDaemonPid();
	}

	bool IsDaemon() const
	{
		return myDaemonizer.IsDaemon();
	}

private:
	void SetupSignalHandlers();

	void SetSignal( int signal );

	bool ValidateSignal( int signal ) const;

	daemonize::Daemon myDaemonizer;
	const std::string myWorkingDirectory;

	const std::array<int, 5> myInterceptedSignals{ { SIGHUP, SIGINT, SIGTERM, SIGUSR1, SIGUSR2 } };
	volatile std::atomic_bool mySignals[_NSIG] = { { false } };

	static void HandleSignal( int signal );

	static Application* mySelf;
};

}