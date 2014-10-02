#include <iostream>
#include <embree2\rtcore.h>
#include <embree2\rtcore_ray.h>

int main(int argc, char* argv[])
{
    rtcInit(NULL);
    rtcExit();
    return 0;
}