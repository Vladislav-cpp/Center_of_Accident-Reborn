#pragma once

template <typename T>
class ImplSingleton {
    public:
    static T& Instance() {
        if(!instance) instance = new T;
        return *instance;
    }

    protected:
    ImplSingleton(const ImplSingleton&)             = delete;
    ImplSingleton& operator=(const ImplSingleton&)  = delete;
    ImplSingleton(ImplSingleton&&)                  = delete;
    ImplSingleton& operator=(ImplSingleton&&)       = delete;

    protected:
    ImplSingleton() = default;
    ~ImplSingleton() = default;

    private:
    static inline T* instance = nullptr;
};

