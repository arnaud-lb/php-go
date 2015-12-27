package phpgo

// #include <stdlib.h>
// #include "phpgo.h"
import "C"

import (
	"fmt"
	"reflect"
	"unsafe"
)

//export PHPGoCall
func PHPGoCall(pes *C.php_export, args *C.php_arg) *C.php_arg {
	pe := (*PHPExport)(unsafe.Pointer(pes.g))
	ins := make([]reflect.Value, len(pe.ins))
	for i, in := range pe.ins {
		var v reflect.Value
		argPtr := unsafe.Pointer(uintptr(unsafe.Pointer(args)) + uintptr(i)*unsafe.Sizeof(*args))
		switch in.kind {
		case "int":
			v = reflect.ValueOf(int(*(*C.int)(argPtr)))
		default:
			panic(fmt.Sprintf("PHPGoCall: Unexpected in kind `%s`", in.kind))
		}
		ins[i] = v
	}
	outs := pe.value.Call(ins)
	if len(outs) != len(pe.outs) {
		panic(fmt.Sprintf("PHPGoCall: Function returned %d outs, expected %d", len(outs), len(pe.outs)))
	}
	couts := C.calloc(C.size_t(len(pe.outs)), C.size_t(unsafe.Sizeof(*args)))
	for i, out := range pe.outs {
		switch out.kind {
		case "int":
			*(*C.int)(unsafe.Pointer(uintptr(couts) + uintptr(i)*unsafe.Sizeof(*args))) = C.int(outs[i].Int())
		default:
			panic(fmt.Sprintf("PHPGoCall: Unexpected out kind `%s`", out.kind))
		}
	}
	return (*C.php_arg)(couts)
}

//export PHPGoExports
func PHPGoExports(name *C.char) *C.php_exports {
	if e, ok := modules[C.GoString(name)]; ok {
		return (*C.php_exports)(e.c)
	}
	return nil
}
