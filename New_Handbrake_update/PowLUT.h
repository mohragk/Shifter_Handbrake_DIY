#ifndef LUT_H
#define LUT_H


class PowLUT
{
	
public:
	PowLUT()
	{

	}

	PowLut(float skew, int steps)
	{
		setLUT(skew, steps);
	}

	void setLUT(float skew = 1.0, int steps = 24)
	{
		tableSize = steps;
		int cols = 2;
		int rows = steps;
		int value = 0;
		int offset = 1024 / rows;

		table.resize(rows, std::vector<int>(cols, init));

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
		
		for (int i = 0; i < table_size - 1; i++)
		{
			if (input >= table[i][0] && input <= table[i+1][0])
			{
				float interpolated = (table[i][1] - table[i+1][1]) / (table[i][0] - table[i+1][0]);
				interpolated *= input - table[i][0];
				interpolated += table[i][1];
				return interpolated; 
			}
		}
		return value;
	}


private:
	std::vector<std::vector<int>> table;
	int tableSize;

	int getSkewedValue(int value, float skew)
	{
	  float normalised = (float)value / 1024;
	  float skewed = std::pow(normalised, skew);
	  
	  return static_cast<int> (skewed * 1024);
	}
};


#endif // LUT_H