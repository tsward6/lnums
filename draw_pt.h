
#ifndef DPT
#define DPT


#include "draw.h"
#include <thread>

#define DEF_JUMBLE_FOR 5000



/* Draw with a running pot is a child class of draw. It uses a multi-threaded range based
   map to draw numbers from. */
class draw_pt : public draw {

    protected:
    
    class pot {

        protected:
        
        int n_jumbles;
        struct draw::draw_range rng;
        int vals[MAX_LOT_RANGE] = {0}; /* pot variable (leave extra room) */
    
        int pot_size;
        std::mutex pmux;
        std::thread thr;

        bool jumbling;

        bool start_jumbling(), stop_jumbling();
      
        public:

        pot(int rl, int ru);

        ~pot();

        void swap2();

        void pluck(int n_vals, int *result);

        bool is_jumbling();

        int get_n_jumbles() { return this->n_jumbles; }

        int get_n_jumbles() const { return this->n_jumbles; }

        const int get_rng_lr() { return this->rng.lower; }

        const int get_rng_ur() { return this->rng.upper; }

        const int get_rng_lr() const { return this->rng.lower; }

        const int get_rng_ur() const { return this->rng.upper; }

        const int *get_vals() { return this->vals; }
        const int *get_vals() const { return this->vals; }

        const int size() { return this->pot_size; }

        const int size() const { return this->pot_size; }
        

    };

    

    pot *pot_ptr, *spec_pot_ptr;

    /* Note: need -std=c++17 for inline variables...*/
    static inline int n_objs = 0;
    static inline pot* pot_map[MAX_LOT_RANGE-5][MAX_LOT_RANGE-1] = {0}; // use the same maps for special
    static inline int pot_map_ninuse[MAX_LOT_RANGE-5][MAX_LOT_RANGE-1] = {0};


    public:
    
    draw_pt();
    draw_pt(int upper, int spec_upper);
    draw_pt(int rng[2], int sp_rng[2]);

    draw_pt(int b1, int b2, int b3, int b4, int b5, int b6);
    draw_pt(int vals[NBALLS]);
    draw_pt(int n_vals, int *vals);

    ~draw_pt();
    //draw_pt(draw_pt &d) {}
    draw_pt(draw_pt &d); // copy constructor
    draw_pt(const draw_pt &d);
    void copy_code(const draw_pt &rhs);

    draw_pt ( draw_pt && d);
    // Move constructor

    void fill();
    void fill(int n_balls);

    void jumble_for(), jumble_for(int usec);

    void set_range(int lower, int upper);
    void set_range_sp(int lower, int upper);
    void set_range_urs(int lower, int upper);

    friend void jumble(draw_pt::pot *p);

    friend std::ostream& operator<< (std::ostream& out, const draw_pt::pot& data);
};

void jumble(draw_pt::pot *p);

std::ostream& operator<<(std::ostream& output, const draw_pt::pot& data);



#endif