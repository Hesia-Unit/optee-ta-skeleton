# optee-ta-skeleton

[![CI](https://github.com/Hesia-Unit/optee-ta-skeleton/actions/workflows/ci.yml/badge.svg)](https://github.com/Hesia-Unit/optee-ta-skeleton/actions/workflows/ci.yml)
[![License](https://img.shields.io/github/license/Hesia-Unit/optee-ta-skeleton)](LICENSE)
[![Tag](https://img.shields.io/github/v/tag/Hesia-Unit/optee-ta-skeleton)](https://github.com/Hesia-Unit/optee-ta-skeleton/tags)

Minimal public OP-TEE Trusted Application skeleton with a matching user-space client.

## Scope

This repository is the generic public deliverable referenced in the HESIA open-engineering plan. It is intentionally stripped of product logic and keeps only reusable OP-TEE patterns:

- TA command definition
- session handling
- persistent object storage
- random generation
- host-side TEEC invocation

## Repository layout

- `host/`: simple command-line client
- `ta/`: Trusted Application source
- `docs/`: build and porting notes
- `ci/stubs/`: syntax-only headers used by CI sanity checks

## Commands exposed

- `version`
- `random`
- `counter-read`
- `counter-write`

## Build

```sh
export TA_DEV_KIT_DIR=/path/to/export-ta_arm64
make -C ta
make -C host
```

## Recommended reading

- [Yocto and build notes](docs/YOCTO_BUILD.md)
- [Porting notes](docs/PORTING_NOTES.md)
- [Roadmap](ROADMAP.md)

## Status

This repository is intentionally minimal. It is meant to be a clean public starting point for OP-TEE experiments, not a drop-in production security boundary.

## License

MIT


