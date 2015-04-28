#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "text_reception.h"
#include "../common_code/common.h"


bool is_end(char c)
{
    return (c == '\n') || (c == '\r') || (c == '\0');
}

void reset_search(struct search_key_t *sk)
{
    for (int i = 0; i < sk->nb_keys; i++) {
        sk->to_search[i] = true;
    };
    sk->index = 0;
}

int search_key(char c, struct search_key_t *sk)
{
    int ret = 0;

    if (is_end(c)) {
        debug(1, "end of line\n");
        return -1;
    }

    for (int i = 0; i < sk->nb_keys; i++) {
        debug(1, "%d : %d\n", i, sk->to_search[i]);
        if (sk->to_search[i]) {
            if (sk->keys[i][sk->index] != c) {
                debug(1, "not to_search : %d\n", i);
                debug(1, "c = %c, key = %c\n", c, sk->keys[i][sk->index]);
                sk->to_search[i] = false;
            } else {
                if (sk->keys[i][sk->index+1] == '\0') {
                    debug(1, "found : %d\n", i);
                    return i;
                }
                // On comptabilise le nombre de clé encore valide
                ret --;
            }
        }
    }

    sk->index++;
    return ret - 1;
}

int read_string(char c, int *index, char *str, int size_str)
{
    if (is_end(c)) {
        debug(1, "new_line\n");
        debug(1, "return : %d\n", *index);
        return *index;
    }

    if (*index >= size_str) {
        debug(1, "buffer overflow\n");
        return -1;
    }

    debug(1, "read_string : %c\n", c);
    str[*index] = c;
    *index = *index + 1;
    debug(1, "index : %d\n", *index);
    return 0;
}

int read_int(char c, int *val)
{
    if (is_end(c)) {
        debug(1, "new_line\n");
        return 1;
    }

    // On s'assure qu'on a reçu un nombre
    if ((c < '0') || (c > '9')) {
        debug(1, "erreur, %c n'est pas un nombre\n", c);
        return -2;
    }

    int pre_val = *val;
    *val *= 10;
    *val += c - '0';

    // overflow
    if (*val < pre_val) {
        debug(1, "overflow");
        return -1;
    }

    return 0;
}
