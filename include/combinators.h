//! @file
//! @brief Combinators.

#ifndef COMBINATORS_H_
#define COMBINATORS_H_

union Parser;
struct TaggedParser;

/** @brief Tags for each of the Combinators. */
enum ParserTag {
    LITERAL_TAG,
    DIGIT_TAG,
    TIMES_TAG,
    BOTH_TAG,
    SEQUENCE_TAG,
    MAYBE_TAG,
    EOF_TAG,
    EITHER_TAG,
    ONE_OF_CHARS_TAG,
    ANY_TAG,
    CONSUME_TAG,
    STRING_TAG,
    MANY_TAG,
    MANY1_TAG,
    WHITESPACE_TAG,
    ALPHA_TAG,
    /* NOT_TAG, */
};

/** @brief Combinator union type. Use `ParserTag` to select each Parser. */
union Parser {
    struct _Literal {
        char c;
    } Literal;
    /* struct _Digit {} Digit; */
    struct _Times {
        unsigned n;
        struct TaggedParser *inner;
    } Times;
    struct _Both {
        struct TaggedParser *first;
        struct TaggedParser *second;
    } Both;
    struct _Sequence {
        unsigned len;
        struct TaggedParser **parsers;
    } Sequence;
    struct _Maybe {
        struct TaggedParser *inner;
    } Maybe;
    /* struct _Eof {} Eof; */
    struct _Either {
        struct TaggedParser *first;
        struct TaggedParser *second;
    } Either;
    struct _OneOfChars {
        char *options;
    } OneOfChars;
    /* struct _Any {} Any; */
    struct _Consume {
        struct TaggedParser *inner;
    } Consume;
    struct _String {
        char *s;
    } String;
    struct _Many {
        struct TaggedParser *inner;
    } Many;
    struct _Many1 {
        struct TaggedParser *inner;
    } Many1;
    /* struct _Whitespace {} Whitespace; */
    /* struct _Alpha {} Alpha; */
    struct _Not {
        struct TaggedParser *inner;
    } Not;
};

/** @brief Parser for one literal character. */
struct TaggedParser *literal(char c);
/** @brief Parser for one digit. */
struct TaggedParser *digit();
/** @brief Parser for one alphabetic character. */
struct TaggedParser *alpha();
/** @brief Parser for one whitespace character. */
struct TaggedParser *whitespace();
/** @brief Parser for any one of given characters. */
struct TaggedParser *one_of_chars(char *options);
/** @brief Parser for any one character. */
struct TaggedParser *any();
/** @brief Parser for the given literal string. */
struct TaggedParser *string(char *s);
/** @brief Combinator for applying `inner` multiple times. */
struct TaggedParser *times(unsigned n, struct TaggedParser *inner);
/** @brief Combinator for applying both `first` and `second` in order. */
struct TaggedParser *both(struct TaggedParser *first,
                          struct TaggedParser *second);
/** @brief Combinator for applying each of the `parsers` in sequence. */
struct TaggedParser *seq(struct TaggedParser *parsers[], unsigned len);
/** @brief Combinator for optionally applying `inner`. */
struct TaggedParser *maybe(struct TaggedParser *inner);
/** @brief Parser for End Of File/Input. */
struct TaggedParser *eof();
/** @brief Combinator for applying either `first` or `second`. */
struct TaggedParser *either(struct TaggedParser *first,
                            struct TaggedParser *second);
/** @brief Combinator for applying `inner` zero or more times. Also see `many()`
 */
struct TaggedParser *many(struct TaggedParser *inner);
/** @brief Combinator for applying `inner` one or more times. */
struct TaggedParser *many1(struct TaggedParser *inner);
/* struct TaggedParser *not(struct TaggedParser *inner); */

/** @brief Meta-Combinator that applies `inner` and "consumes" the matched
 * input. */
struct TaggedParser *consume(struct TaggedParser *inner);

#endif // COMBINATORS_H_
