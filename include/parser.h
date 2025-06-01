//! @file
//! @brief Parser definitions.

#ifndef PARSER_H_
#define PARSER_H_

#include "combinators.h"
#include <stdbool.h>
#include <stddef.h>

/** @brief Full Parser type, containing a tag, specifying which combinator to
   use (see `union Parser`).
 *  @member tag Combinator tag (see `union Parser`).
 *  @member combinator Pointer to the Combinator.
 */
struct TaggedParser {
    unsigned tag;
    union Parser *combinator;
};

typedef struct TaggedParser parser_t;

/**
 * @brief Parses `input` using `parser`.
 * @param[in,out] input Input to parse.
 * @param[in] parser Parser to use.
 * @param[out] matches_arr Array to store "consumed" matches.
 * @param[out] n Count of matches consumed.
 * @param[in] max_n Maximum allowed number of matches to consume/write.
 * @return `true` if the input matched, `false` otherwise.
 *
 * Parses `input`, returning whether it matches `parser`.
 * Writes matches that need to be "consumed" (see `consume()`) into
 * `matches_arr`, setting `n` to their number, up to `max_n`.
 */
bool parse(char **input, parser_t *parser, char *matches[], size_t *n,
           size_t max_n);

/**
 * @brief Matches `input` against `parser`. As opposed to `parse()` never consumes matches.
 * @param[in/out] input Input to parse.
 * @param[in] parser Parser to use.
 * @return `true` if the input matched, `false` otherwise.
 */
bool matches(char **input, parser_t *parser);

/**
 * @brief Frees the parser.
 * @param[in] parser Parser to free.
 *
 * NOTE: SAFETY: Only safe to call on heap-allocated parsers.
 * This shouldn't be a problem, since all the provided functions return pointers
 * to the heap.
 */
void free_parser(parser_t *parser);

#endif // PARSER_H_
