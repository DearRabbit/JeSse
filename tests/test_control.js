a = 12

if (a)
{
	print('use num as condition');
	if (a > 0 && a < 20)
	{
		print('use logic expressions as condition');
	}
}

var sum = 0;
for (var i = 0; i < a; i=i+1)
{
	sum = sum + i;
	print(i + 'loop');
}
print(sum);
