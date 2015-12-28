package main

import "github.com/arnaud-lb/php-go/php-go"

var _ = php.Export("test", map[string]interface{}{
	"boolAnd": func(a, b bool) bool {
		return a && b
	},
	"add": func(a, b int) int {
		return a + b
	},
	"add8": func(a, b int8) int8 {
		return a + b
	},
	"add32": func(a, b int32) int32 {
		return a + b
	},
	"add64": func(a, b int64) int64 {
		return a + b
	},
	"addu": func(a, b uint) uint {
		return a + b
	},
	"addu8": func(a, b uint8) uint8 {
		return a + b
	},
	"addu32": func(a, b uint32) uint32 {
		return a + b
	},
	"addu64": func(a, b uint64) uint64 {
		return a + b
	},
	"addf32": func(a, b float32) float32 {
		return a + b
	},
	"addf64": func(a, b float64) float64 {
		return a + b
	},
	"concat": func(a, b string) string {
		return a + b
	},
})

func main() {
}
