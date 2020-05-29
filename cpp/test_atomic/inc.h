
#include <atomic>
#include <iostream>
#include <thread>
#include <string.h>
#include <time.h>
#include <assert.h>

void test_memory_order_relaxed();
void test_memory_order_acquire_release();
void test_memory_order_acq_rel();
void test_memory_order_consume_release();
void test_memory_order_seq_cst();


#define LOGI printf


void init_random();
int random(int min, int max);
