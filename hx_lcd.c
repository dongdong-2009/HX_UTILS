#include "board.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "hx_utils.h"

#define LCD_COLS_PXL		(240)
#define LCD_COLS_PAGE		(240)
#define LCD_ROWS_PXL		(128)
#define LCD_ROWS_PAGE		(128/8)		//16
//char g_lcd_map[LCD_COLS_PXL * LCD_ROWS_PXL / 8];


//hx_lcd_draw_bmp(int 


//void lcd_init(void)
//{
//    LCD_int();
//}
//void lcd_clear(void)
//{
//    White(1);
//}
//#define CHARICTER_WIDTH		(8)
//void lcd_disp_at_ex(
//	int row,
//	int col,
//	const char *s,
//	int align,		//-1左对齐 0居中 1右对齐
//	int mode,		//0擦除对应行,1不查处
//	int inverse		//0正常,1反色
//)
//{
//	unsigned char _inverse = inverse ? 0 : 1;
//	int n;//实际显示字符数
//    int empty;	//显示区域字符数
//    unsigned char buf[30];
//    memset(buf,' ',30);

//    if(row>=LCD_MAX_ROW || col>=LCD_MAX_COL)
//        return ;

//    n= strlen(s);

//    if(align<0)
//    {
//        empty = LCD_MAX_COL - col;
//        if(n>empty)
//            n = empty;
//        if(mode==0)	//清楚一整行
//        {
//            memcpy(buf+col,s,n);
//            dprintf(0,row*2,buf,_inverse,1,LCD_MAX_COL);
//        }
//        else
//        {
//            dprintf(CHARICTER_WIDTH*col,row*2,(void*)s,_inverse,1,n);
//        }
//    }
//    else if(align == 0)
//    {
//        empty = LCD_MAX_COL-col;
//        if(empty>(LCD_MAX_COL/2))
//            empty = LCD_MAX_COL - empty;
//        empty <<=1;
//        if(n>empty)
//            n = empty;
//        if(mode==0)	//清楚一整行
//        {
//            memcpy(buf+col-(n>>1),s,n);
//            dprintf(0,row*2,buf,_inverse,1,LCD_MAX_COL);
//        }
//        else
//        {
//            dprintf(CHARICTER_WIDTH*(col-(n>>1)),row*2,(void*)s,_inverse,1,n);
//        }
//    }
//    else
//    {
//        empty = LCD_MAX_COL - col;
//        if(n>empty)
//            n = empty;
//        if(mode==0)	//清楚一整行
//        {
//            memcpy(buf+LCD_MAX_COL-n,s,n);
//            dprintf(0,row*2,buf,_inverse,1,LCD_MAX_COL);
//        }
//        else
//        {
//            dprintf(CHARICTER_WIDTH*(LCD_MAX_COL-n),row*2,
//				(void*)s,_inverse,1,n);
//        }
//    }
//}
//void lcd_disp_at(int row,int col,const char *s,int align,int mode)
//{
//    lcd_disp_at_ex(row,col,s,align,mode,0);
//}
//void lcd_disp_text(char *dbuf)
//{
//	lcd_clear();
//	char *p = strtok(dbuf,"\n");
//	p = hx_strtrim2(p,"\r");
//	int i=0;
//	while(p && i<8){
//		lcd_disp_at(i++, 0, p, -1, 0);
//		p = strtok(NULL,"\n");
//		p = hx_strtrim2(p,"\r");
//	}
//}

