#include "../include/parser.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>

bool is_phone_number(char *phone);

#ifdef TEST

// Phone format: `[+\d](\d\d\d)-\d\d\d-\d\d-\d\d`
int main(void) {
    assert(is_phone_number("+1(111)-123-45-67"));
    assert(is_phone_number("(111)-123-45-67"));
    assert(!is_phone_number("+1(111)-123-45-67!"));
    assert(!is_phone_number("amogus"));
    assert(!is_phone_number("+1(111)-123-4"));

    return 0;
}

#else

#include <stdio.h>

int main(void) {
    printf("There's no interactive interface to this example for now.\n");
    printf("Try compiling with `make examples-test` or `gcc -D TEST` for a "
           "testing version.\n");

    return 1;
}

#endif

// Implementation using parsing combinators.
bool is_phone_number(char *phone) {
    parser_t *parsers[11] = {maybe(both(literal('+'), digit())),
                             literal('('),
                             times(3, digit()),
                             literal(')'),
                             literal('-'),
                             times(3, digit()),
                             literal('-'),
                             times(2, digit()),
                             literal('-'),
                             times(2, digit()),
                             eof()};

    return matches(&phone, seq(parsers, sizeof(parsers) / sizeof(*parsers)));
}

// Implementation using procedural parsing.
bool _is_phone_number(char *phone) {
    // Starts with operator code
    if (*phone == '+') {
        phone++;

        if (!isdigit(*(phone++)))
            return false;
    }

    // Region code
    if (*(phone++) != '(')
        return false;

    for (size_t i = 0; i < 3; i++) {
        if (!isdigit(*(phone++)))
            return false;
    }

    if (*(phone++) != ')')
        return false;

    // Rest of the number
    if (*(phone++) != '-')
        return false;

    for (size_t i = 0; i < 3; i++) {
        if (!isdigit(*(phone++)))
            return false;
    }

    if (*(phone++) != '-')
        return false;

    for (size_t i = 0; i < 2; i++) {
        if (!isdigit(*(phone++)))
            return false;
    }

    if (*(phone++) != '-')
        return false;

    for (size_t i = 0; i < 2; i++) {
        if (!isdigit(*(phone++)))
            return false;
    }

    if (*phone == 0)
        return true;
    else
        return false;
}
