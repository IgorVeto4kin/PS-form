#include <stdio.h>
#include "ps_form.h"
#define MAX_LEN 3000

int main() {
    char operator;
    char str1[MAX_LEN];
    
    scanf("%c", &operator);
    while (getchar() != '\n');
    
    fgets(str1, MAX_LEN, stdin);
    str1[strcspn(str1, "\n")] = '\0'; 
    removeSpaces(str1);

    struct PS_form* PS_form1 = build_PS_form(str1);
    char str2[MAX_LEN];
    fgets(str2, MAX_LEN, stdin);
    str2[strcspn(str2, "\n")] = '\0'; 
    removeSpaces(str2);

    struct PS_form* PS_form2 = build_PS_form(str2);
    
        
    if(operator == '='){
        if(equal_check(PS_form1, PS_form2) && equal_check(PS_form2, PS_form1)){
            printf("equal\n");
        }
        else{
            printf("not equal\n");
        }
    }

    else if(operator == '+'){
        cout(canonize(addition(PS_form1, PS_form2)));
    }
    else if (operator == '-'){
        struct PS_form* inverse_iter = PS_form2;
        while(inverse_iter != NULL){ //для реализации вычитания мы просто умножаем все коэффиценты второй PS-формы на -1 и производим сложение
            inverse_iter -> coefficient *= -1;
            inverse_iter = inverse_iter->next;
        }
        cout(canonize(addition(PS_form1, PS_form2)));
    }

    else if(operator == '*'){
        cout(canonize(simplify(multiplication(PS_form2, PS_form1)))); 
        
    }
    else if(operator == 'k'){
        PS_form1 = canonize(PS_form1);
    
    }

    free_ps_form(PS_form1);
    free_ps_form(PS_form2);


    
    return 0;
}