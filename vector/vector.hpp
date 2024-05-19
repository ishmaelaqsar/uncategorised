#ifndef collection_h
#define collection_h

#include <initializer_list>
#include <stdexcept>
#include <type_traits>

namespace local
{

template<typename T>
class collection
{
public:
    virtual ~collection() {}
    virtual T* begin() = 0;
    virtual T* begin() const = 0;
    virtual T* end() = 0;
    virtual T* end() const = 0;
    virtual int size() const = 0;
    virtual void push_back(const T&) = 0;
    virtual T& operator[](int) = 0;
    virtual const T& operator[](int) const = 0;
};

template<typename T>
class vector : public collection<T>
{
private:
    static constexpr int DEFAULT_SIZE_ = 8;

    int capacity_;
    int size_;
    T* data_;

public:
    vector() : vector(DEFAULT_SIZE_) {}

    vector(int capacity)
        : capacity_{capacity},
          size_{0},
          data_{new T[capacity]} {}

    vector(std::initializer_list<T> init)
        : capacity_{static_cast<int>(init.size())},
          size_{static_cast<int>(init.size())},
          data_{new T[init.size()]}
    {
        std::copy(init.begin(), init.end(), data_);
    };

    ~vector()
    {
        capacity_ = 0;
        size_ = 0;
        delete[] data_;
    }

    vector(const vector& o)
        : capacity_{o.capacity()},
          size_{o.size()},
          data_{new T[o.capacity()]}
    {
        for (int i = 0; i != o.size(); ++i) {
            data_[i] = o[i];
        }
    }

    vector<T>& operator=(const vector& o)
    {
        delete[] data_;
        data_ = new T[o.capacity()];
        capacity_ = o.capacity();
        size_ = o.size();
        for (int i = 0; i != o.size(); ++i) {
            data_[i] = o[i];
        }
        return *this;
    }

    vector(vector&& o)
        : capacity_{o.capacity()},
          size_{o.size()},
          data_{o.data_}
    {
        o.capacity(0);
        o.size(0);
        o.data(nullptr);
    }

    vector<T>& operator=(vector&& o)
    {
        delete[] data_;
        data_ = o.data_;
        capacity_ = o.capacity();
        size_ = o.size();
        o.data(nullptr);
        o.size(0);
        return *this;
    }

    T* begin() override
    {
        return size() ? &data_[0] : nullptr;
    }

    T* begin() const override
    {
        return size() ? &data_[0] : nullptr;
    }

    T* end() override
    {
        return size() ? &data_[0] + size() : nullptr;
    }

    T* end() const override
    {
        return size() ? &data_[0] + size() : nullptr;
    }

    int size() const override
    {
        return size_;
    }

    void push_back(const T& item) override
    {
        if (size_ < capacity_) {
            data_[size_++] = item;
        } else {
            T* n_data = new T[capacity_ * 2];
            std::copy(this->begin(), this->end(), n_data);
            delete[] data_;
            data_ = n_data;
            capacity_ = capacity_ * 2;
            this->push_back(item);
        }
    }

    T& operator[](int index) override
    {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("invalid index");
        }
        return data_[index];
    }

    const T& operator[](int index) const override
    {
        if (index < 0 || index >= size_) {
            throw std::out_of_range("invalid index");
        }
        return data_[index];
    }

    int capacity() const
    {
        return capacity_;
    }

    void capacity(int capacity)
    {
        capacity_ = capacity;
    }

    void size(int size)
    {
        size_ = size;
    }

    void data(T* data)
    {
        data_ = data;
    }

    template<typename F, typename ResultType = std::invoke_result_t<F, T>>
    vector<ResultType> map(const F&& func) const
    {
        vector<ResultType> r(size_);        
        for (const auto& elem : *this) {
            r.push_back(func(elem));
        }
        return r;
    }
};

}
#endif
