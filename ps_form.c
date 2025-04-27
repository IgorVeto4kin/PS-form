#include "ps_form.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void removeSpaces(char* str) { //(вспомогательная) удаляет пробелы в строке, приводит к удобному виду для составления списка
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ') {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';  
}

struct PS_form* build_PS_form(char* str){// функция, которая по входной строке строит список
    struct PS_form* PS = (struct PS_form *)malloc(sizeof(struct PS_form));
    PS->next = NULL;
    PS->body = NULL;
    PS ->coefficient = 1;
    if(str[0] == '-'){
        PS ->coefficient = -1;
    }
    
    bool flag = false;
    struct PS_form* read_iterator = PS;
    struct variable * block_iterator;
    long help_number = 1;
    //here we start string iterations for struct building
    for(int i = 0; str[i] != '\0'; ++i){
        if(str[i] == '*'){
                continue;
        }
        if(isdigit(str[i]) ){            
            if ((read_iterator->coefficient == 1 || read_iterator->coefficient == -1) && (((i>0) && (isdigit(str[i-1]) == false)) || i==0 ) ){
                read_iterator->coefficient = read_iterator->coefficient * (long)(str[i] - '0');//
            }
            else if (read_iterator->coefficient > 0){
                read_iterator->coefficient = read_iterator->coefficient*10 + (long)(str[i] - '0');
            }
            else if (read_iterator->coefficient < 0){
                read_iterator->coefficient = read_iterator->coefficient*10 - (long)(str[i] - '0');
            }
        }
        if(str[i]>= 'a' && str[i]<='z' && flag == false){//letter ( variables) start
            flag = true;
            read_iterator -> body = (struct variable *)malloc(sizeof(struct variable));
            block_iterator = read_iterator->body;
            block_iterator ->data = str[i];
            block_iterator ->next = NULL;
            block_iterator ->marked = false;
        }
        else if(str[i]>= 'a' && str[i]<='z' && flag ){
            block_iterator ->next = (struct variable *)malloc(sizeof(struct variable));
            block_iterator = block_iterator ->next;
            block_iterator ->data = str[i];
            block_iterator ->next = NULL;
            block_iterator ->marked = false;
        }
        if( (str[i] == '+' || str[i] == '-') && i!=0){
            flag = false;
            read_iterator -> next = (struct PS_form *)malloc(sizeof(struct PS_form));
            read_iterator = read_iterator -> next;
            read_iterator -> next = NULL;
            if(str[i] == '+'){
                read_iterator->coefficient = 1;
            }
            else{
                read_iterator->coefficient = -1;
            }
        }
        
    }
    return PS;

}

void free_variables(struct variable* var) {
    while (var != NULL) {
        struct variable* temp = var;
        var = var->next;
        free(temp);
    }
}

void free_ps_form(struct PS_form* ps) {// функция для освобождения памяти многочлена (PS_form)
    while (ps != NULL) {
        struct PS_form* temp = ps;
        ps = ps->next;
        // освобождаем память списка переменных одночлена
        free_variables(temp->body);
        // освобождаем память одночлена
        free(temp);
    }
}

bool block_check(struct variable * trace1,struct variable * trace2){ // (вспомогательная) функция, которая проверяет являются ли два одночлена равными
    struct variable * variable_iterator1 = trace1;
    struct variable * variable_iterator2 = trace2;
    while(variable_iterator1 != NULL && variable_iterator2 != NULL){
        if(variable_iterator1 -> data == variable_iterator2 -> data && variable_iterator2 -> marked == false){
            variable_iterator1 = variable_iterator1->next;
            variable_iterator2 -> marked = true;
            variable_iterator2 = trace2;
        }
        else{
            variable_iterator2 = variable_iterator2->next;
        }
    }
    if ( variable_iterator1 == NULL){
        return true;
    }
    else{
        struct variable * clear_marks = trace2;
        while(clear_marks !=NULL){
            clear_marks -> marked = false;
            clear_marks = clear_marks -> next;
        }
        return false;
    }


}

struct PS_form* copy_node(struct PS_form* node) {
    struct PS_form* new_node = (struct PS_form*)malloc(sizeof(struct PS_form));
    new_node->coefficient = node->coefficient;
    new_node->body = node->body;  
    new_node->next = NULL;
    return new_node;
}

struct PS_form* addition(struct PS_form* ps1, struct PS_form* ps2) {// функция, которая реализует сложение
    struct PS_form* result = NULL;
    struct PS_form** last_ptr = &result;
    
    struct PS_form* iterator1 = ps1;
    while (iterator1 != NULL) {
        *last_ptr = copy_node(iterator1);
        last_ptr = &((*last_ptr)->next);
        iterator1 = iterator1->next;
    }
    
    struct PS_form* iterator2 = ps2;
    while (iterator2 != NULL) {
        struct PS_form* iterator1 = result;
        bool found = false;
        while (iterator1 != NULL) {
            if ((block_check(iterator1->body, iterator2->body) && block_check(iterator2->body, iterator1->body)) || (iterator2->body == NULL && iterator1->body == NULL)) {
                iterator1->coefficient += iterator2->coefficient;
                
                found = true;
                break;
            }
            iterator1 = iterator1->next;
        }
        if (!found) {
            *last_ptr = copy_node(iterator2);
            last_ptr = &((*last_ptr)->next);
        }
        iterator2 = iterator2->next;
    }
    
    return result;
}

bool equal_check(struct PS_form *ps1, struct PS_form *ps2){ //функция, котора реализует проверку на равенство

    struct PS_form * current1 = ps1; 
    struct PS_form * current2 = ps2; 
    while(current1 != NULL && current2 != NULL){//итерируемся по первой форме и сравниваем со второй
        struct variable * trace1 = current1 -> body;
        struct variable * trace2 = current2 -> body;
        if(current1 -> coefficient == current2 -> coefficient && block_check(trace1,trace2) == 1){ // if we found same block  
            current1 = current1 -> next;   
            current2 = ps2;

        }
        else{
            current2 = current2 -> next;
        }
    }
    return current1 == NULL;
    
    
}

void cout(struct PS_form* ps) {
    if(ps == NULL){
        printf("0\n");
        return;
    }
    struct PS_form* iter = ps;
    while (iter != NULL) {
        printf("%ld", iter->coefficient);
        if(iter->body != NULL){
                printf("*");
            }
        struct variable* var = iter->body;
        while (var != NULL) {
            printf("%c", var->data);
            if(var->next != NULL){
                printf("*");
            }
            var = var->next;
        }
        if(iter->next != NULL && iter->next->coefficient > 0){
            printf(" + ");
        }
        else if (iter->next != NULL && iter->next->coefficient < 0){
            printf(" - ");
            iter->next->coefficient *= -1;
        }
        
        iter = iter->next;
    }
    printf("\n");
}

void append_term(struct PS_form** result, int coefficient, struct variable* body) { // (вспомогательная) функция для добавления нового одночлена в  многочлена
    struct PS_form* new_term = (struct PS_form*)malloc(sizeof(struct PS_form));
    new_term->coefficient = coefficient;
    new_term->body = body;
    new_term->next = NULL;

    if (*result == NULL) {
        *result = new_term;
    } else {
        struct PS_form* iterator = *result;
        while (iterator->next != NULL) {
            iterator = iterator->next;
        }
        iterator->next = new_term;
    }
}

struct PS_form* multiply_terms(struct PS_form* term1, struct PS_form* term2) { // (вспомогательная)функция, которая перемножает одночлены
    struct PS_form* result = (struct PS_form*)malloc(sizeof(struct PS_form));
    result->coefficient = term1->coefficient * term2->coefficient;
    result->next = NULL;

    struct variable* body1 = term1->body;
    struct variable* body2 = term2->body;

    struct variable* result_body = NULL;
    struct variable** last_ptr = &result_body;

    while (body1 != NULL) {
        *last_ptr = (struct variable*)malloc(sizeof(struct variable));
        (*last_ptr)->data = body1->data;
        (*last_ptr)->marked = body1->marked;
        (*last_ptr)->next = NULL;
        last_ptr = &((*last_ptr)->next);
        body1 = body1->next;
    }

    while (body2 != NULL) {
        *last_ptr = (struct variable*)malloc(sizeof(struct variable));
        (*last_ptr)->data = body2->data;
        (*last_ptr)->marked = body2->marked;
        (*last_ptr)->next = NULL;
        last_ptr = &((*last_ptr)->next);
        body2 = body2->next;
    }

    result->body = result_body;
    return result;
}

struct PS_form* multiplication(struct PS_form* ps1, struct PS_form* ps2) { // функция, реализующая умножение
    struct PS_form* result = NULL;

    struct PS_form* ps1_iterator = ps1;
    while (ps1_iterator != NULL) {
        struct PS_form* ps2_iterator = ps2;
        while (ps2_iterator != NULL) {
            struct PS_form* new_term = multiply_terms(ps1_iterator, ps2_iterator);
            append_term(&result, new_term->coefficient, new_term->body);
            free(new_term);
            ps2_iterator = ps2_iterator->next;
        }
        ps1_iterator = ps1_iterator->next;
    }

    return result;
}

struct PS_form* simplify(struct PS_form* head) { // (вспомогательная) приводим подобные слагаемые
    struct PS_form* current = head;

    while (current != NULL) {
        struct PS_form* prev = current;
        struct PS_form* compare = current->next;

        while (compare != NULL) {
            if (block_check(current->body, compare->body) && block_check(compare->body, current->body)) {
                
                current->coefficient += compare->coefficient;

                
                prev->next = compare->next;
                free_variables(compare->body);
                free(compare);
                compare = prev->next;
            } else {
                prev = compare;
                compare = compare->next;
            }
        }

        current = current->next;
    }

    return head;
}

struct PS_form* canonize(struct PS_form* head){
    struct PS_form* current = head;
    struct PS_form* previous;
    
    bool first = true;

    while (current != NULL) {
        if(current ->coefficient != 0 && first){
            previous = current;
            first = false;
            current = current->next;

        }
        else if(current ->coefficient != 0 && first == false){
            previous = current;
            current = current->next;
        }
        else if(current ->coefficient == 0 && first){
            head = current->next;
            previous = current->next;
            free(current);
            current = previous;
        }
        else if(current ->coefficient == 0 && first == false){
            previous->next = current->next;
            free(current);
            current = previous->next;
        }
    }
    return head;
}
