# point3

POINT/3 explores recursive three-way reconstruction.

## Generations

### POINT/3

The original prototype is a binary repetition code:

```text
0 -> 000
1 -> 111
```

It corrects one flipped physical bit per triple, but two flips in the same triple defeat majority decoding.

### Shatter POINT/3

The Shatter prototype scatters the three witnesses through reversible affine permutations. This improves physical locality/burst behavior but does not increase coding distance; targeted corruption of two witnesses still defeats majority decoding.

### POINT/3-W

POINT/3-W replaces majority-of-three with a recursive authenticated `(3,2)` MDS/XOR node.

For every authenticated 128-bit node `X`:

```text
T = HMAC-SHA256(K, "P3W" || level || node_index || X)[0..127]

A = X
B = T
C = X XOR T
```

Any two of `A,B,C` reconstruct both `X` and `T`. The recovered `X` is accepted only when the path-bound HMAC verifies.

Every child is recursively encoded by the same rule:

```text
1 -> 3 -> 9 -> 27 -> 81 -> ...
```

Only terminal fragments are stored. Reconstruction runs in reverse:

```text
... -> 81 -> 27 -> 9 -> 3 -> 1
```

The decoder gathers authenticated candidates from every surviving pair. If none verify it fails. If all verified candidates agree it returns that node. If authenticated candidates disagree it fails rather than guessing.

This separates the two goals:

- **integrity** comes from the path-bound MAC;
- **availability** comes from the recursive `(3,2)` reconstruction topology.

At one node, any two children are sufficient. At depth `d`, destroying one complete top-level branch still leaves enough information to rebuild the root.

## Files

- `point3.or` — original Orange repetition prototype
- `point3.c` — original terse C repetition prototype
- `point3-shatter.or` — Orange affine-scatter prototype
- `point3-shatter.c` — terse C affine-scatter prototype
- `point3-w.c` — recursive authenticated POINT/3-W implementation
- `Point3W.cry` — Cryptol executable specification of a depth-4 / 81-leaf POINT/3-W tree

## C

POINT/3-W operates on one 128-bit object and a 256-bit HMAC key.

```sh
cc -std=c99 -O2 -Wall -Wextra -Werror point3-w.c your_test.c -lcrypto
```

Public API:

```c
P p3e(const unsigned char x[16],unsigned depth,const unsigned char key[32]);
int p3d(P *p,unsigned depth,const unsigned char key[32],unsigned char out[16]);
void p3f(P *p);
```

`P.x` contains the terminal 16-byte fragments and `P.v` is the one-byte-per-leaf presence mask. Set `P.v[i]=0` to model an erased terminal fragment. Corrupted present fragments are rejected when they cannot participate in a valid authenticated reconstruction.

The implementation requires OpenSSL/libcrypto.

## Cryptol

`Point3W.cry` uses the canonical HMAC-SHA256 implementation from Galois' `cryptol-specs` repository:

```text
Primitive::Symmetric::MAC::HMAC::Instantiations::HMAC_SHA256
```

Place `cryptol-specs` on `CRYPTOLPATH`, then:

```sh
cryptol Point3W.cry
```

Inside Cryptol:

```text
:check roundtrip
:check one_branch_loss
```

`roundtrip` checks the full depth-4 `1 -> 3 -> 9 -> 27 -> 81` encode/decode topology. `one_branch_loss` removes the first complete 27-leaf root branch and checks that the original 128-bit object still reconstructs.
