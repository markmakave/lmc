#pragma once

namespace lmc {

class scope {
    public:

        template <typename... Names>
        scope(Names... names) : _namespaces{"global", names...} {}

        bool operator < (const scope& s) {
            if (size() == s.size()) return false;

            return *this <= s;
        }

        bool operator <= (const scope& s) {
            if (size() < s.size()) return false;

            for (size_t i = 0; i < s.size(); ++i)
                if (_namespaces[i] != s._namespaces[i])
                    return false;

            return true;
        }

        bool operator > (const scope& s) {
            if (size() == s.size()) return false;
            
            return *this >= s;
        }

        bool operator >= (const scope& s) {
            if (size() > s.size()) return false;
            
            for (size_t i = 0; i < size(); ++i)
                if (_namespaces[i] != s._namespaces[i])
                    return false;

            return true;
        }

        bool operator == (const scope& s) {
            if (s.size() != size()) return false;

            for (size_t i = 0; i < size(); ++i)
                if (_namespaces[i] != s._namespaces[i])
                    return false;

            return true;
        }

        size_t size() const {
            return _namespaces.size();
        }

        std::vector<std::string_view>::const_iterator begin() const {
            return _namespaces.begin();
        }

        std::vector<std::string_view>::const_iterator end() const {
            return _namespaces.end();
        }

        friend std::ostream& operator << (std::ostream& stream, const scope& s) {
            for (const auto& name : s) {
                stream << name << "::";
            }
            return stream;
        }

    protected:

        std::vector<std::string_view> _namespaces;
    };

}
