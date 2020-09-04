#ifndef EIGHT_EIGHT_H
#define EIGHT_EIGHT_H

#include <eight/api.h>

HEDLEY_BEGIN_C_DECLS

/**
 * @brief Get the Eight Version Major object
 * 
 * @return unsigned test
 */
HEDLEY_WARN_UNUSED_RESULT
HEDLEY_CONST
HEDLEY_NO_THROW
EIGHT_API
unsigned getEightVersionMajor();

/**
 * @brief Get the Eight Version Minor object
 * 
 * @return This is a test 
 */
HEDLEY_WARN_UNUSED_RESULT
HEDLEY_CONST
HEDLEY_NO_THROW
EIGHT_API
unsigned getEightVersionMinor();

/**
 * @brief Get the Eight Version Revision object
 * 
 * @return This is another test
 */
HEDLEY_WARN_UNUSED_RESULT
HEDLEY_CONST
HEDLEY_NO_THROW
EIGHT_API
unsigned getEightVersionRevision();

HEDLEY_WARN_UNUSED_RESULT
HEDLEY_CONST
HEDLEY_NO_THROW
EIGHT_API
unsigned getEightVersion();

/**
 * @brief Get the Eight Version String object
 * 
 * @return This is the final test
 */
HEDLEY_WARN_UNUSED_RESULT
HEDLEY_RETURNS_NON_NULL
HEDLEY_CONST
HEDLEY_NO_THROW
EIGHT_API
const char* getEightVersionString();

typedef struct compilation {
    const char* outfile;
} compilation_t;

EIGHT_API
int compileEight(const char*, struct compilation);

EIGHT_API
int compileEightFile(const char*, struct compilation);

HEDLEY_END_C_DECLS

#endif
