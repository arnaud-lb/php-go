--TEST--
Test load
--FILE--
<?php 
$module = phpgo_load(__DIR__ . "/fixtures/go/test.so", "test");

echo "boolAnd:\n";
var_dump($module->boolAnd(true, true));
var_dump($module->boolAnd(true, false));

echo "add:\n";
var_dump($module->add(40, 2));

echo "add8:\n";
var_dump($module->add8(40, 2));

echo "add32:\n";
var_dump($module->add32(40, 2));

echo "add64:\n";
var_dump($module->add64(40, 2));

echo "addu:\n";
var_dump($module->add(40, 2));

echo "addu8:\n";
var_dump($module->addu8(40, 2));

echo "addu32:\n";
var_dump($module->addu32(40, 2));

echo "addu64:\n";
var_dump($module->addu64(40, 2));

echo "addf32:\n";
$f = $module->addf32(40.1, 2.2);
printf("%s(%.2f)\n", gettype($f), $f);

echo "addf64:\n";
$f = $module->addf64(40.1, 2.2);
printf("%s(%.2f)\n", gettype($f), $f);

echo "concat:\n";
var_dump($module->concat("foo", 42));

?>
--EXPECT--
boolAnd:
bool(true)
bool(false)
add:
int(42)
add8:
int(42)
add32:
int(42)
add64:
int(42)
addu:
int(42)
addu8:
int(42)
addu32:
int(42)
addu64:
int(42)
addf32:
double(42.30)
addf64:
double(42.30)
concat:
string(5) "foo42"
