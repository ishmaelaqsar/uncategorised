#ifndef OPTIONAL_H
#define OPTIONAL_H

#include <utility>
#include <stdexcept>

namespace local
{
        template<typename T>
        class optional
        {
        public:
                optional() noexcept : _has_value(false), _value(nullptr) {}

                ~optional()
                {
                        if (_value) delete _value;
                }

                explicit optional(const T &value) : _has_value(true), _value(new T(value)) {}
                explicit optional(T &&value) : _has_value(true), _value(new T(std::move(value))) {}

                optional(const optional &other) : _has_value(other._has_value)
                {
                        if (_has_value) _value = new T(*other._value);
                        else _value = nullptr;
                }

                optional(optional &&other) noexcept : _has_value(other._has_value), _value(std::move(other._value))
                {
                        other._has_value = false;
                        other._value = nullptr;
                }

                optional &operator=(const optional &other)
                {
                        if (this != &other) {
                                if (_value) delete _value;
                                _has_value = other._has_value;
                                _value = _has_value ? new T(*other._value) : nullptr;
                        }
                        return *this;
                }

                optional &operator=(optional &&other) noexcept
                {
                        if (this != &other) {
                                if (_value) delete _value;
                                _has_value = other._has_value;
                                _value = other._value;
                                other._has_value = false;
                                other._value = nullptr;
                        }
                        return *this;
                }

                explicit operator bool() const
                {
                        return _has_value;
                }

                T &operator*()
                {
                        if (!_has_value) throw std::runtime_error("No value");
                        return *_value;
                }

                const T &operator*() const
                {
                        if (!_has_value) throw std::runtime_error("No value");
                        return *_value;
                }

                T *operator->()
                {
                        if (!_has_value) throw std::runtime_error("No value");
                        return _value;
                }

                const T *operator->() const
                {
                        if (!_has_value) throw std::runtime_error("No value");
                        return _value;
                }

                bool has_value() const
                {
                        return _has_value;
                }

                T &value()
                {
                        if (!_has_value) throw std::runtime_error("No value");
                        return *_value;
                }

                const T &value() const
                {
                        if (!_has_value) throw std::runtime_error("No value");
                        return *_value;
                }

        private:
                bool _has_value;
                T *_value;
        };
}

#endif //OPTIONAL_H
