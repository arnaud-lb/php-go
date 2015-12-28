package php

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
		case Bool:
			v = reflect.ValueOf(cToBool(*(*C.uchar)(argPtr)))
		case Int:
			v = reflect.ValueOf(int(*(*C.uint64_t)(argPtr)))
		case Int8:
			v = reflect.ValueOf(int8(*(*C.uint64_t)(argPtr)))
		case Int32:
			v = reflect.ValueOf(int32(*(*C.uint64_t)(argPtr)))
		case Int64:
			v = reflect.ValueOf(int64(*(*C.uint64_t)(argPtr)))
		case Uint:
			v = reflect.ValueOf(uint(*(*C.uint64_t)(argPtr)))
		case Uint8:
			v = reflect.ValueOf(uint8(*(*C.uint64_t)(argPtr)))
		case Uint32:
			v = reflect.ValueOf(uint32(*(*C.uint64_t)(argPtr)))
		case Uint64:
			v = reflect.ValueOf(uint64(*(*C.uint64_t)(argPtr)))
		case Float32:
			v = reflect.ValueOf(float32(*(*C.double)(argPtr)))
		case Float64:
			v = reflect.ValueOf(float64(*(*C.double)(argPtr)))
		case String:
			s := (*C.php_arg_string)(argPtr)
			v = reflect.ValueOf(C.GoStringN(s.s, s.l))
		default:
			panic(fmt.Sprintf("PHPGoCall: Unexpected input kind `0x%x`", in.kind))
		}
		ins[i] = v
	}
	outs := pe.value.Call(ins)
	if len(outs) != len(pe.outs) {
		panic(fmt.Sprintf("PHPGoCall: Function returned %d outs, expected %d", len(outs), len(pe.outs)))
	}
	couts := C.calloc(C.size_t(len(pe.outs)), C.size_t(unsafe.Sizeof(*args)))
	for i, out := range pe.outs {
		ptr := unsafe.Pointer(uintptr(couts) + uintptr(i)*unsafe.Sizeof(*args))
		switch out.kind {
		case Bool:
			*(*C.uchar)(ptr) = boolToC(outs[i].Bool())
		case Int, Int8, Int32, Int64:
			*(*C.int64_t)(ptr) = C.int64_t(outs[i].Int())
		case Uint, Uint8, Uint32, Uint64:
			*(*C.int64_t)(ptr) = C.int64_t(outs[i].Uint())
		case Float32, Float64:
			*(*C.double)(ptr) = C.double(outs[i].Float())
		case String:
			val := outs[i].String()
			s := (*C.php_arg_string)(ptr)
			s.s = C.CString(val)
			s.l = C.int(len(val))
		default:
			panic(fmt.Sprintf("PHPGoCall: Unexpected output kind `0x%x`", out.kind))
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

func cToBool(v C.uchar) bool {
	if v == 0 {
		return false
	}
	return true
}

func boolToC(v bool) C.uchar {
	if v {
		return 1
	}
	return 0
}
