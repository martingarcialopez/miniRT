#include "libvct.h"

double		distance(t_p3 p1, t_p3 p2)
{
	double d;

	d = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));
	return (d);
}
