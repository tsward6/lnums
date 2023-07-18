

#include "ticket.h"


ticket::ticket()
{
    time_t now = time(0);
    dt = ctime(&now);
    int cnt = 0;
    for(int i = 0; dt[i]; i++) {
        if(dt[i] == ' ') {
            if(cnt++ == 3) {
                dt[i] = '\0';
                break;
            }
        }
    }
}


draw& ticket::operator[] (int index) 
{
    if(this->draws.empty() || index > this->draws.size()-1 || index < 0) 
        throw std::range_error("ticket::operator[]: invalid index 1");
    return this->draws.at(index);
}


draw& ticket::operator[] (int index) const
{
    if(this->draws.empty() || index > this->draws.size()-1 || index < 0) 
        throw std::range_error("ticket::operator[] const: invalid index 1");
    return (draw&)*(&this->draws.at(index));
}


const draw ticket::get_draw(int index)
{

    if(this->draws.empty() || index > this->draws.size()-1 || index < 0) 
        throw std::range_error("ticket::get_draw: invalid index 1");
    return this->draws.at(index);
}



bool ticket::add_draw(draw &d) {

    if(d.empty()) 
        return false;
    this->draws.push_back(d);
    return true;
}


bool ticket::add_draw_af() 
{
    draw d;
    d.fill();
    this->draws.push_back(d);
    return true;
}



bool ticket::add_draw_af(int n_draws) 
{
    if(n_draws < 1) 
        return false;
    for(int i = 0; i < n_draws; i++) {
        draw d;
        d.fill();
        this->draws.push_back(d);
    }
    return true;
}


bool ticket::match_draw(int index, match *m)
{
    if(this->draws.empty() || index > this->draws.size()-1 || index < 0) 
        return false;
    draw d = this->draws.at(index);
    if(!d.validate()) // || !d.matchable()) // gets checked in match.cpp
        return false;
    m->set_draw(&this->draws.at(index)); 
    return m->run();
}


bool ticket::match_all(std::vector<match*> *matches)
{
    if(this->draws.empty()) 
        return false;
    for(int i = 0; i < this->draws.size(); i++) {
        draw d = this->draws.at(i);
        match *m = new match(d);
        m->run();
        matches->push_back(m);
    }
    return true;
} 


std::ostream& operator<< (std::ostream& out, const ticket& data)
{    
    out << "TICKET: (" << data.draws.size() << " draw"; 
    if(data.draws.empty() || data.draws.size() > 1)
        out << "s";
    out << ")";
    if(!data.draws.empty()) 
        out << std::endl;
    for(int i = 0; i < data.draws.size(); i++)
        out << "draw " << i+1 << ": " << data.draws[i] << std::endl;
    return out;
}