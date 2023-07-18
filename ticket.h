

#ifndef TK
#define TK

#include <vector>
#include "draw.h"
#include "match.h"

class ticket {

    std::vector<draw>draws;

    char* dt; // date and time of ticket

    public:

    ticket();

    draw& operator[] (int index) ;
    draw& operator[] (int index) const;

    char *get_dt() { return this->dt; }

    void clear() { this->draws.clear(); }

    bool match_draw(int index, match *m);  
    bool match_all(std::vector<match*> *matches); 

    int n_draws() { return this->draws.size(); }

    /* Add a draw that has already been filled/constructed */
    bool add_draw(draw &d);
    const draw get_draw(int index);

    /* Add a draw that is auto filled. */
    bool add_draw_af();
    bool add_draw_af(int n_draws);

    friend std::ostream& operator<< (std::ostream& out, const ticket& data);

};

std::ostream& operator<<(std::ostream& output, const ticket& data);

#endif
