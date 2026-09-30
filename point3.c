#include<stdint.h>
#include<stdio.h>
uint32_t e(uint8_t x){uint32_t y=0;for(int i=8;i--;)y=y<<3|((x>>i&1)*7);return y;}
uint8_t d(uint32_t x){uint8_t y=0;for(int i=8;i--;){uint32_t c=x>>(i*3)&7;y=y<<1|(((c>>2&1)+(c>>1&1)+(c&1))>1);}return y;}
int main(){uint32_t x=e(0x5a)^(1u<<13);printf("%02x\n",d(x));}
