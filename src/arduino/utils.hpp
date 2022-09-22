#ifndef __UTILS_HPP__
#define __UTILS_HPP__

//caso seja uma build de debug (como definido no Makefile) cria esse macro
//se nao, ignora toda a vez que ele aparecer
#ifndef NODEBUG
#define PRINTD(arg) Serial.print(arg)
#define PRINTLND(arg) Serial.println(arg)
#else
#define PRINTD(arg) 
#define PRINTLND(arg)
#endif

#endif