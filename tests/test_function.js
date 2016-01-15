x = 10000;

function function_closure (x) {
	var b = x+1;
	print(x);
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