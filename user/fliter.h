#ifndef __FLITER_H
#define __FLITER_H
#include "includes.h"

//ÌîÂúÂË²¨Êý×é
void fill_array(void);

void moving_fliter(float oularoll,float oulapitch,float oulayaw,short gyrox,short gyroy,short gyroz);
void moving_height_fliter(float oularoll,float oulapitch,float oulayaw,short gyrox,short gyroy,short gyroz);
void quanvalue_allocate(u8 max_pin,u8 min_pin,u8* quanvalue_array);
#endif

