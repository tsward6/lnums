


#include "draw.h"

void squishing_boners()
{

    int frumpy = 0;
    int wow = GET_RAND_NUM(3, 10);
    while(1) {
        if(frumpy == 0) {
            std::cout << std::endl << "~-- SQUISHING BONERS --~"<< std::endl; 
            std::cout << "     ___        ___" << std::endl; 
        } else 
            std::cout << "      ^          ^" << std::endl;       
        std::cout << "    /_ _\\      /_ _\\    " << std::endl;
        if(frumpy == 0) 
            std::cout << "     {<}        {<}" << std::endl;
        else
            std::cout << "     |||        |||"<< std::endl;
        for(int i = 0; i < (frumpy); i++) 
            std::cout << "     |||        |||"<< std::endl;
            
        if(frumpy) {
            std::cout << "    {@(@}      {@)@}" << std::endl;
            std::cout << "      X          X" << std::endl;
        } else {
            std::cout << "   ~~^|^~~    ~~^|^~~" << std::endl;
            std::cout << "========================" << std::endl;
            int srnd = GET_RAND_NUM(5, 20);
            srnd *= 10000;
            usleep(srnd); 
            //usleep(300000);
        } 

        frumpy++;
        
        if(frumpy == wow) {
            frumpy = 0;
            wow = GET_RAND_NUM(3, 10);
        } 

        usleep(100000); 
    }
    
}



/* Overloaded range equality operator */
bool range::operator== (const range& r)   
{
    return r.lower == this->lower && r.upper == this->upper;
}

/* Overloaded range inequality operator */
bool range::operator!= (const range& r)   
{
    return r.lower != this->lower || r.upper != this->upper;
}

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

std::string gap::GapException1::what () {
    message = "(generic) GapException1";
    return message;
}

/* Constructor with no args  */
draw::draw() {
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    this->gaps = 0;
    this->ball_const = 0;
}

/* Move constructor */
draw::draw ( draw && d )
{
    this->copy_code(d);
}

/* Constructor to set upper bound on both pots. (lower bound=1) */
draw::draw(int upper, int spec_upper)
{
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(1, upper);
    this->set_range_sp(1, spec_upper);
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
}

draw::~draw()
{
    if(this->gaps) 
        delete [] this->gaps;
    if(this->ball_const) 
        delete [] this->ball_const;
}

/* Constructor to set lower and upper bounds on both pots. */
draw::draw(int lower, int upper, int spec_lower, int spec_upper)
{
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    //memset(this->gaps, 0, sizeof(this->gaps));
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(lower, upper);
    this->set_range_sp(spec_lower, spec_upper);
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
}

/* Constructor to set lower and upper bounds on both pots. (as arrays) */
draw::draw(int rng[2], int sp_rng[2])
{
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(rng[0], rng[1]);
    this->set_range_sp(sp_rng[0], sp_rng[1]);
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
}


/* draw constructor with ball value parameters */
draw::draw(int b1, int b2, int b3, int b4, int b5, int b6) 
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->balls[0] = b1;
    this->balls[1] = b2;
    this->balls[2] = b3;
    this->balls[3] = b4;
    this->balls[4] = b5;
    this->balls[5] = b6;
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    this->gaps = 0;
    this->ball_const = 0;
}

/* draw constructor with ball value as array parameter */
draw::draw(int vals[NBALLS]) 
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    for(int i = 0; i < NBALLS; i++) 
        this->balls[i] = vals[i];
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    this->gaps = 0;
    this->ball_const = 0;
}

draw::draw(int n_vals, int *vals)
{
    if(n_vals < 1 || n_vals > NBALLS) 
        throw DrawException1("Invalid number of balls supplied.");
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    for(int i = 0; i < n_vals; i++) 
        this->balls[i] = vals[i];
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    this->gaps = 0;
    this->ball_const = 0;
}

draw::draw(range dr) 
{ 
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(dr);
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
}

//draw::draw(range dr, range dr_sp) : draw(dr.lower, dr.upper, dr_sp.lower, dr_sp.upper) {}
draw::draw(range dr, range dr_sp) 
{ 
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(dr);
    this->set_range_sp(dr_sp);
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
}

/* Common code for copying draw object */
void draw::copy_code(const draw &rhs)
{
    this->spec_val = &this->balls[NBALLS-1];
    this->gaps = 0;
    this->ball_const = 0;

    if(rhs.gaps) {
        this->gaps = new gap[NGAPS];
        for(int i = 0; i < NGAPS; i++)
            memset(&this->gaps[i], 0, sizeof(this->gaps[i]));
    }

    if(rhs.ball_const) {
        this->ball_const = new int[NBALLS];
        for(int i = 0; i < NBALLS; i++) 
            memset(&this->ball_const[i], 0, sizeof(this->ball_const[i]));
    }

    for(int i = 0; i < NBALLS; i++) {
        this->balls[i] = rhs.balls[i];
        if(i < NGAPS && this->gaps) 
            this->gaps[i] = rhs.gaps[i];
        if(this->ball_const)
            this->ball_const[i] = rhs.ball_const[i];
    }

    this->rng_ff = rhs.rng_ff;
    this->rng_sp = rhs.rng_sp;
    
    this->timestamp = rhs.timestamp;
}


void draw::present()
{
    present(1000000); // seconds in usec
}


void draw::present(int usec_pauses)
{
    if(!this->complete() || !this->validate()) {
        std::cout << "Draw is either incomplete or invalid: " << *this << std::endl;
        return;
    } 

    for(int i = 0; i < NBALLS && this->balls[i]; i++) {
        int ball_val = this->balls[i];
        std::cout << "BALL " << i+1 << ": " << ball_val << std::endl;
        usleep(usec_pauses);
    }
    
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
    for(int i = 0; i < NBALLS; i++) {
        if(this->balls[i] != ((draw)d)[i])  
            return false;
    }
    return true;
}

draw_win::draw_win(int date[3], std::string which_lotto, int vals[6]) : draw(vals)
{
    this->date.year = date[0];
    this->date.month = date[1];
    this->date.day = date[2];
    this->which_lotto = which_lotto;
    this->megaplier = 0;
}

std::string draw_win::get_date_str()
{
    std::string ret; // = "\"";

    std::string frumpster;
    std::stringstream strstream;
    strstream << this->date.year;
    strstream >> frumpster;
    ret += frumpster + "-";

    std::stringstream strstream2;
    strstream2 << this->date.month;
    strstream2 >> frumpster;
    ret += frumpster + "-";

    std::stringstream strstream3;
    strstream3 << this->date.day;
    strstream3 >> frumpster;
    ret += frumpster; // + "\"";

    return ret;

}

bool draw::equal_to(int n_balls, const draw& d)
{
    return this->equal_to(n_balls, d, false);
}

bool draw::equal_to(int n_balls, const draw& d, bool use_gaps)
{
    for(int i = 0; i < n_balls; i++) {
        if(use_gaps && this->balls[i] == DRAW_GAP_VAL)
            continue;
        if(this->balls[i] != d[i])  
            return false;
    }
    return true;
}

/* Overloaded equality operator (const) */
bool draw::operator== ( const draw& d) const  
{
    for(int i = 0; i < NBALLS; i++) {
        if(this->balls[i] != d[i])  
            return false;
    }
    return true;
}

/* Overloaded inequality operator */
bool draw::operator!= (const draw& d1)   
{
    for(int i = 0; i < NBALLS; i++) {
        if(this->balls[i] != d1[i])  
            return true;
    }
    return false;
}

/* Overloaded inequality operator (const) */
bool draw::operator!= ( const draw& d1) const
{
    for(int i = 0; i < NBALLS; i++) {
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

bool draw::empty_ff()
{
    return (this->balls[0] == 0 && this->balls[1] == 0 && this->balls[2] == 0 && 
            this->balls[3] == 0 && this->balls[4] == 0);

}

bool draw::empty_sp()
{
    return this->balls[NBALLS-1] == 0;

}

/* Clear all ball values (make them all zero) */
void draw::clear() 
{
    memset(this->balls, 0, sizeof(this->balls));
}

void draw::clear(int index) {
    if(index < 0 || index > NBALLS-1)
        throw DrawException1("index < 0 || index > NBALLS-1");
    this->balls[index] = 0;
}

/* Check for validation of draw (can have gaps, but must in range and sequential...)*/
bool draw::validate() 
{
    
    int prev_index = -1; 
    int hash_map[100] = {0};
    int last_ball = 0; 

    for(int i = 0; i < NBALLS-1; i++) {
        int ball = this->balls[i];
        if(ball == DRAW_GAP_VAL)
            continue;

        if(ball) {
            if( ((ball <= last_ball) || (ball > MAX_LOT_RANGE)) || ( (prev_index > -1) && ((ball - last_ball) > (i - prev_index)) ) )
                return false;
            else if( (this->rng_ff.lower && (ball < this->rng_ff.lower)) || 
                (this->rng_ff.upper && (ball > this->rng_ff.upper-(((NBALLS-1)-1)-i))) ) 
                return false; 
            else if(hash_map[ball]++) 
                return false;

            last_ball = ball;
            prev_index = i;
        }
    }

    return true;
}


bool draw::valid_gaps() 
{
    std::cout << "GRANDPA'S GETTIN' ANAL: " << this->gaps << std::endl;
    if(!this->gaps)
        return true;
    
    int gval = 0;
    for(int i = 0; i < NGAPS; i++) {
        std::cout << "UNCLE TED'S GETTIN' ANAL: " << this->gaps[i].type << ", " << this->gaps[i].val << std::endl;
        switch(this->gaps[i].type) {
            case GT_NOTSET:
                gval++;
                break;
            case GT_LT:
                std::cout << "GRANDPA'S GETTIN' A BLUMPKIN" << std::endl;
                if(this->gaps[i].val < 2) 
                    return false;
                gval++;
                break;
            case GT_LTE:
                gval++;
                break;
            case GT_GTE:
                std::cout << "GRANDPA'S GETTIN' A BLUMPKIN AGAIN" << std::endl;
                if(this->gaps[i].val < 1) 
                    return false;
                gval += this->gaps[i].val;
                break;
            case GT_GT:
                gval += (this->gaps[i].val + 1);
                break;
        }
    }
    std::cout << "CORNPOP'S GETTIN' ANAL: " << gval << ", " <<
        (this->rng_ff.upper ? this->rng_ff.upper : MAX_LOT_RANGE) << std::endl;
    return gval <= (this->rng_ff.upper ? this->rng_ff.upper : MAX_LOT_RANGE);
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


void draw::show_gaps()
{
    std::cout << "~!-~@- Draw Gaps -@~-!~ " << std::endl; //this->gaps << std::endl;
    if(!this->gaps) {
        std::cout << "Gaps are NOT SET." << std::endl;
        return;
    }
    for(int i = 0; i < NGAPS; i++) {
        std::cout << "~GAP[" << i << "] ";// << std::endl;; // << *this->gaps[i] << std::endl;
        if(this->gaps[i].type)
            std::cout << this->gaps[i] << std::endl;
        else 
            std::cout << "<NOT SET>" << std::endl;
    }
}

void draw::show_constraints()
{
    std::cout << "~!-~@- Draw Restrictions -@~-!~ " << std::endl;
    this->show_gaps();
    this->show_ball_const();
}


void draw::clear_gap(int index)
{
    if(index < 0 || index > NGAPS-1) 
        throw gap::GapException1("index < 0 || index > NGAPS-1");
    memset(&this->gaps[index], 0, sizeof(this->gaps[index]));
    bool last_gap = true;
    for(int i = 0; last_gap && i < NGAPS; i++) {
        if(this->gaps[i].type || this->gaps[i].val) {
            std::cout << "EXPLODING BONERS" << std::endl;
            //exit(9);
            last_gap = false;
        }
    }
    if(last_gap) {
        //squishing_boners();
        delete [] this->gaps;
        this->gaps = 0;
    }
}


void draw::clear_gaps()
{
    for(int i = 0; i < NGAPS; i++) {
        try {
            this->clear_gap(i);
        } catch(gap::GapException1 &ge1) {
            throw gap::GapException1("this->clear_gap(i)");
        }
    }
}


bool draw::check_gaps(int stop_at)
{
    if(!this->gaps)
        return true;
    int gap_val;
    for(int i = 0; i < NGAPS; i++) {

        if(this->gaps[i].type && (this->balls[i] && this->balls[i+1])) {

            gap_val = this->balls[i+1] - this->balls[i];

            switch(this->gaps[i].type) {
                case GT_LT:
                    if(gap_val >= this->gaps[i].val) 
                        return false;
                    break;
                case GT_LTE:
                    if(gap_val > this->gaps[i].val) 
                        return false;
                    break;
                case GT_EQ:
                    if(gap_val != this->gaps[i].val) 
                        return false;
                    break;
                case GT_GTE:
                    if(gap_val < this->gaps[i].val) 
                        return false;
                    break;
                case GT_GT:
                    if(gap_val <= this->gaps[i].val) 
                        return false;
                    break;
                default:
                    throw gap::GapException1("Invalid type");
            }
        }
    }
    return true;
}



void draw::set_gap_type(int index, int type)
{
    if(index < 0 || index > NGAPS-1) 
        throw gap::GapException1("index < 0 || index > NGAPS-1");
    bool gaps_set = false;
    if(!this->gaps) {
        gaps_set = true;
        this->gaps = new gap[NGAPS];
        for(int i = 0; i < NGAPS; i++)
            memset(&this->gaps[i], 0, sizeof(this->gaps[i]));
    } 

    switch(type) {
        case GT_EQ:
        case GT_LTE:
        case GT_GTE:
        case GT_GT:
        case GT_LT:
            /* If the previous type was different from the new type */
            if(this->gaps[index].type != type) {
                
                /* Save the current type in case it fails the test before
                   setting to the new type. */
                int prev_type = this->gaps[index].type;
                this->gaps[index].type = type;
                
                /* We need to make sure that the least possible draw still exists */
                if(this->gaps[index].val) {   
                    try {
                        this->set_gap_val(index, this->gaps[index].val);
                    } catch(gap::GapException1 &ge1) {
                        if(gaps_set) {
                            delete [] this->gaps;
                            this->gaps = 0;
                        } else
                            this->gaps[index].type = prev_type;
                        throw ge1;
                    }
                }
            } 
            break;
        default:
            if(gaps_set) {
                delete [] this->gaps;
                this->gaps = 0;
            }
            throw gap::GapException1("Invalid type");
    }
}



void draw::set_gap_val(int index, int val)
{

    if(index < 0 || index > NGAPS-1) 
        throw gap::GapException1("index < 0 || index > NGAPS-1");
    else if(val < 1 || val > MAX_LOT_RANGE) // -4??? 
        throw gap::GapException1("val < 1 || val > MAX_LOT_RANGE");

    draw d = *this;
    if(!this->gaps) {
        d.gaps = new gap[NGAPS];
        for(int i = 0; i < NGAPS; i++)
            memset(&d.gaps[i], 0, sizeof(d.gaps[i]));
        d.gaps[index].type = this->gaps[index].type;
    }
    
    d.gaps[index].val = val;
    
    
    if(!d.valid()) 
        throw gap::GapException1("!d.valid()");

    /* The tests all worked so we can now set the gap value. */
    this->gaps[index].val = val;

}
    

void draw::set_gap(int index, int type, int val)
{
    if(index < 0 || index > NGAPS-1) 
        throw draw::DrawException1("set_gap(int index, int type, int val): index < 0 || index > NGAPS-1");
    
    int prev_type = 0, prev_val = 0;
    if(this->gaps) {
        prev_type = this->gaps[index].type;
        prev_val = this->gaps[index].val;
    }

    try {
        this->set_gap_type(index, type);
        this->set_gap_val(index, val);
    } catch(gap::GapException1 &ge1) {
        if(this->gaps) {
            this->gaps[index].type = prev_type;
            this->gaps[index].val = prev_val;
        }
        throw ge1;
    }
}



bool draw::valid() 
{

    std::cout << "<*~*~{+/-}~- SCRAMBLING BONERS -~{+/-}*~*~> ~ " << this->valid_range() << std::endl;
    if((this->rng_ff.lower || this->rng_ff.upper) && !this->valid_range()) 
        return false;
    else if((this->rng_sp.lower || this->rng_sp.upper) && !this->valid_sp_range()) 
        return false;
    else if(!this->validate()) 
        return false;
    else if(!this->valid_gaps() || !this->check_gaps(NBALLS-1)) {
        std::cout << "SCRAMBLING VA-JAY-JAYS~ {" << this->valid_gaps() << "} " << *this << std::endl;
        show_constraints();
        //squishing_boners();
        return false;
    } //else if(!this->check_gaps(NBALLS-1)) {
        //std::cout << "RAMBLING JUNKIES~" << std::endl;
        //squishing_boners();
        //return false;
    //}

    // TODO: need to check this if range or gap values changed...
    else if((this->rng_ff.lower || this->rng_ff.upper) && 
        !valid_rng_gap_change(this->rng_ff.lower, this->rng_ff.upper)) {
            std::cout << "SCRAMBLING JUNKIES BIG TIMES~" << std::endl;
            squishing_boners();
    }
    
    if(this->ball_const) {
        draw d;
        for(int i = 0; i < NBALLS-1; i++) 
            d[i] = this->ball_const[i];
        std::cout << "SPLOOGING JABRONIES~ " << d << ", " << d.validate() << std::endl;
        if(!d.validate()) {
            std::cout << "SCRAMBLING BONERS BIG TIMES~" << std::endl;
            return false;
        }

        draw e;
        for(int i = 0; i < NBALLS-1; i++)
            e[i] = this->ball_const[i];
        if(this->gaps) {
            e.gaps = new gap[NGAPS];
            for(int i = 0; i < NGAPS; i++) {
                e.gaps[i] = this->gaps[i];
            }
        }
        std::cout << "SCRAMBLING JUNKIES: " << e << ", " << e.check_gaps(NBALLS-1) << " ~ " << std::endl;
        if(!e.check_gaps(NBALLS-1)) {
            std::cout << "SCRAMBLING VA-JAY-JAYS BIG TIMES~" << std::endl;
            return false;
        }
    }

    std::cout << "GRANDMA'S GETTIN' A BIG ON' BROWN WHALE TONIGHT ;-> "  << *this << std::endl;
        
    
    
    
    /*
    //else if(!this->valid_ball_consts()) {
        std::cout << "SCRAMBLING JUNKIES~" << std::endl;
        //squishing_boners();
      //  return false;
    } else if(!this->check_gaps(NBALLS-1)) {
        std::cout << "RAMBLING JUNKIES~" << std::endl;
        //squishing_boners();
        return false;
    } else if(!this->check_gaps_bc()) {
        std::cout << "RAMBLING BONERS~" << std::endl;
        //squishing_boners();
        return false;
    }
    // else if gaps, ball const and draw doesn't satisfy constraint(s), return false
    */

    std::cout << "UNSCRAMBLING VAGINERS~" << std::endl;
    //squishing_boners();

    //return false;
    return true;
}

/*
bool draw::valid() 
{

    int prev_val = 0, cur_val = 0, prev_bc_val = 0, cur_bc_val = 0, prev_index = -1, prev_bc_index = -1;
    int gval = 0, gval_bc = 0;
    for(int i = 0; i < NBALLS-1; i++) {

        if(this->balls[i]) {

            cur_val = this->balls[i];
            gval = this->balls[i]; 

            if(prev_val && prev_val >= cur_val) 
                return false;

            if(prev_index > -1) {
                int vgap = cur_val - prev_val;
                int igap = i - prev_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                exit(9);
                if(igap > vgap) {
                    while(1) std::cout << "GAPING GRANNIES~";
                }
                //squishing_boners();;
            }


            if(this->gaps && i < NBALLS-1-1 && this->balls[i+1]) {
                while(1) {
                    std::cout << "SPLOOGING JABRONIES [" << i << "] " << *this << ", " << prev_index << " {@;@}~> ";
                    exit(9);
                }
                / ???
                switch(this->gaps[i].type) {
                case GT_NOTSET:
                    std::cout << "DROOPING DILDOS 1" << std::endl;
                    squishing_boners();;
                    break;
                case GT_LT:
                    std::cout << "DROOPING DILDOS 2" << std::endl;
                    squishing_boners();;
                    break;
                case GT_LTE:
                    std::cout << "DROOPING DILDOS 2.5" << std::endl;
                    //squishing_boners();;
                    if(this->balls[i+1] - this->balls[i] > this->gaps[i].val) {
                        std::cout << "GURGLING NANNIES" << std::endl;
                        squishing_boners();;
                    }
                    break;
                case GT_EQ:
                    std::cout << "DROOPING DILDOS 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "DROOPING DILDOS 4" << std::endl;
                    squishing_boners();;
                    
                    gval += this->gaps[i].val;
                    break;
                case GT_GT:
                    std::cout << "DROOPING DILDOS 5" << std::endl;
                    squishing_boners();;
                }
                std::cout << "DROOPING VAGINERS I WAS~ " << *this << std::endl;
                //squishing_boners();;
                /

            
            }

            prev_index = i;
            prev_val = cur_val;

        } else if(this->gaps && i < NGAPS) {
            std::cout << "FISTING GRANNIES[" << i << "] " << gval << std::endl;
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                //case GT_LT:
                  //  std::cout << "SWAPPING TRANNIES 1" << std::endl;
                    //squishing_boners();;
                    //break;
                //case GT_LTE:
                  //  std::cout << "SWAPPING TRANNIES 2" << std::endl;
                    //squishing_boners();;
                    //break;
                case GT_EQ:
                    std::cout << "SWAPPING TRANNIES 3" << std::endl;
                    squishing_boners();
                    break;
                case GT_GTE:
                    
                    
                    gval += this->gaps[i].val;
                    std::cout << "SWAPPING TRANNIES 4: " << gval << std::endl;
                    //squishing_boners();
                    break;
                case GT_GT:
                    std::cout << "SWAPPING TRANNIES 5" << std::endl;
                    squishing_boners();
                    break;
                default:
                    gval++;
                
            }
            std::cout << "PUNCHING BONERS: " << gval << std::endl;
        } else { // ??
            gval++; // ??
        }
        
        //std::cout << "SPLOOGING JABRONIES~ " << cur_val << ", " <<
          //          prev_val << std::endl;

        if(this->ball_const && this->ball_const[i]) {
            std::cout << "HUGE WINKING VA-JAY-JAYS[" << i << "]~ " << this->ball_const[i] << std::endl;
            //squishing_boners();;
            cur_bc_val = this->ball_const[i];
            gval_bc = this->ball_const[i]; // ???
            if(prev_bc_val && prev_bc_val >= cur_bc_val) {
                while(1) std::cout << "HUGE STINKING VA-JAY-JAYS ;~) " << cur_bc_val << ", " <<
                    prev_bc_val << " {@;@}~> ";
            }

            if(prev_bc_index > -1) {
                int vgap = cur_bc_val - prev_bc_val;
                int igap = i - prev_bc_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "GAPING NANNIES~";
                }
                //squishing_boners();;
            }

            if(this->gaps && i < NBALLS-1-1 && this->ball_const[i+1]) {
                this->show_constraints();
                
                //squishing_boners();
                std::cout << "ņņņ GRUNTING GRANNIES I WAS~ " << *this << std::endl;
                //char frumphoused = 'ņ';
                //printf("JUGGLING BONERS: '%c'", frumphoused);
                //exit(9);
                //squishing_boners();;
                switch(this->gaps[i].type) {
                case GT_NOTSET:
                    std::cout << "DROPPING DILDOS 1" << std::endl;
                    squishing_boners();;
                    break;
                case GT_LT:
                    std::cout << "DROPPING DILDOS 2" << std::endl;
                    squishing_boners();;
                    break;
                case GT_LTE:
                    std::cout << "DROPPING DILDOS 2.5" << std::endl;
                    //squishing_boners();;
                    if(this->ball_const[i+1] - this->ball_const[i] > this->gaps[i].val) {
                        std::cout << "GURGLING JUNKIES" << std::endl;
                        squishing_boners();;
                    }
                    break;
                case GT_EQ:
                    std::cout << "DROPPING DILDOS 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "DROPPING DILDOS 4" << std::endl;
                    squishing_boners();;
                    
                    gval_bc += this->gaps[i].val;
                    break;
                case GT_GT:
                    std::cout << "DROPPING DILDOS 5" << std::endl;
                    squishing_boners();;
                }
                std::cout << "GRUNTING VAGINERS I WAS~ " << *this << std::endl;
                //squishing_boners();;
            }


            prev_bc_index = i;
            prev_bc_val = cur_bc_val;
        } else if(this->gaps && i < NGAPS) {
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                //case GT_LT:
                    //std::cout << "SWAPPING BONERS 1" << std::endl;
                    //squishing_boners();;
                    //break;
                //case GT_LTE:
                    //std::cout << "SWAPPING BONERS 2" << std::endl;
                    //squishing_boners();;
                    //break;
                case GT_EQ:
                    std::cout << "SWAPPING BONERS 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "SWAPPING BONERS 4" << std::endl;
                    //squishing_boners();;
                    
                    gval_bc += this->gaps[i].val;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING BONERS 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval_bc++;
                
            }
            std::cout << "PUNCHING VAGINERS: " << gval_bc << std::endl;
        } else { // ??
            gval_bc++; // ??
        }
        
        

        
    }

    //std::cout << "GURGLING GRANNIES(" << cur_val << ")=> " << cur_bc_val << std::endl;
    //std::cout << "THAT OLD GRANNY TOOK IT RIGHT IN THE POOP SHOOT SHE DID(" << gval << ")=> " << gval_bc << std::endl;
    if(this->gaps && this->gaps[3].type) {
        std::cout << "CORNPOP WAS UNFOLDING BONERS~" << std::endl;
        //squishing_boners();;
    }

    / Make sure the "least" possible run did exist, otherwise
       report the error. /
    if(this->rng_ff.upper && (gval > this->rng_ff.upper || gval_bc > this->rng_ff.upper)) {
        //if(gaps_set) {
          //  delete [] this->gaps;
            //this->gaps = 0;
        //}
        this->show_ball_const();
        this->show_gaps();
        std::cout << "TABLING BONERS~ " << *this << " [x;x]++> (" << gval << ", " << gval_bc << "), " << this->rng_ff.upper << "- ";
        //squishing_boners();;
        //throw gap::GapException1("draw_sum > this->rng_ff.upper");
        return false;
    }
    
    return !false; //valid_constraints();




}



bool draw::valid() 
{
    this->show_gaps();
    this->show_ball_const();
    std::cout << "_____________" << std::endl;    
    std::cout << "      ^       " << std::endl;           
        std::cout << "    /_ _\\    " << std::endl;
        std::cout << "     {<}      " << std::endl;
        //std::cout << "     |||      " << std::endl;
        //std::cout << "     | |      " << std::endl;
        std::cout << "    {@(@}   " << std::endl;
        //std::cout << "      X       " << std::endl;
        std::cout << "______X______" << std::endl;    
        std::cout << "SCRUNCHING BONERS~ " << *this << std::endl;
    //squishing_boners();;
    usleep(105000);
    //sleep(1);
    

    int prev_val = 0, cur_val = 0, prev_bc_val = 0, cur_bc_val = 0, prev_index = -1, prev_bc_index = -1;
    int gval = 0, gval_bc = 0;
    for(int i = 0; i < NBALLS-1; i++) {

        std::cout << "TWISTING GRANNIES[" << i << "] " << std::endl;
        if(this->balls[i]) {
            std::cout << "HUGE WINKING JUNKIES[" << i << "]~ " << this->balls[i] << std::endl;
            //squishing_boners();;
            cur_val = this->balls[i];

            gval = this->balls[i]; // ???

            if(prev_val && prev_val >= cur_val) {
                while(1) std::cout << "HUGE WINKING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> ";
            }

            if(prev_index > -1) {
                int vgap = cur_val - prev_val;
                int igap = i - prev_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "GAPING GILBERT~";
                }
                //squishing_boners();;
            }

            if(i < NBALLS-1-1 && this->balls[i+1]) {
                while(1) std::cout << "SPLOOGING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> ";
            }

            prev_index = i;
            prev_val = cur_val;

        } else if(this->gaps && i < NGAPS) {
            std::cout << "FISTING GRANNIES[" << i << "] " << gval << std::endl;
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                //case GT_LT:
                  //  std::cout << "SWAPPING TRANNIES 1" << std::endl;
                    //squishing_boners();;
                    //break;
                //case GT_LTE:
                  //  std::cout << "SWAPPING TRANNIES 2" << std::endl;
                    //squishing_boners();;
                    //break;
                case GT_EQ:
                    std::cout << "SWAPPING TRANNIES 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    
                    
                    gval += this->gaps[i].val;
                    std::cout << "SWAPPING TRANNIES 4: " << gval << std::endl;
                    //squishing_boners();;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING TRANNIES 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval++;
                
            }
            std::cout << "PUNCHING BONERS: " << gval << std::endl;
        } else { // ??
            gval++; // ??
        }
        std::cout << "SPLOOGING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> " << std::endl;

        if(this->ball_const && this->ball_const[i]) {
            std::cout << "HUGE STINKING JUNKIES[" << i << "]~ " << this->ball_const[i] << std::endl;
            //squishing_boners();;
            cur_bc_val = this->ball_const[i];
            gval_bc = this->ball_const[i]; // ???
            if(prev_bc_val && prev_bc_val >= cur_bc_val) {
                while(1) std::cout << "HUGE STINKING JABRONIES ;~) " << cur_bc_val << ", " <<
                    prev_bc_val << " {@;@}~> ";
            }

            if(prev_bc_index > -1) {
                int vgap = cur_bc_val - prev_bc_val;
                int igap = i - prev_bc_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "GAPING BUSEY~";
                }
                //squishing_boners();;
            }

            if(i < NBALLS-1-1 && this->ball_const[i+1]) {
                int frumpy = 0;
                int wow = GET_RAND_NUM(3, 10);
                while(1) {
                    //std::cout << "SHNOODLING JABRONIES ;~) " << cur_bc_val << ", " <<
                    //prev_bc_val << " {@;@}~> ";
                    //std::cout << "_____________" << std::endl;  
                    if(frumpy == 0) {
           
            std::cout << std::endl << "~-- SCRUNCHING BONERS --~";
            std::cout << std::endl; 
            
             
        }  
                std::cout << "      ^          ^" << std::endl;           
                std::cout << "    /_ _\\      /_ _\\    " << std::endl;
                std::cout << "     {<}        {<}" << std::endl;
                //std::cout << "     |||      " << std::endl;
                for(int i = 0; i < (frumpy); i++) {
                    std::cout << "     |||        |||"<< std::endl;
                }
        
        //std::cout << "     | |      " << std::endl;
        std::cout << "    {@(@}      {@)@}" << std::endl;
        std::cout << "      X          X" << std::endl;
        //std::cout << "______X______"; // << std::endl; 
        if(!frumpy)
        usleep(300000); 
        frumpy++;
        
        if(frumpy == wow) {
            frumpy = 0;
            wow = GET_RAND_NUM(3, 10);
            
            //std::cout << "SCRUNCHING WEINERS~"; // << std::endl; 
        } //else //{
        
        //usleep(1000000); 
            //std::cout << std::endl; 
            
        //}
            usleep(100000); 
            //sleep(2);
            //squishing_boners();;
                }
            }


            prev_bc_index = i;
            prev_bc_val = cur_bc_val;
        } else if(this->gaps && i < NGAPS) {
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                //case GT_LT:
                    //std::cout << "SWAPPING BONERS 1" << std::endl;
                    //squishing_boners();;
                    //break;
                //case GT_LTE:
                    //std::cout << "SWAPPING BONERS 2" << std::endl;
                    //squishing_boners();;
                    //break;
                case GT_EQ:
                    std::cout << "SWAPPING BONERS 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "SWAPPING BONERS 4" << std::endl;
                    //squishing_boners();;
                    
                    gval_bc += this->gaps[i].val;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING BONERS 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval_bc++;
                
            }
            std::cout << "PUNCHING VAGINERS: " << gval_bc << std::endl;
        } else { // ??
            gval_bc++; // ??
        }
        
        

        
    }

    std::cout << "GURGLING GRANNIES(" << cur_val << ")=> " << cur_bc_val << std::endl;
    std::cout << "THAT OLD GRANNY TOOK IT RIGHT IN THE POOP SHOOT SHE DID(" << gval << ")=> " << gval_bc << std::endl;
    if(this->gaps && this->gaps[3].type) {
        std::cout << "CORNPOP WAS UNFOLDING BONERS~" << std::endl;
        //squishing_boners();;
    }

    / Make sure the "least" possible run did exist, otherwise
       report the error. /
    if(this->rng_ff.upper && (gval > this->rng_ff.upper || gval_bc > this->rng_ff.upper)) {
        //if(gaps_set) {
          //  delete [] this->gaps;
            //this->gaps = 0;
        //}
        std::cout << "TABLING BONERS~ (" << gval << ", " << gval_bc << "), " << this->rng_ff.upper << "- ";
        //squishing_boners();;
        throw gap::GapException1("draw_sum > this->rng_ff.upper");
    }
    
    return !false; //valid_constraints();




}


bool draw::valid() //_constraints()
{
    this->show_gaps();
    this->show_ball_const();
    std::cout << "_____________" << std::endl;    
    std::cout << "      ^       " << std::endl;           
        std::cout << "    /_ _\\    " << std::endl;
        std::cout << "     {<}      " << std::endl;
        //std::cout << "     |||      " << std::endl;
        //std::cout << "     | |      " << std::endl;
        std::cout << "    {@(@}   " << std::endl;
        //std::cout << "      X       " << std::endl;
        std::cout << "______X______" << std::endl;    
        std::cout << "SCRUNCHING BONERS~ " << *this << std::endl;
    //squishing_boners();;
    //usleep(15000);
    

    int prev_val = 0, cur_val = 0, prev_bc_val = 0, cur_bc_val = 0, prev_index = -1, prev_bc_index = -1;
    int gval = 0, gval_bc = 0;
    for(int i = 0; i < NBALLS-1; i++) {

        std::cout << "TWISTING GRANNIES[" << i << "] " << std::endl;
        if(this->balls[i]) {
            std::cout << "HUGE WINKING JUNKIES[" << i << "]~ " << this->balls[i] << std::endl;
            //squishing_boners();;
            cur_val = this->balls[i];

            gval = this->balls[i]; // ???

            if(prev_val && prev_val >= cur_val) {
                while(1) std::cout << "HUGE WINKING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> ";
            }

            if(prev_index > -1) {
                int vgap = cur_val - prev_val;
                int igap = i - prev_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "GAPING GILBERT~";
                }
                //squishing_boners();;
            }

            if(i < NBALLS-1-1 && this->balls[i+1]) {
                while(1) std::cout << "SPLOOGING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> ";
            }

            prev_index = i;
            prev_val = cur_val;

        } else if(this->gaps && i < NGAPS) {
            std::cout << "FISTING GRANNIES[" << i << "] " << gval << std::endl;
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                //case GT_LT:
                  //  std::cout << "SWAPPING TRANNIES 1" << std::endl;
                    //squishing_boners();;
                    //break;
                //case GT_LTE:
                  //  std::cout << "SWAPPING TRANNIES 2" << std::endl;
                    //squishing_boners();;
                    //break;
                case GT_EQ:
                    std::cout << "SWAPPING TRANNIES 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    
                    
                    gval += this->gaps[i].val;
                    std::cout << "SWAPPING TRANNIES 4: " << gval << std::endl;
                    //squishing_boners();;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING TRANNIES 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval++;
                
            }
            std::cout << "PUNCHING BONERS: " << gval << std::endl;
        } else { // ??
            gval++; // ??
        }
        std::cout << "SPLOOGING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> " << std::endl;

        if(this->ball_const && this->ball_const[i]) {
            std::cout << "HUGE STINKING JUNKIES[" << i << "]~ " << this->ball_const[i] << std::endl;
            //squishing_boners();;
            cur_bc_val = this->ball_const[i];
            gval_bc = this->ball_const[i]; // ???
            if(prev_bc_val && prev_bc_val >= cur_bc_val) {
                while(1) std::cout << "HUGE STINKING JABRONIES ;~) " << cur_bc_val << ", " <<
                    prev_bc_val << " {@;@}~> ";
            }

            if(prev_bc_index > -1) {
                int vgap = cur_bc_val - prev_bc_val;
                int igap = i - prev_bc_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "GAPING BUSEY~";
                }
                //squishing_boners();;
            }

            if(i < NBALLS-1-1 && this->ball_const[i+1]) {
                while(1) std::cout << "SHNOODLING JABRONIES ;~) " << cur_bc_val << ", " <<
                    prev_bc_val << " {@;@}~> ";
            }


            prev_bc_index = i;
            prev_bc_val = cur_bc_val;
        } else if(this->gaps && i < NGAPS) {
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                //case GT_LT:
                    //std::cout << "SWAPPING BONERS 1" << std::endl;
                    //squishing_boners();;
                    //break;
                //case GT_LTE:
                    //std::cout << "SWAPPING BONERS 2" << std::endl;
                    //squishing_boners();;
                    //break;
                case GT_EQ:
                    std::cout << "SWAPPING BONERS 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "SWAPPING BONERS 4" << std::endl;
                    //squishing_boners();;
                    
                    gval_bc += this->gaps[i].val;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING BONERS 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval_bc++;
                
            }
            std::cout << "PUNCHING VAGINERS: " << gval_bc << std::endl;
        } else { // ??
            gval_bc++; // ??
        }
        
        

        
    }

    std::cout << "GURGLING GRANNIES(" << cur_val << ")=> " << cur_bc_val << std::endl;
    std::cout << "THAT OLD GRANNY TOOK IT RIGHT IN THE POOP SHOOT SHE DID(" << gval << ")=> " << gval_bc << std::endl;
    if(this->gaps && this->gaps[3].type) {
        std::cout << "CORNPOP WAS UNFOLDING BONERS~" << std::endl;
        //squishing_boners();;
    }

    / Make sure the "least" possible run did exist, otherwise
       report the error. /
    if(this->rng_ff.upper && (gval > this->rng_ff.upper || gval_bc > this->rng_ff.upper)) {
        //if(gaps_set) {
          //  delete [] this->gaps;
            //this->gaps = 0;
        //}
        std::cout << "TABLING BONERS~ (" << gval << ", " << gval_bc << "), " << this->rng_ff.upper << "- ";
        //squishing_boners();;
        throw gap::GapException1("draw_sum > this->rng_ff.upper");
    }
    
    return !false; //valid_constraints();




}
*/

        
/*        
        if(this->ball_const && this->ball_const[i]) {
            std::cout << "HUGE STINKING VA-JAY-JAYS[" << i << "(" << prev_bc_val << ")]~ " << this->ball_const[i] << std::endl;
            //squishing_boners();;
            cur_bc_val = this->ball_const[i];
            if(prev_bc_val && prev_bc_val >= cur_bc_val) {
                while(1) std::cout << "HUGE WINKING BONERS ;~) " << cur_bc_val << ", " <<
                    prev_bc_val << " {@;@}~> ";
                

            }
            

            if(prev_bc_index > -1) {
                std::cout << "CLICKING BONERS 11" << std::endl;
                //squishing_boners();;
                int vgap = cur_bc_val - prev_bc_val;
                int igap = i - prev_bc_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "CLICKING VA-JAY-JAYS~";
                }
            }


            prev_bc_index = i;
            prev_bc_val = cur_bc_val;
        } else if(this->gaps && i < NGAPS) {
            std::cout << "SWAPPING BONERS[" << i << "] " << this->gaps[i] << std::endl;
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
            
                case GT_LT:
                    std::cout << "SWAPPING GRANNIES 1" << std::endl;
                    squishing_boners();;
                    break;
                case GT_LTE:
                    std::cout << "SWAPPING GRANNIES 2" << std::endl;
                    squishing_boners();;
                    break;
                case GT_EQ:
                    std::cout << "SWAPPING GRANNIES 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "SWAPPING GRANNIES 4" << std::endl;
                    //squishing_boners();;
                    if(this->ball_const[i] && this->ball_const[i+1]) {
                    //if(g.type == GT_LT) {
                        if( (this->ball_const[i+1] - this->ball_const[i]) >= this->gaps[i].val ) {
                            //if(gaps_set) {
                              //  delete [] this->gaps;
                                //this->gaps = 0;
                            //}
                            while(3) std::cout << "CHUCKLING VAGINERS~ ";
                            //throw gap::GapException1("VIOLATING NANNIES 1~");
                        }
                    //} else if((g.type == GT_LTE) && (this->balls[i+1] - this->balls[i]) > g.val) {
                      //  if(gaps_set) {
                        //    delete [] this->gaps;
                          //  this->gaps = 0;
                        //}
                        //throw gap::GapException1("VIOLATING GRANNIES 2~");
                    }
                    gval += this->gaps[i].val;
                    std::cout << "SWAPPING GRANNIES 44" << std::endl;
                    //squishing_boners();;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING GRANNIES 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval++;
                
            }
            std::cout << "~-~ TERP SLURPING GRANNIES ~-~ " << gval << std::endl;
            //squishing_boners();;
            
        }

        if(this->balls[i]) {
            std::cout << "HUGE WINKING JUNKIES[" << i << "]~ " << this->balls[i] << std::endl;
            //squishing_boners();;
            cur_val = this->balls[i];
            if(prev_val && prev_val >= cur_val) {
                while(1) std::cout << "HUGE WINKING JABRONIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> ";
            }

            if(prev_index > -1) {
                int vgap = cur_val - prev_val;
                int igap = i - prev_index;
                std::cout << "CLICKING BONERS TOO: " << vgap << ", " << igap << std::endl;
                if(igap > vgap) {
                    while(1) std::cout << "GAPING JABRONIES~";
                }
                //squishing_boners();;
            }

            if(i < NBALLS-1-1 && this->balls[i+1]) {

            }


            prev_index = i;
            prev_val = cur_val;
        } else if(this->gaps && i < NGAPS) {
            switch(this->gaps[i].type) {
                //case GT_NOTSET:
                case GT_LT:
                    std::cout << "SWAPPING TRANNIES 1" << std::endl;
                    squishing_boners();;
                    break;
                case GT_LTE:
                    std::cout << "SWAPPING TRANNIES 2" << std::endl;
                    squishing_boners();;
                    break;
                case GT_EQ:
                    std::cout << "SWAPPING TRANNIES 3" << std::endl;
                    squishing_boners();;
                    break;
                case GT_GTE:
                    std::cout << "SWAPPING TRANNIES 4" << std::endl;
                    squishing_boners();;
                    if(this->balls[i] && this->balls[i+1]) {
                    //if(g.type == GT_LT) {
                        if( (this->balls[i+1] - this->balls[i]) >= this->gaps[i].val ) {
                            //if(gaps_set) {
                              //  delete [] this->gaps;
                                //this->gaps = 0;
                            //}
                            while(3) std::cout << "CHUCKLING BONERS~ ";
                            //throw gap::GapException1("VIOLATING NANNIES 1~");
                        }
                    //} else if((g.type == GT_LTE) && (this->balls[i+1] - this->balls[i]) > g.val) {
                      //  if(gaps_set) {
                        //    delete [] this->gaps;
                          //  this->gaps = 0;
                        //}
                        //throw gap::GapException1("VIOLATING GRANNIES 2~");
                    }
                    gval += this->gaps[i].val;
                    break;
                case GT_GT:
                    std::cout << "SWAPPING TRANNIES 5" << std::endl;
                    squishing_boners();;
                    break;
                default:
                    gval++;
                
            }

            std::cout << "~-~ TERP SLURPING NANNIES ~-~ " << gval_bc << std::endl;
            //squishing_boners();;
            
        }

        
        //prev_bc_val = cur_bc_val;
        //prev_val = cur_val;


        


    }

    if(this->gaps) { //} && this->gaps[i].type) {
        std::cout << "HUGE WINKING GRANNIES~ " << this->gaps << std::endl;
        //squishing_boners();;

        for(int i = 0; i < NGAPS; i++) {
            switch(this->gaps[i].type) {
                //case 
            }
        }
        std::cout << "HUGE STINKING GRANNIES~ " << this->gaps << std::endl;
        squishing_boners();;
    }

    std::cout << "HUGE STINKING JUNKIES~ " << *this << std::endl;
    return true; //valid_constraints();




}
*/

/*
// TODO: this needs to be redone (too much before valid test...)
void draw::set_ball_const(int index, int val)
{
    if(index < 0 || index > NBALLS-1) 
        //throw gap::GapException1("index < 0 || index > NGAPS-1");
        while(1) std::cout << "GRINDING GRANNIES 99~ ";
    else if(val < 1 || val > MAX_LOT_RANGE) {
        while(1) std::cout << "GRINDING GRANNIES -99~ ";
    } 


    

    std::cout << "OVULATING GRANNIES BIG TIME" << std::endl;
    squishing_boners();;
    
   // bool fc_set = false;
    if(!this->ball_const) {
        fc_set = true;
        std::cout << "GRINDING GRANNIES 1~ " << std::endl;
        this->ball_const = new int[NBALLS];
        for(int i = 0; i < NBALLS; i++)
            memset(&this->ball_const[i], 0, sizeof(this->ball_const[i]));
    }

    
    if(index < NBALLS-1) {

        //   { A, B, C, D, E, F }
        // LB:  { r1l, r1l+1, r1l+2, r1l+3, r2l }
        // UB:  { r1u, r1u-1, r1u-2, r1u-3, r2u }

        if(!this->valid_range()) {

            if(fc_set) {
                delete [] this->ball_const;
                this->ball_const = 0;
            }
            while(1) std::cout << "GRINDING GRANNIES 999~ ";

        }
        //} else if(this->ball_const) {
            if( (val + ((NBALLS-2)-index)) > this->rng_ff.upper) {
            
                if(fc_set) {
                    delete [] this->ball_const;
                    this->ball_const = 0;
                }
                while(1) std::cout << "DOINKING GRANNIES 99~ "; // << *this ;
 
            } else if((val - index) < this->rng_ff.lower) {
                if(fc_set) {
                    delete [] this->ball_const;
                    this->ball_const = 0;
                }
                while(1) std::cout << "DOINKING GRANNIES QUIVE~ (" << 
                val << ", " << index << ", (" << (val - (index)) << ")), " <<
                     *this;
            }

            std::cout << "GRINDING GRANNIES 2999~ (" << 
                val << ", " << index << ", (" << (val - (index)) << ")), " <<
                     *this << std::endl;
        //if( (val + ((NBALLS-2)-index)) > this->rng_ff.upper) {
            //while(1) std::cout << "DOINKING GRANNIES 99~ "; // << *this ;
        //}


            std::cout << "COME ON GRANDPA, PUT THE COCK RINGS AWAY!!!" << std::endl;
            //squishing_boners();;
            int prev_ball_val = 0;
            for(int i = 0; i < NBALLS-1; i++) {
                if(this->ball_const[i]) {
                    if(prev_ball_val && this->ball_const[i] <= prev_ball_val) {
                        if(fc_set) {
                            delete [] this->ball_const;
                            this->ball_const = 0;
                        }
                        std::cout << "SLURPING NANNIES~ ";
                        squishing_boners();;
                    }


                    prev_ball_val = this->ball_const[i];
                }

            }
        //}
    } else {
        if(!this->valid_sp_range()) {
            if(fc_set) {
                delete [] this->ball_const;
                this->ball_const = 0;
            }
            while(1) std::cout << "GRINDING GRANNIES 9999~ ";
        } else if(val < this->rng_sp.lower || val > this->rng_sp.upper) {
            if(fc_set) {
                delete [] this->ball_const;
                this->ball_const = 0;
            }
            while(1) std::cout << "SLURPING GRANNIES~ ";
        }

        std::cout << "SLURPING JUNKIES~ " << val << std::endl;
    }
    
    std::cout << "PROLAPSING JABRONIES~" << std::endl;

    / *
    if(this->gaps) {
        
        int draw_val = 0;
        draw d(this->rng_ff, this->rng_sp);
        for(int i = 0; i < NBALLS-1; i++) {
            std::cout << "FISTING GRANNIES~ [" << i << "]" << std::endl;
            
            if(i == index) {
                std::cout << "TWISTING GRANNIES~" << std::endl;
                draw_val = val;
            } else {
                if(this->ball_const[i]) {
                    std::cout << "FISTING JABRONIES NAMES DANNY~ " << this->ball_const[i] << std::endl;
                    draw_val = val;
                } else {
                    std::cout << "PROLAPSING JABRONIES~ [" << i << "]" << std::endl;;
                }
            }

            if(i < NGAPS) { //} && this->gaps[i].type) {

                //std::cout << "PROLAPSING VA-JAY-JAYS~" << std::endl;
                switch(this->gaps[i].type) {
                    case GT_LT:
                        std::cout << "COLLAPSING VAGINERS~" << std::endl;
                        squishing_boners();;
                        break;
                    // ...
                    default:
                        break;


                }
                
                draw_val += this->gaps[i].type;
            }
            
        }

    }
    */
   
void draw::set_ball_const(int index, int val)
{
    if(index < 0 || index > NBALLS-1) 
        //throw gap::GapException1("index < 0 || index > NGAPS-1");
        while(1) std::cout << "UNGRINDING GRANNIES 99~ ";
    else if(val < 1 || val > MAX_LOT_RANGE) {
        while(1) std::cout << "UNGRINDING GRANNIES -99~ ";
    } 


    if(index < NBALLS-1) {
        //if(val > this->rng_ff.upper) 
        if(!this->rng_ff.lower || !this->rng_ff.upper) 
             throw DrawException1("!this->rng_ff.lower || !this->rng_ff.upper");
        else if( (val + ((NBALLS-2)-index)) > this->rng_ff.upper) {
            //throw InvalidRangeException("draw::set_ball: Invalid range 2");
            while(1) std::cout << "OVULATING JABRONIES WON too# ";
        } else if((val - index) < this->rng_ff.lower) {
            while(1) std::cout << "OVULATING JABRONIES WON+WON too#";
        }
    } else {
        if(!this->rng_sp.lower || !this->rng_sp.upper) 
             throw DrawException1("!this->rng_sp.lower || !this->rng_sp.upper");
        else if(val < this->rng_sp.lower || val > this->rng_sp.upper) 
            throw InvalidRangeException("draw::set_ball_const: Invalid range 3");
    }


    

    std::cout << "OVULATING GRANNIES BIG TIME" << std::endl;
    //squishing_boners();;


    std::cout << "SCRAMBLING VA-JAY-JAYS 1" << std::endl;
    //squishing_boners();;
    draw d = *this;
    
    if(!d.ball_const) {
        //fc_set = true;
        std::cout << "SCRAMBLING GRANNIES 1~ " << std::endl;
        d.ball_const = new int[NBALLS];
        for(int i = 0; i < NBALLS; i++)
            memset(&d.ball_const[i], 0, sizeof(d.ball_const[i]));
    }
    

    //draw e = *this;
    d.show_gaps();
    d.show_ball_const();
    std::cout << "SCRAMBLING TRANNIES: " << d << std::endl;
    //squishing_boners();;
    //d.ball_const = new int[NBALLS];
   //memset(d.ball_const, 0, sizeof(*d.ball_const));
    
    /*
    for(int i = 0; i < NBALLS; i++) {
        std::cout << "SCRAMBLING BONERS BIG TIME[" << i << "] " << index << std::endl;
        //memset(&d.ball_const[i], 0, sizeof(d.ball_const[i]));
        if(i != index) {
            std::cout << "KNOCKING BONERS: "<< std::endl;
            if(this->ball_const) {
                std::cout << "KNOCKING WEINERS: "<< this->ball_const[i] << std::endl;
                d.ball_const[i] = this->ball_const[i];
            }
        } else {
            std::cout << "ROCKING BONERS"<< std::endl;
            d.ball_const[i] = val;
        }
    }
    */

   
    
    
    //memset(&d.ball_const[index], 0, sizeof(d.ball_const[index]));
    d.ball_const[index] = val;
    d.show_gaps();
    d.show_ball_const();
    std::cout << "OVULATING JABRONIES TOO: " << d << std::endl;
    //squishing_boners();;
    
    if(!d.valid()) { //_constraints()) {
        std::cout << "OVULATING JABRONIES BIG TIME" << std::endl;
        //squishing_boners();;
        throw DrawException1("OVULATING JABRONIES BIG TIME");
    }


    std::cout << "~~~~COLLAPSING VA-JAY-JAYS~ "; // << std::endl;
    //squishing_boners();;

    
    //this->balls[index] = this->ball_const[index] = val;
    if(!this->ball_const) {
        //fc_set = true;
        std::cout << "SCRAMBLING GRANNIES 2~ " << std::endl;
        //squishing_boners();
        this->ball_const = new int[NBALLS];
        for(int i = 0; i < NBALLS; i++)
            memset(&this->ball_const[i], 0, sizeof(this->ball_const[i]));
    }

    this->ball_const[index] = val;


}

void draw::clear_ball_const(int index)
{
    if(index < 0 || index > NBALLS-1) 
        //throw gap::GapException1("index < 0 || index > NGAPS-1");
        while(1) std::cout << "GRINDING BONERS 99~ ";
    else if(!this->ball_const) 
        while(1) std::cout << "GRINDING BONERS 2~ ";

}

void draw::clear_ball_consts()
{
    if(!this->ball_const) 
        while(1) std::cout << "GRINDING BONERS 22~ ";

}

void draw::show_ball_const()
{
    std::cout << "~!-~@- Draw Ball Constants -@~-!~ " << std::endl; // this->gaps << std::endl;
    if(!this->ball_const) {
        std::cout << "Ball Constants are NOT SET." << std::endl;
        return;
    }
    for(int i = 0; i < NBALLS; i++) {
        std::cout << "~BCONST[" << i << "] ";// << std::endl;; // << *this->gaps[i] << std::endl;
        if(this->ball_const[i])
            std::cout << this->ball_const[i] << std::endl;
        else 
            std::cout << "<NOT SET>" << std::endl;
    }

}



/* (Auto) fill a draw with a random seqential run in range. */
void draw::fill()
{
    this->fill(NBALLS);
    //this->fill_v2(NBALLS);
}


void draw::fill_v2(int n_balls) 
{
    while(1)
    std::cout << "UNDERFILLING VAGINERS: " << n_balls << std::endl;
}


void draw::fill(int n_balls) 
{
    std::cout << "UNDERFILLING VA-JAY-JAYS: " << n_balls << std::endl;
    if(n_balls < 1 || n_balls > 6) 
        throw draw::DrawException1("draw::fill: Invalid Index");
    else if(n_balls == 6 && !this->valid_sp_range()) 
        throw InvalidRangeException("draw::fill: Invalid range 1");
    if(!this->valid_range()) 
        throw InvalidRangeException("draw::fill: Invalid range 2");
    
    if(!this->valid()) {
        std::cout << "SCRUNCHING JUNKIES~ " << *this;
        squishing_boners();;
    }

    //this->clear(); // ???

    //while(this->ball_const) {
      //  std::cout << "SCRUNCHING JUNKIES~ " << *this;
        //this->show_ball_const();
    //}
    
    int n_gap_tries = 0, n_fc_tries = 0;
    FILL_START:
    //std::cout << "SCRUNCHING BONERS~ " << this->rng_ff << std::endl;
    int hash_map[100] = {0};
    int stop_at = n_balls < NBALLS ? n_balls : NBALLS-1;
    for(int i = 0; i < stop_at; i++) {
        int num = GET_RAND_NUM(this->rng_ff.lower, this->rng_ff.upper);
        if(hash_map[num]++) {
            i--;
            continue;
        } 
        this->balls[i] = num;
    }

    /* sort the first five vals and leave space for special ball at the end. */
    BALL_SORT(stop_at, this->balls);

    //std::cout << "SCRUNCHING VAGINERS~ " << this->rng_ff << std::endl;

    /* If all balls were chosen, the special ball has its own range. */
    if(n_balls == NBALLS) 
        this->balls[NBALLS-1] = GET_RAND_NUM(this->rng_sp.lower, this->rng_sp.upper); 

    /*
    if(!this->check_gaps(stop_at)) {
        std::cout << "      ^       " << std::endl;           
        std::cout << "    /_ _\\    " << std::endl;
        std::cout << "     { }      " << std::endl;
        //std::cout << "     |||      " << std::endl;
        //std::cout << "     | |      " << std::endl;
        std::cout << "    {@;@}     " << std::endl;
        //std::cout << "      X   CHUCKLING BONERS I WAS: " << *this << std::endl;
        //squishing_boners();;
        std::cout << "~SCRUNCHING BONERS I WAS: " << *this << ", " << n_gap_tries << std::endl;
        memset(this->balls, 0, sizeof(this->balls));
        n_gap_tries++;
        goto FILL_START;
    } else if(this->ball_const) {

        for(int i = 0; i < NBALLS; i++) {
            if(this->ball_const[i] && (this->balls[i] != this->ball_const[i])) {
                //std::cout << "~CHUCKLING JUNKIES I WAS[" << i << "]: " << *this << std::endl;
                memset(this->balls, 0, sizeof(this->balls));
                n_fc_tries++;
                goto FILL_START;
            }
        }
    }
    */


    if(!this->check_gaps(stop_at)) {
        /*
        std::cout << "      ^       " << std::endl;           
        std::cout << "    /_ _\\    " << std::endl;
        std::cout << "     { }      " << std::endl;
        //std::cout << "     |||      " << std::endl;
        //std::cout << "     | |      " << std::endl;
        std::cout << "    {@;@}     " << std::endl;
        //std::cout << "      X   CHUCKLING BONERS I WAS: " << *this << std::endl;
        //squishing_boners();;
        std::cout << "~SCRUNCHING BONERS I WAS: " << *this << ", " << n_gap_tries << std::endl;
        */
        memset(this->balls, 0, sizeof(this->balls));
        n_gap_tries++;
        goto FILL_START;
    } else if(this->ball_const) {

        for(int i = 0; i < stop_at; i++) {
            if(this->ball_const[i] && (this->balls[i] != this->ball_const[i])) {
                std::cout << "~CHUCKLING JUNKIES I WAS[" << i << "]: " << *this << std::endl;
                std::cout << "CRYING BONERS: " << this->ball_const[i] << ", " <<
                     this->balls[i] << std::endl;
                memset(this->balls, 0, sizeof(this->balls));
                n_fc_tries++;
                goto FILL_START;
            }
        }
    } 


    if(0 && this->gaps) {
        std::cout << "      ^       " << std::endl;           
        std::cout << "    /_ _\\    " << std::endl;
        std::cout << "     {<}      " << std::endl;
        //std::cout << "     |||      " << std::endl;
        //std::cout << "     | |      " << std::endl;
        std::cout << "____{@;@}_____" << std::endl;
        std::cout << "      X       " << std::endl;
        std::cout << "SCRUNCHING BONERS~(" << n_gap_tries << ")~" << std::endl;

    }

    //std::cout << "SCRUNCHING BONERS~(" << n_gap_tries << ", " << n_fc_tries << ")~" << std::endl;

    
    this->timestamp = std::chrono::system_clock::now();

}





bool draw::valid_range()
{
    return VALID_RANGE(this->rng_ff.lower, this->rng_ff.upper);
}

bool draw::valid_sp_range()
{
    return VALID_RANGE_SP(this->rng_sp.lower, this->rng_sp.upper);;
}

int huge_frump = 0;

/* Upon range change, determinine if gaps (if any exist) can still
   produce possible draws. */
bool draw::valid_rng_gap_change(int lr, int ur)
{
    std::cout << "UNCLAPPING VAGINERS!~ " << std::endl;
    //squishing_boners();
    draw d;
    //d.set_range(lr, ur); // THIS CAUSES IL...
    d.rng_ff.lower = lr;
    d.rng_ff.upper = ur;
    std::cout << std::endl << std::endl << "UNCLAPPING BONERS!~ " << d << std::endl;
    d.show_constraints();
    //squishing_boners();
    for(int i = 0; i < NGAPS; i++) {
        std::cout << "UNCLAPPING JUNKIES!~ " << ++huge_frump << std::endl;
        //squishing_boners();
        if(this->gaps[i].val) {
            try {
                d.set_gap(i, this->gaps[i].type, this->gaps[i].val);
                //std::cout << "SPARKLING BONERS" << std::endl;
            } catch(gap::GapException1 &ge1) {
                squishing_boners();
                return false;
            }
        }
    }
    std::cout << "UNSLAPPING VAGINERS!~ " << std::endl;
    //squishing_boners();
    return true;
}


void draw::set_range(int lr, int ur) 
{
    std::cout << "SCRUNCHING VAGINERS~ " << *this << std::endl;
    if(!VALID_RANGE(lr, ur)) 
        throw InvalidRangeException("draw::set_range: !VALID_RANGE");
    //else if(this->gaps && !this->valid_rng_gap_change(lr, ur)) {
      //  throw InvalidRangeException("draw::set_range: !this->valid_rng_gap_change(lr, ur)");
    //} 
    
    
    /*
    else if(!this->empty()) {
        std::cout << "THIS OLD GRANNY TAKES IT RIGHT IN THE POOP SHOOT~ " << *this << std::endl;
        //squishing_boners();;
        for(int i = 0; i < NBALLS-1; i++) {
            if(this->balls[i] && 
                (this->balls[i] < lr || this->balls[i] > ur)) {
                std::cout << "BONE SMUGGLING GRANNIES~ " << *this << 
                    ", " << lr << ", " << ur << std::endl;
                //squishing_boners();;
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            }
        }
    }

    if(this->ball_const) {
        while(1) std::cout << "DROOPY BONERS 1~ ";
    }
    */

    draw d = *this;
    //d.set_range(lr, ur); //, this->rng_sp);
    d.rng_ff.lower = lr;
    d.rng_ff.upper = ur;
    d.show_gaps();
    d.show_ball_const();
    std::cout << "~~~~PROLAPSING JABRONIES~ " << d << std::endl;


    //try {
        if(!d.valid()) { //_constraints()) {
            std::cout << "THIS OLD JABRONI TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
            //squishing_boners();;
            throw DrawException1("THIS OLD GRANNY TAKES IT RIGHT IN THE POOP SHOOT~");
        }
    //} catch(gap::GapException1 &ge1) {
      //  std::cout << "~~~~COLLAPSING MONKEYS~ " << std::endl;
        //exit(9);
        //throw ge1;
    //}

    std::cout << "~~~~COLLAPSING JUNKIES~ " << std::endl;
    //squishing_boners();;


    this->rng_ff.lower = lr;
    this->rng_ff.upper = ur;
    
}

void draw::set_range_sp(int lr, int ur) 
{

    if(!VALID_RANGE_SP(lr, ur)) 
        throw InvalidRangeException("draw::set_range_sp: !VALID_RANGE");

    this->rng_sp.lower = lr;
    this->rng_sp.upper = ur;
}

void draw::set_range_urs(int ur, int sp_ur)
{
    
    int lr;
    if(this->rng_ff.lower)
        lr = this->rng_ff.lower;
    else
        lr = 1;

    if(!VALID_RANGE(lr, ur)) 
        throw InvalidRangeException("draw::set_range_urs(range dr): !VALID_RANGE");
    else if(!VALID_RANGE_SP(lr, sp_ur)) 
        throw InvalidRangeException("draw::set_range_urs: !VALID_RANGE_SP");
    else if(this->gaps && !this->valid_rng_gap_change(lr, ur)) 
        throw InvalidRangeException("draw::set_range: !this->valid_rng_gap_change(lr, ur)");
    
    /*
    else if(!this->empty_ff()) {
        std::cout << "THIS OLD TRANNY TAKES IT RIGHT IN THE POOP SHOOT" << *this << std::endl;
        squishing_boners();;
        for(int i = 0; i < NBALLS-1; i++) {
            if(this->balls[i] && 
                (this->balls[i] < lr || this->balls[i] > ur)) {
                while(1) std::cout << "BONE SMUGGLING JABRONIES~ ";
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            }
        }
    }

    if(this->ball_const) {
        while(1) std::cout << "DROOPY BONERS 3~ ";
    }
    */

    draw d = *this;

    if(!d.valid()) { //_constraints()) {
        std::cout << "THIS OLD NANNY TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
        squishing_boners();;
    }

    while(1) std::cout << "LET'S FUCK SOME TRANNIES~ ";

    if(!this->rng_ff.lower)
        this->rng_ff.lower = 1;
    this->rng_ff.upper = ur;
    if(!this->rng_sp.lower)
        this->rng_sp.lower = 1;
    
    this->rng_sp.upper = sp_ur;
}

void draw::set_range(range dr) 
{
    if(!VALID_RANGE(dr.lower, dr.upper)) 
        throw InvalidRangeException("draw::set_range(range dr): !VALID_RANGE");
    else if(this->gaps && !this->valid_rng_gap_change(dr.lower, dr.upper)) 
        throw InvalidRangeException("draw::set_range: !this->valid_rng_gap_change(lr, ur)");
    /*
    //if(!this->empty()) {
        for(int i = 0; i < NBALLS-1; i++) {
            if(this->balls[i] && 
                (this->balls[i] < dr.lower || this->balls[i] > dr.upper)) {
                while(1) std::cout << "BONE SMUGGLING JABRONIES~ ";
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            } else if(this->ball_const && this->ball_const[i] && 
                     (this->ball_const[i] < dr.lower || this->ball_const[i] > dr.upper) )  {
                
                while(1) std::cout << "LET'S FUCK SOME WHORES~ ";
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            
            }
        }
        std::cout << "THIS OLD JABRONI NAMED DANNY TAKES IT RIGHT IN THE POOP SHOOT " << *this << std::endl;
        //squishing_boners();;
    //}
    */
    //while(1) std::cout << "BONE SMUGGLING JABRONIES~ ";
    draw d = *this;
    //while(1) std::cout << "BONE SMUGGLING JABRONIES~ " <<* this;

    if(!d.valid()) { //_constraints()) {
        std::cout << "THIS OLD JABRONI NAMED DANNY TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
        squishing_boners();;
    }

    std::cout << "LET'S FUCK SOME WHORES~ ";

    /*
    if(this->ball_const) {

        for(int i = 0; i < NBALLS-1; i++) {
            if(this->ball_const[i] && 
                (this->ball_const[i] < dr.lower || this->ball_const[i] > dr.upper)) {
                while(1) std::cout << "LET'S FUCK SOME WHORES~ ";
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            }
        }
        show_ball_const();
        std::cout << "DROOPY BONERS 4~ ";
        //squishing_boners();;
        
    }
    */

    this->rng_ff = dr;
}

void draw::set_ranges(range dr, range dr_sp)
{

    if(!VALID_RANGE(dr.lower, dr.upper)) 
        throw InvalidRangeException("draw::set_range(range dr): !VALID_RANGE");
    else if(!VALID_RANGE_SP(dr_sp.lower, dr_sp.upper)) 
        throw InvalidRangeException("draw::set_range(range dr): !VALID_RANGE_SP");
    else if(this->gaps && !this->valid_rng_gap_change(dr.lower, dr.upper)) 
        throw InvalidRangeException("draw::set_range: !this->valid_rng_gap_change(lr, ur)");
    
    /*
    else if(!this->empty_ff()) {
        std::cout << "THIS OLD JUNKIE TAKES IT RIGHT IN THE POOP SHOOT" << *this << std::endl;
        squishing_boners();;
        for(int i = 0; i < NBALLS-1; i++) {
            if(this->balls[i] && 
                (this->balls[i] < dr.lower || this->balls[i] > dr.upper)) {
                while(1) std::cout << "BONE SMUGGLING JABRONIES~ ";
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            }
        }
    } 
    */

    draw d = *this;

    if(!d.valid()) { //_constraints()) {
        std::cout << "THIS OLD JUNKIE NAMED DANNY TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
        squishing_boners();;
    }

    while(1) std::cout << "LET'S FUCK SOME JUNKIES~ ";

    
    this->rng_ff = dr;
    this->rng_sp = dr_sp;
}

void draw::set_range_sp(range dr_sp)
{
    if(!VALID_RANGE_SP(dr_sp.lower, dr_sp.upper)) 
        throw InvalidRangeException("draw::set_range_sp(range dr): !VALID_RANGE_SP");
    while(1) std::cout << "LET'S FUCK SOME MONKEYS~ ";
    this->rng_sp = dr_sp;
}


int draw::get_ball(int index) 
{
    if(index > NBALLS-1 || index < 0) 
        throw std::range_error("draw::get_ball: Invalid ball index. (I={0,5})");
    return this->balls[index];
}

/*
else if(this->ball_const) {
            if( (val + ((NBALLS-2)-index)) > this->rng_ff.upper) {
            
                if(fc_set) {
                    delete [] this->ball_const;
                    this->ball_const = 0;
                }
                while(1) std::cout << "DOINKING GRANNIES 99~ "; // << *this ;
 
            } else if((val - index) < this->rng_ff.lower) {
                if(fc_set) {
                    delete [] this->ball_const;

*/


void draw::set_ball(int index, int val) 
{
    if(index > NBALLS-1 || index < 0) 
        throw DrawException1("draw::set_ball: Invalid ball index. (I={0,5})");
    
    //else if(val < 1 || val > MAX_LOT_RANGE)
      //  throw InvalidRangeException("draw::set_ball: Invalid range 1");
    else if(index < NBALLS-1) {
        //if(val > this->rng_ff.upper) 
        if(!this->rng_ff.lower || !this->rng_ff.upper) 
             throw DrawException1("!this->rng_ff.lower || !this->rng_ff.upper");
        else if( (val + ((NBALLS-2)-index)) > this->rng_ff.upper) {
            //throw InvalidRangeException("draw::set_ball: Invalid range 2");
            while(1) std::cout << "OVULATING JABRONIES WON# ";
        } else if((val - index) < this->rng_ff.lower) {
            while(1) std::cout << "OVULATING JABRONIES WON+WON#";
        }
    } else {
        if(!this->rng_sp.lower || !this->rng_sp.upper) 
             throw DrawException1("!this->rng_sp.lower || !this->rng_sp.upper");
        else if(val < this->rng_sp.lower || val > this->rng_sp.upper) 
            throw InvalidRangeException("draw::set_ball: Invalid range 3");
    }
    
    if(this->ball_const && this->ball_const[index] && this->ball_const[index] != val) {
        while(2) std::cout << "$-SHNOODLING JABRONIES~ " << *this << ", "; // <<  std::endl;
    }

    /* Value is in range, so see if it already exists in the draw */
    for(int i = 0; index < NBALLS-1 && i < NBALLS-1; i++) {
        if(this->balls[i] == val) 
            throw DrawException1("draw::set_ball: duplicate ball value found.");
    }

    draw d = *this;
    d[index] = val;
    std::cout << "$-SHNOODLING JABRONIES~ " << d << ", " <<  std::endl;
    //squishing_boners();;

    /*
    for(int i = 0; i < NBALLS; i++) {
        if(i == index)
            d[i] = val;
        else
            d[i] = this->balls[i];
    }
    */

    //if(!d.validate()) 
      //  throw DrawException1("draw::set_ball: !d.validate().");
    
    /*
    else if(index < NBALLS-1 && this->gaps) { //} && ) {
        
        std::cout << "$-SHNOODLING JABRONIES~ " << d << ", " << this->rng_ff << std::endl;

        // need to try catch this...
        d.set_range(this->rng_ff);

        for(int i = 0; i < NGAPS; i++) {
            if(this->gaps[i].type) {
                try {
                    d.set_gap(i, this->gaps[i].type, this->gaps[i].val);
                } catch(gap::GapException1 &ge1) {
                    throw ge1;
                }
            }
        }
    } 

    if(this->ball_const && this->ball_const[index] &&
        val != this->ball_const[index]) {
        while(1) std::cout << "DROOPY BONERS 69[" << index << "]~ " <<
            this->ball_const[index] << ", " << val << " === ";
    }
    */

    std::cout << "BALL BASHING NANNIES~ " << *this << std::endl;
    //squishing_boners();;
    if(!d.valid()) { //_constraints()) {
        std::cout << "OVULATING JABRONIES BIG TIME" << std::endl;
        squishing_boners();;
    }
    std::cout << "LET'S FUCK SOME GRANNIES~ "<< std::endl;;

    this->balls[index] = val;
    this->show_gaps();
    std::cout << "BALL BASHING GRANNIES~ " << *this << std::endl;

}


void draw::set_balls(int balls[NBALLS]) 
{
    for(int i = 0; i < NBALLS; i++) 
        this->set_ball(i, balls[i]); 
}


std::ostream& operator<< (std::ostream& out, const draw& data) 
{
    out << "(" << data.rng_ff << 
        "," << data.rng_sp << ") ~"; 
        if(0 && data.balls[0] == data.rng_ff.lower) {
            out << std::endl;
            out << "      ^       " << std::endl;           
            out << "    /_ _\\    " << std::endl;
            out << "     {>}  " << std::endl; //SCRUNCHING BONERS    " << std::endl;
            //out << "     |||      " << std::endl;
            //out << "     | |      " << std::endl;
            out << "    {@;@}    " << std::endl;
            out << "      X       SCRUNCHING BONERS BIG TIME" << std::endl;
            //out << "____{@;@}_____" << std::endl;
            //out << "      X       SCRUNCHING BONERS BIG TIMES: " << std::endl;
            //out << std::endl;
        }
    for(int i = 0; i < NBALLS; i++) {
        
        
        out << "[";
        if(data.balls[i] < 10 && data.balls[i] >= 0)
            out << " ";
        out << data.balls[i] << "]";
        if(i < 4)
            out << "-";
        else if(i == 4)
            out << "==";
    }
    return out;
}

std::ostream& operator<< (std::ostream& out, const draw_win& data) 
{
    out << (draw&)data << std::endl;
    out  << data.get_date()->year << "-" << data.get_date()->month << "-" << data.get_date()->day;
    out << ", " << data.get_which_lotto() << ", megaplier=" << data.get_megaplier() << std::endl;
    return out;
}

std::ostream& operator<< (std::ostream& out, const range& data) 
{
    out << "{";
    if(data.lower)
        out << data.lower;
    else
        out << "X";
    out << ",";    
    if(data.upper)
        out << data.upper;
    else
        out << "X";
    out << "}"; 
    return out;
}

std::ostream& operator<< (std::ostream& out, const gap& data) 
{
    out << "<GAP: type="; 
    switch(data.type) {
        case GT_NOTSET:
            out << "NS";
            break;
        case GT_LT:
            out << "LT";
            break;
        case GT_LTE:
            out << "LTE";
            break;
        case GT_EQ:
            out << "EQ";
            break;
        case GT_GTE:
            out << "GTE";
            break;
        case GT_GT:
            out << "GT";
            break;
    }
    out << ", val=" << data.val << ">";
    return out;
}
