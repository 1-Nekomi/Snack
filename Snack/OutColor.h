#ifndef _OUTCOLOR_H_
#define _OUTCOLOR_H_

/*color define*/
#define COLOR_END           "\033[0m"     
#define BROWN               "\033[0;33m"         //��ƣ���ɫ�� 
#define GOLDEN              "\033[1;33m"         //��ɫ��ǳ�ƣ� 
#define GREEN               "\033[0;32m"         //���̣�����
#define L_GREEN             "\033[1;32m"         //����
#define BLUE                "\033[0;34m"         //����������
#define L_BLUE              "\033[1;34m"         //������ƫ�׻�
#define PURPLE              "\033[0;35m"         //��ۣ����ۣ�ƫ����
#define L_PURPLE            "\033[1;35m"         //���ۣ�ƫ�׻�
#define RED                 "\033[0;31m"         //��죬����
#define L_RED               "\033[1;31m"         //�ʺ�
#define DARK_GRAY          "\033[1;30m"			// ��ң�ԭ����ɫ���Ӵ֣�
#define L_GRAY             "\033[0;37m"			// ǳ�ң�ԭ����ɫ��δ�Ӵ֣�
#define WHITE              "\033[1;37m"			// ����
#define CYAN               "\033[0;36m"			// ��ɫ�������̣�
#define L_CYAN             "\033[1;36m"			// ����ɫ��ǳ���̣�
#define ORANGE             "\033[38;5;208m"		// ��ɫ��256ɫģʽ��
#define PINK               "\033[38;5;206m"		// ��ɫ��256ɫģʽ��

/*�������ɫ*/
#define BACK_BLACK          "\033[0;40m"         //�� 
#define BACK_RED            "\033[0;41m"         //�� 
#define BACK_GREEN        "\033[0;42m"           //��
#define BACK_YELLOW         "\033[0;43m"         //�� 
#define BACK_BLUE           "\033[0;44m"         //�� 
#define BACK_PURPLE         "\033[0;45m"         //�� 
#define BACK_CYAN          "\033[1;46m"         //��ɫ 
#define BACK_WHITE          "\033[1;47m"         //��

/* 256ɫģʽ */
#define COLOR_256(n)       "\033[38;5;" #n "m"   // ǰ��ɫ��nΪ0-255
#define BG_COLOR_256(n)    "\033[48;5;" #n "m"   // ����ɫ��nΪ0-255

#endif
