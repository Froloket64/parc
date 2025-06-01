#include "../include/parser.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define ERR_ARGS_N 1
#define MAX_MATCHES 2

parser_t *variable_assignment();

#ifdef TEST

void test_variables();

int main(void) {
    test_variables();

    return 0;
}

#else

#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: variables INPUT\n");
        return ERR_ARGS_N;
    }

    char *matches_arr[MAX_MATCHES];
    char *input = argv[1];
    size_t matches_n = 0;
    bool is_match;

    parser_t *parser = variable_assignment();
    is_match = parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES);

    printf("is_match: %s\n", is_match ? "true" : "false");

    if (!is_match)
        printf("input ptr: %c\n", *input);

    if (matches_n == 2) {
        printf("name: %s\n", matches_arr[0]);
        printf("value: %s\n", matches_arr[1]);
    } else {
        printf("only %zu matches found", matches_n);
    }

    return 0;
}

#endif

/**
 * @brief Returns a parser for variable assignment
 * @return Parser for variable assignment
 *
 * Input format: `let NAME = VALUE;`, where
    `NAME` is a C identifier,
    `VALUE` is a float with format '[SIGN] INT [. INT] [e|E INT]'
 */
parser_t *variable_assignment() {
    parser_t *ident = both(
        // May start with alphabetic or underscore
        either(alpha(), literal('_')),
        // May contain either alphanumeric or underscore characters
        many(either(alpha(), either(digit(), literal('_')))));
    parser_t *_int = both(maybe(one_of_chars("+-")), many1(digit()));
    parser_t *exponent = both(one_of_chars("eE"), _int);
    parser_t *fract = both(literal('.'), many(digit()));

    // FIXME: Meh code
    parser_t **_float = malloc(3 * sizeof(parser_t *));
    _float[0] = _int;
    _float[1] = maybe(fract);
    _float[2] = maybe(exponent);

    // FIXME: Meh code
    parser_t **parsers = malloc(9 * sizeof(parser_t *));
    parsers[0] = string("let");
    parsers[1] = many(whitespace());
    parsers[2] = consume(ident);
    parsers[3] = many(whitespace());
    parsers[4] = literal('=');
    parsers[5] = many(whitespace());
    parsers[6] = consume(seq(_float, 3));
    parsers[7] = literal(';');
    parsers[8] = eof();

    return seq(parsers, 9);
}

void test_variables() {
    char *matches_arr[MAX_MATCHES];
    char *input;
    size_t matches_n = 0;

    parser_t *parser = variable_assignment();

    input = "let x = 42;";
    matches_n = 0;

    // TODO? Check the `input` ptr after parsing
    assert(parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));
    assert(matches_n == 2);
    assert(strcmp(matches_arr[0], "x") == 0);
    assert(strcmp(matches_arr[1], "42") == 0);

    input = "let _x_1 = 42;";
    matches_n = 0;

    assert(parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));
    assert(matches_n == 2);
    assert(strcmp(matches_arr[0], "_x_1") == 0);
    assert(strcmp(matches_arr[1], "42") == 0);

    input = "let foo = 12.0;";
    matches_n = 0;

    assert(parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));
    assert(matches_n == 2);
    assert(strcmp(matches_arr[0], "foo") == 0);
    assert(strcmp(matches_arr[1], "12.0") == 0);

    input = "let foo = +12.0e-4;";
    matches_n = 0;

    assert(parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));
    assert(matches_n == 2);
    assert(strcmp(matches_arr[0], "foo") == 0);
    assert(strcmp(matches_arr[1], "+12.0e-4") == 0);

    input = "let foo = 12e4;";
    matches_n = 0;

    assert(parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));
    assert(matches_n == 2);
    assert(strcmp(matches_arr[0], "foo") == 0);
    assert(strcmp(matches_arr[1], "12e4") == 0);

    input = "let 1 = 12e4;";
    assert(!parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));

    input = "let foo = 1"; // NB: No semicolon
    assert(!parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));

    input = "let foo = f;";
    assert(!parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));

    input = "let foo = 1+;";
    assert(!parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));

    input = "foo = 1;";
    assert(!parse(&input, parser, matches_arr, &matches_n, MAX_MATCHES));
}
