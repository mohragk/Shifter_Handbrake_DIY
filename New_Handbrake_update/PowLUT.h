//
// MAKE SURE TO ONLY USE MAXIMUM OF 32 STEPS!
//



#ifndef LUT_H
#define LUT_H

#include <assert.h>

class PowLUT
{
	
public:

	PowLUT() : _tableSize(32)
	{
	}

  PowLUT(float skewFactor, int steps = 32, int range = 1024) : _tableSize(steps)
  {
    setLUT(skewFactor, steps, range);
  }


  ~PowLUT() {}

	void setLUT(float skew, int steps, int range)
	{
    assert(steps <= 32);
    
		_tableSize = steps;
		int cols = 2;
		int rows = steps;
		int value = 0;
		int offset = range / rows;

    //initialize table to 0's
		memset(table,0,sizeof(table[0][0] * rows * cols));

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
		
		for (int i = 0; i < _tableSize - 1; i++)
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
	int _tableSize;


	int getSkewedValue(int value, float skew)
	{
	  float normalised = (float)value / 1024;
	  float skewed = pow(normalised, skew);
	  
	  return static_cast<int> (skewed * 1024);
	}

 void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp) 
 {
    // transmit diagnostic informations through serial link. 
    Serial.println(__func);
    Serial.println(__file);
    Serial.println(__lineno, DEC);
    Serial.println(__sexp);
    Serial.flush();
    // abort program execution.
    abort();
 }
 
};


#endif // LUT_H
