package phpgo

// #cgo CFLAGS: -Wall
// #include "phpgo.h"
// static int32_t _PHPGO_API_VERSION() { return PHPGO_API_VERSION; }
import "C"
import "unsafe"

type CPHPExports C.php_exports

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
		cin.kind = C.CString(in.kind)
	}
	cpe.num_ins = C.size_t(len(pe.ins))
	for i, out := range pe.outs {
		cout := C.init_php_export_out(cpe, C.size_t(i))
		cout.kind = C.CString(out.kind)
	}
	cpe.num_outs = C.size_t(len(pe.outs))
}
