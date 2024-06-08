#pragma once
#pragma warning(disable : 4201)

class Quat
{
public:
	static const Quat Identity;

	Quat()
		: v(Vect::Zero) {}

	Quat(const Quat& tmp)
		: v(tmp.v) {}

	Quat& operator=(const Quat& tmp)
	{
		v = tmp.v;
		return *this;
	}

	~Quat() {}

	Quat(const Vect& inV)
		: v(inV) {}

	Quat(const Matrix& tmp);

	Quat GetInverse() const
	{
		return Quat(-v);
	}

	Quat operator-() const
	{
		return *this;
	}

	Quat operator-(const Quat& tmp) const
	{
		float w1 = w;
		Vect v1 = v;
		float w2 = tmp.w;
		Vect v2 = tmp.v;


		Quat q(v1 - v2);
		q.w = w1 - w2;
		return q;
	}

	Quat operator+(const Quat& tmp) const
	{
		float w1 = w;
		Vect v1 = v;
		float w2 = tmp.w;
		Vect v2 = tmp.v;


		Quat q(v1 + v2);
		q.w = w1 + w2;
		return q;
	}

	Quat operator*(float s) const
	{
		Quat q(v * s);
		q.w = w * s;
		return q;
	}

	Quat operator/(float s) const
	{
		float inv = 1.0f / s;
		Quat q(v * inv);
		q.w = w * inv;
		return q;
	}

	Quat& operator*(const Quat& tmp) const
	{
		float w1 = w;
		Vect v1 = v;
		float w2 = tmp.w;
		Vect v2 = tmp.v;

		Quat q(w1 * v2 + w2 * v1 + v1.cross(v2));
		q.w = w1 * w2 - v1.dot(v2);
		return q;
	}

	float dot(const Quat& other) const
	{
		return (w * other.w) + (v.dot(other.v));
	}

	void Normalize()
	{
		float tmpW = w;
		float norm = 1.0f / sqrtf(this->dot(*this));
		v = v * norm;
		w = tmpW * norm; // ???
	}
	
	friend Quat operator * (float scale, Quat& inQ); // noted...


	union
	{
		struct {
			float x, y, z, w;
		};

		Vect v;
	};
};

#pragma warning(default : 4201)
