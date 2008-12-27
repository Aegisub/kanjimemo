/*********************************************************\
         __      __           __  __
        / /     / / _____    / / / / ______   __     __
       / /     / / /___  \  / / / / / ___  | / /    / /
      / /_____/ / _____| / / / / / / /__/ / / /    / /
     / ______  / / ___  / / / / / / _____/ / /    / /
    / /     / / / /__/ / / / / / / |____  / |____/ /
   /_/     /_/ |______/ /_/ /_/  \_____/  \_____  /
                                         ______/ /
            High Level Game Framework   /_______/

  ------------------------------------------------------

  Copyright (c) 2007 - Rodrigo Braz Monteiro.
  This file is subject to the terms of license.txt.

\*********************************************************/

#pragma once

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795f
#endif


namespace Halley {
	///////////////
	// Angle class
	template <typename T>
	class Angle {
	private:
		// Angle value in radians
		T value;

		// TODO: rewrite this
		inline void Limit()
		{
			while (value < 0.0f) value += 2*M_PI;
			while (value >= 2*M_PI) value -= 2*M_PI;
		}

	public:
		// Constructors
		Angle () { value = 0; }
		Angle (const Angle &angle) { value = angle.value; }

		// Comparison
		inline bool operator== (const Angle &param) const { return value == param.value; }
		inline bool operator!= (const Angle &param) const { return value != param.value; }

		// Basic arithmetics
		inline Angle operator+ (const Angle &param)
		{
			Angle final;
			final.value = value + param.value;
			final.Limit();
			return final;
		}
		inline Angle operator- (const Angle &param)
		{
			Angle final;
			final.value = value - param.value;
			final.Limit();
			return final;
		}

		// In-place operations
		inline void operator= (const Angle &angle) { value = angle.value; }
		inline void operator+= (const Angle &angle) { value += angle.value; Limit(); }
		inline void operator-= (const Angle &angle) { value -= angle.value; Limit(); }

		// Accessors
		inline void SetDegrees(const T degrees) { value = DegToRad(degrees); Limit(); }
		inline void SetRadians(const T radian) { value = radian; Limit();}
		inline T GetDegrees() const { return RadToDeg(value); }
		inline T GetRadians() const { return value; }

		// Which side should it turn to to reach the parameter angle?
		inline T TurnSide(const Angle &param) const
		{
			float res = sin(angle.value - value);
			if (res > 0.0f) return 1.0f;
			else return -1.0f;
		}

		// Turns a specific number of radians towards a target
		inline void TurnRadiansTowards(const Angle &angle,const T radians)
		{
			float side = TurnSide(angle);
			value += radians * side;
			Limit();
			float newSide = TurnSide(angle);
			if (side != newSide) value = angle.value;
		}

		// Turns a specific number of degrees towards a target
		inline void TurnDegreesTowards(const Angle &angle,const T degrees) { TurnRadiansTowards(angle,DegToRad(degrees)); }

		// Distance to another angle
		inline Angle Distance(const Angle &param) const
		{
			float v = value - param.value;
			if (v > M_PI) v = 2.0f*M_PI - v;
			return FromRadians(v);
		}

		// Conversion
		static inline T DegToRad(const T degrees) { return degrees * 0.01745329252f; }
		static inline T RadToDeg(const T radians) { return radians * 57.295779513f; }

		// Trigonometric functions
		inline T Sin() const { return sin(value); }
		inline T Cos() const { return cos(value); }
		inline T Tan() const { return tan(value); }

		// Builder methods
		inline static Angle FromRadians (const T radians) { Angle ang; ang.SetRadians(radians); return ang; }
		inline static Angle FromDegrees (const T radians) { Angle ang; ang.SetDegrees(radians); return ang; }
	};

	// Typedefs
	typedef Angle<double> Angle1d;
	typedef Angle<float> Angle1f;
}
