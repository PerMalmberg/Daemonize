// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

#include <vector>
#include <csignal>
#include <atomic>
#include <array>
#include <algorithm>
#include <functional>

#include <daemonize/Daemon.h>

namespace daemonize
{

    class Application
    {
        public:
            explicit Application(const std::string& workingDirectory, const std::vector<int>& intercept_signals);

            int Run();

            // Pass a callback to this method if you require checks to be done when running as a daemon.
            // Return true if the daemon should continue to run.
            int RunAsDaemon(
                    std::function<bool(void)> as_daemon = []()
                    {
                        return true;
                    }
            );

            // Method called once the application is initialized, possibly as a daemon.
            virtual int Main() = 0;

        protected:

            // Opens the standard streams, with output to the specified files.
            void OpenStandardStreams(const std::string& stdOut, const std::string& stdErr)
            {
                myDaemonizer.OpenStreams(stdOut, stdErr);
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
            virtual void SignalReceived(int signal) = 0;

        private:
            void SetupSignalHandlers();


            daemonize::Daemon myDaemonizer;
            const std::string myWorkingDirectory;

            static bool ValidateSignal(int signal);

            static std::vector<int> myInterceptedSignals;

            static void HandleSignal(int signal);

            static Application *mySelf;
    };

}