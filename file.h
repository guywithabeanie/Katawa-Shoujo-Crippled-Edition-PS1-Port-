#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <libcd.h>

#include "error.h"

#define WHOLE_FILE 0
#define CD_SECTOR_SIZE 2048

//Make sure it reads atleast 1 sector.
#define ByteToSector(n) ( ( (n) + CD_SECTOR_SIZE - 1 ) / CD_SECTOR_SIZE )
#define SectorToByte(n) ( (n) * CD_SECTOR_SIZE )
#define File_Init() CdInit()

typedef CdlFILE File;

Status File_Open( File* file, char* path );

Status File_Read( File* file, void* buffer, size_t count );

#endif