/******************************************************************************* 
 * Name:Amanullakhan J Pathan
 * Title            : Division
 * Description      : Divides the first number by the second (integer quotient).
 *                    Both operands are stored as doubly-linked lists
 *                    (one digit per node, MSB at head).
 * Prototype        : int division(Dlist **head1, Dlist **tail1,
 *                                 Dlist **head2, Dlist **tail2,
 *                                 Dlist **headR);
 * Output           : SUCCESS / FAILURE  (FAILURE also on divide-by-zero)
 *
 * Algorithm: long division – bring down one digit at a time from the dividend,
 * find how many times the divisor fits, record the quotient digit.
 *******************************************************************************/
#include "apc.h"

/* ── helpers local to this file ──────────────────────── */

/* Multiply list by single digit d, result in new list */
static Dlist *mul_digit(Dlist *head, int d)
{
    if (d == 0) { Dlist *z = create_node(0); return z; }

    /* collect digits */
    int len = 0;
    for (Dlist *c = head; c; c = c->next) len++;
    int *a = (int *)malloc(len * sizeof(int));
    if (!a) return NULL;
    int i = 0;
    for (Dlist *c = head; c; c = c->next) a[i++] = c->data;

    int *r = (int *)calloc(len + 1, sizeof(int));
    if (!r) { free(a); return NULL; }

    int carry = 0;
    for (int k = len - 1; k >= 0; k--) {
        int p = a[k] * d + carry;
        r[k + 1] = p % 10;
        carry = p / 10;
    }
    r[0] = carry;
    free(a);

    /* build list */
    Dlist *head_r = NULL, *tail_r = NULL;
    int started = 0;
    for (int k = 0; k <= len; k++) {
        if (!started && r[k] == 0) continue;
        started = 1;
        append_node(&head_r, &tail_r, r[k]);
    }
    free(r);
    if (!head_r) { head_r = create_node(0); }
    return head_r;
}

/* Compare two lists: -1 / 0 / 1  (same as compare_abs but takes heads) */
static int cmp(Dlist *a, Dlist *b) { return compare_abs(a, b); }

/* Subtract b from a (a >= b), return new list */
static Dlist *sub_lists(Dlist *a, Dlist *b)
{
    /* find tails */
    Dlist *ta = a, *tb = b;
    while (ta && ta->next) ta = ta->next;
    while (tb && tb->next) tb = tb->next;

    Dlist *headR = NULL;
    int borrow = 0;
    while (ta) {
        int diff = ta->data - borrow;
        if (tb) { diff -= tb->data; tb = tb->prev; }
        if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
        prepend_node(&headR, diff);
        ta = ta->prev;
    }
    /* strip leading zeros */
    while (headR && headR->next && headR->data == 0) {
        Dlist *tmp = headR;
        headR = headR->next;
        headR->prev = NULL;
        free(tmp);
    }
    if (!headR) headR = create_node(0);
    return headR;
}

/*******************************************************************************
 * division()
 *******************************************************************************/
int division(Dlist **head1, Dlist **tail1,
             Dlist **head2, Dlist **tail2,
             Dlist **headR)
{
    if (!head1 || !*head1 || !head2 || !*head2 || !headR)
        return FAILURE;

    *headR = NULL;

    /* Divide by zero check */
    if ((*head2)->data == 0 && (*head2)->next == NULL)
    {
        printf("  Error: Division by zero.\n");
        return FAILURE;
    }

    /* If dividend < divisor, quotient = 0 */
    if (cmp(*head1, *head2) < 0)
    {
        prepend_node(headR, 0);
        return SUCCESS;
    }

    /* Long division */
    Dlist *cur     = *head1;   /* current digit of dividend  */
    Dlist *partial = NULL;     /* current partial dividend   */
    Dlist *ptail   = NULL;
    Dlist *qtail   = NULL;     /* quotient tail              */

    while (cur)
    {
        /* bring down next digit */
        append_node(&partial, &ptail, cur->data);

        /* strip leading zeros from partial */
        while (partial && partial->next && partial->data == 0) {
            Dlist *tmp = partial;
            partial = partial->next;
            partial->prev = NULL;
            free(tmp);
            ptail = partial;
            while (ptail && ptail->next) ptail = ptail->next;
        }

        /* find quotient digit (0-9) by trial subtraction */
        int q = 0;
        for (int d = 9; d >= 1; d--)
        {
            Dlist *trial = mul_digit(*head2, d);
            if (trial && cmp(partial, trial) >= 0)
            {
                q = d;
                free_list(&trial);
                break;
            }
            free_list(&trial);
        }

        /* append quotient digit */
        append_node(headR, &qtail, q);

        /* partial = partial - q * divisor */
        if (q > 0)
        {
            Dlist *prod = mul_digit(*head2, q);
            Dlist *rem  = sub_lists(partial, prod);
            free_list(&prod);
            free_list(&partial);
            partial = rem;
            ptail = partial;
            while (ptail && ptail->next) ptail = ptail->next;
        }

        cur = cur->next;
    }

    free_list(&partial);

    /* strip leading zeros in quotient */
    while (*headR && (*headR)->next && (*headR)->data == 0) {
        Dlist *tmp = *headR;
        *headR = (*headR)->next;
        (*headR)->prev = NULL;
        free(tmp);
    }
    if (!*headR) prepend_node(headR, 0);

    return SUCCESS;
}
