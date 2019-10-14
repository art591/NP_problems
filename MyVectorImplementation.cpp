#include <iostream>
#include <memory>
#include <algorithm>


template<class T>
class Vector {
private:
    T * base = nullptr;
    size_t len = 0;
    size_t cap = 0;

public:
    Vector() {}
    Vector(size_t n) {
        base = static_cast<T*>(operator new(sizeof(T) * n));
        cap = n;
        len = n;
    }
    Vector(const Vector& v2) {
        try {
            base = static_cast<T *>(operator new(sizeof(T) * v2.len));
        } catch(...) {
            throw;
        }

        size_t i;
        try {
            for (i = 0; i != len; ++i) {
                new(base + i) T(v2[i]);
            }
        } catch(...) {
            for (size_t j = 0; j != i; ++j) {
                base[j].~T();
            }
            operator delete(base);
        }
        len = v2.len;
        cap = len;
    }
    Vector& operator=(const Vector& v2) {
        for (size_t i = 0; i != len; ++i) {
            base[i].~T();
        }
        operator delete(base);
        base = static_cast<T*>(operator new(sizeof(T) * v2.cap));
        for (size_t i = 0; i != v2.len; ++i) {
            new (base + i) T(v2[i]);
        }
        len = v2.len;
        cap = v2.cap;
        return *this;
    }
    void push_back(const T& x) {
        if (cap == len) {
            size_t newcap;
            if (len == 0) {
                newcap = 1;
            } else {
                newcap = len * 2;
            }
            (*this).reserve(newcap);
            new (base + len) T(x);
            ++len;
        } else {
            new(base + len) T(x);
            ++len;
        }
    }
    void push_back(T&& x) {
        if (cap == len) {
            if (len == 0) {
                cap = 1;
            } else {
                cap = len * 2;
            }
            (*this).reserve(cap);
            new (base + len) T(std::move(x));
            ++len;
        } else {
            new(base + len) T(std::move(x));
            ++len;
        }
    }
    size_t size() const {
        return len;
    }
    ~Vector() {
        for (size_t i = 0; i != len; ++i) {
            base[i].~T();
        }
        operator delete(base);
    }
    T& operator[](size_t i) const {
        return base[i];
    }
    T& operator[](size_t i) {
        return base[i];
    }
    void pop_back() {
        if (len > 0) {
            --len;
            base[len].~T();
        }
    }
    size_t capacity() const {
        return cap;
    }
    size_t capacity() {
        return cap;
    }
    void reserve(size_t n) {
        if (n > cap) {
            T * newv = static_cast<T*>(operator new(sizeof(T) * n));
            size_t i;
            try {
                for (i = 0; i != len; ++i) {
                    new(newv + i) T(base[i]);
                }
            } catch(...) {
                for (size_t j = 0; j != i; ++j) {
                    base[j].~T();
                }
            }
            for (size_t i = 0; i != len; ++i) {
                base[i].~T();
            }
            operator delete(base);
            base =  newv;
            cap = n;
        }
    }
    void resize(size_t n) {
        if (n > len) {
            if (n > cap) {
                T * newv = static_cast<T*>(operator new(sizeof(T) * n));
                size_t i = 0;
                try {
                    for (i = 0; i != len; ++i) {
                        new(newv + i) T(base[i]);
                    }
                    for (i = len; i != n; ++i) {
                        new(newv + i) T();
                    }
                } catch(...) {
                    for (size_t j = 0; j != i; ++j) {
                        base[j].~T();
                    }
                }
                for (size_t i = 0; i != len; ++i) {
                    base[i].~T();
                }
                operator delete(base);
                base =  newv;
                cap = n;
            } else {
                size_t i = 0;
                try {
                    for (i = len; i != n; ++i)
                        new(base + i) T();
                } catch(...) {
                    for (size_t j = 0; j != i; ++j) {
                        base[j].~T();
                    }
                }
            }
        } else {
            size_t lenTemp = len;
            for (size_t i = n; i != lenTemp; ++i) {
                (*this).pop_back();
            }
        }
        len = n;
    }
    void clear() {
        for (size_t i = 0; i != len; ++i) {
            base[i].~T();
        }
        len = 0;
    }
    void swap(Vector<T>& v2) {
        std::swap(base, v2.base);
        std::swap(cap, v2.cap);
        std::swap(len, v2.len);
    }
    T * begin() {
        return base;
    }
    T * end() {
        return base + len;
    }
};

