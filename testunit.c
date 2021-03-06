#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "runtime.h"

extern JsObject* vmresult;

#define PRINTOBJ(obj) (Js_Type(obj)->tp_print(obj, stdout))
#define HASHOBJ(obj) (Js_Type(obj)->tp_hash(obj))
#define TOSTRINGOBJ(obj) (Js_Type(obj)->tp_tostr(obj))

int main()
{
    _JsNum_Init();
    _JsBool_Init();
    _JsBaseVar_Init();
    _JsDict_Init();

    
    JsDictObject* mydict = (JsDictObject*)JsDict_New();

    JsStringObject* var1 = JsString_FromString("var1");
    JsStringObject* var2 = JsString_FromString("var2");
    JsStringObject* var3 = JsString_FromString("var3");
    JsNumObject* value1 = JsNum_FromDouble(1);
    JsNumObject* value2 = JsNum_FromDouble(2);
    JsNumObject* value3 = JsNum_FromDouble(999999);

    JsDict_SetItem(mydict, var1, value1);
    JsDict_SetItem(mydict, var2, value2);
    JsDict_SetItem(mydict, var3, value3);

    Js_PRINTLNOBJ(mydict);
    

    // vmcode test[13] = {
    //  JSVM_EQ,
    //  _(one),
    //  _(string_two),
    //  JSVM_SetJump_Offset(-4),
    //  JSVM_HALT
    // };

    // jsvm_execute(test);

    //PRINTOBJ(vmresult);

    // Js_DECREF(one);
    // Js_DECREF(string_two);

    _JsDict_Deinit();
    _JsBaseVar_Deinit();
    _JsBool_Deinit();
    _JsNum_Deinit();

    Js_Exit();

    return 0;
}