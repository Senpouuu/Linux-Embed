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
	int (*init) (int which);
	int (*ctl) (int which, char status); 
};

struct drv_operations *get_board_drv_opr(void);


#endif

