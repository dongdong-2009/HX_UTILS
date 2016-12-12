#ifndef HX_CLCD_H__
#define HX_CLCD_H__

extern void hx_clcd_disp_char(int x,int y,char c,int inverse);
extern int hx_clcd_disp_chars(
	int x,		//列
	int y,		//行
	const char *s,	//数据
	int len,		//数据长度
	int inverse		//0正常,1反色
);
extern int hx_clcd_disp_str(
	int x,			//列
	int y,			//行
	const char *s,	//数据
	int align,		//N=0左对齐, N>0右对齐按N个字符数, N<0中间对齐按照-N个字符数
	int mode,		//0擦除整行,1不擦除
	int inverse		//0正常,1反色
);
extern void hx_clcd_disp_str2(int x,int y,const char *s,int align,int mode);

#endif
