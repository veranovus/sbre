#include "common.h"


#include <ft2build.h>
#include FT_FREETYPE_H


void SBRE_test_freetype(void) {

    const char* font_path = "../res/font/press_start_2p/PressStart2P.ttf";


    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {

        printf("[Freetype ERROR][Freetype could not be initialized.]\n");
        return;   
    }


    FT_Face face;
    if(FT_New_Face(ft, font_path, 0, &face)) {

        printf("[Freetype ERROR][Failed to load font \"%s\".]\n", font_path);
        return;
    }
    FT_Set_Pixel_Sizes(face, 0, 48);


        
}
