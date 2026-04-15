#ifndef TEXTURE_H
#define TEXTURE_H
#include "rtweekend.h";
class texture {
public:
	virtual ~texture() = default;
	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {

};

#endif // !TEXTURE_H
