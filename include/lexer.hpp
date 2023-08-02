#pragma once

#include <istream>
#include <ostream>

namespace lmc {

class lexer {
public:

    struct token {
        enum type {
            eof = -1,
            unknown,
            whitespace,
            newline,
            integer,
            identifier,
            string,
            comment
        } type;

        char lexem[64];
        size_t length;

        token() : type(unknown), length(0)
        {}

        // append character to token lexem
        void append(char c) {
            lexem[length++] = c;
        }

        void append(const char* s, size_t size) {
            for (size_t i = 0; i < size; ++i)
                append(s[i]);
        }

        // erase token lexem
        void erase() {
            length = 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const token& t) {
            switch (t.type) {
            case type::eof:
                os << "<eof>";
                break;
            case type::unknown:
                os << "<unknown>";
                break;
            case type::whitespace:
                os << "<whitespace>";
                break;
            case type::newline:
                os << "<newline>";
                break;
            default:
                os << "<" << t.type << "> \"";
                os.write(t.lexem, t.length);
                os << "\"";
            }
    
            return os;
        }
    };

    struct iterator {
        lexer& parent;

        enum mode {
            begin,
            end
        } mode;

        token& operator *() {
            return parent.peek();
        }

        token* operator ->() {
            return &parent._last;
        }

        iterator& operator ++() {
            parent.next();
            return *this;
        }

        bool
        operator == (const iterator& other) const {
            return other.mode == mode::end && parent._last.type == token::eof;
        }

        bool
        operator != (const iterator& other) const {
            return !(*this == other);
        }

    };

public:

    lexer(std::istream& source) : _source(source) {}
    ~lexer() {}

    void lex(auto&& condition) {
        while (condition(_source.peek()))
        {
            _last.append(_source.get());
        }
    }

    token& next()
    {
        _last.erase();

        switch (_source.peek())
        {
        case std::char_traits<char>::eof():
            _last.type = token::eof;
            break;

        case ' ':
        case '\t':
        case '\r':
            _last.type = token::whitespace;
            lex([](char c) { return c == ' ' || c == '\t' || c == '\r'; });
            break;

        case '\n':
            _last.type = token::newline;
            lex([](char c) { return c == '\n'; });
            break;

        case '"':
            _last.type = token::string;
            _last.append(_source.get());
            lex([escape = false](char c) mutable {
                switch (c) {
                case '\\':
                    if (escape)
                        escape = false;
                    return true;
                case '"':
                    if (escape) {
                        escape = false;
                        return true;
                    }
                    else
                        return false;
                default:
                    escape = false;
                    return true;
                }
            });
            _last.append('"');
            break;

        case 'a'...'z':
        case 'A'...'Z':
        case '_':
            _last.type = token::identifier;
            lex([](char c) { return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || (c > '0' && c <= '9'); });
            break;

        default:
            _last.type = token::unknown;
            _last.append(_source.get());
            break;
        }

        return _last;
    }

    token& peek()
    {
        auto backup = _source.tellg();
        next();
        _source.seekg(backup);
        return _last;
    }

    iterator begin() {
        return {*this, iterator::begin};
    }

    iterator end() {
        return {*this, iterator::end};
    }

protected:
    std::istream& _source;

    token _last;
};

}
