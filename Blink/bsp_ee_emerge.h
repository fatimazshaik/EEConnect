#ifndef BSP_EE_EMERGE_H_
#define BSP_EE_EMERGE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void BSP_Init(void);

uint8_t Button1_Get(void);
uint8_t Button2_Get(void);
uint8_t Button3_Get(void);
uint8_t Button4_Get(void);
uint8_t Button5_Get(void);
uint8_t Button6_Get(void);


#ifdef __cplusplus
}
#endif

#endif // BSP_EE_EMRGE_H_
