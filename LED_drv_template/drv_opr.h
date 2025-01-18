/**------------------------------------------------------------------------
 * ?                                ABOUT
 * @author         :  Senpouuu
 * @email          :  
 * @repo           :  
 * @createdOn      :  2025-1-18 17:51:49
 * @description    :  接口头文件 
 *------------------------------------------------------------------------**/
#ifndef _DRV_OPR_H
#define _DRV_OPR_H



struct drv_operations {
	int (*init) (int which); /* 初始化LED, which-哪个LED */       
	int (*ctl) (int which, char status); /* 控制LED, which-哪个LED, status:1-亮,0-灭 */
};

struct drv_operations *get_board_drv_opr(void);


#endif

