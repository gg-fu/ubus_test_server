#ifndef _UBUS_SERVER_H
#define _UBUS_SERVER_H

#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include "libubus.h"
#include "blobmsg_json.h"

int Send_user_message();
void Bru_create_pthread(void *(*func)(void *),void *arg);
void ubus_thread(char *arg);


#endif


