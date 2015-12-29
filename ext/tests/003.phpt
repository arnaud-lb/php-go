--TEST--
Reflection
--FILE--
<?php
$module = phpgo_load(__DIR__ . "/fixtures/go/test.so", "test");
ReflectionClass::export($module);
--EXPECTF--
Class [ <internal:phpgo> final class PHPGo\Module\test_%s ] {

  - Constants [0] {
  }

  - Static properties [0] {
  }

  - Static methods [0] {
  }

  - Properties [0] {
  }

  - Methods [12] {
    Method [ <internal:phpgo> public method add ] {

      - Parameters [2] {
        Parameter #0 [ <required> $int_0 ]
        Parameter #1 [ <required> $int_1 ]
      }
    }

    Method [ <internal:phpgo> public method add32 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $int32_0 ]
        Parameter #1 [ <required> $int32_1 ]
      }
    }

    Method [ <internal:phpgo> public method add64 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $int64_0 ]
        Parameter #1 [ <required> $int64_1 ]
      }
    }

    Method [ <internal:phpgo> public method add8 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $int8_0 ]
        Parameter #1 [ <required> $int8_1 ]
      }
    }

    Method [ <internal:phpgo> public method addf32 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $float32_0 ]
        Parameter #1 [ <required> $float32_1 ]
      }
    }

    Method [ <internal:phpgo> public method addf64 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $float64_0 ]
        Parameter #1 [ <required> $float64_1 ]
      }
    }

    Method [ <internal:phpgo> public method addu ] {

      - Parameters [2] {
        Parameter #0 [ <required> $uint_0 ]
        Parameter #1 [ <required> $uint_1 ]
      }
    }

    Method [ <internal:phpgo> public method addu32 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $uint32_0 ]
        Parameter #1 [ <required> $uint32_1 ]
      }
    }

    Method [ <internal:phpgo> public method addu64 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $uint64_0 ]
        Parameter #1 [ <required> $uint64_1 ]
      }
    }

    Method [ <internal:phpgo> public method addu8 ] {

      - Parameters [2] {
        Parameter #0 [ <required> $uint8_0 ]
        Parameter #1 [ <required> $uint8_1 ]
      }
    }

    Method [ <internal:phpgo> public method boolAnd ] {

      - Parameters [2] {
        Parameter #0 [ <required> $bool_0 ]
        Parameter #1 [ <required> $bool_1 ]
      }
    }

    Method [ <internal:phpgo> public method concat ] {

      - Parameters [2] {
        Parameter #0 [ <required> $string_0 ]
        Parameter #1 [ <required> $string_1 ]
      }
    }
  }
}
