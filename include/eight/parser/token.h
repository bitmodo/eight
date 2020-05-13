#ifndef EIGHT_PARSER_TOKEN_H
#define EIGHT_PARSER_TOKEN_H

# include <string>

namespace Eight {
    namespace Parser {
        class Token {
        public:
            virtual inline bool isPositional(void) = 0;

            virtual inline bool isKeyword(void) = 0;
        };

        class PositionToken : public Token {
            unsigned int start;
            unsigned int length;
        public:
            PositionToken(unsigned int, unsigned int);

            unsigned int start(void);

            unsigned int length(void);

            inline bool isPositional(void) override {
                return true;
            }

            inline bool isKeyword(void) override {
                return false;
            }
        };

        class KeywordToken : public Token {
        public:
            virtual inline std::string text(void) = 0;

            inline bool isPositional(void) override {
                return false;
            }

            inline bool isKeyword(void) override {
                return true;
            }
        };
    }
}

#define POSITION_TOKEN(name)                              \
    class name : public Eight::Parser::PositionToken { }  \

#define KEYWORD_TOKEN(name, text)                       \
    class name : public Eight::Parser::KeywordToken {   \
    public:                                             \
        inline std::string text(void) {                 \
            return #text ;                              \
        }                                               \
    }

#endif
