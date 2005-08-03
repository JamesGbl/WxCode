#ifndef __THEAPP_H__
#define __THEAPP_H__

#include <wx\wxprec.h>

class CClockThread;
class wxLCDWindow;

class CLCDSampleApp : public wxApp
{
public:
	wxLCDWindow *mClock;
	CClockThread *mUpdThread;

public:
	virtual bool OnInit( void );
	int OnExit( void );

	void UpdateClock( void );
};

DECLARE_APP( CLCDSampleApp )

#endif // __THEAPP_H__
