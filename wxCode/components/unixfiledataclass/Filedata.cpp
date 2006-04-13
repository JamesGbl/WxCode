/////////////////////////////////////////////////////////////////////////////
// Name:        Filedata.cpp
// Purpose:    Encapsulates lstat and stat, plus misc others
// Author:      David Hart
// Modified by:
// Created:     17/6/04
// Copyright:   (c) David Hart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop 
#endif

#ifndef WX_PRECOMP
    #include "wx/log.h"

    #include "wx/app.h"
    #include "wx/frame.h" 
#endif


#include <wx/dynarray.h>
#include <wx/arrimpl.cpp>

#include <pwd.h>                   // For finding usernames from ids
#include <grp.h>                   // Ditto groupnames
#include <sys/statvfs.h>        // For statvfs(), used in CanTHISUserWrite()

#include "Filedata.h"


FileData::FileData( wxString filepath, bool defererence /*=false*/ )  :  Filepath(filepath)
{
statstruct = new struct stat;

if ( ! defererence )
  {  result = lstat(Filepath.mb_str(wxConvUTF8), statstruct);  // lstat rather than stat as it doesn't dereference symlinks
    if ( !result && IsSymlink() )                                               // If we have a valid result & it's a symlink
        symlinkdestination = new FileData( filepath, true );     //   load another FileData with its reference
     else symlinkdestination = NULL;
  }
 else
  {  result = stat(Filepath.mb_str(wxConvUTF8), statstruct);  // Unless we WANT to dereference, in which case use stat
    if ( result != -1 )
      {              // Strangely, stat/lstat don't return any filepath data.  Here we need it, so we have to use readlink
          char buf[500];          // We have to specify a char buffer.  500 bytes should be long enough: it gets truncated anyway
          int ans = readlink(Filepath.mb_str(wxConvUTF8), buf, 500 );    // Readlink inserts into buf the destination filepath of the symlink
          if ( ans != -1 ) 	Filepath = wxString( buf, wxConvUTF8, ans );  // If it worked, ans holds the length of the destination filepath. Put it into Filepath
            else 
              {  BrokenlinkName = Filepath;         // If readlink was unsuccessful, store the broken-link name here
                  Filepath = wxEmptyString;           //      and kill the symlink's name
              }
      }

      else BrokenlinkName = Filepath;               // If result was unsuccessful, store the broken-link name here

    symlinkdestination = NULL;                       // We don't use symlinkdestination, so NULL it
  }
}

wxString FileData::GetPath()
{
if ( ! IsValid() ) return wxEmptyString;

wxString Path = GetFilepath();
if ( Path.Right(1) == wxFILE_SEP_PATH )  Path = Path.BeforeLast(wxFILE_SEP_PATH);  // Avoid problems with any terminal '/'
       // Chop off any filename & add a '/' for luck.  This also means we return '/' if the original filepath was itself '/' !
return wxString( Path.BeforeLast(wxFILE_SEP_PATH) + wxFILE_SEP_PATH );  
 }

bool FileData::IsFileExecutable()      // See if the file is executable (by SOMEONE, not necessarily us)
{
if ( ! IsValid() ) return false;                          // If lstat() returned an error, the answer must be 'No'

unsigned int st_mode = ((unsigned int)statstruct->st_mode);    // For readability

return ( !!(st_mode & S_IXUSR) || !!(st_mode & S_IXGRP) || !!(st_mode & S_IXOTH) ); // If user, group or other has execute permission, return true
}

bool FileData::CanTHISUserRead()    // Do WE have permission to read this file
{
if ( ! IsValid() ) return false;
       // We can't use access() if this is a symlink, as it reports on the target instead.  This is a particular problem if the symlink is broken!
if ( IsSymlink() ) return true;                      // All symlinks seem to have global rwx permissions

int result = access(Filepath.mb_str(wxConvUTF8), R_OK);        // Use 'access' to see if we have read permission
return ( result==0 ?  true : false );
}

bool FileData::CanTHISUserWrite()    // Do WE have permission to write to this file
{
if ( ! IsValid() ) return false;
       // We can't use access() if this is a symlink, as it reports on the target instead.  This is a particular problem if the symlink is broken!
if ( IsSymlink() )                                      // All symlinks seem to have global rwx permissions, so no need to check for these
  { struct statvfs buf;                              // However we do need to check for a ro filesystem
    int ans = statvfs( GetPath().mb_str(wxConvUTF8), &buf );   // Use the path here, rather than the filepath, as statvfs can't cope with broken symlinks
    return ( ans==0 && (buf.f_flag & ST_RDONLY)==0 );
  }

int result = access(Filepath.mb_str(wxConvUTF8), W_OK);    // Use 'access' to see if we have write permission
return ( result==0 ?  true : false );
}

bool FileData::CanTHISUserExecute()  // Do WE have permission to execute this file
{
if ( ! IsValid() ) return false;
   // I _am_ using access() here, even for symlinks. Otherwise, as symlinks all have rwx permissions, they'll all appear to be executable, even if the target isn't!
int result = access(Filepath.mb_str(wxConvUTF8), X_OK);   // Use 'access' to see if we have execute permission
return ( result==0 ?  true : false );
}

bool FileData::CanTHISUserWriteExec()  // Do WE have write & exec permissions for this file
{
if ( ! IsValid() ) return false;
       // We can't use access() if this is a symlink, as it reports on the target instead.  This is a particular problem if the symlink is broken!
if ( IsSymlink() ) return CanTHISUserWrite();                 // All symlinks seem to have global rwx permissions, but use CanTHISUserWrite() to check for a ro filesystem

int result = access(Filepath.mb_str(wxConvUTF8), W_OK | X_OK);    // Use 'access' to see if we have write/exec permissions
return ( result==0 ?  true : false );
}

bool FileData::CanTHISUserRename()  // See if the file's PARENT DIR is Writable by US
{
if ( ! IsValid() ) return false;
                 // To rename a file or dir, we need to have 'write' permission for its parent dir (and exec permission, otherwise we can't stat it)
FileData dir( Filepath.BeforeLast(wxFILE_SEP_PATH) );	       // Make a FileData for the parent dir
if ( ! dir.IsValid() ) return false;                                      // If stat() returned an error, the answer must be 'No'

if ( ! dir.CanTHISUserWriteExec() ) return false;              // Return false if parent dir hasn't permissions for us

                                   // If we HAVE got write permission, we still have to check that the Sticky bit isn't set
if ( ! dir.IsSticky() ) return true;                                     // If it isn't, we're OK

uid_t id = getuid();                                                           // Get user's numerical id
return ( id == OwnerID()  ||  id == dir.OwnerID() );         // We're still OK if we own either the file or the parent dir.  Otherwise we're not
}

bool FileData::CanTHISUserChmod()  // See if the file's permissions are changable by US
{
if ( ! IsValid() ) return false;
                        // To change a file or dir's permissions, we need to own it or be root
uid_t id = getuid();                                                           // Get user's numerical id
return ( id == OwnerID()  ||  id == 0 );                           // We're OK if we own either the file or are root.  Otherwise we're not
}

bool FileData::DoChmod( mode_t newmode )  // If appropriate, change the file's permissions to newmode
{
if ( ! IsValid() ) return false;
if ( ! CanTHISUserChmod()	) return false;                      // Check we have the requisite authority (we need to own it or be root)

if ( newmode == (statstruct->st_mode & 07777) ) return false;  // Check the new mode isn't identical to the current one!

return (chmod( Filepath.mb_str(wxConvUTF8), newmode ) == 0);   // All's well, so do the chmod. Success is flagged by zero return
}

bool FileData::DoChangeOwner( uid_t owner )  // If appropriate, change the file's owner
{
unsigned int invalid = (unsigned int)-1;                          // This is to prevent compiler warnings about -1 in unsigned ints
if ( ! IsValid() ) return false;
if ( owner == invalid || owner == OwnerID() )  return false;     // because there's nothing to do
if ( getuid() != 0 )	return false;                                       // Only root can change ownership

return (lchown( Filepath.mb_str(wxConvUTF8), owner, invalid ) == 0);    // Try to do the chown (actually lchown as this works on links too).  Zero return means success
}

bool FileData::DoChangeGroup( gid_t group )  // If appropriate, change the file's group
{
unsigned int invalid = (unsigned int)-1;                                  // This is to prevent compiler warnings about -1 in unsigned ints
if ( ! IsValid() ) return false;
if ( group == invalid || group == GroupID() )  return false;	    // because there's nothing to do
if ( getuid()!=0 && getuid()!=OwnerID() )  return false;          // Only the file-owner or root can change a group

return (lchown( Filepath.mb_str(wxConvUTF8), invalid, group ) == 0);   // Try to do the chown (actually lchown as this works on links too).  Zero return means success
}

wxString FileData::GetParsedSize()  // Returns the size, but in bytes, KB or MB according to magnitude
{
return ParseSize( Size() );                                               // Uses global function
}

wxString FileData::PermissionsToText()  // Returns a string describing the filetype & permissions eg -rwxr--r--, just like ls -l does
{
wxString text;
text.Printf(wxT("%c%c%c%c%c%c%c%c%c%c"), IsRegularFile() ? wxT('-') : wxT('d'),  // To start with, assume all non-regular files are dirs
    IsUserReadable() ? wxT('r') : wxT('-'), IsUserWriteable() ? wxT('w') : wxT('-'), IsUserExecutable() ? wxT('x') : wxT('-'),
    IsGroupReadable() ? wxT('r') : wxT('-'), IsGroupWriteable() ? wxT('w') : wxT('-'), IsGroupExecutable() ? wxT('x') : wxT('-'),
    IsOtherReadable() ? wxT('r') : wxT('-'), IsOtherWriteable() ? wxT('w') : wxT('-'), IsOtherExecutable() ? wxT('x') : wxT('-')
              );

if ( text.GetChar(0) != wxT('-') )                                       // If it wasn't a file, 
    if ( IsDir() ) ;                                                                //   check it really IS a dir (& if so, do nothing).  If it's NOT a dir, it'll be one of the miscellany
    else if ( IsSymlink() ) text.SetChar(0, wxT('l'));	
    else if ( IsCharDev() ) text.SetChar(0, wxT('c'));
    else if ( IsBlkDev() ) text.SetChar(0, wxT('b'));
    else if ( IsSocket() ) text.SetChar(0, wxT('s'));
    else if ( IsFIFO() ) text.SetChar(0, wxT('p'));
                      // Now amend string if the unusual permissions are set.
if ( IsSetuid() ) text.GetChar(3) == wxT('x') ?  text.SetChar(3, wxT('s')) : text.SetChar(3, wxT('S'));  //  If originally 'x', use lower case, otherwise upper
if ( IsSetgid() ) text.GetChar(6) == wxT('x') ?  text.SetChar(6, wxT('s')) : text.SetChar(6, wxT('S'));
if ( IsSticky() ) text.GetChar(9) == wxT('x') ?  text.SetChar(9, wxT('t')) : text.SetChar(9, wxT('T'));

return text;
}

wxString FileData::Type()   // Returns string version of type eg "Directory"
{
wxString type;

if ( IsRegularFile() )         type = wxT("Regular File");
else if ( IsDir() )             type = wxT("Directory");
else if ( IsSymlink() )      type = wxT("Symbolic Link");
else if ( IsCharDev() )     type = wxT("Character Device");
else if ( IsBlkDev() )        type = wxT("Block Device");
else if ( IsSocket() )        type = wxT("Socket");
else if ( IsFIFO() )            type = wxT("FIFO");
  else type = wxT("Unknown Type?!");

return type;
}

wxString FileData::GetOwner()  // Returns owner's name as string
{
struct passwd* p = getpwuid( OwnerID() );                       // Use getpwuid to fill password struct
wxString name(p->pw_name, wxConvUTF8);
return p == NULL ? wxT("") : name;
}

wxString FileData::GetGroup()	 // Returns group name as string
{
struct group* g=getgrgid( GroupID() );                            // Use getgrgid to fill group struct
wxString name(g->gr_name, wxConvUTF8);
return g == NULL ? wxT("") : name;
}

wxString FileData::GetUltimateDestination()  // Returns the filepath at the end of a series of symlinks (or the original filepath if not a symlink)
{
char buf[500];
// This function absolutes a relative path, and undoes any symlinks within the path eg /foo/subdir where foo is a symlink for /bar
if ( realpath( GetFilepath().mb_str(wxConvUTF8), buf ) == NULL ) return wxEmptyString;

return wxString(buf, wxConvUTF8);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


              // Global helper function


wxString ParseSize( size_t bytes )    // Returns a string filled with the size, but in bytes, KB or MB according to magnitude
{
wxString text;

if ( bytes < 1024 )	text.Printf( wxT("%uB"), bytes );
 else if  ( bytes < 1048576 )	text.Printf( wxT("%.1fK"), ((float)bytes) / 1024 );
   else text.Printf( wxT("%.1fM"), ((float)bytes) / 1048576 );

return text;
}

