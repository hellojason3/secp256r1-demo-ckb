# secp256r1 Crypto Helper
This is a demonstration demo for contract development on the ckb(Nervos) chain, using secp256r1 to verify the signature.

<strong>Update in 2024: Please replace the submodule's `libecc` library with https://github.com/cryptape/libecc, which was optimized by the nervos team. </strong>

## Features
* Context Initialization: Initializes the secp256r1 context with ECDSA and SHA-256 as default signature and hash algorithms.
* Public Key Recovery: Recovers public keys from ECDSA signatures.
* Public Key Export: Exports public keys into an affine coordinate buffer.
* Signature Verification: Verifies ECDSA signatures against given messages.
* Conversion Utilities: Converts affine public key buffers to projective buffers.

## Code Overview
### secp256r1_context_t
This structure holds the elliptic curve and hashing context for secp256r1 operations:

* `sig_algo`: Signature algorithm (ECDSA)
* `hash_algo`: Hash algorithm (SHA-256)
* `ec_params`: Elliptic curve parameters


### Functions
`secp256r1_context_init`: Initializes the secp256r1 context.
`secp256r1_recover_public_key_from_signature`: Recovers public keys from a given signature.
`secp256r1_pub_key_export_to_aff_buf`: Exports a public key to an affine buffer.
`secp256r1_verify_signature`: Verifies a signature using the public key and a message.
`convert_aff_buf_to_prj_buf`: Converts an affine buffer to a projective buffer.


## Example Usage
The main function demonstrates the following:

1. Context Initialization:
* Initializes the secp256r1 context.

2.Public Key Recovery:
* Converts a signature string into binary, then recovers the public keys.

3. Public Key Export:
* Exports the recovered public keys to buffers and prints them.

4. Signature Verification:
* Verifies the signature against the original message.



### Dependencies
* `lib_ecc_types.h`, `libec.h`, `libsig.h`: Headers for elliptic curve cryptography.
* `ckb_dlfcn.h`, `ckb_syscalls.h`: CKB (Nervos CKB) system call libraries.
* `utils.h`, `secp256r1_helper.h`: Helper utilities and specific secp256r1 implementations.


## Compilation
To compile the project, ensure all dependencies are correctly linked and included. A typical compilation command might look like this:

```bash
make debug-all-via-docker
```


## License
This project is licensed under the MIT License. See the LICENSE file for more details.

