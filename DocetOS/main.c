#include "../OS/os.h"
#include "../DataStructures/heap.h"
#include <stdio.h>
#include "../OS/serial.h"
#include "../OS/stochasticScheduler.h"
#include <stdlib.h>
#include "../DataStructures/mutex.h"
#include "../DataStructures/hashtable.h"
#include "../OS/memcluster.h"
#include "../OS/structs.h"
#include "../DataStructures/queue.h"
#include "../DataStructures/channel.h"

#define MEMPOOL_SIZE 26384 
__align(8)
static uint32_t memory[MEMPOOL_SIZE]; 

static OS_mutex_t printLock, task5_8Lock;
static volatile uint32_t taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8 = 0;

void task1(void const *const args) {
	while(1){
		taskcounter1++;
		if(OS_mutex_acquire_non_blocking(&task5_8Lock)){
			OS_mutex_acquire(&printLock);
			printf("\t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \r\n",
						 taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
			OS_mutex_release_noYield(&printLock);
			OS_mutex_release(&task5_8Lock);
		}
	}
}

void task2(void const *const args) {
	while(1){
		taskcounter2++;
		OS_mutex_acquire(&printLock);
		printf("\t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \r\n",
					 taskcounter1, taskcounter2, taskcounter3, taskcounter4, taskcounter5, taskcounter6, taskcounter7, taskcounter8);
		OS_mutex_release(&printLock);
	}
}

void task3(void const *const args) {
	while(1){
		taskcounter3++;
		OS_mutex_acquire(&printLock);
		printf("\t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \r\n",
					 taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
		OS_mutex_release(&printLock);
	}
}

void task4(void const *const args) {
	while (1) {
		taskcounter4++;
		OS_mutex_acquire(&printLock);
		printf("\t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \t\t %04d \t\t %04d \t\t %04d \r\n",
					 taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
		OS_mutex_release(&printLock);
	}
}

void task5(void const *const args) {
	while(1){
		OS_mutex_acquire(&task5_8Lock);
		OS_mutex_acquire(&printLock);
		taskcounter5++;
		printf("\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \t\t %04d \t\t %04d \r\n",
               taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
		OS_mutex_release(&printLock);
		OS_mutex_release(&task5_8Lock);
	}
}

void task6(void const *const args) {
	while(1){
		OS_mutex_acquire(&printLock);
		taskcounter6++;
		printf("\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \t\t %04d \r\n",
               taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
		OS_mutex_release(&printLock);
		OS_sleep(rand()%100);
	}
}

void task7(void const *const args) {
	while(1){
		OS_mutex_acquire(&printLock);
		taskcounter7++;
		printf("\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\t\t %04d \r\n",
               taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
		OS_mutex_release(&printLock);
		OS_sleep(rand()%100);
	}
}

void task8(void const *const args) {
	OS_channel_t * channel_task9 = OS_channel_connect(1,16);
	while (1) {
		for(int i =0; i<100;i++){
			taskcounter8++;
			OS_mutex_acquire(&printLock);
			printf("\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\r\n",
                   taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
			OS_mutex_release(&printLock);
		}
        OS_mutex_acquire(&task5_8Lock);
		for(int i =0; i<100;i++){
			taskcounter8++;
			OS_mutex_acquire(&printLock);
			printf("\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\r\n",
                   taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8);
			OS_mutex_release(&printLock);
		}
        OS_mutex_release(&task5_8Lock);
	}
}

void task9(void const *const args) {
	OS_channel_t * channel = OS_channel_connect(2,8);
	uint32_t num = 2;
	while(1){
		if(num % 2 != 0){
			channel_write(channel,num);
		}
		num++;
	}
}

void task10(void const *const args) {
	OS_channel_t * channel = OS_channel_connect(3,8);
	uint32_t num = 3;
	while(1){
		if(num % 3 != 0){
			channel_write(channel,num);
		}
		num++;
	}
}

void task11(void const *const args) {
	OS_channel_t * channel = OS_channel_connect(4,8);
	uint32_t num = 5;
	while(1){
		if(num % 5 != 0){
			channel_write(channel,num);
		}
		num++;
	}
}

void task12(void const *const args) {
	OS_channel_t * channel = OS_channel_connect(5,8);
	uint32_t num = 1;
	while(1){
		if(num % 7 != 7){
			channel_write(channel,num);
		}
		num++;
	}
}

void task13(void const *const args) {
	OS_channel_t * channel_task9 = OS_channel_connect(2,8);
	OS_channel_t * channel_task10 = OS_channel_connect(3,8);
	OS_channel_t * channel_task11 = OS_channel_connect(4,8);
	OS_channel_t * channel_task12 = OS_channel_connect(5,8);
	OS_TCB_t * TCB9 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
	OS_TCB_t * TCB10 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
	OS_TCB_t * TCB11 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
	OS_TCB_t * TCB12 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
	uint32_t * stack9 = OS_alloc(64);
	uint32_t * stack10 = OS_alloc(64);
	uint32_t * stack11 = OS_alloc(64);
	uint32_t * stack12 = OS_alloc(64);
	OS_initialiseTCB(TCB9, stack9+64, task9, 0);
	OS_initialiseTCB(TCB10, stack10+64, task10, 0);
	OS_initialiseTCB(TCB11, stack11+64, task11, 0);
	OS_initialiseTCB(TCB12, stack12+64, task12, 0);
	OS_addTask(TCB9,9);
	OS_addTask(TCB10,10);
	OS_addTask(TCB11,11);
	OS_addTask(TCB12,12);
	uint32_t readNum1, readNum2, readNum3, readNum4 = 2;
	while (1) {
		readNum1 = channel_read(channel_task9);
		while(readNum2 < readNum1){
			readNum2 = channel_read(channel_task10);
		}
		while(readNum3 < readNum1){
			readNum3 = channel_read(channel_task11);
		}
		while(readNum4 < readNum1){
			readNum4 = channel_read(channel_task12);
		}
		if(readNum1 == (readNum2 & readNum3 & readNum4) || readNum1 == 2 || readNum1 == 3 || readNum1 == 5 || readNum1 == 7){ //
			OS_mutex_acquire(&printLock);
			printf("\t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t %04d \t\t\u001b[31m[%04d]\u001b[0m\r\n",taskcounter1,taskcounter2,taskcounter3,taskcounter4,taskcounter5,taskcounter6,taskcounter7,taskcounter8,readNum1);
			OS_mutex_release(&printLock);
		}
	}
}

void task0(void const *const args) {
    OS_TCB_t * TCB1 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB2 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB3 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB4 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB5 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB6 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB7 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB8 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    OS_TCB_t * TCB13 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
    uint32_t * stack1 = OS_alloc(64);
    uint32_t * stack2 = OS_alloc(64);
    uint32_t * stack3 = OS_alloc(64);
    uint32_t * stack4 = OS_alloc(64);
    uint32_t * stack5 = OS_alloc(64);
    uint32_t * stack6 = OS_alloc(64);
    uint32_t * stack7 = OS_alloc(64);
    uint32_t * stack8 = OS_alloc(64);
    uint32_t * stack13 = OS_alloc(64);
    OS_initialiseTCB(TCB1, stack1+64, task1, 0);
    OS_initialiseTCB(TCB2, stack2+64, task2, 0);
    OS_initialiseTCB(TCB3, stack3+64, task3, 0);
    OS_initialiseTCB(TCB4, stack4+64, task4, 0);
    OS_initialiseTCB(TCB5, stack5+64, task5, 0);
    OS_initialiseTCB(TCB6, stack6+64, task6, 0);
    OS_initialiseTCB(TCB7, stack7+64, task7, 0);
    OS_initialiseTCB(TCB8, stack8+64, task8, 0);
    OS_initialiseTCB(TCB13, stack13+64, task13, 0);
    OS_addTask(TCB1,5);
    OS_addTask(TCB2,2);
    OS_addTask(TCB3,3);
    OS_addTask(TCB4,4);
    OS_addTask(TCB5,1);
    OS_addTask(TCB6,6);
    OS_addTask(TCB7,7);
    OS_addTask(TCB8,8);
    OS_addTask(TCB13,13);
}

/* MAIN FUNCTION */
int main(void) {
	//SCnSCB->ACTLR = SCnSCB_ACTLR_DISDEFWBUF_Msk; //DEBUG, converting IMPRECISERR into PRECISERR
	/* Initialise the serial port so printf() works */
	serial_init();
	printf("\r\n***********");
	printf("\r\n* DocetOS *");
	printf("\r\n***********\r\n");

	/* Initialise the OS */
	
	OS_init(&stochasticScheduler,memory,MEMPOOL_SIZE);

	OS_TCB_t * TCB0 = (OS_TCB_t*)OS_alloc(sizeof(OS_TCB_t));
	uint32_t * stack0 = OS_alloc(64);
	OS_initialiseTCB(TCB0, stack0+64, task0, 0);
	OS_addTask(TCB0,1);

	OS_init_mutex(&printLock);
	OS_init_mutex(&task5_8Lock);
	
	
	OS_start();
}
