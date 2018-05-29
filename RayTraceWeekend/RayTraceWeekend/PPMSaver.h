#pragma once

#include <fstream>
#include <algorithm>


template<typename T>
T bilinear(
	const float &tx,
	const float &ty,
	const T &c00,
	const T &c10,
	const T &c01,
	const T &c11)
{
#if 1
	T a = c00 * (1.f - tx) + c10 * tx;
	T b = c01 * (1.f - tx) + c11 * tx;
	return a * (1.f - ty) + b * ty;
#else
	return (1 - tx) * (1 - ty) * c00 +
		tx * (1 - ty) * c10 +
		(1.f - tx) * ty * c01 +
		tx * ty * c11;
#endif
}



template<typename T>
class Color3
{
public:
	Color3() : r(0), g(0), b(0) {}
	Color3(T rr) : r(rr), g(rr), b(rr) {}
	Color3(T rr, T gg, T bb) : r(rr), g(gg), b(bb) {}
	Color3 operator * (const T &v) const
	{
		return Color3(r*v, g*v, b*v);
	}
	Color3 operator + (const Color3 &c) const
	{
		return Color3(r + c.r, g + c.g, b + c.b);
	}
	friend Color3 operator * (const float &f, const Color3 &c)
	{
		return Color3(c.r * f, c.g * f, c.b * f);
	}
	friend std::ostream & operator << (std::ostream &os, const Color3 &c)
	{
		os << c.r << " " << c.g << " " << c.b;
		return os;
	}
	T r, g, b;
};

typedef Color3<int> Color3f;

void saveToPPM(const char *fn, const Color3f *c, const int &width, const int &height)
{
	std::ofstream ofs;
	// flags are necessary if your compile on Windows
	ofs.open(fn, std::ios::out | std::ios::binary);
	if (ofs.fail()) {
		fprintf(stderr, "ERROR: can't save image to file %s\n", fn);
	}
	else {
		ofs << "P6\n" << width << " " << height << "\n255\n";
		const Color3f *pc = c;
		for (int j = 0; j < height; ++j) {
			for (int i = 0; i < width; ++i) {
				char r = static_cast<char>(std::min(255.f, 255 * pc->r + 0.5f));
				char g = static_cast<char>(std::min(255.f, 255 * pc->g + 0.5f));
				char b = static_cast<char>(std::min(255.f, 255 * pc->b + 0.5f));
				ofs << r << g << b;
				pc++;
			}
		}
	}
	ofs.close();
}