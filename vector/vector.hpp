#ifndef VECTOR_H
#define VECTOR_H
#include <initializer_list>
#include <type_traits>
#include <utility>
#include <stdexcept>

namespace local
{
        constexpr size_t DEFAULT_SIZE = 10;

        template<typename T>
        class vector
        {
        public:
                vector() : vector(DEFAULT_SIZE) {}

                explicit vector(const size_t capacity) : capacity_{capacity}, size_{0}
                {
                        data_ = static_cast<T *>(::operator new(sizeof(T) * capacity));
                }

                vector(std::initializer_list<T> init)
                {
                        const auto size = static_cast<int>(init.size());
                        capacity_ = size;
                        size_ = size;
                        data_ = static_cast<T *>(::operator new(sizeof(T) * size));
                        size_t i = 0;
                        for (const auto &element: init) {
                                new(data_ + i) T(element);
                                i++;
                        }
                }

                vector(const vector &other) : capacity_{other.capacity_}, size_{other.size_}
                {
                        data_ = static_cast<T *>(::operator new(sizeof(T) * other.capacity_));
                        for (size_t i = 0; i < size_; i++) {
                                new(data_ + i) T(other.data_[i]);
                        }
                }

                vector(vector &&other) noexcept : capacity_{other.capacity_}, size_{other.size_}, data_{other.data_}
                {
                        other.capacity_ = 0;
                        other.size_ = 0;
                        other.data_ = nullptr;
                }

                ~vector()
                {
                        for (size_t i = 0; i < size_; i++) {
                                data_[i].~T();
                        }
                        ::operator delete(data_);
                }

                vector &operator=(const vector &other)
                {
                        if (this != &other) {
                                auto *new_data = static_cast<T *>(::operator new(sizeof(T) * other.capacity_));
                                for (size_t i = 0; i < other.size_; i++) {
                                        new(new_data + i) T(other.data_[i]);
                                }

                                for (size_t i = 0; i < size_; i++) {
                                        data_[i].~T();
                                }
                                ::operator delete(data_);

                                capacity_ = other.capacity_;
                                size_ = other.size_;
                                data_ = new_data;
                        }
                        return *this;
                }

                vector &operator=(vector &&other) noexcept
                {
                        if (this != &other) {
                                for (size_t i = 0; i < size_; i++) {
                                        data_[i].~T();
                                }
                                ::operator delete(data_);
                                capacity_ = other.capacity_;
                                size_ = other.size_;
                                data_ = other.data_;
                                other.capacity_ = 0;
                                other.size_ = 0;
                                other.data_ = nullptr;
                        }
                        return *this;
                }

                T &operator[](const size_t index)
                {
                        if (index >= size_) {
                                throw std::out_of_range("index out of range");
                        }
                        return data_[index];
                }

                const T &operator[](const size_t index) const
                {
                        if (index >= size_) {
                                throw std::out_of_range("index out of range");
                        }
                        return data_[index];
                }

                explicit operator bool() const
                {
                        return size_ > 0;
                }

                T *begin()
                {
                        return data_;
                }

                const T *begin() const
                {
                        return data_;
                }

                T *end()
                {
                        return data_ + size_;
                }

                const T *end() const
                {
                        return data_ + size_;
                }

                size_t capacity() const
                {
                        return capacity_;
                }

                size_t size() const
                {
                        return size_;
                }

                bool empty() const
                {
                        return size_ == 0;
                }

                void push_back(const T &element)
                {
                        ensure_capacity(size_ + 1);
                        new(data_ + size_) T(element);
                        size_++;
                }

                void push_back(T &&element)
                {
                        ensure_capacity(size_ + 1);
                        new(data_ + size_) T(std::move(element));
                        size_++;
                }

                void pop_back()
                {
                        if (size_ > 0) {
                                size_--;
                                data_[size_].~T();
                        }
                }

                template<typename F, typename ResultType = std::invoke_result_t<F, T> >
                vector<ResultType> map(F &&func)
                {
                        vector<ResultType> r(size_);
                        for (const auto &elem: *this) {
                                r.push_back(func(elem));
                        }
                        return r;
                }

                template<typename F, typename ResultType = std::invoke_result_t<F, T> >
                vector<ResultType> map(F &&func) const
                {
                        vector<ResultType> r(size_);
                        for (const auto &elem: *this) {
                                r.push_back(func(elem));
                        }
                        return r;
                }

        private:
                size_t capacity_;
                size_t size_;
                T *data_;

                void ensure_capacity(const size_t required_capacity)
                {
                        if (capacity_ < required_capacity) {
                                const size_t new_capacity = std::max(1ULL, required_capacity * 2);
                                auto new_data = static_cast<T *>(::operator new(sizeof(T) * new_capacity));
                                for (size_t i = 0; i < size_; i++) {
                                        new(new_data + i) T(std::move(data_[i]));
                                        data_[i].~T();
                                }
                                ::operator delete(data_);
                                capacity_ = new_capacity;
                                data_ = new_data;
                        }
                }
        };
}

#endif //VECTOR_H
