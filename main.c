#include "renderer.h"
#include "scene.h"
#include "font.h"
#include "fontData.h"

#include <libgte.h>
#include <libetc.h>

int main() {
    Renderer renderer;
    Renderer_Init( &renderer, (Color){ 0, 0, 0 } );

    static unsigned char heap[0xFFFFF];
    InitHeap( (unsigned long*) heap, sizeof(heap) );

    File_Init();

    SceneInfo sceneInfo = {
        .actorCount = 0,
        .background = SnowyWoods,
        .effect = SnowFall
    };

    Font font;
    Font_Load( &font, "\\GFX\\FONT\\PLAYTIME.TIM;1", &playtimeFontData );

    Scene scene;
    int status = Scene_Init( &scene, &sceneInfo, NULL );

    while( 1 ) {
        FntPrint("Rin's Arms\n");

        Scene_Update( &scene );

        Renderer_DrawText( &renderer, &font, "A light breeze causes the naked branches overhead to rattle like\nwooden windchimes.", 5, 350, DEFAULT );
        Renderer_DrawScene( &renderer, &scene );

        Renderer_Refresh( &renderer );
    }

    return 0;
}