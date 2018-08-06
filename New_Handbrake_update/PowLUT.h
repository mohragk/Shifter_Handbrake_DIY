//
// MAKE SURE TO ONLY USE MAXIMUM OF 32 STEPS!
//



#ifndef LUT_H
#define LUT_H

#include <assert.h>

class PowLUT
{
	
public:

	PowLUT() 
		: _tableSize(32), currentSkewFactor(1.0)
	{
	}

  	PowLUT(float skewFactor, int steps = 32, int range = 1024) 
		: _tableSize(steps), currentSkewFactor(1.0)
  	{
    	setLUT(skewFactor, steps, range);
      
  	}


  	~PowLUT() {}

	bool setLUT(float skew, int steps, int range)
	{
	    steps = constrain(steps, 0, 32);
	    
	    if(steps > 32)
	      return false;
	      
	    if (currentSkewFactor != skew)
	    {
			_tableSize = steps;
			int rows = steps;
			int cols = 2;
			int value = 0;
			int offset = range / rows;

			//initialize table to 0's
			memset(table,0,sizeof(table[0][0] * 32 * 2));

			for (int i = 0; i < rows; i++)
			{ 
				table[i][0] = value;
				table[i][1] = getSkewedValue(value, skew, range);

				value += offset;
			}

			currentSkewFactor = skew;
		}

		return true;
	}

	int getMappedValue(int input)
	{
		int value = 0;
		
		for (int i = 0; i < _tableSize - 1; i++)
		{
			if (input >= table[i][0] && input <= table[i+1][0])
			{
				float interpolated = (table[i][1] - table[i+1][1]) / (table[i][0] - table[i+1][0]);
				interpolated *= input - table[i][0];
				interpolated += table[i][1];
				value = static_cast<int>( floor(interpolated) ); 
			}
		}
		return value;
	}


private:
	int table[32][2];
	int _tableSize;
	float currentSkewFactor;
	int currentRange;


	int getSkewedValue(int value, float skew, int range)
	{
		if (skew = 1.0)
			return value;

		float normalised = (float)value / range;
		float skewed = pow(normalised, skew);

		return static_cast<int> (skewed * range);
	}

};


#endif // LUT_H
