function foo(count) {
	var x=0;
	return function () {
		return x+1;
	};
}

a = foo(1);	