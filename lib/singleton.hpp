#pragma once

template <typename T>
class Singleton {
public:
    static auto getInstance( ) -> T * {
        if ( !instance_ ) {
            instance_ = new T;
        }
        return instance_;
    }

    static auto killInstance( ) -> void {
        if ( instance_ ) {
            delete instance_;
            instance_ = nullptr;
        }
    }

protected:
    Singleton( ) {}
    Singleton( const Singleton<T> & ) = delete;
    virtual ~Singleton( ) {}
    auto operator=( const Singleton<T> & ) -> Singleton<T> & = delete;

private:
    static T *instance_;
};

// init
template <typename T>
T *Singleton<T>::instance_ = nullptr;

