// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once


#include <daemonize/Application.h>

class TestApp : public daemonize::Application
{
public:
	TestApp( const std::string& workingDirectory );

	TestApp( const std::string& workingDirectory, const std::string& daemonPidFile );

protected:
	int Main();

private:
	const std::string myDaemonPidFile;
};


