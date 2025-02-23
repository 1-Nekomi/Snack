#ifndef _OUTCOLOR_H_
#define _OUTCOLOR_H_

/*color define*/
#define COLOR_END           "\033[0m"     
#define BROWN               "\033[0;33m"         //深黄（棕色） 
#define GOLDEN              "\033[1;33m"         //金色（浅黄） 
#define GREEN               "\033[0;32m"         //深绿，暗绿
#define L_GREEN             "\033[1;32m"         //鲜绿
#define BLUE                "\033[0;34m"         //深蓝，暗蓝
#define L_BLUE              "\033[1;34m"         //亮蓝，偏白灰
#define PURPLE              "\033[0;35m"         //深粉，暗粉，偏暗紫
#define L_PURPLE            "\033[1;35m"         //亮粉，偏白灰
#define RED                 "\033[0;31m"         //深红，暗红
#define L_RED               "\033[1;31m"         //鲜红
#define DARK_GRAY          "\033[1;30m"			// 深灰（原“黑色”加粗）
#define L_GRAY             "\033[0;37m"			// 浅灰（原“白色”未加粗）
#define WHITE              "\033[1;37m"			// 亮白
#define CYAN               "\033[0;36m"			// 青色（深蓝绿）
#define L_CYAN             "\033[1;36m"			// 亮青色（浅蓝绿）
#define ORANGE             "\033[38;5;208m"		// 橙色（256色模式）
#define PINK               "\033[38;5;206m"		// 粉色（256色模式）

/*背景填充色*/
#define BACK_BLACK          "\033[0;40m"         //黑 
#define BACK_RED            "\033[0;41m"         //红 
#define BACK_GREEN        "\033[0;42m"           //绿
#define BACK_YELLOW         "\033[0;43m"         //黄 
#define BACK_BLUE           "\033[0;44m"         //蓝 
#define BACK_PURPLE         "\033[0;45m"         //紫 
#define BACK_CYAN          "\033[1;46m"         //青色 
#define BACK_WHITE          "\033[1;47m"         //白

/* 256色模式 */
#define COLOR_256(n)       "\033[38;5;" #n "m"   // 前景色，n为0-255
#define BG_COLOR_256(n)    "\033[48;5;" #n "m"   // 背景色，n为0-255

#endif
