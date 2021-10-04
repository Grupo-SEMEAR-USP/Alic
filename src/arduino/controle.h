#ifndef __CONTROLE_H__
#define __CONTROLE_H__

/*defines movimento*/
#define KP 1
#define KI 0.001
#define KD 0.005

#define SENSOR_FRENTE 4


void pid(void);
void leitura_ultra(void);
void controle(void);

#endif