#include "../include/combinators.h"
#include "../include/parser.h"
#include <stdlib.h>

#define alloc_parser(COMBINATOR)                                               \
    union Parser *COMBINATOR = malloc(sizeof(union Parser))
#define alloc_tagged_parser(PARSER, TAG, COMBINATOR)                           \
    {                                                                          \
        PARSER = malloc(sizeof(parser_t));                                     \
        PARSER->tag = TAG;                                                     \
        PARSER->combinator = COMBINATOR;                                       \
    }

parser_t *literal(char c) {
    // C is very strange..
    // but i kinda love it
    alloc_parser(comb);
    comb->Literal.c = c;

    parser_t *parser;
    alloc_tagged_parser(parser, LITERAL_TAG, comb);

    return parser;
}

parser_t *digit() {
    alloc_parser(comb);

    parser_t *parser;
    alloc_tagged_parser(parser, DIGIT_TAG, comb);

    return parser;
}

parser_t *both(parser_t *first, parser_t *second) {
    alloc_parser(comb);
    comb->Both.first = first;
    comb->Both.second = second;

    parser_t *parser;
    alloc_tagged_parser(parser, BOTH_TAG, comb);

    return parser;
}

parser_t *times(unsigned n, parser_t *inner) {
    alloc_parser(comb);
    comb->Times.n = n;
    comb->Times.inner = inner;

    parser_t *parser;
    alloc_tagged_parser(parser, TIMES_TAG, comb);

    return parser;
}

// NOTE: It's assumed that all `parsers` are heap-allocated
// NOTE: It's the caller's responsibility to free `parsers`
parser_t *seq(parser_t *parsers[], unsigned len) {
    alloc_parser(comb);
    comb->Sequence.parsers = parsers;
    comb->Sequence.len = len;

    parser_t *parser;
    alloc_tagged_parser(parser, SEQUENCE_TAG, comb);

    return parser;
}

parser_t *maybe(parser_t *inner) {
    alloc_parser(comb);
    comb->Maybe.inner = inner;

    parser_t *parser;
    alloc_tagged_parser(parser, MAYBE_TAG, comb);

    return parser;
}

parser_t *eof() {
    alloc_parser(comb);

    parser_t *parser;
    alloc_tagged_parser(parser, EOF_TAG, comb);

    return parser;
}

parser_t *either(struct TaggedParser *first, struct TaggedParser *second) {
    alloc_parser(comb);
    comb->Either.first = first;
    comb->Either.second = second;

    parser_t *parser;
    alloc_tagged_parser(parser, EITHER_TAG, comb);

    return parser;
}

struct TaggedParser *one_of_chars(char *options) {
    alloc_parser(comb);
    comb->OneOfChars.options = options;

    parser_t *parser;
    alloc_tagged_parser(parser, ONE_OF_CHARS_TAG, comb);

    return parser;
}

struct TaggedParser *any() {
    alloc_parser(comb);

    parser_t *parser;
    alloc_tagged_parser(parser, ANY_TAG, comb);

    return parser;
}

struct TaggedParser *consume(struct TaggedParser *inner) {
    alloc_parser(comb);
    comb->Consume.inner = inner;

    parser_t *parser;
    alloc_tagged_parser(parser, CONSUME_TAG, comb);

    return parser;
}

struct TaggedParser *string(char *s) {
    alloc_parser(comb);
    comb->String.s = s;

    parser_t *parser;
    alloc_tagged_parser(parser, STRING_TAG, comb);

    return parser;
}

struct TaggedParser *many(struct TaggedParser *inner) {
    alloc_parser(comb);
    comb->Many.inner = inner;

    parser_t *parser;
    alloc_tagged_parser(parser, MANY_TAG, comb);

    return parser;
}

struct TaggedParser *many1(struct TaggedParser *inner) {
    alloc_parser(comb);
    comb->Many1.inner = inner;

    parser_t *parser;
    alloc_tagged_parser(parser, MANY1_TAG, comb);

    return parser;
}

struct TaggedParser *whitespace() {
    alloc_parser(comb);

    parser_t *parser;
    alloc_tagged_parser(parser, WHITESPACE_TAG, comb);

    return parser;
}

struct TaggedParser *alpha() {
    alloc_parser(comb);

    parser_t *parser;
    alloc_tagged_parser(parser, ALPHA_TAG, comb);

    return parser;
}

/* struct TaggedParser *not(struct TaggedParser *inner) { */
/*     alloc_parser(comb); */
/*     comb->Not.inner = inner; */

/*     parser_t *parser; */
/*     alloc_tagged_parser(parser, NOT_TAG, comb); */

/*     return parser; */
/* } */
