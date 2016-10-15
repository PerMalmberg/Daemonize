// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#include <daemonize/Daemon.h>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>

namespace daemonize {


bool Daemon::Daemonize( const std::string& workingDir, ProcessIdentity& identity )
{
	// http://web.archive.org/web/20120914180018/http://www.steve.org.uk/Reference/Unix/faq_2.html#SEC16
	bool result;
	identity = ORIGINAL_PROCESS;

	// Fork so the parent process can exit, this will return control to the command line or shell that invoked us.
	int pid = fork();
	if( pid == FORK_ERROR )
	{
		// Could not fork
		result = false;
	}
	else if( pid == FORK_NEW_PROCESS )
	{
		// New process
		// setsid() to become a process group and session group leader. Since a controlling terminal is
		// associated with a session, and this new session has not yet acquired a controlling terminal our
		// process now has no controlling terminal, which is a Good Thing for daemons.
		identity = INTERMEDIATE_PROCESS;
		setsid();

		// Fork again so the parent (the session group leader) can exit.
		// This means that we, as a non-session group leader, can never regain a controlling terminal.
		pid = fork();
		if( pid == FORK_ERROR )
		{
			// Could not a second time
			result = false;
		}
		else if( pid == FORK_NEW_PROCESS )
		{
			// Daemon process
			myDaemonPid = setsid();
			identity = DAEMON_PROCESS;

			// chdir to ensure that our process doesn't keep any directory in use. Failure to do this could
			// make it so that an administrator couldn't unmount a filesystem, because it was our current directory.
			chdir( workingDir.c_str() );

			// umask(0) so that we have complete control over the permissions of anything we write.
			// We don't know what umask we may have inherited. [This step is optional]
			umask( 0 );

			// Close stdout, stdin and stderr
			// close() fds 0, 1, and 2. This releases the standard in, out, and error we inherited
			// from our parent process. We have no way of knowing where these fds might have been
			// redirected to. Note that many daemons use sysconf() to determine the limit _SC_OPEN_MAX.
			// _SC_OPEN_MAX tells you the maximum open files/process. Then in a loop, the daemon can
			// close all possible file descriptors. You have to decide if you need to do this or not.
			// If you think that there might be file-descriptors open you should close them, since there's
			// a limit on number of concurrent file descriptors.
			for( int i = 0; i < sysconf( _SC_OPEN_MAX ); ++i )
			{
				close( i );
			}

			result = true;
		}
		else
		{
			// Old process, second fork succeeded
			result = true;
		}
	}
	else
	{
		// Old process, first fork
		result = true;
	}

	return result;
}

void Daemon::OpenStreams( const std::string& stdOut, const std::string& stdErr )
{
	freopen( stdOut.c_str(), "a+", stdout );
	freopen( stdErr.c_str(), "a+", stderr );
	freopen( "/dev/null", "r", stdin );
}

}