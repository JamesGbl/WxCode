%module gpib

%{
#include "wx/ctb/linux/gpib.h"
%}

%include ../gpibx.i

%typemap(in) void * dcs (wxGPIB_DCS tmp) {
	/* dont check for list */
	$1 = &tmp;
}

class wxGPIB : public wxGPIB_x
{
protected:
    int CloseDevice();
    int OpenDevice(const char* devname, void* dcs);
public:
    wxGPIB();
    ~wxGPIB();
};
