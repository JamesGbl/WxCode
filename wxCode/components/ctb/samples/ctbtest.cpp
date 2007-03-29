#include "wx/ctb/getopt.h"
#if ( GPIB )
# include "wx/ctb/gpib.h"
#endif
#include "wx/ctb/iobase.h"
#include "wx/ctb/serport.h"
#include "wx/ctb/timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------- options -------------------------------
const char* options="a:b:hd:t:";

const char* helpMessage =
{
    "A simple serial port and GPIB class test\n"
    "Simply sends \"*idn?\" throughout the given interface and\n"
    "reads the answer\n"
    "(SCPI command for identify the connected device)\n"
    "ctbtest [options]\n"
    "available options are:\n"
    "-a     : GPIB device address, default is 1\n"
    "-b     : baudrate [1200...115200], default is 38400\n"
    "-d     : connected device, default is COM1\n"
    "-h     : print this\n"
    "-t     : communication timeout in ms (default is 1000ms)\n"
};

// ----------------- globals -------------------------------
wxBaud baudrate = wxBAUD_38400;
char *devname = wxCOM1;
int timeout = 1000;

int main(int argc,char* argv[])
{
    int adr = 1;
    char buf[1024];
    int quit = 0;
    int val;

    while ((val=getopt(argc,argv,(char*)options))!=EOF) {
	   switch (val) {
	   case 'a' : {
	       adr = strtol(optarg,NULL,10);
	       if((adr < 1) || (adr > 30)) {
		   printf("Wrong address, must be 1...30!\n");
		   return -1;
	       }
	       break;
	   }
	   case 'b' : baudrate = (wxBaud)strtol(optarg,NULL,10); break;
	   case 'd' : devname = optarg; break;
	   case 'h' : printf(helpMessage); exit(0);
	   case 't' : timeout = strtol(optarg,NULL,10); break;
	   }
    }
    
    // like a virtual instrument in NI
    wxIOBase* dev;

#if ( GPIB )
    if(!strncmp(devname,wxGPIB1,strlen(wxGPIB1))) {
	   // device is GPIB
	   dev = new wxGPIB();
	   // check for connected listeners
	   int listeners = wxGPIB::FindListeners();
	   if(listeners > 0) {
		  for(int i=1;i<31;i++) {
			 if(listeners & (1 << i)) {
				printf("Found listener at address: %i\n",i);
			 }
		  }
	   }
	   else if(!listeners) {
		  printf("No listener found!\n");
	   }
	   else {
		  printf("GPIB error!\n");
	   }
	   // try to open the device (default address is 1)
	   wxGPIB_DCS dcs;
	   dcs.m_address1 = adr;
	   if(dev->Open(devname,&dcs) < 0) {
	       // try again (win98 hack)
	       if(dev->Open(devname,&dcs) < 0) {
		  printf("Cannot open %s\n",devname);
		  delete dev;
		  return -1;
	       }
	   }
    }
    else {
#endif
	   // device is a serial port
	   dev = new wxSerialPort();
	   // try to open the given port
	   if(dev->Open(devname) < 0) {
		  printf("Cannot open %s\n",devname);
		  delete dev;
		  return -1;
	   }
	   // set the baudrate
	   ((wxSerialPort*)dev)->SetBaudRate(baudrate);
#if ( GPIB )
    }
#endif
    // ok, device is ready for communication
    
    // we are using a timer for timeout test. After the given timeout
    // is reached, the to value will be set to 1

    int to = 0;
    timer t(timeout,&to,NULL);
    t.start();

    char* receivedString = NULL;
    size_t readedBytes = 0;
    dev->Writev("*idn?\n",6,&to);
    int state = dev->ReadUntilEOS(receivedString,
						    &readedBytes,
						    "\n",
						    timeout);
    if(readedBytes > 0) {
	   receivedString[readedBytes] = 0;
	   printf("answer (%i): %s\n",readedBytes,receivedString);
    }

    dev->Close();
    delete dev;
    return 0;
}
