/*
 * number_place.c
 *
 *      Author: Nine
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct phone_number {
    int flag;
    struct phone_number *number[10];
    char place[32];
};
struct phone_number *phone_number[10];


int phone_number_insert(struct phone_number **phone, char *number, char *place)
{
    *phone = (struct phone_number*)malloc(sizeof(struct phone_number));

    if (strlen(number) == 1) {
        (*phone)->flag = 1;
        strcpy((*phone)->place, place);
        return 0;
    }

    return phone_number_insert(&(*phone)->number[number[1]-'0'], number+1, place);
}


int phone_number_find(struct phone_number *phone, char *number)
{
    if (phone == NULL) {
        return -1;
    }

    if (phone->flag == 1) {
        printf("%s\n", phone->place);
        return 0;
    }

    return phone_number_find(phone->number[number[1] - '0'], number+1);
}


int main()
{
    int t,n,m;

    scanf("%d", &t);
    while (t--) {
        memset(phone_number, 0, sizeof(phone_number));
        scanf("%d", &n);
        while (n--) {
            char number[12];
            char place[32];

            scanf("%s %s", number, place);
            char t_number[12] = "";

            strncpy(t_number, number, strcspn(number, "x"));
            phone_number_insert(&phone_number[t_number[0]-'0'], t_number+1, place);
        }

        scanf("%d", &m);
        while (m--) {
            char number[12];
            scanf("%s", number);

            if (phone_number_find(phone_number[number[0] - '0'], number+1) != 0) {
                printf("unkonwn\n");
            }
        }

    }
}
