#include "minirt.h"

int			apply_t(int texture, t_inter inter)
{
	int		black;
	int		white;
	t_p3	coords;
	t_p3	val;
	int		party_mix;

	texture += 1;
	black = 0x000000;
	white = 0xffffff;
	coords.x = abs((int)floor(inter.p.x));
	coords.y = abs((int)floor(inter.p.y));
	coords.z = abs((int)floor(inter.p.z));
	val.x = (int)coords.x % 2;
	val.y = (int)coords.y % 2;
	val.z = (int)coords.z % 2;
	party_mix = ((int)val.x ^ (int)val.y) ^ (int)val.z;
	return (party_mix ? black : white);
}
