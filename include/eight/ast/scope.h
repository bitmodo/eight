#ifndef EIGHT_AST_SCOPE_H
#define EIGHT_AST_SCOPE_H

#include <map>
#include <vector>
#include <string>
#include <stdexcept>

namespace Eight {
    template<typename T>
    struct Frame {
        std::map<std::string, T> entries;
    };

    template<typename T>
    struct Scope {
        std::vector<Frame<T>> m_frames;
    public:
        Scope() : m_frames() {
            pushFrame();
        }

        void pushFrame() {
            Frame<T> frame;
            m_frames.push_back(std::move(frame));
        }

        void popFrame() {
            if (m_frames.size() > 1)
                m_frames.pop_back();
        }

        void set(const std::string &key, const T &value) {
            m_frames.back().entries.insert_or_assign(key, value);
        }

        T get(const std::string &key) const {
            for (auto i = m_frames.rbegin(), e = m_frames.rend(); i != e; i++) {
                auto entries = (*i).entries;
                if (entries.find(key) != entries.end())
                    return entries[key];
            }

            throw std::out_of_range("Cannot find key");
        }

        bool contains(const std::string &key) const {
            for (auto i = m_frames.rbegin(), e = m_frames.rend(); i != e; i++) {
                auto entries = (*i).entries;
                if (entries.find(key) != entries.end())
                    return true;
            }

            return false;
        }
    };
}

#endif
