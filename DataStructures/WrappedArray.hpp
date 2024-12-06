#pragma once
#include <new>

// ds == data structs
namespace ds {

template <class T>
class BaseWrappedArray {
protected:
  size_t m_size;
  size_t m_start;
  T *m_data;
public:
  BaseWrappedArray(T *data, size_t size) : m_data(data), m_size(size), m_start(0) {}

  size_t size() { return this->m_size; }

  void write(T *in) {
    memcpy(&this->m_data[m_start], in, m_size - m_start);
    memcpy(&this->m_data, &in[m_size - m_start], this->m_start);
  }
  void read(T *in) {
    memcpy(in, &this->m_data[m_start], m_size - m_start);
    memcpy(&in[m_size - m_start], &this->m_data, this->m_start);
  }
  void push_back(const T& val) {
    ++this->m_start;
    if (this->m_start == this->m_size) {
      this->m_start = 0;
    }
    this->m_data[this->m_start] = val;
  }
  void push_back(T&& val) {
    ++this->m_start;
    if (this->m_start == this->m_size) {
      this->m_start = 0;
    }
    this->m_data[this->m_start] = static_cast<T&&>(val); // simple move
  }
};

template <class T, size_t T_size>
class StaticWrappedArray : public BaseWrappedArray<T> {
  static T static_data[T_size];
public:
  StaticWrappedArray() : BaseWrappedArray(static_data, T_size) {}
  StaticWrappedArray(StaticWrappedArray&& other) = delete; // TODO { memcpy(this->m_data, other.m_data, other.m_size); }
  StaticWrappedArray(const StaticWrappedArray& other) = delete; // TODO { memcpy(this->m_data, other.m_data, other.m_size); }
  ~StaticWrappedArray() = default;

};
// Classic new and delete Wrapped Array
template <class T>
class WrappedArray : public BaseWrappedArray<T> {
  WrappedArray(size_t size) : BaseWrappedArray(new T[size], size) {}
  WrappedArray(WrappedArray&& other) = delete; // TODO // { this->m_data = other.m_data; other.m_data = nullptr; }
  WrappedArray(const WrappedArray& other) = delete; // TODO
  ~WrappedArray() { if (this->m_data) { delete[] this->m_data; } }
};

};