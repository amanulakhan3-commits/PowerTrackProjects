/*******************************************************************************
 * Name:Amanullakhan J Pathan
 * Title            : Subtraction
 * Description      : Subtracts the second number from the first.
 *                    Both operands are stored as doubly-linked lists
 *                    (one digit per node, MSB at head).
 * Prototype        : int subtraction(Dlist **head1, Dlist **tail1,
 *                                    Dlist **head2, Dlist **tail2,
 *                                    Dlist **headR);
 * Output           : SUCCESS / FAILURE
 *
 * This function computes |num1| - |num2|.  The sign of the result is
 * determined by main() using compare_abs(); main() prefixes '-' when needed.
 *******************************************************************************/
#include "apc.h"  

int subtraction(Dlist **head1, Dlist **tail1,
                Dlist **head2, Dlist **tail2,
                Dlist **headR)
{
    if (!head1 || !*head1 || !head2 || !*head2 || !headR)
        return FAILURE;

    *headR = NULL;

    int cmp = compare_abs(*head1, *head2);

    Dlist *t_big   = (cmp >= 0) ? *tail1 : *tail2;
    Dlist *t_small = (cmp >= 0) ? *tail2 : *tail1;

    int borrow = 0;

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

    /* if result is zero, ensure single "0" node */
    if (!*headR) prepend_node(headR, 0);

    return SUCCESS;
}
