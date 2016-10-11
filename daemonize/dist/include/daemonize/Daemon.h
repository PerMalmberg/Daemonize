// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

namespace daemonize {

class Daemon
{
public:
	virtual ~Daemon()
	{
	}


protected:
	bool Daemonize();
};

}