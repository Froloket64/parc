#include "../include/parser.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void free_combinator(parser_t *parser);

// TODO: Reliably stop `input` at the character that failed to parse
// TODO? Use a flag variable and only return at the end of function (no early
// returns)
bool parse(char **input, parser_t *parser, char *matches_arr[], size_t *n,
           size_t max_n) {
    char *c_ptr;

    switch (parser->tag) {
    case LITERAL_TAG:
        if (**input != parser->combinator->Literal.c)
            return false;

        (*input)++;

        break;
    case TIMES_TAG:
        for (size_t i = 0; i < parser->combinator->Times.n; i++)
            if (!parse(input, parser->combinator->Times.inner, matches_arr, n,
                       max_n))
                return false;
        break;
    case BOTH_TAG:
        // NOTE: Be careful with order of operations, especially with
        // mutable data. Technically, since `&&` is short-circuiting, it
        // should evaluate these in order.
        if (!(parse(input, parser->combinator->Both.first, matches_arr, n,
                    max_n) &&
              parse(input, parser->combinator->Both.second, matches_arr, n,
                    max_n)))
            return false;
        break;
    case DIGIT_TAG:
        /* if (!isdigit(*((*input)++))) */
        /*     return false; */
        if (!isdigit(**input))
            return false;

        (*input)++;

        break;
    case SEQUENCE_TAG:
        for (size_t i = 0; i < parser->combinator->Sequence.len; i++)
            if (!parse(input, parser->combinator->Sequence.parsers[i],
                       matches_arr, n, max_n))
                return false;

        break;
    case MAYBE_TAG:
        c_ptr = *input;

        if (!parse(input, parser->combinator->Maybe.inner, matches_arr, n,
                   max_n))
            while (*input > c_ptr)
                (*input)--;

        break;
    case EOF_TAG:
        if (**input)
            return false;
        break;
    case EITHER_TAG:
        c_ptr = *input;

        // NOTE: First parser is tried first (duh)
        if (parse(input, parser->combinator->Either.first, matches_arr, n,
                  max_n))
            return true;

        // Rewind
        while (*input > c_ptr)
            (*input)--;

        return parse(input, parser->combinator->Either.second, matches_arr, n,
                     max_n);

        break;
    case ONE_OF_CHARS_TAG:
        c_ptr = parser->combinator->OneOfChars.options;

        while (*c_ptr) {
            if (**input == *c_ptr) {
                (*input)++;
                return true;
            }

            c_ptr++;
        }

        return false;
        break;
    case ANY_TAG:
        // Return false if EOF
        if (**input)
            (*input)++;
        else
            return false;
        /* return **input ? (*input)++ != 0 : false; */

        break;
    case CONSUME_TAG:
        c_ptr = *input;

        if (!parse(input, parser->combinator->Consume.inner, matches_arr, n,
                   max_n))
            return false;

        // HACK?
        if (*n >= max_n)
            return true;

        // HACK?
        assert(*input - c_ptr >= 0);

        matches_arr[*n] = calloc((size_t)(*input - c_ptr + 1), sizeof(char));
        memcpy(matches_arr[*n], c_ptr, (size_t)(*input - c_ptr));

        (*n)++;

        break;
    case STRING_TAG:
        c_ptr = parser->combinator->String.s;

        while (*c_ptr) {
            if (**input != *c_ptr)
                return false;

            (*input)++;
            c_ptr++;
        }

        break;
    case MANY_TAG:
        while (
            parse(input, parser->combinator->Many.inner, matches_arr, n, max_n))
            ;

        break;
    case MANY1_TAG:
        // Has to match at least once
        if (!parse(input, parser->combinator->Many1.inner, matches_arr, n,
                   max_n))
            return false;

        while (parse(input, parser->combinator->Many1.inner, matches_arr, n,
                     max_n))
            ;

        break;
    case WHITESPACE_TAG:
        if (!isspace(**input))
            return false;

        (*input)++;

        break;
    case ALPHA_TAG:
        if (isalpha(**input))
            (*input)++;
        else
            return false;

        break;
    // NOTE: I'm actually unsure as to how this would work
    /* case NOT_TAG: */
    /*     break; */
    default:
        // NOTE: Panic, should be unreachable
        exit(255);
    }

    // TODO?
    /* free_parser(parser); */

    return true;
}

bool matches(char **input, parser_t *parser) {
    // HACK?
    return parse(input, parser, NULL, 0, 0);
}

void free_parser(parser_t *parser) {
    free_combinator(parser);
    free(parser);
}

void free_combinator(parser_t *parser) {
    // TODO
    switch (parser->tag) {
    case BOTH_TAG:
        free_parser(parser->combinator->Both.first);
        free_parser(parser->combinator->Both.second);

        break;
    case TIMES_TAG:
        free_parser(parser->combinator->Times.inner);

        break;
    case EITHER_TAG:
        free_parser(parser->combinator->Either.first);
        free_parser(parser->combinator->Either.second);

        break;
    default:
        // NOTE: Panic, should be unreachable
        exit(255);
    }

    free(parser->combinator);
}
