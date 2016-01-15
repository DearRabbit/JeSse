x = 10000;

function printparam(x, y, z, w) {
	print(x);
	print(y);
	print(z);
	print(w);
}
print('First:');
printparam(1, 2, 3);
print('Second:');
printparam(1, 2, 3, 4, 5);


print('-----------recursive:-------------');

function Fib(x) {
	if (x == 0)
		return 0;
	else if (x == 1)
		return 1;

	return Fib(x-1)+Fib(x-2);
}

var i = 0;
while (i < 10)
{
	print(Fib(i));
	i = i + 1;
}

print('-----------closure:-------------');

function function_closure (x) {
	var b = x + 1;
	return function (y) {
		x = x + y;
		return x;
	};
}

a = function_closure(2);
b = a(2);
c = a(3);
print(b);
print(c);

print(x);