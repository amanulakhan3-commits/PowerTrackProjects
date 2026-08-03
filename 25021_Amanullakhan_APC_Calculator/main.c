/*******************************************************************************
 * Name:Amanullakhan J Pathan
 * Title       : main – driver for the Arbitrary Precision Calculator
 * Description : Reads two large integers and an operator (+, -, *, /) from
 *               the user, dispatches to the appropriate arithmetic function,
 *               prints the result, and asks whether to continue.
 *******************************************************************************/
#include "apc.h"

/* ── small helper: read a single non-whitespace character ──── */
static char read_operator(void)
{
    char op;
    printf("  Enter operator (+, -, *, /): ");
    scanf(" %c", &op); 
    return op;
}

/* ── print a signed result ─────────────────────────────────── */
static void print_result(int negative, Dlist *head)
{
    /* suppress '-' for zero */
    int is_zero = (head && !head->next && head->data == 0);
    if (negative && !is_zero) 
    printf("-");
    print_list(head);
    printf("\n");
}

/*******************************************************************************
 * main()
 *******************************************************************************/
int main(void)
{
    Dlist *head1, *tail1, *head2, *tail2, *headR;
    int    neg1, neg2, negR;
    char   option;

    printf("========================================\n");
    printf("   Arbitrary Precision Calculator (APC) \n");
    printf("========================================\n");

    do
    {
        head1 = tail1 = head2 = tail2 = headR = NULL;
        neg1  = neg2  = negR  = 0;

        /* ── read operands ───────────────────────────────── */
        if (read_number("  Enter first  number : ", &head1, &tail1, &neg1) == FAILURE)
        {
            printf("  Input error.\n");
            goto cleanup;
        }
        if (read_number("  Enter second number : ", &head2, &tail2, &neg2) == FAILURE)
        {
            printf("  Input error.\n");
            goto cleanup;
        }

        char op = read_operator();

        /* ── dispatch ────────────────────────────────────── */
        switch (op)
        {
            /* -------------------------------------------------- */
            case '+':
            {
                /*  (+a) + (+b) =  add_abs,  sign +
                 *  (-a) + (-b) =  add_abs,  sign -
                 *  (+a) + (-b) =  sub_abs(a,b), sign = sign of larger
                 *  (-a) + (+b) =  sub_abs(a,b), sign = sign of larger  */
                if (neg1 == neg2)
                {
                    negR = neg1;
                    if (addition(&head1, &tail1, &head2, &tail2, &headR) == FAILURE)
                    { printf("  Addition failed.\n"); goto cleanup; }
                }
                else
                {
                    int cmp = compare_abs(head1, head2);
                    negR = (cmp >= 0) ? neg1 : neg2;
                    if (subtraction(&head1, &tail1, &head2, &tail2, &headR) == FAILURE)
                    { printf("  Subtraction failed.\n"); goto cleanup; }
                }
                printf("  Result: ");
                print_result(negR, headR);
                break;
            }

            /* -------------------------------------------------- */
            case '-':
            {
                /*  a - b  ==  a + (-b): flip sign of b, then add */
                neg2 = !neg2;
                if (neg1 == neg2)
                {
                    negR = neg1;
                    if (addition(&head1, &tail1, &head2, &tail2, &headR) == FAILURE)
                    { printf("  Addition failed.\n"); goto cleanup; }
                }     
                else
                {  
                    int cmp = compare_abs(head1, head2);
                    negR = (cmp >= 0) ? neg1 : neg2;
                    if (subtraction(&head1, &tail1, &head2, &tail2, &headR) == FAILURE)
                    { printf("  Subtraction failed.\n"); goto cleanup; }
                }
                printf("  Result: ");
                print_result(negR, headR);
                break;
            }

            /* -------------------------------------------------- */
            case '*':
            {
                negR = (neg1 != neg2) ? 1 : 0;
                if (multiplication(&head1, &tail1, &head2, &tail2, &headR) == FAILURE)
                { printf("  Multiplication failed.\n"); goto cleanup; }
                printf("  Result: ");
                print_result(negR, headR);
                break;
            }

            /* -------------------------------------------------- */
            case '/':
            {
                negR = (neg1 != neg2) ? 1 : 0;
                if (division(&head1, &tail1, &head2, &tail2, &headR) == FAILURE)
                { /* error message already printed */ goto cleanup; }
                printf("  Result: ");
                print_result(negR, headR);
                break;
            }

            /* -------------------------------------------------- */
            default:
                printf("  Invalid operator. Try again...\n");
        }

    cleanup:
        free_list(&head1);
        free_list(&head2);
        free_list(&headR);

        printf("  Want to continue? Press [yY | nN]: ");
        scanf(" %c", &option);

    } while (option == 'y' || option == 'Y');

    printf("  Goodbye!\n");
    return 0;
}
