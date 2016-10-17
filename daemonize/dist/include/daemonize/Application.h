// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <vector>
#include <signal.h>
#include <atomic>
#include <array>
#include <algorithm>
#include <daemonize/Daemon.h>

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

	int GetDaemonPid() const
	{
		return myDaemonizer.GetDaemonPid();
	}

	bool IsDaemon() const
	{
		return myDaemonizer.IsDaemon();
	}

	// Called when a signal is intercepted
	virtual void SignalReceived( int signal ) = 0;
private:
	void SetupSignalHandlers();


	daemonize::Daemon myDaemonizer;
	const std::string myWorkingDirectory;

	static bool ValidateSignal( int signal );
	static constexpr const int myInterceptedSignals[] = { SIGHUP, SIGINT, SIGTERM, SIGUSR1, SIGUSR2 };

	static void HandleSignal( int signal );

	static Application* mySelf;
};

}