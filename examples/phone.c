#include "../include/parser.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

parser_t *phone_number();

#ifdef TEST

int main(void) {
    parser_t *parser = phone_number();
    char *input;

    input = "+1(111)-123-45-67";
    assert(matches(&input, parser));

    input = "(111)-123-45-67";
    assert(matches(&input, parser));

    input = "+1(111)-123-45-67!";
    assert(!matches(&input, parser));

    input = "amogus";
    assert(!matches(&input, parser));

    input = "+1(111)-123-4";
    assert(!matches(&input, parser));

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

/**
 * @brief Returns a parser for phone numbers.
 * @return Parser for phone numbers.
 *
 * Input format: `[+\d](\d\d\d)-\d\d\d-\d\d-\d\d`
 */
parser_t *phone_number() {
    parser_t **parsers = malloc(11 * sizeof(parser_t *));

    parsers[0] = maybe(both(literal('+'), digit()));
    parsers[1] = literal('(');
    parsers[2] = times(3, digit());
    parsers[3] = literal(')');
    parsers[4] = literal('-');
    parsers[5] = times(3, digit());
    parsers[6] = literal('-');
    parsers[7] = times(2, digit());
    parsers[8] = literal('-');
    parsers[9] = times(2, digit());
    parsers[10] = eof();

    return seq(parsers, 11);
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
