


#include "draw.h"


/* Invalid range exception R={1,100} for both pots.*/
std::string draw::InvalidRangeException::what () {
    message = "Invalid Range: First five: R={LR={1,100-4},UR={LR+4,100}} - ";
    message += "Special: R={LR={1,100-0},UR={LR+0,100}}";
    return message;
}

/* Generic exception (1) */
std::string draw::DrawException1::what () {
    message = "(generic) DrawException1";
    return message;
}

/* Constructor with no args  */
draw::draw() {
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->range.lower = 0;
    this->range.upper = 0;
    this->range_sp.lower = 0;
    this->range_sp.upper = 0;
}

/* Move constructor */
draw::draw ( draw && d)
{
    this->copy_code(d);
}

/* Constructor to set upper bound on both pots. (lower bound=1) */
draw::draw(int upper, int spec_upper)
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->set_range(1, upper);
    this->set_range_sp(1, spec_upper);
}

/* Constructor to set lower and upper bounds on both pots. */
draw::draw(int lower, int upper, int spec_lower, int spec_upper)
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->set_range(lower, upper);
    this->set_range_sp(spec_lower, spec_upper);
}

draw::draw(int rng[2], int sp_rng[2])
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->set_range(rng[0], rng[1]);
    this->set_range_sp(sp_rng[0], sp_rng[1]);
}


/* draw constructor with ball value parameters */
draw::draw(int b1, int b2, int b3, int b4, int b5, int b6) 
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->range.lower = 0;
    this->range.upper = 0;
    this->range_sp.lower = 0;
    this->range_sp.upper = 0;
    this->balls[0] = b1;
    this->balls[1] = b2;
    this->balls[2] = b3;
    this->balls[3] = b4;
    this->balls[4] = b5;
    this->balls[5] = b6;
}

/* draw constructor with ball value as array parameter */
draw::draw(int vals[NBALLS]) 
{
    this->spec_val = &this->balls[NBALLS-1];
    for(int i = 0; i < NBALLS; i++) 
        this->balls[i] = vals[i];
    this->range.lower = 0;
    this->range.upper = 0;
    this->range_sp.lower = 0;
    this->range_sp.upper = 0;
}

draw::draw(int n_vals, int *vals)
{
    if(n_vals < 1 || n_vals > NBALLS) 
        throw DrawException1("Invalid number of balls supplied.");
    this->spec_val = &this->balls[NBALLS-1];
    for(int i = 0; i < n_vals; i++) 
        this->balls[i] = vals[i];
    this->range.lower = 0;
    this->range.upper = 0;
    this->range_sp.lower = 0;
    this->range_sp.upper = 0;
}



/* Common code for copying draw object */
void draw::copy_code(const draw &rhs)
{
    this->spec_val = &this->balls[NBALLS-1];
    for(int i = 0; i < NBALLS; i++) 
        this->balls[i] = rhs.balls[i];
    this->range.lower = rhs.range.lower;
    this->range.upper = rhs.range.upper;
    this->range_sp.lower = rhs.range_sp.lower;
    this->range_sp.upper = rhs.range_sp.upper;
}


void draw::present_draw()
{
    present_draw(0);
}


void draw::present_draw(int usec_pauses)
{
    if(!this->complete() || !this->validate()) {
        std::cout << "Draw is either incomplete or invalid." << std::endl;
        std::cout << *this << std::endl;
        return;
    } 

    std::cout << "DRAW: " << *this << std::endl;

}

/* Assignment operator */
void draw::operator= (const draw& d) 
{
    copy_code(d);
}


/* Overloaded subscript operator */
int& draw::operator[] (int index) 
{
    if(index > NBALLS-1 || index < 0) 
        throw InvalidRangeException("overloaded draw subscript out of range.");
    return this->balls[index];
}

/* Overloaded subscript operator (const) */
const int draw::operator[] (int index) const
{
    if(index > NBALLS-1 || index < 0) 
        throw InvalidRangeException("overloaded draw (const) subscript out of range.");
    return this->balls[index];
}

/* Overloaded equality operator */
bool draw::operator== (const draw& d)   
{
    for(int i = 0; i < 6; i++) {
        if(this->balls[i] != ((draw)d)[i])  
            return false;
    }
    return true;
}

/* Overloaded equality operator (const) */
bool draw::operator== ( const draw& d) const  
{
    for(int i = 0; i < 6; i++) {
        if(this->balls[i] != d[i])  
            return false;
    }
    return true;
}

/* Overloaded inequality operator */
bool draw::operator!= (const draw& d1)   
{
    for(int i = 0; i < 6; i++) {
        if(this->balls[i] != d1[i])  
            return true;
    }
    return false;
}

/* Overloaded inequality operator (const) */
bool draw::operator!= ( const draw& d1) const
{
    for(int i = 0; i < 6; i++) {
        if(this->balls[i] != d1[i])  
            return true;
    }
    return false;
}

/* Determine if the draw has any values (or is empty=[0,0,0,0,0,-0])*/
bool draw::empty() {
    return (this->balls[0] == 0 && this->balls[1] == 0 && this->balls[2] == 0 && 
            this->balls[3] == 0 && this->balls[4] == 0 && this->balls[5] == 0);
}

/* Clear all ball values (make them all zero) */
void draw::clear() {
    memset(this->balls, 0, sizeof(this->balls));
}

/* Check for validation of draw (can have gaps, but must in range and sequential...)*/
bool draw::validate() 
{
    int hash_map[100] = {0};
    int last_ball = 0;
    for(int i = 0; i < NBALLS-1; i++) {
        int ball = this->balls[i];
        if(ball) {
            if( (this->range.lower && (ball < this->range.lower)) || (this->range.upper && (ball > this->range.upper))) 
                return false; 
            else if(hash_map[ball]++) 
                return false;
            else if(ball <= last_ball) 
                return false;
            last_ball = ball;
        }
    }
    return true;
}

/* Validate a draw and clear if invalid. */
bool draw::validate(bool clear) 
{
    bool ret = this->validate();
    if(!ret && clear)
        this->clear();
    return ret;
}

/* Determine if a draw is complete with no gaps (zeros) */
bool draw::complete()
{
    for(int i = 0; i < 6; i++) {
        if(!this->balls[i])
            return false;
    }
    return true;
}

/* Sort the first five balls (special ball is not part of sequential run) */
void draw::ball_sort(int n_balls) 
{
    if(n_balls < 1 || n_balls > 6)
        throw DrawException1("draw::ball_sort: Invalid Index");
    int tmps[5] = {0};
    for(int i = 0; i < 5; i++)
        tmps[i] = this->balls[i];
    int *iptr = std::begin(tmps);
    for(int i = 0; i < n_balls; i++)
        iptr++;
    std::sort(std::begin(tmps), iptr);
    for(int i = 0; i < n_balls; i++)
        this->balls[i] = tmps[i];
}


/* (Auto) fill a draw with a random seqential run in range. */
void draw::fill()
{
    this->fill(NBALLS);
}



void draw::fill(int n_balls) 
{
    if(n_balls < 1 || n_balls > 6) 
        throw DrawException1("draw::fill: Invalid Index");
    else if(n_balls == 6 && !this->valid_sp_range()) 
        throw InvalidRangeException("draw::fill: Invalid range 1");
    if(!this->valid_range()) 
        throw InvalidRangeException("draw::fill: Invalid range 2");
    //this->clear(); // ???
    int hash_map[100] = {0};
    int stop_at = n_balls < NBALLS ? n_balls : NBALLS-1;
    for(int i = 0; i < stop_at; i++) {
        int min = this->range.lower + i, max = this->range.upper-(n_balls-i);
        int num = GET_RAND_NUM;
        if(hash_map[num]++) {
            i--;
            continue;
        }
        this->balls[i] = num;
    }
    /* sort the first five vals and leave space for special ball at the end*/
    ball_sort(stop_at);
    /* If all balls were chosen, the special ball has its own range. */
    if(n_balls == 6) {
        int min = this->range_sp.lower, max = this->range_sp.upper;
        this->balls[5] = GET_RAND_NUM; 
    }
}


bool draw::valid_range()
{
    int lr = this->range.lower, ur = this->range.upper;
    return VALID_RANGE;
}

bool draw::valid_sp_range()
{
    int lr = this->range_sp.lower, ur = this->range_sp.upper;
    return VALID_RANGE_SP;
}


void draw::set_range(int lr, int ur) 
{
    if(!VALID_RANGE) 
        throw InvalidRangeException("draw::set_range: !VALID_RANGE");
    this->range.lower = lr;
    this->range.upper = ur;
}

void draw::set_range_sp(int lr, int ur) 
{
    if(!VALID_RANGE_SP) 
        throw InvalidRangeException("draw::set_range_sp: !VALID_RANGE");
    this->range_sp.lower = lr;
    this->range_sp.upper = ur;
}


int draw::get_ball(int index) 
{
    if(index > NBALLS-1 || index < 0) 
        throw std::range_error("draw::get_ball: Invalid ball index. (I={0,5})");
    return this->balls[index];
}


void draw::set_ball(int index, int val) 
{
    if(index > NBALLS-1 || index < 0) 
        throw DrawException1("draw::set_ball: Invalid ball index. (I={0,5})");
    else if(val < 1 || val > MAX_LOT_RANGE)
        throw InvalidRangeException("draw::set_ball: Invalid range 1");
    else if(index < NBALLS-1) {
        if(val > this->range.upper) 
            throw InvalidRangeException("draw::set_ball: Invalid range 2");
    } else if(val > this->range_sp.upper) 
        throw InvalidRangeException("draw::set_ball: Invalid range 3");

    /* Value is in range, so see if it already exists in the draw */
    for(int i = 0; index < NBALLS-1 && i < NBALLS-1; i++) {
        if(this->balls[i] == val) 
            throw DrawException1("draw::set_ball: duplicate ball value found.");
    }
    this->balls[index] = val;
}

void draw::set_balls(int balls[NBALLS]) 
{
    for(int i = 0; i < NBALLS; i++) 
        this->set_ball(i, balls[i]); 
}


std::ostream& operator<< (std::ostream& out, const draw& data) 
{
    out <<"{{" << data.range.lower << "," << data.range.upper 
        << "},{" << data.range_sp.lower << "," << data.range_sp.upper << "}} --"; 
    for(int i = 0; i < NBALLS; i++) {
        out << "[";
        if(data.balls[i] < 10)
            out << " ";
        out << data.balls[i] << "]";
        if(i < 4)
            out << "-";
        else if(i == 4)
            out << "==";
    }
    return out;
}

