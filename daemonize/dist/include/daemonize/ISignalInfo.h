// Copyright (c) 2016 Per Malmberg
// Licensed under MIT, see LICENSE file.
// Give credit where credit is due.

#pragma once

class ISignalInfo
{
public:
	// Retrieves a value for the the specified signal and resets the stored value.
	// Return value is true if the signal has been received. Note that the value is
	// reset on each read so sequential polls are very unlikely to return true.
	virtual bool PollSignal( int signal ) = 0;
};
