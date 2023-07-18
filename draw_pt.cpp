
#include "draw_pt.h"


draw_pt::draw_pt() : draw::draw() 
{ 
    this->pot_ptr = this->spec_pot_ptr = 0;
    this->n_objs++;
}

draw_pt::draw_pt(int upper, int spec_upper) : draw(upper, spec_upper) 
{
    this->pot_ptr = this->spec_pot_ptr = 0;
    this->n_objs++;
    this->set_range(1, upper);
    this->set_range_sp(1, spec_upper);
}

draw_pt::draw_pt(int rng[2], int sp_rng[2]) : draw(rng, sp_rng) 
{
    this->pot_ptr = this->spec_pot_ptr = 0;
    this->n_objs++;
    this->set_range(rng[0], rng[1]);
    this->set_range_sp(sp_rng[0], sp_rng[1]);
}


draw_pt::pot::pot(int lr, int ur) { // Needs no range error check (draw must have it here)
    this->rng.lower = lr;
    this->rng.upper = ur;
    this->pot_size = (this->rng.upper+1)-this->rng.lower;
    for(int i = lr, j = 0; i < ur+1; i++, j++) 
        this->vals[j] = i;
    this->n_jumbles = 0;
    this->start_jumbling();
}

draw_pt::draw_pt(int b1, int b2, int b3, int b4, int b5, int b6) : draw(b1,b2,b3,b4,b5,b6)
{
    this->pot_ptr = this->spec_pot_ptr = 0;
    this->n_objs++;
}

/* draw constructor with ball value as array parameter */
draw_pt::draw_pt(int vals[NBALLS]) : draw(vals)
{
    this->pot_ptr = this->spec_pot_ptr = 0;
    this->n_objs++;
}

draw_pt::draw_pt(int n_vals, int *vals) : draw(n_vals, vals)
{
    this->pot_ptr = this->spec_pot_ptr = 0;
    this->n_objs++;
}


draw_pt::pot::~pot() 
{
    this->pmux.lock();
    if(this->jumbling) {
        this->pmux.unlock();
        this->stop_jumbling(); // thread joins here
    } else 
        this->pmux.unlock();
}

bool draw_pt::pot::start_jumbling()
{
    if(this->size() > 1) {
        this->pmux.lock();
        if(this->jumbling) {
            this->pmux.unlock();
            return false;
        }
        this->jumbling = true;
        this->pmux.unlock();
        this->thr = std::thread(jumble, this);
    }
    return true;
}

bool draw_pt::pot::stop_jumbling()
{
    this->pmux.lock();
    if(!this->jumbling) {
        this->pmux.unlock();
        return false;
    }
    this->jumbling = false;
    this->pmux.unlock();
    this->thr.join();
    return true;

}

void draw_pt::pot::swap2()
{
    int min = 0, max;
    int rand_ind1, rand_ind2;
    int tmp;

    this->pmux.lock();
    max = this->pot_size-1;
    rand_ind1 = GET_RAND_NUM; 
    rand_ind2 = GET_RAND_NUM; 
    while(rand_ind2 == rand_ind1)
        rand_ind2 = GET_RAND_NUM; 
    tmp = this->vals[rand_ind1];
    this->vals[rand_ind1] = this->vals[rand_ind2];
    this->vals[rand_ind2] = tmp;
    this->pmux.unlock();
    this->n_jumbles++;
}

bool draw_pt::pot::is_jumbling()
{
    bool ret;
    this->pmux.lock();
    ret = this->jumbling;
    this->pmux.unlock();
    return ret;
}


void jumble(draw_pt::pot *p)
{
    while(p->is_jumbling()) 
        p->swap2();
}


std::ostream& operator<< (std::ostream& out, const draw_pt::pot& data) 
{    
    out <<"---size=" << data.size() << " balls {" << data.get_rng_lr() << "," << data.get_rng_ur()
        << "} (" << data.get_n_jumbles() << " jumbles)" << std::endl; 
    const int *tmps = data.get_vals();
    for(int i = 0; i < data.get_rng_ur()-data.get_rng_lr()+1; i++) {
        if(i && (i%8)== 0)
            out << std::endl;
        out << "[";
        if(i < 10)
            out << " ";
        out <<  i;
        out << "] ";
        if(tmps[i] < 10)
            out << " ";
        out << tmps[i] << " "; 
    }
    return out;
}

draw_pt::draw_pt(draw_pt &d) :  draw(d)// copy constructor
{
    this->copy_code(d);
}


draw_pt::draw_pt(const draw_pt &d) :  draw(d)
{
    this->copy_code(d);
}


void draw_pt::copy_code(const draw_pt &rhs)
{
    bool range_set = false;
    if(valid_range()) {
        this->pot_ptr = rhs.pot_ptr;
        this->pot_map_ninuse[this->range.lower-1][this->range.upper-1]++;
        range_set = true;
    }

    if(valid_sp_range()) {
        if(!valid_range()) {
            std::cout << "draw_pt::copy_code test1" << std::endl;
            exit(1);
        }
        this->spec_pot_ptr = rhs.spec_pot_ptr;
        this->pot_map_ninuse[this->range_sp.lower-1][this->range_sp.upper-1]++;
    }
    this->n_objs++;
    if(!range_set)
        this->pot_ptr = this->spec_pot_ptr = 0;
}

draw_pt::draw_pt ( draw_pt && d) : draw(d) 
{
    this->copy_code(d);
}

draw_pt::~draw_pt() 
{
    if(valid_range()) {
        int lr_index = this->pot_ptr->get_rng_lr()-1, ur_index = this->pot_ptr->get_rng_ur()-1;
        //--sanity tests-----------------------
        if(!this->pot_map_ninuse[lr_index][ur_index]) {
            std::cout << "draw_pt::~draw_pt test1" << std::endl;
            exit(1);
        } else if(!this->pot_map[lr_index][ur_index]) {
            std::cout << "draw_pt::~draw_pt test2" << std::endl;
            exit(1);
        }
        //--------------------------------------

        if(!--this->pot_map_ninuse[lr_index][ur_index]) {
            delete this->pot_map[lr_index][ur_index];
            this->pot_map[lr_index][ur_index] = 0;
        }
    //--sanity test------------------------
    } else if(this->pot_ptr) {
        int lr_index = this->pot_ptr->get_rng_lr()-1, ur_index = this->pot_ptr->get_rng_ur()-1;
        std::cout << "draw_pt::~draw_pt test3[" << lr_index << "][" << ur_index << "] " << std::endl;
        exit(1);
    }

    if(valid_sp_range()) {
        int lr_index = this->spec_pot_ptr->get_rng_lr()-1, ur_index = this->spec_pot_ptr->get_rng_ur()-1;
        //--sanity tests-----------------------
        if(!this->pot_map_ninuse[lr_index][ur_index]) {
            std::cout << "draw_pt::~draw_pt test4" << std::endl;
            exit(1);
        } else if(!this->pot_map[lr_index][ur_index]) {
            std::cout << "draw_pt::~draw_pt test5" << std::endl;
            exit(1);
        }
        //--------------------------------------

        if(!--this->pot_map_ninuse[lr_index][ur_index]) {
            delete this->pot_map[lr_index][ur_index];
            this->pot_map[lr_index][ur_index] = 0;
        }
    //--sanity test------------------------
    } else if(this->spec_pot_ptr) {
        std::cout << "draw_pt::~draw_pt test6: " << *this << std::endl;
        exit(1);
    }
    //--------------------------------------

    //--sanity tests-----------------------
    if(!--this->n_objs) {
        for(int r = 0; r < MAX_LOT_RANGE-5; r++) {
            for(int c = r+4; c < MAX_LOT_RANGE-1; c++) {
                int lr = r+1, ur = c+1;
                if(!VALID_RANGE) {
                    std::cout << "draw_pt::~draw_pt test7[" << r << "][" << c << "] " << std::endl;
                    exit(1);
                }
                if(this->pot_map_ninuse[r][c] || this->pot_map[r][c]) {
                    std::cout << "draw_pt::~draw_pt test8[" << r << "][" << c << "] " << 
                    this->pot_map_ninuse[r][c] << ", " << this->pot_map[r][c] << std::endl;
                    exit(1);
                }
            }
        }
    }
    //--------------------------------------
}



void draw_pt::set_range(int lr, int ur) {

    if(!VALID_RANGE)
        throw InvalidRangeException("draw_pt::set_range Invalid range 1"); 
    else if(valid_range() && !this->pot_ptr) { // if first time through (superconstructor just called)
        int cur_lr = this->range.lower, cur_ur = this->range.upper;
        if(this->range.lower != lr && this->range.upper != ur) {
            std::cout << "draw_pt::set_range: test1" << std::endl;
            exit(1);
        }
        goto ADD_POT;

    } else if(!(this->range.lower == lr && this->range.upper == ur)) {
        /* If the range was previously set... */
        if(valid_range()) {
            this->pot_ptr = 0;
            int cur_lr = this->range.lower, cur_ur = this->range.upper;
            if(!--this->pot_map_ninuse[cur_lr-1][cur_ur-1]) {
                delete this->pot_map[cur_lr-1][cur_ur-1];
                this->pot_map[cur_lr-1][cur_ur-1] = 0;
            }
        }
        draw::set_range(lr, ur);

        ADD_POT:
        int lr_index = lr-1, ur_index = ur-1;
        if(!this->pot_map_ninuse[lr_index][ur_index]) 
            this->pot_map[lr_index][ur_index] = new pot(lr, ur);
        this->pot_map_ninuse[lr_index][ur_index]++;
        this->pot_ptr = this->pot_map[lr_index][ur_index];
    } 
}

void draw_pt::set_range_sp(int lr, int ur) 
{
    if(!VALID_RANGE_SP)
        throw InvalidRangeException("draw_pt::set_range_sp Invalid range 1"); 
    else if(valid_sp_range() && !this->spec_pot_ptr)  // if first time through (superconstructor just called)
        goto ADD_POT;
    else if(!(this->range_sp.lower == lr && this->range_sp.upper == ur)) {
        /* If the range was previously set... */
        if(valid_sp_range()) {
            this->pot_ptr = 0;
            int cur_lr = this->range_sp.lower, cur_ur = this->range_sp.upper;
            if(!--this->pot_map_ninuse[cur_lr-1][cur_ur-1]) {
                delete this->pot_map[cur_lr-1][cur_ur-1];
                this->pot_map[cur_lr-1][cur_ur-1] = 0;
            }
        }
       
        draw::set_range_sp(lr, ur);

        ADD_POT:
        int lr_index = lr-1, ur_index = ur-1;
        if(!this->pot_map_ninuse[lr_index][ur_index]) 
            this->pot_map[lr_index][ur_index] = new pot(lr, ur);
        this->pot_map_ninuse[lr_index][ur_index]++;
        this->spec_pot_ptr = this->pot_map[lr_index][ur_index];
    } 
}

void draw_pt::set_range_urs(int ur, int sp_ur)
{
    this->set_range(1, ur);
    this->set_range_sp(1, sp_ur);
}


void draw_pt::pot::pluck(int n_vals, int *result)
{

    int tmp_indecies[6] = {0}, hmap[MAX_LOT_RANGE] = {0};

    /* Check number of values fits in range {1,6} */
    if(n_vals < 1 || n_vals > 6) 
        throw DrawException1("draw_pt::pot::pluck: Invalid input 1"); 
    
    /* Gather n random unique indecies from the pot. */
    for(int i = 0; i < n_vals; i++) {
        int tmp_index;
        REDO:
        int min = 0, max = this->size()-1;
        tmp_index = GET_RAND_NUM;
        if(hmap[tmp_index]++)
            goto REDO; 
        tmp_indecies[i] = tmp_index;
    }

    /* Freeze the pot and pluck the values. 
        (need the lock for this loop so values don't change/jumble) */
    this->pmux.lock(); 
    for(int i = 0; i < n_vals; i++) 
        result[i] = this->vals[tmp_indecies[i]];
    this->pmux.unlock();
}


void draw_pt::fill() {
    return this->fill(NBALLS);
}

void draw_pt::fill(int n_balls) 
{

    int stop_at;

    /* Need a valid range set for this operation. */
    if(n_balls < 1 || n_balls > 6) 
        throw DrawException1("draw_pt:fill: Invalid input 1");
    else if(n_balls == NBALLS && !this->valid_sp_range()) 
        throw InvalidRangeException("draw_pt:fill: Invalid input 2");
    if(!this->valid_range()) 
        throw InvalidRangeException("draw_pt:fill: Invalid range 1");
    //this->clear(); // ???
    /* Only deal with the first five balls first, then the special ball. (if chosen) */
    stop_at = n_balls < 6 ? n_balls : 5;

    /* Pluck the balls from the jumbling pot and sort. */
    this->pot_ptr->pluck(stop_at, this->balls);

    this->ball_sort(stop_at);

    /* Check if the user chose the special ball (ball 6, index 5) */
    if(n_balls == 6) {
        if(!valid_sp_range()) {
            this->clear();
            throw InvalidRangeException("draw_pt:fill: Invalid range 2");
        }
        this->spec_pot_ptr->pluck(1, this->spec_val);        
    }
}

void draw_pt::jumble_for()
{
    this->jumble_for(DEF_JUMBLE_FOR);
}


void draw_pt::jumble_for(int usec)
{
    if(!this->pot_ptr || !this->pot_ptr->is_jumbling()) 
        return;
    usleep(usec);
}

