
#ifndef MTCH
#define MTCH

#include <ostream>
#include <vector>
#include <chrono>

#include "draw.h"

class match {

    protected:

    draw vals; 
    long n_tries;
    double duration;
    bool searching;

    public:

    class Unmatchable : public std::exception {
        std::string message;
        public:
        Unmatchable(std::string msg) : message(msg) {}
        std::string what();
    };

    match();

    ~match() {/*...*/}

    match(draw &d);

    match(match &m) {/*...*/}

    match(const match &m) {/*...*/} 

    int& operator[] (int index) ;
    const int operator[] (int index) const;

    bool matchable();

    void set_draw(draw *d); // { this->vals = *d; }

    const draw get_vals() { return this->vals; }

    long run(); 

    const long get_n_tries() { return this->n_tries; }

    friend std::ostream& operator<< (std::ostream& out, const match& data);

};

std::ostream& operator<<(std::ostream& output, const match& data);



#endif

