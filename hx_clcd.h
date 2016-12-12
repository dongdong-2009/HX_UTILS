#ifndef HX_CLCD_H__
#define HX_CLCD_H__

extern void hx_clcd_disp_char(int x,int y,char c,int inverse);
extern int hx_clcd_disp_chars(
	int x,		//��
	int y,		//��
	const char *s,	//����
	int len,		//���ݳ���
	int inverse		//0����,1��ɫ
);
extern int hx_clcd_disp_str(
	int x,			//��
	int y,			//��
	const char *s,	//����
	int align,		//N=0�����, N>0�Ҷ��밴N���ַ���, N<0�м���밴��-N���ַ���
	int mode,		//0��������,1������
	int inverse		//0����,1��ɫ
);
extern void hx_clcd_disp_str2(int x,int y,const char *s,int align,int mode);

#endif
