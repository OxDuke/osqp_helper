# osqp_helper
Some helper functions for osqp

## Motivation
OSQP <https://github.com/oxfordcontrol/osqp> is a good open source QP solver. This repo intends to add some useful functions to enhance OSQP. 

## Demo
Take a look at demo.cpp. In order to run the demo, you will have to do the following, which is a standard building process with cmake:

```bash
cd osqp_helper
mkdir build
cd build
cmake ..
make
./demo
```
## Misc
All the naming conventions follow osqp's documentation, make sure you are familiar with them.

## Documentation
All the helper functions are defined in osqp_helper.c and some of them are declared in osqp_helper.h. Their purpose shall be very easy to guess from their names. And there are docs in osqp_helper.h.

## Notes
There was a bug in function triplet_to_csr, the bug was fixed on July-13-2018<https://github.com/oxfordcontrol/osqp/commit/0f3533877fb15c017c96f1f9cb7c620c220be5dc>, so make sure you are using osqp released after this date.
