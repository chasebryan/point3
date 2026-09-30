# point3

POINT/3 is a three-witness self-correcting transport experiment.

The original form uses a binary repetition code:

```text
0 -> 000
1 -> 111
```

That corrects one flipped physical bit per triple, but two flips in the same triple can silently invert the decoded logical bit.

## Shatter POINT/3

The Shatter variant keeps exactly three witnesses but removes fixed physical locality.

For each logical bit `i`, three reversible positions are used at epoch `e`:

```text
P0(i,e) = ( i       + 11e) mod 128
P1(i,e) = (45i + 17 + 29e) mod 128
P2(i,e) = (61i + 73 + 47e) mod 128
```

The odd multipliers make each lane a permutation of all 128 logical bit positions.

A transfer does not decode and re-encode the object. It moves each witness directly from its position at epoch `e` to its position at epoch `e+1`. Reconstruction reads the three positions for the current epoch and majority-votes them back into the original 128 logical bits.

This spreads neighboring physical corruption across different logical points instead of keeping all witnesses for one bit adjacent.

It does **not** create new information: a targeted corruption that changes two witnesses belonging to the same logical point can still defeat majority decoding. Shatter POINT/3 is therefore a locality/interleaving experiment, not standalone encryption or authentication.

## Files

- `point3.or` — original Orange POINT/3 repetition-code prototype
- `point3.c` — original minimal C encoder/decoder
- `point3-shatter.or` — Orange Shatter POINT/3
- `point3-shatter.c` — minified C Shatter POINT/3

## Orange

```sh
orangec eval point3.or
orangec eval point3-shatter.or
```

The Orange Shatter implementation represents the 128 logical bits directly as `Word[8]^128`, with every element equal to `0` or `1`.

## C

```sh
cc -std=c99 -O2 -Wall -Wextra -Werror -c point3.c
cc -std=c99 -O2 -Wall -Wextra -Werror -c point3-shatter.c
```

The Shatter C implementation stores the three 128-bit lanes in 48 bytes plus a 32-bit epoch.

Core operations:

```text
p3(message)   -> create the three shattered witnesses
t(&state)     -> reversible transfer to the next epoch
r(&state,out) -> rebuild by inverse addressing + majority vote
```

The C source is intentionally minified and contains no comments.
