# point3

POINT/3 is a tiny self-correcting transport primitive.

```text
0 -> 000
1 -> 111
```

Each logical bit is represented by three physical witnesses. Decoding is a majority vote, so any one flipped physical bit in a three-bit cell is corrected.

```text
000 001 010 100 -> 0
111 110 101 011 -> 1
```

## Orange

```sh
orangec eval point3.or
```

The Orange implementation includes byte encoding/decoding, 16-byte frames, a round trip, and a deliberate one-bit fault demonstration.

## C

```sh
cc -std=c99 -O2 -c point3.c
```

The C implementation is intentionally minified. `e()` encodes one byte into 24 physical bits; `d()` repairs/decodes it.

POINT/3 is an error-correction layer, not standalone encryption or authentication. Pair it with authenticated encryption when confidentiality and tamper detection are required.
