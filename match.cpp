

#include "match.h"

std::string match::Unmatchable::what () {
    message = "Match is UNMATCHABLE. (must be sequential, no duplicates and in range.)";
    return message;
}



match::match() {
    this->n_tries = 0;
    this->searching = false;
    this->duration = 0;
}

match::match(draw &d) {
    this->vals = d;  
    this->n_tries = 0;
    this->searching = false;
}


int& match::operator[] (int index) 
{
    if(index > 5 || index < 0) 
        throw std::range_error("match::operator[]: invalid index");
    return this->vals[index];
}


const int match::operator[] (int index) const
{
    if(index > 5 || index < 0) 
        throw std::range_error("match::operator[] const: invalid index");
    return this->vals[index]; 
}

bool match::matchable() 
{
    if(!this->vals.get_ball(0)) 
        return false;
    int zero_found = 0;
    for(int i = 0; i < 6; i++) {
        const int ball_val = this->vals.get_ball(i);
        if(!ball_val) 
            zero_found++;
        else if(zero_found) 
            return false;
    }
    return true;
}

void match::set_draw(draw *d) 
{ 
    this->vals = *d;
}


long match::run() 
{

    if(!this->vals.validate() || this->vals.empty() || !this->matchable()) //{
        throw Unmatchable("match::run(): Unmatchable 1"); 
    
    int n_vals_to_match;
    for(n_vals_to_match = 0; n_vals_to_match < 6; n_vals_to_match++) {
        const int ball_val = this->vals.get_ball(n_vals_to_match);
        if(!ball_val)
            break;
    }

    
    draw d; 
    d.set_range(this->vals.get_range_lr(), this->vals.get_range_ur());
    d.set_range_sp(this->vals.get_range_sp_lr(), this->vals.get_range_sp_ur());
    
    int n_to_match;
    draw tmp = this->vals;
    for(n_to_match = 0; n_to_match < 6 && this->vals[n_to_match] > 0; n_to_match++);
    this->searching = true;
    auto start = std::chrono::steady_clock::now();
    do {
        d.fill(n_to_match);
        this->n_tries++;
    } while(d != this->vals);
    auto end = std::chrono::steady_clock::now();
    this->duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000000;
    //cout << "Elapsed time in nanoseconds: "
      //  << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
        //<< " ns" << endl;
    this->searching = false;
    return this->n_tries;
}


std::ostream& operator<< (std::ostream& out, const match& data)
{    
    out << "MATCH: " << data.vals; // << std::endl;
    if(data.searching) 
        out << "is currently searching... ";
    else if(data.n_tries) 
        out << std::endl << "found match in " << data.n_tries << " tries.";
    else if(!(draw(data.vals)).empty())
        out << " has not yet run.";
    else
        out << " (empty)";
    return out;
}