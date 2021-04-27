double almostPI(int n)
{
	double pi;
	double denom;

	for(int i = 0; i < 2*n; i++)
	{
		denom = 1.0 + 2*i;

		if(i % 2 == 0)
			pi += 1.0/denom;
		else
			pi -= 1.0/denom;
	}

	return 4*pi;
}
