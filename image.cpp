#include "image.h"
#include "object_image.h"

void Image::notify()
{
    parent->notify();
}
