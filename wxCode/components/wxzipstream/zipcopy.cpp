#include <wx/wx.h>
#include <wx/wfstream.h>

#include "zipstream.h"
#include "zip/zip.h"

//MSVC pragmas
#ifdef __VISUALC__
	#pragma comment(lib, "wxmsw.lib")
	#pragma comment(lib, "comctl32.lib")
	#pragma comment(lib, "wsock32.lib")
//	    #pragma comment(lib, "rpcrt4.lib")
 //	#pragma comment(lib, "strmiids.lib")
#endif


class wxZipCopyApp : public wxApp
{
public:
	bool OnInit()
	{
		wxFileDialog f(NULL);
		if (f.ShowModal() == wxID_OK)
		{
			wxInputStream* p1;
			wxOutputStream *p2;
			wxZipInputStream* zi = new wxZipInputStream(*(p1=new wxFileInputStream(f.GetPath())));
			wxZipOutputStream* zo = new wxZipOutputStream(*(p2=new wxFileOutputStream(f.GetPath() + " Copy")));


			wxZipFileInfo Info;
			do
			{
				zi->GetFileInfo(Info);

				void* pBuffer = new wxByte[Info.dwUncompressedSize];

				if (zi->Read(pBuffer, Info.dwUncompressedSize).LastRead() != Info.dwUncompressedSize)
				{
					wxMessageBox(wxString("Couldn't read in file:") +
								Info.szName);
					return false;
				}

				if (!zo->MakeFile(Info, -1))
				{
					wxMessageBox(wxString("Couldn't create file:") +
								Info.szName);
					return false;
				}

//				if (
//					zo->Write(pBuffer, Info.dwUncompressedSize);// != Info.dwUncompressedSize)
					zipWriteInFileInZip(zo->GetHandleO(), (void*) pBuffer, Info.dwUncompressedSize);
				{
//					wxMessageBox(wxString::Format(wxString("Wrote %i bytes, but couldn't write %i bytes to file:") +
//								Info.szName, zo.LastWrite(), Info.dwUncompressedSize));
//					return false;

				}

				delete (wxByte*) pBuffer;
			}while(zi->OpenNextFile());
			delete zo;
			delete zi;
			delete (wxFileInputStream*) p1;
			delete (wxFileOutputStream*) p2;
			wxMessageBox("Done!");
		}

		return false;
	}
};

IMPLEMENT_APP(wxZipCopyApp);
