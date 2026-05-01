# Yocto and Build Notes

## Assumptions

- OP-TEE OS and client packages are already integrated in the BSP
- the exported TA dev kit is available on the build machine
- `libteec` headers and runtime are present for the host client

## Typical build

```bash
export TA_DEV_KIT_DIR=/path/to/export-ta_arm64
make -C ta
make -C host
```

## Publication guardrails

- keep UUIDs generic
- do not publish production attestation flows here
- keep persistent object names example-only

