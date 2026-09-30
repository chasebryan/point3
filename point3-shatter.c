#include<stdint.h>
#include<string.h>
typedef	struct{uint8_t	l[3][16];uint32_t	e;}P;
unsigned	q(unsigned	k,unsigned	i,uint32_t	e){unsigned	a[3]={1,45,61},b[3]={0,17,73},s[3]={11,29,47};return(a[k]*i+b[k]+s[k]*e)&127;}
unsigned	g(uint8_t*x,unsigned	i){return	x[i>>3]>>(i&7)&1;}
void	z(uint8_t*x,unsigned	i,unsigned	v){uint8_t	m=1u<<(i&7);x[i>>3]=(x[i>>3]&~m)|(v?m:0);}
P	p3(uint8_t*m){P	p={0};for(unsigned	i=0;i<128;i++)for(unsigned	k=0;k<3;k++)z(p.l[k],q(k,i,0),g(m,i));return	p;}
void	t(P*p){uint8_t	n[3][16]={0};for(unsigned	i=0;i<128;i++)for(unsigned	k=0;k<3;k++)z(n[k],q(k,i,p->e+1),g(p->l[k],q(k,i,p->e)));memcpy(p->l,n,48);p->e++;}
void	r(P*p,uint8_t*m){memset(m,0,16);for(unsigned	i=0;i<128;i++){unsigned	s=0;for(unsigned	k=0;k<3;k++)s+=g(p->l[k],q(k,i,p->e));z(m,i,s>1);}}
