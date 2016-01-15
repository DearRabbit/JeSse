
function function_closure (x) {
	var b = x+1;
	return function (y) {
		print(b+y);
		return x;
	};
}

a = function_closure(2);
b = a();
print(b);