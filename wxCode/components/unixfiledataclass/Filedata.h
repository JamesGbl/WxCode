#ifndef FILEDATAH
#define FILEDATAH

#include "wx/dir.h"
#include "wx/wx.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>


class FileData         // Utility class.  Does an lstat, stores the data, has accessor methods.  Also misc similar functions
{
public:
FileData( wxString Filename, bool defererence = false );                    // If dereference, use stat instead of lstat
~FileData(){ if ( symlinkdestination != NULL ) delete symlinkdestination; delete statstruct; }

bool IsValid(){ return result != -1; }
wxString GetFilepath(){ return result ? wxString(wxT("")) : Filepath; }
wxString GetFilename(){ return result ? wxString(wxT("")) : Filepath.AfterLast(wxFILE_SEP_PATH); }
wxString GetPath();

bool IsFileExecutable();                                                         // See if the file is executable (by SOMEONE, not necessarily us)
bool CanTHISUserRead();
bool CanTHISUserWrite();
bool CanTHISUserExecute();
bool CanTHISUserWriteExec();
bool CanTHISUserRename();                                                 // See if the file's PARENT DIR is Writable by THIS USER
bool CanTHISUserChmod();                                                  // See if the file's permissions are changable by US

uid_t OwnerID(){ return statstruct->st_uid; }                      // Owner ID
gid_t GroupID(){ return statstruct->st_gid; }                       // Group ID
wxString GetOwner();                                                            // Returns owner's name as string
wxString GetGroup();                                                            // Returns group name as string


time_t AccessTime(){ return statstruct->st_atime; }          // Time last accessed
time_t ModificationTime(){ return statstruct->st_mtime; }  // Time last modified
time_t ChangedTime(){ return statstruct->st_ctime; }        // Time last Admin-changed

size_t Size(){ return statstruct->st_size; }                         // Size in bytes
wxString GetParsedSize();                                                     // Returns the size, but in bytes, KB or MB according to magnitude
blksize_t GetBlocksize(){ return statstruct->st_blksize; }    // Returns filesystem's blocksize
blkcnt_t GetBlockNo(){ return statstruct->st_blocks; }       // Returns no of allocated blocks for the file

ino_t GetInodeNo(){ return statstruct->st_ino; }                 // Returns inode no
dev_t GetDeviceID() { return  statstruct->st_dev; }            // Returns the device ie which disk or partition the file is on
nlink_t GetHardLinkNo(){ return statstruct->st_nlink; }        // Returns no of hard links

bool IsRegularFile(){ if ( ! IsValid() ) return false; return S_ISREG(statstruct->st_mode); }  // Is Filepath a Regular File?
bool IsDir(){ if ( ! IsValid() ) return false; return S_ISDIR(statstruct->st_mode); }	             // Is Filepath a Dir?
bool IsSymlink(){ if ( ! IsValid() ) return false; return S_ISLNK(statstruct->st_mode); }      // Is Filepath a Symlink?
bool IsCharDev(){ if ( ! IsValid() ) return false; return S_ISCHR(statstruct->st_mode); }     // Is Filepath a character device?
bool IsBlkDev(){ if ( ! IsValid() ) return false; return S_ISBLK(statstruct->st_mode); }        // Is Filepath a block device?
bool IsSocket(){ if ( ! IsValid() ) return false; return S_ISSOCK(statstruct->st_mode); }     // Is Filepath a Socket?
bool IsFIFO(){ if ( ! IsValid() ) return false; return S_ISFIFO(statstruct->st_mode); }           // Is Filepath a FIFO?

wxString Type();                                                                    // Returns string version of type eg "Directory"

bool IsUserReadable(){ return !!(statstruct->st_mode & S_IRUSR); }         // Permissions
bool IsUserWriteable(){ return !!(statstruct->st_mode & S_IWUSR); }
bool IsUserExecutable(){ return !!(statstruct->st_mode & S_IXUSR); }

bool IsGroupReadable(){ return !!(statstruct->st_mode & S_IRGRP); }
bool IsGroupWriteable(){ return !!(statstruct->st_mode & S_IWGRP); }
bool IsGroupExecutable(){ return !!(statstruct->st_mode & S_IXGRP); }

bool IsOtherReadable(){ return !!(statstruct->st_mode & S_IROTH); }
bool IsOtherWriteable(){ return !!(statstruct->st_mode & S_IWOTH); }
bool IsOtherExecutable(){ return !!(statstruct->st_mode & S_IXOTH); }

bool IsSetuid(){ return !!(statstruct->st_mode & S_ISUID); }
bool IsSetgid(){ return !!(statstruct->st_mode & S_ISGID); }
bool IsSticky(){ return !!(statstruct->st_mode & S_ISVTX); }

wxString PermissionsToText();                                                // Returns a string describing the filetype & permissions eg -rwxr--r--
size_t GetPermissions(){ return statstruct->st_mode & 07777; }       // Returns Permissions in numerical form

class FileData* GetSymlinkData(){ return symlinkdestination; }
wxString GetSymlinkDestination(){ if ( ! GetSymlinkData()->GetFilepath().IsEmpty() ) return GetSymlinkData()->GetFilepath(); 
                                                       else return GetSymlinkData()->BrokenlinkName;    // If the symlink's broken, this is where the original target name is stored
                                                   }
wxString GetUltimateDestination();                                         // Returns the file at the end of a series of symlinks (or original filepath if not a link)

bool DoChmod( mode_t newmode );                                     // If appropriate, change the file's permissions to newmode
bool DoChangeOwner( uid_t owner );                                    // If appropriate, change the file's owner
bool DoChangeGroup( gid_t group );                                     // If appropriate, change the file's group

wxString Filepath;
wxString BrokenlinkName;                                                     // If the file is a symlink, & it's broken, the original target name is stored here

protected:
struct stat *statstruct;
class FileData* symlinkdestination;                                       // Another FileData instance if this one is a symlink
int result;                                                                             // Stores whether the FileData instance was successfully created, eg whether the file exists
};


#endif
    // FILEDATAH 

