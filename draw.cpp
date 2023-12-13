


#include "draw.h"


//bool draw::range_eq(range dr1, range dr2)
//{
  //  return dr1.lower == dr2.lower && dr1.upper == dr2.upper;
//}

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
    std::cout << "UNCLAPPING BONERS" << std::endl;
    exit(9);
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
    //this->timestamp = std::__1::chrono::system_clock::time_point();
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
}

/* Constructor to set lower and upper bounds on both pots. (as arrays) */
draw::draw(int rng[2], int sp_rng[2])
{
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    //memset(this->gaps, 0, sizeof(this->gaps));
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(rng[0], rng[1]);
    this->set_range_sp(sp_rng[0], sp_rng[1]);
    //this->timestamp = std::__1::chrono::system_clock::time_point();
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
}


/* draw constructor with ball value parameters */
draw::draw(int b1, int b2, int b3, int b4, int b5, int b6) 
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    //memset(this->gaps, 0, sizeof(this->gaps));
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    this->balls[0] = b1;
    this->balls[1] = b2;
    this->balls[2] = b3;
    this->balls[3] = b4;
    this->balls[4] = b5;
    this->balls[5] = b6;
    //this->timestamp = std::__1::chrono::system_clock::time_point();
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    this->gaps = 0;
    this->ball_const = 0;
}

/* draw constructor with ball value as array parameter */
draw::draw(int vals[NBALLS]) 
{
    this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    //memset(this->gaps, 0, sizeof(this->gaps));
    for(int i = 0; i < NBALLS; i++) 
        this->balls[i] = vals[i];
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    //this->timestamp = std::__1::chrono::system_clock::time_point();
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
    //memset(this->gaps, 0, sizeof(this->gaps));
    for(int i = 0; i < n_vals; i++) 
        this->balls[i] = vals[i];
    memset(&this->rng_ff, 0, sizeof(this->rng_ff));
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    //this->timestamp = std::__1::chrono::system_clock::time_point();
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    this->gaps = 0;
    this->ball_const = 0;
}

draw::draw(range dr) { //: draw(dr.lower, dr.upper) {}

    
     this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    //memset(this->gaps, 0, sizeof(this->gaps));
    //this->set_range(rng[0], rng[1]);
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(dr);
    memset(&this->rng_sp, 0, sizeof(this->rng_sp));
    //this->set_range_sp(sp_rng[0], sp_rng[1]);
    //this->timestamp = std::__1::chrono::system_clock::time_point();
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
    
}

//draw::draw(range dr, range dr_sp) : draw(dr.lower, dr.upper, dr_sp.lower, dr_sp.upper) {}
draw::draw(range dr, range dr_sp) { //: draw(dr.lower, dr.upper) {}

    
     this->spec_val = &this->balls[NBALLS-1];
    memset(this->balls, 0, sizeof(this->balls));
    //memset(this->gaps, 0, sizeof(this->gaps));
    //this->set_range(rng[0], rng[1]);
    this->gaps = 0;
    this->ball_const = 0;
    this->set_range(dr);
    this->set_range_sp(dr_sp);
    //this->set_range_sp(sp_rng[0], sp_rng[1]);
    //this->timestamp = std::__1::chrono::system_clock::time_point();
    this->timestamp = std::chrono::time_point<std::chrono::system_clock>();
    
    
}

/* Common code for copying draw object */
void draw::copy_code(const draw &rhs)
{
    std::cout << "SMOOCHING VA-JAY-JAYS~ " << rhs.gaps << std::endl;
    this->gaps = 0;
    if(rhs.gaps) {
        //std::cout << "GAPING JUNKIES NAMES FRANNIE: " << *this->gaps << ", " << rhs << std::endl;
        std::cout << "KNOCKING BONERS" << std::endl;
        //exit(9);
        this->gaps = new gap[NGAPS];
        //memset(this->gaps, 0, sizeof(*this->gaps));
        for(int i = 0; i < NGAPS; i++)
            memset(&this->gaps[i], 0, sizeof(this->gaps[i]));
    }
    //else { //if(this->gaps) {
        //std::cout << "EVERY TIME GRANDPA GET'S ANAL THE ANGELS CRY: " << *this << std::endl;
        //exit(9);
        //this->gaps = 0;
    //}
    this->spec_val = &this->balls[NBALLS-1];
    //memset(this->gaps, 0, sizeof(*this->gaps));

    for(int i = 0; i < NBALLS; i++) {
        //std::cout << "SMOOCHING PENISES[" << i << "] " << rhs.balls[i] <<
          ///       ", " << this->balls[i] << std::endl;
        this->balls[i] = rhs.balls[i];
        if(i < NGAPS && rhs.gaps) {
            std::cout << "GAPING JUNKIES[" << i << "]" << std::endl;
            this->gaps[i] = rhs.gaps[i];
        }
    }

    this->ball_const = 0;
    if(rhs.ball_const) {
        this->ball_const = new int[NBALLS];
        for(int i = 0; i < NBALLS; i++) {
            std::cout << "VAPING JUNKIES[" << i << "]" << std::endl;
            //memset(&this->ball_const[i], 0, sizeof(this->ball_const[i]));
            this->ball_const[i] = rhs.ball_const[i];
        }
    }
    
    
    //this->range.lower = rhs.range.lower;
    //this->range.upper = rhs.range.upper;
    //this->range_sp.lower = rhs.range_sp.lower;
    //this->range_sp.upper = rhs.range_sp.upper;
    this->rng_ff = rhs.rng_ff;
    this->rng_sp = rhs.rng_sp;
    std::cout << "EVERY TIME BUSEY GET'S ANAL THE ANGELS WANT TO KNOW WHY: "
             << *this << std::endl;
    //exit(9);
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

    std::cout << "SPINNING BONERS [@;@]~~~> " << std::endl << *this << std::endl;
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
    /*
    for(int i = 0; i < n_balls; i++) {
        std::cout << "GRANDMA'S GETTIN' MOOSE BONERS: " << d << std::endl;
        //exit(9);
        if(this->balls[i] == DRAW_GAP_VAL)
            continue;
        if(this->balls[i] != d[i])  
            return false;
    }

    (*this) = d;
    std::cout << "GRANDPA'S RAGIN' MOOSE BONERS: " << d << std::endl;
    //exit(9);
    return true;
    */
    return this->equal_to(n_balls, d, false);
}

bool draw::equal_to(int n_balls, const draw& d, bool use_gaps)
{

    for(int i = 0; i < n_balls; i++) {
        //std::cout << "GRANDPA'S GETTIN' MOOSE BONERS: " << d << std::endl;
        //exit(9);
        if(use_gaps && this->balls[i] == DRAW_GAP_VAL)
            continue;
        if(this->balls[i] != d[i])  
            return false;
    }

    //(*this) = d;
    //std::cout << "GRANDPA'S GETTIN' GOOSE BONERS: " << d << std::endl;
    //exit(9);
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
void draw::clear() {
    memset(this->balls, 0, sizeof(this->balls));
}

void draw::clear(int index) {
    if(index < 0 || index > NBALLS-1)
        while(1) std::cout << "SMACKING BONERS~";
    
    //memset(this->balls, 0, sizeof(this->balls));
    this->balls[index] = 0;
}

/* Check for validation of draw (can have gaps, but must in range and sequential...)*/
bool draw::validate() 
{
    int hash_map[100] = {0};
    int last_ball = 0;
    for(int i = 0; i < NBALLS-1; i++) {
        int ball = this->balls[i];
        if(ball == DRAW_GAP_VAL)
            continue;
        if(ball) {
            if( (this->rng_ff.lower && (ball < this->rng_ff.lower)) || 
                (this->rng_ff.upper && (ball > this->rng_ff.upper-(((NBALLS-1)-1)-i))) ) 
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
/*
void draw::ball_sort(int n_balls) 
{
    if(n_balls < 1 || n_balls > 6)
        throw DrawException1("draw::ball_sort: Invalid Index");
    int tmps[5] = {0};
    for(int i = 0; i < NBALLS-1; i++)
        tmps[i] = this->balls[i];
    auto iptr = std::begin(tmps);
    for(int i = 0; i < n_balls; i++)
        iptr++;
    std::sort(std::begin(tmps), iptr);
    for(int i = 0; i < n_balls; i++)
        this->balls[i] = tmps[i];
}
*/



void draw::show_gaps()
{
    std::cout << "~!-~@- GAPING GRANNIES -@~-!~ " << std::endl; //this->gaps << std::endl;
    if(!this->gaps) {
        std::cout << "Gaps are NOT SET." << std::endl;
        return;
    }
    for(int i = 0; i < NGAPS; i++) {
        std::cout << "~SPLASHING BONERS[" << i << "] ";// << std::endl;; // << *this->gaps[i] << std::endl;
        if(this->gaps[i].type)
            std::cout << this->gaps[i] << std::endl;
        else 
            std::cout << "<NOT SET>" << std::endl;
    }
}


void draw::clear_gap(int index)
{

    if(index < 0 || index > NGAPS-1) 
        throw gap::GapException1("SQUISHING BONERS 11");
    memset(&this->gaps[index], 0, sizeof(this->gaps[index]));
}


void draw::clear_gaps()
{
    for(int i = 0; i < NGAPS; i++) {
        try {
            this->clear_gap(i);
        } catch(gap::GapException1 &ge1) {
            throw gap::GapException1("SQUISHING BONERS 1211");
        }
    }
}

// called from "fill" routine
bool draw::check_gaps(int stop_at)
{
    //std::cout << "#CRACK VAPING GRANNIES: " << this->gaps << std::endl;
    //this->show_gaps();
    if(!this->gaps)
        return true;
    int gap_val;
    //for(int i = 1; i < stop_at; i++) {
    for(int i = 0; i < NGAPS; i++) {

        //if(i < NGAPS && this->gaps[i-1].type) {
        if(this->gaps[i].type && (this->balls[i] && this->balls[i+1])) {

            //std::cout << "GAPING JABRONIES[ " << i << "] " << this->gaps[i] << std::endl;

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

    bool gaps_set = false;
    if(index < 0 || index > NGAPS-1) 
        throw gap::GapException1("index < 0 || index > NGAPS-1");
    else if(val < 1 || val > MAX_LOT_RANGE) 
        throw gap::GapException1("val < 1 || val > MAX_LOT_RANGE");
    else if(!this->gaps) {
        gaps_set = true;
        this->gaps = new gap[NGAPS];
        for(int i = 0; i < NGAPS; i++)
            memset(&this->gaps[i], 0, sizeof(this->gaps[i]));
    }

    //int fc_dif = 0;
    //if(this->ball_const && (this->ball_const[index] && this->ball_const[index+1])) {
        
      //  fc_dif = this->ball_const[index+1] - this->ball_const[index];
        //while(1) std::cout << "GRINDING WEINERS[" << index << "]~ " << fc_dif << "~ ";
        
    //}

    draw d = *this;

    if(!d.valid()) { //_constraints()) {
        std::cout << "~~~~COLLAPSING VA-JAY-JAYS~ " << std::endl;
        exit(9);
    }
    
    std::cout << "~~~~COLLAPSING VAGINERS~ " << std::endl;
    exit(9);
    
    

    switch(this->gaps[index].type) {
        case GT_NOTSET:
            break;
        case GT_LT:
            if(val < 2) {
                if(gaps_set) {
                    delete [] this->gaps;
                    this->gaps = 0;
                } 
                throw gap::GapException1("val < 2");
            }
            break;
        case GT_EQ:
        case GT_LTE:
        case GT_GTE:
        case GT_GT:
            if(val < 1) {
                if(gaps_set) {
                    delete [] this->gaps;
                    this->gaps = 0;
                }
                throw gap::GapException1("val < 1");
            }
            break;
        default: 
            if(gaps_set) {
                delete [] this->gaps;
                this->gaps = 0;
            }
            throw gap::GapException1("Invalid gap type");
    }

    int draw_sum = this->rng_ff.lower; 

    for(int i = 0; i < NGAPS; i++) {
        /* Create a gap to test if it is the index we are adding/modifying. */
        gap g;
        if(i == index) {
            g.type = this->gaps[i].type;
            g.val = val;
        } else /* Otherwise get the gap from the gaps array. */
            g = this->gaps[i];
                
        /* Determine the gap type and make the appropriate calculation. */
        //int dif;
        switch(g.type) {
            case GT_NOTSET:
            case GT_LT:
            case GT_LTE:
                if(this->balls[i] && this->balls[i+1]) {
                    if(g.type == GT_LT) {
                        if( (this->balls[i+1] - this->balls[i]) >= g.val ) {
                            if(gaps_set) {
                                delete [] this->gaps;
                                this->gaps = 0;
                            }
                            throw gap::GapException1("VIOLATING GRANNIES 1~");
                        }
                    } else if((g.type == GT_LTE) && (this->balls[i+1] - this->balls[i]) > g.val) {
                        if(gaps_set) {
                            delete [] this->gaps;
                            this->gaps = 0;
                        }
                        throw gap::GapException1("VIOLATING GRANNIES 2~");
                    }
                } else if(this->ball_const &&
                         (this->ball_const[index] && this->ball_const[index+1])) {
                    if(g.type == GT_LT) {
                        if( (this->ball_const[i+1] - this->ball_const[i]) >= g.val ) {
                            //if(gaps_set) {
                              //  delete [] this->gaps;
                                //this->gaps = 0;
                            //}
                            //throw gap::GapException1("VIOLATING GRANNIES 1~");
                            while(1) std::cout << "VIOLATING GRANNIES 11~";
                        }
                    } else if((g.type == GT_LTE) && (this->ball_const[i+1] - this->ball_const[i]) > g.val) {
                        
                        while(1) std::cout << "VIOLATING GRANNIES 22~";
                    }

                    //while(1) std::cout << "VIOLATING NANNIES~ " << g << "~ ";
                }
                draw_sum++;
                break;
            case GT_EQ:
            case GT_GTE:
                
                if(this->balls[i] && this->balls[i+1]) {
                    if(g.type == GT_EQ) {
                        if( (this->balls[i+1] - this->balls[i]) != g.val ) {
                            if(gaps_set) {
                                delete [] this->gaps;
                                this->gaps = 0;
                            }
                            throw gap::GapException1("VIOLATING GRANNIES 3~");
                        }
                    } else if((this->balls[i+1] - this->balls[i]) < g.val) {
                        if(gaps_set) {
                            delete [] this->gaps;
                            this->gaps = 0;
                        }
                        throw gap::GapException1("VIOLATING GRANNIES 4~");
                    }
                } else if(this->ball_const &&
                         (this->ball_const[index] && this->ball_const[index+1])) {
                        
                    if(g.type == GT_EQ) {
                        if( (this->ball_const[i+1] - this->ball_const[i]) != g.val ) {
                            
                            throw gap::GapException1("VIOLATING GRANNIES 33~");
                        }
                    } else if((this->ball_const[i+1] - this->ball_const[i]) < g.val) {
                        
                        throw gap::GapException1("VIOLATING GRANNIES 49~");
                    }
                }
                draw_sum += g.val;
                break;
            case GT_GT:
                if((this->balls[i] && this->balls[i+1]) && 
                  ((this->balls[i+1] - this->balls[i]) <= g.val)) {
                    if(gaps_set) {
                        delete [] this->gaps;
                        this->gaps = 0;
                    }
                    throw gap::GapException1("VIOLATING GRANNIES QUIVE~");
                } else if((this->ball_const[i] && this->ball_const[i+1]) && 
                  ((this->ball_const[i+1] - this->ball_const[i]) <= g.val)) {
                     throw gap::GapException1("VIOLATING NANNIES QUIVE~");
                }
                draw_sum += (g.val + 1);
                break;
        }
    }

    

    /* Make sure the "least" possible run did exist, otherwise
       report the error. */
    if(draw_sum > this->rng_ff.upper) {
        if(gaps_set) {
            delete [] this->gaps;
            this->gaps = 0;
        }
        std::cout << "TABLING BONERS~ " << draw_sum << ", " << this->rng_ff.upper << "- ";
        //exit(9);
        throw gap::GapException1("draw_sum > this->rng_ff.upper");
    }

    std::cout << "THAT OLD GRANNY TOOK IT RIGHT IN THE POOP SHOOT SHE DID: (" <<
        draw_sum << ", " << this->rng_ff << ", " << val << ")" << std::endl;

    /* The tests all worked so we can now set the gap value. */
    this->gaps[index].val = val;

}


void draw::set_gap(int index, int type, int val)
{
    if(index < 0 || index > NGAPS-1) 
        throw draw::DrawException1("set_gap(int index, int type, int val): index < 0 || index > NGAPS-1");
    
    int prev_type = 0, prev_val = 0;// = this->gaps[index].type;
    if(this->gaps) {
        //while(1) std::cout << "KNOCKING BONERS~";
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
    std::cout << "EXPOSING GRANNIES~ " << *this << std::endl;
}

bool draw::valid() //_constraints()
{
    std::cout << " TWISTING GRANNIES~ " << *this  <<std::endl;
    //usleep(15000);
    

    int prev_val = 0, cur_val = 0, prev_bc_val = 0, cur_bc_val = 0;
    for(int i = 0; i < NBALLS-1; i++) {

        std::cout << "FISTING GRANNIES[" << i << "] " << std::endl;
        //cur_val = this->balls[i];
        //if(this->ball_const)
          //  cur_bc_val = this->ball_const[i];
        

        if(i < NGAPS && this->gaps && this->gaps[i].type) {
            std::cout << "HUGE WINKING GRANNIES[" << i << "]~ " << this->gaps[i] << std::endl;
            exit(9);
        }
        
        if(this->ball_const && this->ball_const[i]) {
            std::cout << "HUGE STINKING VA-JAY-JAYS[" << i << "]~ " << this->ball_const[i] << std::endl;
            exit(9);
        }

        if(this->balls[i]) {
            std::cout << "HUGE WINKING JUNKIES[" << i << "]~ " << this->balls[i] << std::endl;
            //exit(9);
            cur_val = this->balls[i];
            if(prev_val) {
                while(1) std::cout << "HUGE WINKING JUNKIES ;~) " << cur_val << ", " <<
                    prev_val << " {@;@}~> ";
                

            }
            prev_val = cur_val;
        }

        
        //prev_bc_val = cur_bc_val;
        //prev_val = cur_val;


    }

    std::cout << "HUGE STINKING JUNKIES~ " << *this << std::endl;
    return true; //valid_constraints();




}

void draw::set_ball_const(int index, int val)
{
    if(index < 0 || index > NBALLS-1) 
        //throw gap::GapException1("index < 0 || index > NGAPS-1");
        while(1) std::cout << "GRINDING GRANNIES 99~ ";
    else if(val < 1 || val > MAX_LOT_RANGE) {
        while(1) std::cout << "GRINDING GRANNIES -99~ ";
    } 


    

    std::cout << "OVULATING GRANNIES BIG TIME" << std::endl;
    //exit(9);
    
    bool fc_set = false;
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
            //exit(9);
            int prev_ball_val = 0;
            for(int i = 0; i < NBALLS-1; i++) {
                if(this->ball_const[i]) {
                    if(prev_ball_val && this->ball_const[i] <= prev_ball_val) {
                        if(fc_set) {
                            delete [] this->ball_const;
                            this->ball_const = 0;
                        }
                        std::cout << "SLURPING NANNIES~ ";
                        exit(9);
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

    /*
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
                        exit(9);
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

    std::cout << "SCRAMBLING VA-JAY-JAYS 1" << std::endl;
    //exit(9);
    draw d = *this;
    /*
    if(!d.ball_const) {
        //fc_set = true;
        std::cout << "GRINDING GRANNIES 1~ " << std::endl;
        d.ball_const = new int[NBALLS];
        for(int i = 0; i < NBALLS; i++)
            memset(&d.ball_const[i], 0, sizeof(d.ball_const[i]));
    }
    */

    //draw e = *this;
    d.show_gaps();
    d.show_ball_const();
    std::cout << "SCRAMBLING GRANNIES: " << d << std::endl;
    //exit(9);
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
    //exit(9);
    
    if(!d.valid()) { //_constraints()) {
        std::cout << "OVULATING JABRONIES BIG TIME" << std::endl;
        exit(9);
    }


    while(1) std::cout << "~~~~COLLAPSING VA-JAY-JAYS~ "; // << std::endl;
    exit(9);

    
    //this->balls[index] = this->ball_const[index] = val;

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
    std::cout << "~!-~@- SLURPING GRANNIES -@~-!~ " << std::endl; // this->gaps << std::endl;
    if(!this->ball_const) {
        std::cout << "Fill Constants are NOT SET." << std::endl;
        return;
    }
    for(int i = 0; i < NBALLS; i++) {
        std::cout << "~SPLASHING VA-JAY-JAYS[" << i << "] ";// << std::endl;; // << *this->gaps[i] << std::endl;
        if(this->ball_const[i])
            std::cout << this->ball_const[i] << std::endl;
        else 
            std::cout << "<NOT SET>" << std::endl;
    }

}



/* (Auto) fill a draw with a random seqential run in range. */
void draw::fill()
{
    //this->fill(NBALLS);
    this->fill_v2(NBALLS);
}


void draw::fill_v2(int n_balls) 
{
    while(1)
    std::cout << "UNDERFILLING VA-JAY-JAYS: " << n_balls << std::endl;
}


void draw::fill(int n_balls) 
{
    if(n_balls < 1 || n_balls > 6) 
        throw draw::DrawException1("draw::fill: Invalid Index");
    else if(n_balls == 6 && !this->valid_sp_range()) 
        throw InvalidRangeException("draw::fill: Invalid range 1");
    if(!this->valid_range()) 
        throw InvalidRangeException("draw::fill: Invalid range 2");

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
        //exit(9);
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
        //exit(9);
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

/* Upon range change, determinine if gaps (if any exist) can still
   produce possible draws. */
bool draw::valid_rng_gap_change(int lr, int ur)
{
    draw d;
    d.set_range(lr, ur);
    for(int i = 0; i < NGAPS; i++) {
        if(this->gaps[i].val) {
            try {
                d.set_gap(i, this->gaps[i].type, this->gaps[i].val);
            } catch(gap::GapException1 &ge1) {
                return false;
            }
        }
    }
    return true;
}


void draw::set_range(int lr, int ur) 
{
    std::cout << "SCRUNCHING VAGINERS~ " << *this << std::endl;
    if(!VALID_RANGE(lr, ur)) 
        throw InvalidRangeException("draw::set_range: !VALID_RANGE");
    else if(this->gaps && !this->valid_rng_gap_change(lr, ur)) {
        throw InvalidRangeException("draw::set_range: !this->valid_rng_gap_change(lr, ur)");
    } 
    
    
    /*
    else if(!this->empty()) {
        std::cout << "THIS OLD GRANNY TAKES IT RIGHT IN THE POOP SHOOT~ " << *this << std::endl;
        //exit(9);
        for(int i = 0; i < NBALLS-1; i++) {
            if(this->balls[i] && 
                (this->balls[i] < lr || this->balls[i] > ur)) {
                std::cout << "BONE SMUGGLING GRANNIES~ " << *this << 
                    ", " << lr << ", " << ur << std::endl;
                //exit(9);
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            }
        }
    }

    if(this->ball_const) {
        while(1) std::cout << "POOPY BONERS 1~ ";
    }
    */

    draw d = *this;
    d.show_gaps();
    d.show_ball_const();
    std::cout << "~~~~PROLAPSING JABRONIES~ " << d << std::endl;



    if(!d.valid()) { //_constraints()) {
        std::cout << "THIS OLD GRANNY TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
        exit(9);
    }

    std::cout << "~~~~COLLAPSING JUNKIES~ " << std::endl;
    //exit(9);


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
        exit(9);
        for(int i = 0; i < NBALLS-1; i++) {
            if(this->balls[i] && 
                (this->balls[i] < lr || this->balls[i] > ur)) {
                while(1) std::cout << "BONE SMUGGLING JABRONIES~ ";
                throw InvalidRangeException("draw::set_range: BONE SMUGGLING JABRONIES~");
            }
        }
    }

    if(this->ball_const) {
        while(1) std::cout << "POOPY BONERS 3~ ";
    }
    */

    draw d = *this;

    if(!d.valid()) { //_constraints()) {
        std::cout << "THIS OLD NANNY TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
        exit(9);
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
        //exit(9);
    //}
    */

    draw d = *this;

    if(!d.valid()) { //_constraints()) {
        std::cout << "THIS OLD JABRONI NAMED DANNY TAKES IT RIGHT IN THE POOP SHOOT~" << std::endl;
        exit(9);
    }

    while(1) std::cout << "LET'S FUCK SOME WHORES~ ";

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
        std::cout << "POOPY BONERS 4~ ";
        //exit(9);
        
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
        exit(9);
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
        exit(9);
    }

    while(1) std::cout << "LET'S FUCK SOME JUNKIES~ ";

    
    this->rng_ff = dr;
    this->rng_sp = dr_sp;
}

void draw::set_range_sp(range dr_sp)
{
    if(!VALID_RANGE_SP(dr_sp.lower, dr_sp.upper)) 
        throw InvalidRangeException("draw::set_range_sp(range dr): !VALID_RANGE_SP");

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
        if( (val + ((NBALLS-2)-index)) > this->rng_ff.upper) {
            //throw InvalidRangeException("draw::set_ball: Invalid range 2");
            while(1) std::cout << "OVULATING JABRONIES WON# ";
        } else if((val - index) < this->rng_ff.lower) {
            while(1) std::cout << "OVULATING JABRONIES WON+WON#";
        }
    } else if(val < this->rng_sp.lower || val > this->rng_sp.upper) 
        throw InvalidRangeException("draw::set_ball: Invalid range 3");
    else if(this->ball_const && this->ball_const[index] != val) {
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
    //exit(9);

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
        while(1) std::cout << "POOPY BONERS 69[" << index << "]~ " <<
            this->ball_const[index] << ", " << val << " === ";
    }
    */

    std::cout << "BALL BASHING NANNIES~ " << *this << std::endl;
    //exit(9);
    if(!d.valid()) { //_constraints()) {
        std::cout << "OVULATING JABRONIES BIG TIME" << std::endl;
        exit(9);
    }
    std::cout << "LET'S FUCK SOME GRANNIES~ ";

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
