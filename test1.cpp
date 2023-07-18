

#include <iostream>
using namespace std;


#include "draw.h"
#include "draw_pt.h"



int main()
{
    long jumble_for = 100000;
    int rng[] = { 5, 75 }, srng[] = {2, 25};
    draw_pt dpt1(rng, srng); //, dpt2;
    dpt1.jumble_for(jumble_for);
    try {
        dpt1.fill();
        int tmp = dpt1[9];
       
    } catch(draw::InvalidRangeException ire) {
        cout << "InvalidRangeException 1: " << ire.what() << endl;
    }
 
    cout << "dpt1: " << dpt1 << endl;
    dpt1.present(1000);

    draw_pt dpt2 = dpt1;


    const draw_pt cdpt1 = draw_pt(1,2,3,4,5,6);

    const draw_pt cdpt2 = cdpt1;

    //cdpt2.set_ball(1, 1);  won't let it
    const draw tmpd1;
    const draw_pt tmpd2;

    bool tmpb = tmpd1 == tmpd2;


    draw_pt tmpd3 = draw_pt(10,20,23,24,55,16);

    tmpb = tmpd1 == cdpt2;

    
    try{
        tmpd3.set_range_urs(50, 20);
        //tmpd3.fill(4);
    } catch(draw::InvalidRangeException ire) {
        cout << "InvalidRangeException 2: " << ire.what() << endl;
    }

    tmpd3.fill(2);

    tmpd3.fill();

    int tmp_ints1[] = { 3, 4, 5, 6, 7, 8 };
    draw_pt a(tmp_ints1);
    a.present();


    int tmp_ints2[] = { 6, 7, 8 };
    draw_pt b(sizeof(tmp_ints2)/sizeof(tmp_ints2[0]), tmp_ints2);
    b.present();

    return 0;
}
