/*
 */
#include <avr/io.h>
#include <util/delay.h>
#include "h_bridge.h"

#define _BV(bit) (1 << (bit))

void init(void)
{
    init_h_bridge();
}

int main(void)
{

    init();

    while(1)
    {
        h_bridge_set_percentage_rechts(80);
        h_bridge_set_percentage_links(-80);
    }
    return 0;
}
