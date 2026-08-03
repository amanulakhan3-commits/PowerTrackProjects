/*******************************************************************************
 * Name:Amanullakhan J Pathan
 * Title       : List helpers
 * Description : Utility functions for the doubly-linked-list number
 *               representation used by the APC library.
 *               Each node stores ONE decimal digit (0-9).
 *               head → most-significant digit, tail → least-significant digit.
 *******************************************************************************/
#include "apc.h"

/* ── node allocation ──────────────────────────────────── */
Dlist *create_node(data_t data)
{
    Dlist *node = (Dlist *)malloc(sizeof(Dlist));
    if (!node) return NULL;
    node->data = data;
    node->prev = node->next = NULL;
    return node;
}

/* Append to tail (used when building MSB→LSB) */
int append_node(Dlist **head, Dlist **tail, data_t data)
{
    Dlist *node = create_node(data);
    if (!node) return FAILURE;

    if (!*head) {
        *head = *tail = node;
    } else {
        node->prev  = *tail;
        (*tail)->next = node;
        *tail = node;
    }
    return SUCCESS;
}

/* Prepend to head (used when building LSB→MSB) */
void prepend_node(Dlist **head, data_t data)
{
    Dlist *node = create_node(data);
    if (!node) return;

    node->next = *head;
    if (*head) (*head)->prev = node;
    *head = node;
}

/* ── cleanup ──────────────────────────────────────────── */
void free_list(Dlist **head)
{
    Dlist *cur = *head, *tmp;
    while (cur) { tmp = cur->next; free(cur); cur = tmp; }
    *head = NULL;
}

/* ── display ──────────────────────────────────────────── */
void print_list(Dlist *head)
{
    if (!head) { printf("0"); return; }
    for (Dlist *cur = head; cur; cur = cur->next)
        printf("%d", cur->data);
}

/* ── parse a decimal string into a list ──────────────── */
/*
 * Fills *head / *tail with one node per digit (MSB first).
 * Sets *negative if the string starts with '-'.
 * Returns SUCCESS / FAILURE.
 */
int parse_number(const char *str, Dlist **head, Dlist **tail, int *negative)
{
    *head = *tail = NULL;
    *negative = 0;

    int i = 0;
    if (str[i] == '-') { *negative = 1; i++; }
    else if (str[i] == '+') { i++; }

    /* validate: must be at least one digit */
    if (!str[i]) return FAILURE;
    for (int k = i; str[k]; k++)
        if (!isdigit((unsigned char)str[k])) return FAILURE;

    /* skip leading zeros (but keep at least one) */
    while (str[i] == '0' && str[i+1]) i++;

    for (; str[i]; i++)
        if (append_node(head, tail, str[i] - '0') == FAILURE) return FAILURE;

    return SUCCESS;
}

/* ── interactive read ─────────────────────────────────── */
int read_number(const char *prompt, Dlist **head, Dlist **tail, int *negative)
{
    char buf[4096];
    while (1) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof(buf), stdin)) return FAILURE;
        buf[strcspn(buf, "\n")] = '\0';
        if (parse_number(buf, head, tail, negative) == SUCCESS) return SUCCESS;
        printf("  Invalid number. Please enter digits only (optional leading '-').\n");
    }
}

/* ── compare absolute values ──────────────────────────── */
/*  Returns  1 if |h1| > |h2|
 *           0 if |h1| == |h2|
 *          -1 if |h1| < |h2|           */
int compare_abs(Dlist *h1, Dlist *h2)
{
    /* count digits */
    int len1 = 0, len2 = 0;
    for (Dlist *c = h1; c; c = c->next) len1++;
    for (Dlist *c = h2; c; c = c->next) len2++;
    if (len1 != len2) return len1 > len2 ? 1 : -1;

    /* same length – compare digit by digit (MSB first) */
    Dlist *c1 = h1, *c2 = h2;
    while (c1) {
        if (c1->data != c2->data)
            return c1->data > c2->data ? 1 : -1;
        c1 = c1->next;
        c2 = c2->next;
    }
    return 0;
}
