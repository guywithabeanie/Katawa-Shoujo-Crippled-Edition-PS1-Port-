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

int Renderer_DrawImage( Renderer* renderer, Image* image, int x, int y, Color tint, TransparencyMode transparencyMode ) {
    TIM_IMAGE* tim = &image->tim;
    int imageDepth = tim->mode & 0x03;

    //Rendering works with DWords (16 bits), not with pixels.
    //So when you have a 4 bit picture, a single DWord holds 4 pixels, and with 8 bit it's 2 pixels.
    int pixelPerWord = Is4Bit( image ) ? 4 : Is8Bit( image ) ? 2 : 1;
    int wordPerTPage = Is4Bit( image ) ? 64 : Is8Bit( image ) ? 128 : 256;

    //Used for bitshifting (optimization).
    int bitsPerTPage = Is4Bit( image ) ? 6 : Is8Bit( image ) ? 7 : 8;

    //Find out the TPages where the image spans in VRam on both axis.
    int startPageX = tim->prect->x >> bitsPerTPage; 
    int endPageX = ( tim->prect->x + tim->prect->w - 1 ) >> bitsPerTPage;
    int startPageY = tim->prect->y >> 8;
    int endPageY = ( tim->prect->y + tim->prect->h - 1 ) >> 8;

    //Use the TPages that the image spans to find out the amount of TPages on both axis.
    int tpageCountX = endPageX - startPageX + 1;
    int tpageCountY = endPageY - startPageY + 1;

    int drawY = y;
    int currentVramY = tim->prect->y;
    int remainingHeight = tim->prect->h;

    for( int j = 0; j < tpageCountY; j++) {
        int drawX = x;
        int currentVramX = tim->prect->x;
        int remainingWidth = tim->prect->w;

        int verticalOffset = currentVramY & 0xFF;
        int sliceHeight = 256 - verticalOffset;
        if( sliceHeight > remainingHeight ) sliceHeight = remainingHeight;
        
        for( int i = 0; i < tpageCountX; i++ ) {
            Texture* TextureObj = TextureTable_Fetch( renderer );
            if( TextureObj == NULL ) return -1;

            int horizontalOffset = currentVramX & ( wordPerTPage - 1 );
            int sliceWidth = wordPerTPage - horizontalOffset;
            if( sliceWidth > remainingWidth ) sliceWidth = remainingWidth;

            SPRT* Texture = &TextureObj->sprt;
            DR_TPAGE* TPage = &TextureObj->tPage;

            SetSprt( Texture );
            setXY0( Texture, drawX, drawY );
            setWH( Texture, sliceWidth * pixelPerWord, sliceHeight );
            setUV0( Texture, ( currentVramX & ( wordPerTPage - 1 ) ) * pixelPerWord, currentVramY & 0xFF );
            setRGB0( Texture, tint.red, tint.green, tint.blue );
            if( transparencyMode != Opaque )    setSemiTrans( Texture, 1 );

            //Retrieve the CLUTs if 4-bit / 8-bit.
            if( IsIndexed( image ) ) {
                setClut( Texture, image->tim.crect->x, tim->crect->y );
            }
            
            int TPageX = ( currentVramX >> bitsPerTPage ) << bitsPerTPage;
            int TpageY = ( currentVramY >> 8 ) << 8;

            setDrawTPage( TPage, 0, 1, getTPage( imageDepth, transparencyMode, TPageX, TpageY ) );

            addPrim( renderer->orderingTable, Texture );
            addPrim( renderer->orderingTable, TPage );

            currentVramX += sliceWidth;
            remainingWidth -= sliceWidth;
            drawX += sliceWidth * pixelPerWord;
        }

        currentVramY += sliceHeight;
        remainingHeight -= sliceHeight;
        drawY += sliceHeight;
    }
} 