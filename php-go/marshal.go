package php

// #cgo CFLAGS: -Wall
// #include "phpgo.h"
// static int32_t _PHPGO_API_VERSION() { return PHPGO_API_VERSION; }
import "C"
import "unsafe"

type CPHPExports C.php_exports

type CKind C.int

const (
	Bool    CKind = C.PHPGO_KIND_BOOL
	Int     CKind = C.PHPGO_KIND_INT
	Int8    CKind = C.PHPGO_KIND_INT8
	Int16   CKind = C.PHPGO_KIND_INT16
	Int32   CKind = C.PHPGO_KIND_INT32
	Int64   CKind = C.PHPGO_KIND_INT64
	Uint    CKind = C.PHPGO_KIND_UINT
	Uint8   CKind = C.PHPGO_KIND_UINT8
	Uint16  CKind = C.PHPGO_KIND_UINT16
	Uint32  CKind = C.PHPGO_KIND_UINT32
	Uint64  CKind = C.PHPGO_KIND_UINT64
	Float32 CKind = C.PHPGO_KIND_FLOAT32
	Float64 CKind = C.PHPGO_KIND_FLOAT64
	String  CKind = C.PHPGO_KIND_STRING
)

func marshalExports(pes *PHPExports) *CPHPExports {
	cpes := C.new_php_exports(C.size_t(len(pes.exports)))
	nth := -1
	for _, pe := range pes.exports {
		nth++
		cpe := C.init_php_exports_export(cpes, C.size_t(nth), C.size_t(len(pe.ins)), C.size_t(len(pe.outs)))
		marshalExport(cpe, pe)
	}
	cpes.num_exports = C.size_t(len(pes.exports))
	cpes.version = C._PHPGO_API_VERSION()
	return (*CPHPExports)(cpes)
}

func marshalExport(cpe *C.php_export, pe *PHPExport) {
	cpe.name = C.CString(pe.name)
	cpe.g = unsafe.Pointer(pe)
	for i, in := range pe.ins {
		cin := C.init_php_export_in(cpe, C.size_t(i))
		cin.kind = C.int(in.kind)
		cin.name = C.CString(in.name)
	}
	cpe.num_ins = C.size_t(len(pe.ins))
	for i, out := range pe.outs {
		cout := C.init_php_export_out(cpe, C.size_t(i))
		cout.kind = C.int(out.kind)
		cout.name = C.CString(out.name)
	}
	cpe.num_outs = C.size_t(len(pe.outs))
}

func freeMarshalled(cpe *CPHPExports) {
	C.free_php_exports((*C.php_exports)(cpe))
}
