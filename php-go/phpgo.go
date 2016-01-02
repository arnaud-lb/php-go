package php

import (
	"fmt"
	"reflect"
	"runtime"
	"sort"
)

type PHPExports struct {
	exports []*PHPExport
	c       *CPHPExports
}

type PHPExport struct {
	name  string
	value reflect.Value
	ins   []*PHPArgDesc
	outs  []*PHPArgDesc
}

type PHPArgDesc struct {
	kind CKind
	name string
}

var modules = map[string]*PHPExports{}

var kindMap = map[reflect.Kind]CKind{
	reflect.Bool:    Bool,
	reflect.Int:     Int,
	reflect.Int8:    Int8,
	reflect.Int16:   Int16,
	reflect.Int32:   Int32,
	reflect.Int64:   Int64,
	reflect.Uint:    Uint,
	reflect.Uint8:   Uint8,
	reflect.Uint16:  Uint16,
	reflect.Uint32:  Uint32,
	reflect.Uint64:  Uint64,
	reflect.Float32: Float32,
	reflect.Float64: Float64,
	reflect.String:  String,
}

func Export(name string, exports map[string]interface{}) *PHPExports {
	pe, err := newPHPExports(exports)
	if err != nil {
		panic(err)
	}
	modules[name] = pe
	return pe
}

func newPHPExports(exports map[string]interface{}) (*PHPExports, error) {

	phpExports := &PHPExports{}
	runtime.SetFinalizer(phpExports, phpExportsFinalizer)

	for name, e := range exports {
		if pe, err := newPHPExport(name, e); err != nil {
			return nil, fmt.Errorf("Failed exporting `%s`: %s", name, err)
		} else {
			phpExports.exports = append(phpExports.exports, pe)
		}
	}

	sort.Sort(byName(phpExports.exports))

	phpExports.c = marshalExports(phpExports)

	return phpExports, nil
}

func phpExportsFinalizer(pes *PHPExports) {
	if pes.c != nil {
		freeMarshalled(pes.c)
	}
}

func newPHPArgDesc(t reflect.Type, nth int) (*PHPArgDesc, error) {

	kind, ok := kindMap[t.Kind()]
	if !ok {
		return nil, fmt.Errorf("Arguments and return values of kind %s are not yet supported", t.Kind())
	}

	pa := &PHPArgDesc{
		kind: kind,
		name: fmt.Sprintf("%s_%d", t.Kind(), nth),
	}

	return pa, nil
}

func (e *PHPExport) addIn(in reflect.Type) error {
	if pa, err := newPHPArgDesc(in, len(e.ins)); err != nil {
		return err
	} else {
		e.ins = append(e.ins, pa)
		return nil
	}
}

func (e *PHPExport) addOut(out reflect.Type) error {
	if pa, err := newPHPArgDesc(out, len(e.outs)); err != nil {
		return err
	} else {
		e.outs = append(e.outs, pa)
		return nil
	}
}

func newPHPExport(name string, export interface{}) (*PHPExport, error) {

	t := reflect.TypeOf(export)
	pe := &PHPExport{
		name:  name,
		value: reflect.ValueOf(export),
	}

	switch t.Kind() {
	case reflect.Func:
		for i := 0; i < t.NumIn(); i++ {
			if err := pe.addIn(t.In(i)); err != nil {
				return nil, err
			}
		}
		for i := 0; i < t.NumOut(); i++ {
			if err := pe.addOut(t.In(i)); err != nil {
				return nil, err
			}
		}
	default:
		return nil, fmt.Errorf("Exporting values of kind `%s` is not yet supported", t.Kind())
	}

	return pe, nil
}

type byName []*PHPExport

func (b byName) Len() int {
	return len(b)
}

func (b byName) Less(i, j int) bool {
	return b[i].name < b[j].name
}

func (b byName) Swap(i, j int) {
	b[i], b[j] = b[j], b[i]
}
