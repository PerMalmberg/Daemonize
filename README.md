# Daemonize
A library for [daemonizing](https://en.wikipedia.org/wiki/Daemon_(computing)) an application.

#Why
I was interested in how an application can become a daemon, and what it would take to implement a reusable framework for it.
 
#What
Daemonize is a framework consisting of two classes, Application and Daemon, that provides a way
to run an application both as a daemon and a non-daemonized application.

#How
Using this framework is really easy:

1. Create a class inheriting from the daemonize::Application. This class becomes your 'main class'.
2. Override the Main() method. This becomes the entry point to your program where you start your program flow.
3. In your ```int main( int argc, char* argv[] )``` function, do:
    1. Instantiate an instance of the 'main class', providing the initial desired working directory to be used when run as a daemon.
    2. Call ```app.RunAsDaemon()``` to run as daemon, or ```app.Run()``` to run as a regular application.
    3. Return the value returned by the ```Run*()``` method.

##Code flow when running as a daemon
When calling ```RunAsDaemon()```, your application will actually be cloned twice due to how fork() works and how a daemon is started.
 As such, you need to handle the fact that your application will behave as if it *returns three times* from the ```RunAsDaemon()``` method,
 once for the original instance, secondly for an intermediate instance and thirdly from the actual daemon process. So if you do things
  in your ```int main( int argc, char* argv[] )``` after calling ```RunAsDaemon()``` that code will be run a total of three times.
  As such, it is recommended to do no additional work and only return whatever ```RunAsDaemon()``` returns.

#Signals
The ```Application``` class intercepts the signals specified in the ```myInterceptedSignals``` member. Among these are 
SIGINT and SIGTERM. This means that you must manually poll the state if these signals via the ```ISignalInfo``` interface in the
application in order to determine when it is time to perform a shutdown. The other signals may be used to trigger other
 functions in the application.