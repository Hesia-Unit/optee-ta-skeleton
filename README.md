# optee-ta-skeleton

Minimal public OP-TEE Trusted Application skeleton with a matching user-space client.

## Scope

This repository is the generic public deliverable referenced in the HESIA marketing plan. It is intentionally stripped of HESIA product logic and keeps only reusable OP-TEE patterns:

- TA command definition
- session handling
- persistent object storage
- random generation
- host-side TEEC invocation

## Repository layout

- `host/`: simple command-line client
- `ta/`: Trusted Application source
- `docs/`: build and porting notes

## Commands exposed

- `version`
- `random`
- `counter-read`
- `counter-write`

## License

MIT

