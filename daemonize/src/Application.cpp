// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <iostream>
#include "daemonize/Application.h"

using namespace daemonize;

Application* Application::mySelf;
std::vector<int> Application::myInterceptedSignals;

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int Application::Run()
{
	SetupSignalHandlers();
	// Simply run the main method
	return Main();
}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
int Application::RunAsDaemon(std::function<bool(void)> as_daemon)
{
	int result = 1;

	Daemon::ProcessIdentity identity;
	bool res = myDaemonizer.Daemonize( myWorkingDirectory, identity );

	if( res )
	{
		if( identity == Daemon::ProcessIdentity::DAEMON_PROCESS )
		{
			SetupSignalHandlers();
			if(as_daemon())
            {
                result = Main(); // Call the main method
            }
		}
		else
		{
			// Original or intermediate process
			result = 0;
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
Application::Application( const std::string& workingDirectory, const std::vector<int>& intercept_signals )
		: myWorkingDirectory( workingDirectory )
{
	myInterceptedSignals = intercept_signals;
	if(std::find(myInterceptedSignals.begin(), myInterceptedSignals.end(), SIGINT) == myInterceptedSignals.end())
	{
		myInterceptedSignals.push_back(SIGINT);
	}

	if(std::find(myInterceptedSignals.begin(), myInterceptedSignals.end(), SIGTERM) == myInterceptedSignals.end())
	{
		myInterceptedSignals.push_back(SIGTERM);
	}


	// Set the static reference to ourselves.
	mySelf = this;
}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void Application::SetupSignalHandlers()
{
	struct sigaction sa;
	sa.sa_handler = &HandleSignal;
	sa.sa_flags = SA_RESTART;
	sigfillset( &sa.sa_mask );

	// Add handles for wanted signals
	for( auto signal : myInterceptedSignals )
	{
		sigaction( signal, &sa, nullptr );
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void Application::HandleSignal( int signal )
{
	if( ValidateSignal( signal ) )
	{
		mySelf->SignalReceived( signal );
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
void Application::SignalReceived( int signal )
{

}

///////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////
bool Application::ValidateSignal( int signal )
{
	return std::find( myInterceptedSignals.begin(), myInterceptedSignals.end(), signal ) != myInterceptedSignals.end();
}


