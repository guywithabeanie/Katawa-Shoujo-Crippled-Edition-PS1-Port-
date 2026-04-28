#include "file.h"

int File_Open( File* file, char* path ) {
    return CdSearchFile( file, path ) != NULL;
}

int File_Read( File* file, void* buffer, size_t count ) {
    int bytesToRead = ( count != WHOLE_FILE ) ? count : file->size;
    //Move the disk to position where the data starts.
    CdControl( CdlSetloc, (u_char*)&file->pos, NULL );
    //Read the file ( has to read in sectors unfortunately ).
    int readStatus = CdRead( ByteToSector( bytesToRead ), (u_long*) buffer, CdlModeSpeed );
    //Wait for the read to finish.
    CdReadSync(0, 0);
    return readStatus;
}