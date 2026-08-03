#ifndef APC_H
#define APC_H
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS  0
#define FAILURE -1

typedef int data_t;

typedef struct node
{
    struct node *prev;
    data_t       data;
    struct node *next;
} Dlist;

/* ── List helpers ─────────────────────────────────────── */
Dlist *create_node(data_t data);
int    append_node(Dlist **head, Dlist **tail, data_t data);
void   prepend_node(Dlist **head, data_t data);
void   free_list(Dlist **head);
void   print_list(Dlist *head);

/* ── Input helpers ────────────────────────────────────── */
int  read_number(const char *prompt, Dlist **head, Dlist **tail, int *negative);
int  parse_number(const char *str, Dlist **head, Dlist **tail, int *negative);
void get_operator(const char *expr, char *op);
int  compare_abs(Dlist *h1, Dlist *h2);   /* -1 / 0 / 1 */

/* ── Arithmetic ───────────────────────────────────────── */
int addition      (Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR);

int subtraction   (Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR);

int multiplication(Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR);

int division      (Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR);

#endif /* APC_H */
