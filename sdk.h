// GPL License - see http://opensource.org/licenses/gpl-license.php
// Copyright 2006 *nixCoders team - don't forget to credit us

#pragma once

// i named this file 'sdk', but it actually includes stuff that
// we don't get by including the sdk
// i.e. stuff from the engine (q3 source)

typedef void *unzFile;

typedef struct fileInPack_s {
	char					*name;		// name of the file
	unsigned long			pos;		// file info position in zip
	struct	fileInPack_s*	next;		// next file in the hash
} fileInPack_t;

typedef struct {
	char			pakFilename[MAX_OSPATH];	// c:\quake3\baseq3\pak0.pk3
	char			pakBasename[MAX_OSPATH];	// pak0
	char			pakGamename[MAX_OSPATH];	// baseq3
	unzFile			handle;						// handle to zip file
	int				checksum;					// regular checksum
	int				pure_checksum;				// checksum for pure
	int				numfiles;					// number of files in pk3
	int				referenced;					// referenced file flags
	int				hashSize;					// hash table size (power of 2)
	fileInPack_t*	*hashTable;					// hash table
	fileInPack_t*	buildBuffer;				// buffer with the filenames etc.
} pack_t;

typedef struct searchpath_s {
	struct searchpath_s *next;
	pack_t *pack;
} searchpath_t;

typedef struct {
	byte ip[4];
	byte UNUSED[10];
	unsigned short port;
} netadr_t;
