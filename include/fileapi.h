#pragma once

#include "libwin_msvcrt.h"

#include <stdio.h>
#include <sys/stat.h>

#include "windef.h"
#include "winnt.h"

typedef struct {
} FILETIME;

typedef FILETIME* LPFILETIME;

enum {
	DRIVE_UNKNOWN = 0,
	DRIVE_NO_ROOT_DIR,
	DRIVE_REMOVABLE,
	DRIVE_FIXED,
	DRIVE_REMOTE,
	DRIVE_CDROM,
	DRIVE_RAMDISK
};

static inline UINT GetDriveTypeA(LPCSTR lpRootPathName) {
	printf("Stubbed function GetDriveTypeA called\n");
	return DRIVE_UNKNOWN;
}

/*
FILE_ATTRIBUTE_READONLY	0x00000001	The file or directory is read-only.
FILE_ATTRIBUTE_HIDDEN	0x00000002	The file or directory is hidden (not shown in a standard directory listing).
FILE_ATTRIBUTE_SYSTEM	0x00000004	The operating system uses this file or directory part-time or exclusively.
FILE_ATTRIBUTE_DIRECTORY	0x00000010	The path identifies a directory.
FILE_ATTRIBUTE_ARCHIVE	0x00000020	The file or directory is an archive file (often used by backup software).
FILE_ATTRIBUTE_NORMAL	0x00000080	The file has no other attributes set. This attribute is valid only if used alone.
FILE_ATTRIBUTE_TEMPORARY	0x00000100	The file is being used for temporary storage.
FILE_ATTRIBUTE_REPARSE_POINT	0x00000400	The file or directory has an associated reparse point (e.g., a symlink or junction).
FILE_ATTRIBUTE_COMPRESSED	0x00000800	The file or directory is compressed.
FILE_ATTRIBUTE_ENCRYPTED	0x00004000	The file or directory is encrypted.
*/

static inline DWORD GetFileAttributesA(LPCSTR lpFileName) {
	DWORD attr = 0;

	struct stat stat_attr;

	if (stat(lpFileName, &stat_attr) != 0) {
		return INVALID_FILE_ATTRIBUTES;
	}

	if (stat_attr.st_mode & S_IRUSR || stat_attr.st_mode & S_IRGRP || stat_attr.st_mode & S_IROTH) {
		attr |= FILE_ATTRIBUTE_READONLY;
	}

	// Windows uses flags for hiding files, on linux it's convention to add . at the start of the filename.
	if (lpFileName[0] == '.') {
		attr |= FILE_ATTRIBUTE_SYSTEM;
	}

	// FILE_ATTRIBUTE_SYSTEM would be here

	if (stat_attr.st_mode & S_IFDIR) {
		attr |= FILE_ATTRIBUTE_DIRECTORY;
	}

	// FILE_ATTRIBUTE_ARCHIVE would be here

	if(strncmp(lpFileName, "/tmp", strlen("/tmp"))) {
		attr |= FILE_ATTRIBUTE_TEMPORARY;
	}

	if (stat_attr.st_mode & S_IFLNK) {
		attr |= FILE_ATTRIBUTE_REPARSE_POINT;
	}

	// FILE_ATTRIBUTE_COMPRESSED would be here

	// FILE_ATTRIBUTE_ENCRYPTED would be here

	if (attr == 0) {
		return FILE_ATTRIBUTE_NORMAL;
	}

	return attr;
}

static inline BOOL DeleteFileA(LPCSTR lpFileName) {
	return remove(lpFileName) == 0;
}
