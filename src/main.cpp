#include <Arduino.h>

#include "board.h"
#include "conway.h"
#include "anti_conway.h"
#include "ulam.h"
#include "anti_ulam.h"
#include "langton.h"
#include "pong.h"
#include "text.h"

#define LENGTH 21
#define WIDTH 21

#define CA(name)\
{\
 board = (BoolBoard*)malloc(sizeof(name));\
 board = new (board) name(LENGTH, WIDTH, seed_func, rng_func);\
}

//Invoke the destructor before freeing the object
#define DA()\
{\
 board->BoolBoard::~BoolBoard();\
 free(board);\
}

//Overload for placement new (no memory is allocated, constructor hierarchy is invoked)
void * operator new(size_t size, void *p) {
 return p;
}

/*
//Overload for sized delete (circa c++14)
void operator delete(void *p, size_t size) {
// Serial.println("void operator delete(void *p, size_t size)");
// Serial.println((unsigned long int)p);
// Serial.println((unsigned long int)size);
// delay(2000);
 free(p);
}
*/

void handle_input();
void clear();
void print_board(BoolBoard *board);
void display_board(BoolBoard *board);

extern "C" {
 int free_memory();
}

extern unsigned int __heap_start;
extern void *__brkval;

//The free list structure as maintained by the avr-libc memory allocation routines.
struct __freelist {
  size_t sz;
  struct __freelist *nx;
};

//The head of the free list structure
extern struct __freelist *__flp;

//Calculates the size of the free list
int free_list_size() {
  struct __freelist* current;
  int total = 0;
  for (current = __flp; current; current = current->nx) {
    //Add two bytes for the memory block's header.
    total += 2;
    total += (int) current->sz;
  }
  return total;
}

int free_memory() {
  int free_memory_c;
  if ((int)__brkval == 0) {
    free_memory_c = ((int)&free_memory_c) - ((int)&__heap_start);
  } else {
    free_memory_c = ((int)&free_memory_c) - ((int)__brkval);
    free_memory_c += free_list_size();
  }
  return free_memory_c;
}

const SEED seed_func = [] () {
 randomSeed(analogRead(0));
};

const RNG rng_func = [] (int min, int max) {
 return (int)random(min, max);
};

BoolBoard *board;

void setup() {
 //TODO setup pins for display columns and rows
 //Setup serial
 Serial.begin(38400);
 CA(AntiUlam);
// board = new AntiUlam(LENGTH, WIDTH, seed_func, rng_func);
}

bool update = true;

void loop() {
 handle_input();
 if (update) {
  clear();
  print_board(board);
  display_board(board);
  board->update();
  Serial.flush();
  delay(1000);
 }
}

void handle_input() {
 if (Serial.available()) {
  String input = Serial.readString();
  //Remove newline character
  input = input.substring(0, input.lastIndexOf("\n"));
  if (input.indexOf("pause") != -1) {
   update = false;
  } else if (input.indexOf("resume") != -1) {
   update = true;
  } else if (input.indexOf("print") != -1) {
   print_board(board);
  } else if (input.indexOf("clear board") != -1) {
   board->clear_board();
  } else if (input.indexOf("clear") != -1) {
   clear();
  } else if (input.indexOf("random") != -1) {
   board->generate_random_board();
  } else if (input.indexOf("set") != -1) {
   //TODO set cells on the grid as 0 or 1
   char *c_str = input.c_str();
   char *subs = strtok(c_str, " ");
  } else if (input.indexOf("free") != -1) {
   Serial.println(free_memory());
  } else if (input.indexOf("AntiConway") != -1) {
   DA();
   CA(AntiConway);
  } else if (input.indexOf("Conway") != -1) {
   DA();
   CA(Conway);
  } else if (input.indexOf("AntiUlam") != -1) {
   DA();
   CA(AntiUlam);
  } else if (input.indexOf("Ulam") != -1) {
   DA();
   CA(Ulam);
  } else if (input.indexOf("Langton") != -1) {
   DA();
   CA(Langton);
  } else if (input.indexOf("Pong") != -1) {
   DA();
   board = (BoolBoard*)malloc(sizeof(Pong));
   board = new (board) Pong(LENGTH, WIDTH, 2, false, seed_func, rng_func);
   board->clear_board();
  } else if (input.indexOf("Text") != -1) {
   DA();
   board = (BoolBoard*)malloc(sizeof(Pong));
   board = new (board) Text(LENGTH, WIDTH, "Test", 4, seed_func, rng_func);
   board->clear_board();
  } else {
   Serial.print("'");
   Serial.print(input);
   Serial.print("' was not understood.\n");
  }
 }
}

//Clears a terminal that supports escape codes
//Arduino studio serial console does not support escape codes
void clear() {
 Serial.write(27);
 Serial.print("[2J");
 Serial.write(27);
 Serial.println("[H");
}

void print_board(BoolBoard *board) {
 static unsigned int buff_length = board->width + 2;
 static char *buff = new char[buff_length];
 if (buff_length - 2 != board->width) {
  buff_length = board->width + 2;
  delete[] buff;
  buff = new char[buff_length];
 }
 for (unsigned int x = 0; x < board->len; x++) {
  for (unsigned int y = 0; y < board->width; y++) {
   buff[y] = ((*board)[x * board->len + y] ? 'x' : '.');
  }
  buff[buff_length - 2] = '\n';
  buff[buff_length - 1] = '\0';
  Serial.print(buff);
 }
}

void display_board(BoolBoard *board) {
 //TODO display board in 6x5 LED board
}
