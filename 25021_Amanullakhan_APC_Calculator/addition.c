/*******************************************************************************
 * Name:Amanullakhan J Pathan
 * Title            : Addition
 * Description      : Adds two arbitrary-precision integers stored as
 *                    doubly-linked lists (one digit per node, MSB at head).
 * Prototype        : int addition(Dlist **head1, Dlist **tail1,
 *                                 Dlist **head2, Dlist **tail2,
 *                                 Dlist **headR);
 * Input Parameters : head1/tail1 – first  operand list (with sign flag via
 *                                  the caller's neg1 / neg2 variables)
 *                    head2/tail2 – second operand list
 *                    headR       – result list (caller passes address of ptr)
 * Output           : SUCCESS / FAILURE
 *
 * Sign convention handled by main(); this function adds absolute values.
 * The sign-aware wrapper add_signed() is used by main.c for +/- dispatch.
 *******************************************************************************/
#include "apc.h" 

/* ── internal: add two non-negative lists, result into *headR ── */
static int add_abs(Dlist *t1, Dlist *t2, Dlist **headR)
{
    int carry = 0;
    *headR = NULL;

    /* traverse from LSB (tail) to MSB (head) */
    while (t1 || t2 || carry)
    {
        int sum = carry;
        if (t1) { sum += t1->data; t1 = t1->prev; }
        if (t2) { sum += t2->data; t2 = t2->prev; }
        carry = sum / 10;
        prepend_node(headR, sum % 10);
    }
    return (*headR) ? SUCCESS : FAILURE;
}

/* ── internal: subtract smaller absolute value from larger ──── */
/*  Assumes |t_big| >= |t_small| (caller guarantees this).        */
static int sub_abs(Dlist *t_big, Dlist *t_small, Dlist **headR)
{
    int borrow = 0;
    *headR = NULL;

    while (t_big)
    {
        int diff = t_big->data - borrow;
        if (t_small) { diff -= t_small->data; t_small = t_small->prev; }
        if (diff < 0) { diff += 10; borrow = 1; } else { borrow = 0; }
        prepend_node(headR, diff);
        t_big = t_big->prev;
    }

    /* strip leading zeros */
    while (*headR && (*headR)->next && (*headR)->data == 0)
    {
        Dlist *tmp = *headR;
        *headR = (*headR)->next;
        (*headR)->prev = NULL;
        free(tmp);
    }
    return (*headR) ? SUCCESS : FAILURE;
}

/*******************************************************************************
 * addition()
 *
 * Adds the two numbers represented by [head1,tail1] and [head2,tail2].
 * Both numbers are treated as NON-NEGATIVE here.
 * The caller (main) is responsible for interpreting signs and calling
 * subtraction() when needed.
 *******************************************************************************/
int addition(Dlist **head1, Dlist **tail1,
             Dlist **head2, Dlist **tail2,
             Dlist **headR)
{
    if (!head1 || !*head1 || !head2 || !*head2 || !headR)
        return FAILURE;

    return add_abs(*tail1, *tail2, headR);
}
