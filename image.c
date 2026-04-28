#include "image.h"

static int vramStack = 320;
static int clutStack = 480;

int Image_Load( Image* image, char* path, int imageVX, int imageVY, int clutVX, int clutVY ) {
    //Open the TIM file.
    File fp;
    int status = File_Open( &fp, path );
    if( status == NULL ) {
        return -1;
    }
    
    //Allocate space for the TIM file (must allocate by sector).
    int sectorCount = SectorToByte( ByteToSector ( fp.size ) );
    char* buffer = malloc( sectorCount );
    if( buffer == NULL ) {
        return -1;
    }
    
    //Read the file.
    int readStatus = File_Read( &fp, buffer, WHOLE_FILE );
    if( readStatus == 0 ) {
        return -1;
    }

    //Get the information out of the TIM file.
    OpenTIM( (u_long*) buffer );
    ReadTIM( &image->tim );

    //Make the pointers inside TIM_IMAGE point to our own prect / crect
    //So that when we free() the TIM we don't lose the data.
    image->prect = *image->tim.prect;
    image->tim.prect = &image->prect;

    image->crect = *image->tim.crect;
    image->tim.crect = &image->crect;

    if( imageVX != AUTO )   image->tim.prect->x = imageVX;
    else {
        image->tim.prect->x = vramStack;
        vramStack += image->tim.prect->w;
    }

    if( imageVY != AUTO )   image->tim.prect->y = imageVY;
    else                    image->tim.prect->y = 0;

    if( clutVX != AUTO )   image->tim.crect->x = clutVX;
    else                   image->tim.crect->x = 0;

    if( clutVY != AUTO )   image->tim.crect->y = clutVY;
    else                   image->tim.crect->y = clutStack++;

    //Load the pixel data into VRAM.
    LoadImage( image->tim.prect, image->tim.paddr );
    DrawSync( 0 );

    //If it uses CLUTs, also load them to VRAM.
    if( IsIndexed( image ) ) {
        LoadImage( image->tim.crect, image->tim.caddr );
        DrawSync( 0 );
    }

    //clean
    free( buffer );

    return 0;
}