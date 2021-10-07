#ifndef __CONTROLE_HPP__
#define __CONTROLE_HPP__

#include "pins.hpp"


/*defines movimento*/
#define KP 1
#define KI 0.001
#define KD 0.005


void pid(void);
void ultra_read(void);
void controle(void);
void movimento_aleatorio(void);

#endif