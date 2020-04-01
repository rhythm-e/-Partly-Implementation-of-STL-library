#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu {
    template<typename T>
    class vector {
    private:
        size_t theSize;
        size_t theCapacity;
        T* objects;
    public:
        class const_iterator;
        class iterator{
        private:
            vector* curVec;
            size_t index;
        public:
            explicit iterator(vector* Vec, size_t num):curVec(Vec),index(num){}
            iterator operator+(const int &n) const {
                iterator x(*this);
                x.index += n;
                return x;
            }

            iterator operator-(const int &n) const {
                iterator x(*this);
                x.index -= n;
                return x;
            }

            int operator-(const iterator &rhs) const {
                if(curVec != rhs.curVec) throw invalid_iterator();
                else return ((index - rhs.index) > 0 ? index - rhs.index: rhs.index - index);
            }

            iterator &operator+=(const int &n) {
                index += n;
                return *this;
            }

            iterator &operator-=(const int &n) {
                index -= n;
                return *this;
            }

            iterator operator++(int) {
                iterator tmp(*this);
                index++;
                return tmp;
            }

            iterator &operator++() {
                index++;
                return *this;
            }

            iterator operator--(int) {
                iterator tmp(*this);
                index--;
                return tmp;
            }

            iterator &operator--() {
                index--;
                return *this;
            }

            T &operator*() const {
                return curVec -> objects[index];
            }

            bool operator==(const iterator &rhs) const {
                return (index == rhs.index && curVec == rhs.curVec);
            }

            bool operator==(const const_iterator &rhs) const {
                return (index == rhs.index && curVec == rhs.curVec);
            }

            bool operator!=(const iterator &rhs) const {return index != rhs.index || curVec != rhs.curVec;}

            bool operator!=(const const_iterator &rhs) {return index != rhs.index || curVec != rhs.curVec;}
        };
        class const_iterator {
        private:
            const vector * curVec;
            size_t index;
        public:
            explicit const_iterator(const vector* Vec,size_t num):curVec(Vec),index(num){}
            const_iterator operator+(const int &n) const {
                const_iterator x(*this);
                x.index += n;
                return x;
            }

            const_iterator operator-(const int &n) const {
                const_iterator x(*this);
                x.index -= n;
                return x;
            }

            int operator-(const iterator &rhs) const {
                if(curVec != rhs.curVec ) throw invalid_iterator();
                else return ((index - rhs.index) > 0 ? index - rhs: rhs - index);
            }

            const_iterator &operator+=(const int &n) {
                index += n;
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                index -= n;
                return *this;
            }

            const_iterator operator++(int) {
                const_iterator tmp(*this);
                index++;
                return tmp;
            }

            const_iterator &operator++() {
                index++;
                return *this;
            }

            const_iterator operator--(int) {
                const_iterator tmp(*this);
                index--;
                return tmp;
            }

            const_iterator &operator--() {
                index--;
                return *this;
            }

            T operator*() const {
                return curVec -> objects[index];
            }

            bool operator==(const iterator &rhs) const {
                return (index == rhs.index && curVec == rhs.curVec);
            }

            bool operator==(const const_iterator &rhs) const {
                return (index == rhs.index && curVec == rhs.curVec);
            }

            bool operator!=(const iterator &rhs) const {return index != rhs.index || curVec != rhs.curVec;}

            bool operator!=(const const_iterator &rhs) const {return index != rhs.index || curVec != rhs.curVec;}
        };

        vector()
        {
            theSize = 0;
            theCapacity = 1;
            objects = (T*)operator new[](theCapacity * sizeof(T));
        }

        vector(const vector &other)
        {
            theSize = other.theSize;
            theCapacity = other.theCapacity;
            objects = (T*)operator new[](theCapacity * sizeof(T));
            for(size_t i = 0; i < theSize; i++)
                new (objects + i)T(other.objects[i]);
    }

        ~vector() {
            for (size_t i = 0; i < theSize; i++)
                objects[i].~T();
            operator delete[](objects,theCapacity * sizeof(T));
        }

        vector &operator=(const vector &other) {
            if(this == & other)  return *this;
            for(size_t i  = 0; i < theSize; i++)
                 objects[i].~T();
            delete [] objects;
            theSize = other.theSize;
            theCapacity = other.theCapacity;
            objects = (T*)operator new[](theCapacity * sizeof(T));
            for(size_t  i = 0; i < theSize; i++)
                new (objects + i)T(other.objects[i]);
            return *this;
        }

        T &at(const size_t &pos)
        {
            if(pos >= 0 && pos < theSize) return objects[pos];
            else throw index_out_of_bound();
        }

        const T &at(const size_t &pos) const {
            if(pos >= 0 && pos < theSize) return objects[pos];
            else throw index_out_of_bound();
        }


        T &operator[](const size_t &pos)
        {
            if(pos >= 0 && pos < theSize) return objects[pos];
            else throw index_out_of_bound();
        }

        const T &operator[](const size_t &pos) const
        {
            if(pos >= 0 && pos < theSize) return objects[pos];
            else throw index_out_of_bound();
        }


        const T &front() const
        {
            if(theSize == 0) throw container_is_empty();
            else return objects[0];
        }

        const T &back() const
        {
            if(theSize == 0) throw container_is_empty();
            else return objects[theSize - 1];
        }


        iterator begin()
        {
            return iterator(this, 0);
        }

        const_iterator cbegin() const
        {
            return const_iterator(this, 0);
        }


        iterator end() {
            return iterator(this, theSize);
        }

        const_iterator cend() const
        {
            return const_iterator (this, theSize);
        }


        bool empty() const{ return theSize == 0;}


        size_t size() const { return theSize;}


        void clear() {
            for(unsigned int i = 0; i < theSize; i++)
                objects[i].~T();
            operator delete[](objects,theCapacity * sizeof(T));
            theSize = 0;
        }
        bool isFull(){ return theSize == theCapacity;}
        void doubleSpace()
        {
            T* tmp = objects;
            theCapacity *= 2;
            objects = (T*)operator new[](theCapacity * sizeof(T));
            for(size_t i = 0; i < theSize; i++)
                 new (objects + i) T (tmp[i]);
            for(size_t  i = 0; i < theSize; i++)
                tmp[i].~T();
            operator delete[](tmp, theCapacity * sizeof(T));
        }

        iterator insert(iterator pos, const T &value) {
            if(isFull()) doubleSpace();
            int dis = pos - begin();
            new (objects + theSize) T();
            size_t i = theSize;
            while( i > dis) {
                objects[i] = objects[i - 1];
                i--;
            }
            objects[dis] = value;
            theSize++;
            return pos;
        }

        iterator insert(const size_t &ind, const T &value) {
            if(ind > theSize) throw index_out_of_bound();
            if(isFull()) doubleSpace();
            new (objects + theSize) T();
            size_t i = theSize;
            while( i > ind) {
                objects[i] = objects[i - 1];
                i--;
            }
            theSize++;
            objects[ind] = value;
            return iterator(this, ind);
        }

        iterator erase(iterator pos) {
            size_t dis = pos - begin();
            size_t i = dis;
            while(i < theSize - 1) {
                objects[i] = objects[i + 1];
                i++;
            }
            objects[theSize - 1].~T();
            theSize--;
            return iterator(this, dis);
        }

        iterator erase(const size_t &ind) {
            if(ind >= theSize) throw index_out_of_bound();
            size_t i = ind;
            while(i < theSize - 1) {
                objects[i] = objects[i + 1];
                i++;
            }
            objects[theSize - 1].~T();
            theSize--;
            return iterator(this, ind);
        }

        void push_back(const T &value) {
            if(isFull()) doubleSpace();
            theSize++;
            new (objects + theSize - 1) T(value);
        }

        void pop_back() {
            if(theSize == 0) throw container_is_empty();
            objects[theSize - 1].~T();
            theSize--;
        }
    };
};

#endif
