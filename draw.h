
#ifndef DR
#define DR

#include <iostream>
#include <unistd.h>

/** 
 * Draw class consists of the six integer values and provides error checking for filling.
 **/

/* Every draw consists of five white balls and one special ball. */
#define NBALLS 6

/* Lottery range is contained to R={1,100} for both first five and special. */
#define MAX_LOT_RANGE 100

/*-- Valid range macros --*/
/* First five: R={LR={1,100-4},UR={LR+4,100}} */
#define VALID_RANGE  (!(!lr || !ur || (ur - lr < 4) || (ur > MAX_LOT_RANGE))) 
/* Special: R={LR={1,100-0},UR={LR+0,100}} */
#define VALID_RANGE_SP (!(!lr || !ur || (ur - lr < 0) || (ur > MAX_LOT_RANGE)))

#define GET_RAND_NUM ((rand() % (max - min + 1)) + min)

class draw {

    protected:

    /*==Member vars */
    int balls[NBALLS];
    int *spec_val; 
    void copy_code(const draw &rhs); //, copy_code(const draw &rhs) const;
    void ball_sort(int n_balls);

    public:

    struct draw_range {
        int lower, upper; 
    } range, range_sp;

    /* Exception for draw range exceptions. */
    class InvalidRangeException : public std::exception {
        std::string message;
        public:
        InvalidRangeException(std::string msg) : message(msg) {}
        std::string what();
    };
    
    /* All other draw range exceptions. */
    class DrawException1 : public std::exception {
        std::string message;
        public:
        DrawException1(std::string msg) : message(msg) {}
        std::string what();
    };

    draw(); // default constructor
    ~draw() {} // destructor
    /* draw copy constructor */
    draw(draw &d) {this->copy_code(d);}
    /* draw copy constructor (const) */
    draw(const draw &d) {this->copy_code(d);}

    draw ( draw && d);
    // Move constructor

    /* Range driven/specified (randomized) draws */
    draw(int upper, int spec_upper);
    draw(int lower, int upper, int spec_lower, int spec_upper);
    draw(int rng[2], int sp_rng[2]);
    
    /* Hardwired (no randomization or plucking) draws */
    draw(int b1, int b2, int b3, int b4, int b5, int b6);
    draw(int vals[NBALLS]);
    draw(int n_vals, int *vals);

    /* Overloaded operators */
    int& operator[] (int index) ;
    const int operator[] (int index) const;
    void operator=( const draw& d);
    //void operator=(const draw& d);
    
    bool operator == (const draw &d);
    bool operator == (const draw &d) const;
    bool operator != (const draw &d);
    bool operator != (const draw &d) const;

    /*==Member functions */
    bool valid_range(), valid_sp_range();
    bool valid_range() const { return ((draw*)this)->valid_range(); }
    bool valid_sp_range() const { return ((draw*)this)->valid_sp_range(); }
    bool validate(), validate(bool clear);
    bool validate() const {return ((draw*)this)->validate();}
    bool validate(bool clear) const {return ((draw*)this)->validate(clear);}
    bool complete();
    bool complete() const {return ((draw*)this)->complete();}
    bool empty();
    bool empty() const {return ((draw*)this)->empty();}
    void clear();
    void clear() const {((draw*)this)->clear();}
    void fill(), fill(int n_balls);
    void present_draw(), present_draw(int usec_pauses);
    void present_draw() const {((draw*)this)->present_draw();}
    void present_draw(int usec_pauses) const {((draw*)this)->present_draw(usec_pauses);}
    friend std::ostream& operator<< (std::ostream& out, const draw& data);

    /*--Get(ter) routines */
    int get_range_lr() { return this->range.lower; }
    int get_range_lr() const { return this->range.lower; }
    int get_range_ur() { return this->range.upper; }
    int get_range_ur() const { return this->range.upper; }
    int get_range_sp_lr() { return this->range_sp.lower; }
    int get_range_sp_lr() const { return this->range_sp.lower; }
    int get_range_sp_ur() { return this->range_sp.upper; }
    int get_range_sp_ur() const { return this->range_sp.upper; }
    int get_spec_val() { return *this->spec_val; }
    int get_spec_val() const { return *this->spec_val; }
    int get_ball(int index), get_ball(int index) const;

    /*--Set(ter) routines */
    void set_ball(int index, int val), set_balls(int balls[NBALLS]);
    void set_range(int lower, int upper), set_range_sp(int lower, int upper);

};

std::ostream& operator<<(std::ostream& output, const draw& data);


#endif