#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <ft2build.h>
#include <wchar.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <stdint.h>

#include FT_FREETYPE_H
#include FT_GLYPH_H

static struct fb_var_screeninfo var;
static char* fb_base;
FT_Library  library;


void lcd_put_pixel(int x, int y, unsigned int color) 
{ 
    int line_width = var.xres * var.bits_per_pixel / 8; 
    int pixel_width = var.bits_per_pixel / 8; 

    unsigned char *pen_8 = fb_base + y * line_width + x * pixel_width; 
    unsigned short *pen_16; 
    unsigned int *pen_32; 

    unsigned int red, green, blue; 

    pen_16 = (unsigned short *)pen_8; 
    pen_32 = (unsigned int *)pen_8; 

    // Extract RGB components
    red   = (color >> 16) & 0xff; 
    green = (color >> 8) & 0xff; 
    blue  = (color >> 0) & 0xff; 

    switch (var.bits_per_pixel) 
    { 
        case 8: 
            *pen_8 = color; 
            break; 
        case 16: 
            // Convert RGB888 to RGB565
            color = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3); 
            *pen_16 = color; 
            break; 
        case 32: 
            *pen_32 = color; 
            break; 
        default: 
            printf("can't support %dbpp\n", var.bits_per_pixel); 
            break; 
    } 
}

int LCD_Init() 
{
    int fd = open("/dev/fb0", O_RDWR);
    int fb_len; 

    if (fd < 0) 
    {
        perror("open fb0 failed");
        return 1;
    }

    if (ioctl(fd, FBIOGET_VSCREENINFO, &var) < 0) 
    {
        perror("get vscreeninfo failed");
        close(fd);
        return 1;
    }

    fb_len = var.xres * var.yres * var.bits_per_pixel / 8;
    fb_base = mmap(0, fb_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fb_base == MAP_FAILED) 
    {
        perror("mmap failed");
        close(fd);
        return 1;
    }

}

void LCD_show_bmp(FT_Bitmap* bitmap,int x,int y,int color)    
{
    int offset = bitmap->pitch;
    for(int i=0;i<bitmap->rows;i++)
        for(int j=0;j<bitmap->width;j++)
        {
            uint8_t color = bitmap->buffer[i*offset+j];
            if(color!=0)
                lcd_put_pixel(x+j,y+i,color);
        }
}


void LCD_Show_String(FT_Face face, wchar_t* str, int x, int y,int color,size_t size)
{
    int err,i;
    int offset_x = 0;
    int offset_y = 0;
    FT_BBox bbox = {0,0,0,0};
    FT_Vector pen = {0,0};
    FT_Glyph glyph;
    FT_GlyphSlot slot = face->glyph;

    err = FT_Set_Pixel_Sizes(face, size, 0);
    if(err)
    {
        fprintf(stderr,"FT_Set_Char_Size error %d\n",err);
        exit(1);
    }
    
    
    printf("len of str:%d\n",wcslen(str));

    for(i = 0;i < wcslen(str);i++)
    {
        FT_BBox bbox_i;
        offset_x += slot->advance.x/64;
        offset_y += slot->advance.y/64;
        
        err = FT_Load_Char(face, str[i], FT_LOAD_RENDER);
        
        FT_Get_Glyph(slot, &glyph);
        FT_Glyph_Get_CBox(glyph, FT_GLYPH_BBOX_TRUNCATE, &bbox_i);

        bbox.xMin = (bbox_i.xMin < bbox.xMin) ? bbox_i.xMin : bbox.xMin;
        bbox.yMin = (bbox_i.yMin < bbox.yMin) ? bbox_i.yMin : bbox.yMin;
        bbox.xMax = (bbox_i.xMax > bbox.xMax) ? bbox_i.xMax : bbox.xMax;
        bbox.yMax = (bbox_i.yMax > bbox.yMax) ? bbox_i.yMax : bbox.yMax;

        pen.x = bbox_i.xMin;
        pen.y = (bbox.yMax - bbox_i.yMax);

        if(err)
        {   
            fprintf(stderr,"FT_Load_Char error %d\n",err);
            exit(1);
        }
        if(x+offset_x+slot->bitmap.width>var.xres)
        {
            offset_x = 0;
            y=y+slot->bitmap.rows;
        }
        LCD_show_bmp(&slot->bitmap,x+pen.x+offset_x,y+pen.y+offset_y,color);
    }
}

int main()
{
    int err;
    wchar_t str[] = L"森哥真是帅!";

    LCD_Init();
    
    err = FT_Init_FreeType(&library);
    if(err)
    {
        fprintf(stderr,"FT_Init_FreeType error %d\n",err);
        return 1;
    }

    FT_Face face;
    err = FT_New_Face(library, "MSYH.TTC", 0, &face);
    if(err)
    {
        fprintf(stderr,"FT_New_Face error %d\n",err);
        return 1;
    }

    printf("Loaded font %s\n", face->family_name);

    LCD_Show_String(face, str, 100, 100, 0x660066, 49);

}


