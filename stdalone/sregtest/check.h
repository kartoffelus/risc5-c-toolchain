/*
 * check.h -- low level access to special registers
 */


#ifndef _CHECK_H_
#define _CHECK_H_


int checkH(Word psw, Word val);
void checkPSW(Word psw, Word *p, Word *q);


#endif /* _CHECK_H_ */
