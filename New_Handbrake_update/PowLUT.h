//
// MAKE SURE TO ONLY USE MAXIMUM OF 32 STEPS!
//



#ifndef LUT_H
#define LUT_H


class PowLUT
{
	
public:

	PowLUT() : tableSize(32)
	{
	}

  PowLUT(float skewFactor, int steps) : tableSize(steps)
  {
    setLUT(skewFactor, steps);
  }


  ~PowLUT() {}

	void setLUT(float skew = 1.0, int steps = 32)
	{
		tableSize = steps;
		int cols = 2;
		int rows = steps;
		int value = 0;
		int offset = 1024 / rows;

		memset(table,0,sizeof(table));

		for (int i = 0; i < rows; i++)
		{	
			table[i][0] = value;
			table[i][1] = getSkewedValue(value, skew);

			value += offset;
		}
	}

	int getMappedValue(int input)
	{
		int value = 0;
		
		for (int i = 0; i < tableSize - 1; i++)
		{
			if (input >= table[i][0] && input <= table[i+1][0])
			{
				float interpolated = (table[i][1] - table[i+1][1]) / (table[i][0] - table[i+1][0]);
				interpolated *= input - table[i][0];
				interpolated += table[i][1];
				value =  interpolated; 
			}
		}
		return value;
	}


private:
	int table[32][2];
	int tableSize;

	int getSkewedValue(int value, float skew)
	{
	  float normalised = (float)value / 1024;
	  float skewed = pow(normalised, skew);
	  
	  return static_cast<int> (skewed * 1024);
	}
};


#endif // LUT_H
