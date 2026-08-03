/*******************************************************************************
 * Name:Amanullakhan J Pathan
 * Title            : Multiplication
 * Description      : Multiplies two arbitrary-precision integers stored as
 *                    doubly-linked lists (one digit per node, MSB at head).
 * Prototype        : int multiplication(Dlist **head1, Dlist **tail1,
 *                                       Dlist **head2, Dlist **tail2,
 *                                       Dlist **headR);
 * Output           : SUCCESS / FAILURE
 *
 * Algorithm: grade-school O(n*m) long multiplication on an integer array,
 * then the result array is converted back to a linked list.  
 *******************************************************************************/
#include "apc.h"

int multiplication(Dlist **head1, Dlist **tail1,
                   Dlist **head2, Dlist **tail2,
                   Dlist **headR)
{
    if (!head1 || !*head1 || !head2 || !*head2 || !headR)
        return FAILURE;

    *headR = NULL;

    /* Count digits */
    int len1 = 0, len2 = 0;
    for (Dlist *c = *head1; c; c = c->next) len1++;
    for (Dlist *c = *head2; c; c = c->next) len2++;

    int rlen = len1 + len2;
    int *res = (int *)calloc(rlen, sizeof(int));
    if (!res) return FAILURE;

    /* Copy digits into arrays (index 0 = MSB) */
    int *a = (int *)malloc(len1 * sizeof(int));
    int *b = (int *)malloc(len2 * sizeof(int));
    if (!a || !b) { free(res); free(a); free(b); return FAILURE; }

    int i = 0;
    for (Dlist *c = *head1; c; c = c->next) a[i++] = c->data;
    i = 0;
    for (Dlist *c = *head2; c; c = c->next) b[i++] = c->data;

    /* Grade-school multiplication (MSB-indexed) */
    for (int p = len1 - 1; p >= 0; p--)
    {
        for (int q = len2 - 1; q >= 0; q--)
        {
            int mul  = a[p] * b[q];
            int pos1 = p + q;
            int pos2 = p + q + 1;
            int sum  = mul + res[pos2];

            res[pos2] = sum % 10;
            res[pos1] += sum / 10;
        }
    }

    free(a); free(b);

    /* Build result linked list, skipping leading zeros */
    Dlist *tailR = NULL;
    int started = 0;
    for (int k = 0; k < rlen; k++)
    {
        if (!started && res[k] == 0) continue;
        started = 1;
        if (append_node(headR, &tailR, res[k]) == FAILURE)
        {
            free(res);
            return FAILURE;
        }
    }
    free(res);

    /* Edge case: product is zero */
    if (!*headR) prepend_node(headR, 0);

    return SUCCESS;
}
