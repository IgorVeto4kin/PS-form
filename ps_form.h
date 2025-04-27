#ifndef LIB_H
#define LIB_H

#include <stdbool.h> // Для типа bool

struct variable { // структура для хранения переменной
    char data;
    bool marked;
    struct variable* next;
};

struct PS_form { // структура для хранения одночленов
    long coefficient;
    struct variable* body;
    struct PS_form * next;
};

void removeSpaces(char* str);
struct PS_form* build_PS_form(char* str);
void free_variables(struct variable* var);
void free_ps_form(struct PS_form* ps);
bool block_check(struct variable * trace1,struct variable * trace2);
struct PS_form* copy_node(struct PS_form* node);
struct PS_form* addition(struct PS_form* ps1, struct PS_form* ps2);
bool equal_check(struct PS_form *ps1, struct PS_form *ps2);
void cout(struct PS_form* ps);
void append_term(struct PS_form** result, int coefficient, struct variable* body);
struct PS_form* multiply_terms(struct PS_form* term1, struct PS_form* term2);
struct PS_form* multiplication(struct PS_form* ps1, struct PS_form* ps2);
struct PS_form* simplify(struct PS_form* head) ;
struct PS_form* canonize(struct PS_form* head);


#endif