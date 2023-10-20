#pragma once

#include "source.hpp"

namespace lmc {

class lexer {
    friend class token;
    friend class iterator;

private:

    class token {
        friend class lexer;

    protected:
        source* _source;
        size_t _begin, _end;
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

        token(source& source) : _source(&source), type(unknown) {}

        void begin() {
            _begin = _source->position();
        }

        void end() {
            _end = _source->position();
        }

        friend std::ostream& operator << (std::ostream& stream, enum type type) {
            switch (type) {
            case type::eof:
                stream << "<eof>";
                break;
            case type::unknown:
                stream << "<unknown>";
                break;
            case type::whitespace:
                stream << "<whitespace>";
                break;
            case type::newline:
                stream << "<newline>";
                break;
            case type::integer:
                stream << "<integer>";
                break;
            case type::identifier:
                stream << "<identifier>";
                break;
            case type::string:
                stream << "<string>";
                break;
            case type::comment:
                stream << "<comment>";
                break;
            default:
                stream << "<unsupported>";
                break;
            }
            return stream;
        }

        friend std::ostream& operator<<(std::ostream& stream, const token& token) {
            stream << token.type;

            switch (token.type) {
            case type::eof:
            case type::whitespace:
            case type::newline:
                break;
            default:
                {
                    std::string lexem(token._source->begin() + token._begin, token._source->begin() + token._end);
                    stream << ' ' << lexem;
                }
            }
    
            return stream;
        }
    };

    class iterator {
        friend class lexer;

    public:

        token& operator * () {
            return t;
        }

        iterator& operator ++ () {
            t = parent.next();
            return *this;
        }

        bool operator != (const iterator& it) {
            return t.type != token::eof;
        }

    protected:

        iterator(lexer& lexer, token token) : parent(lexer), t(token)
        {} 

    protected:

        lexer& parent;
        token t;       
    };

public:

    lexer(std::istream& source) : _source(source) {}
    ~lexer() {}

    token next()
    {
        token t(_source);
        t.begin();

        char c = _source.get();
        if (_source.eof()) {
            t.type = token::eof;
        } else {
            switch (c)
            {
            case ' ':
            case '\t':
            case '\r':
                t.type = token::whitespace;

                c = _source.peek();
                while (c == ' ' || c == '\t' || c == '\r') {
                    _source.get();
                    c = _source.peek();
                }
                break;

            case '\n':
                t.type = token::newline;
                break;

            case '"':
                t.type = token::string;
                break;

            case 'a'...'z':
            case 'A'...'Z':
            case '_':
                t.type = token::identifier;

                c = _source.peek();
                while ((c >= '0' && c <= '9') || 
                    (c >= 'a' && c <= 'z') ||
                    (c >= 'A' && c <= 'Z') ||
                    (c == '_')
                ) {
                    _source.get();
                    c = _source.peek();
                }
                break;

            default:
                break;
            }
        }

        t.end();

        return t;
    }

    // token peek()
    // {
    //     auto backup = _source.position();
    //     next();
    //     _source.seekg(backup);
    // }

    iterator begin() {
        return { *this, next() };
    }

    iterator end() {
        return { *this, {_source} };
    }

protected:

    lmc::source _source;
};

}
